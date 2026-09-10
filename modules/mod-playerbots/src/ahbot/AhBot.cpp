/*
 * Stock/valuation derived from CMaNGOS AuctionHouseBot (see CMaNGOS AUTHORS).
 * Turtle native lifecycle adaptation. SPDX-License-Identifier: GPL-2.0-or-later
 * Free software, without any warranty; see COPYING.
 */
#include "AhBot.h"
#include "Chat/Chat.h"
#include "Config/Config.h"
#include "DBCStores.h"
#include "Database/DatabaseEnv.h"
#include "Item.h"
#include "LootMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "SQLStorages.h"
#include "SystemConfig.h"
#include "WorkSlice.h"
#include "World.h"
#include "WorldSession.h"
#include "playerbot/PlayerbotAIConfig.h"
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <memory>
#include <sstream>
using namespace ahbot;
extern bool IsPlayerHardcore(uint32 lowGuid);

bool AhBot::HandleAhBotCommand(ChatHandler* handler, char const* args)
{
    return auctionbot.HandleCommand(handler, args ? args : "");
}
uint32 AhBot::auctionIds[3] = {1, 6, 7};
namespace
{
uint64 ClockUs()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch())
        .count();
}
bool ReadNumbers(std::string text, std::vector<int64>& values)
{
    std::replace(text.begin(), text.end(), ',', ' ');
    std::istringstream input(text);
    std::string token;
    while (input >> token)
    {
        char* end = nullptr;
        errno = 0;
        int64 v = std::strtoll(token.c_str(), &end, 10);
        if (errno == ERANGE || end == token.c_str() || *end)
            return false;
        values.push_back(v);
    }
    return true;
}
bool QueryIds(char const* sql, std::vector<uint32>& ids)
{
    // A sentinel distinguishes an empty valid source from a failed DB read.
    std::string query = std::string("SELECT * FROM (") + sql + ") ahbot_source UNION ALL SELECT 0";
    std::unique_ptr<QueryResult> result(WorldDatabase.Query(query.c_str()));
    if (!result)
        return false;
    if (result)
        do
        {
            uint32 id = result->Fetch()[0].GetUInt32();
            if (id)
                ids.push_back(id);
        } while (result->NextRow());
    return true;
}
AuctionHouseObject* House(uint32 index)
{
    auto entry = sAuctionHouseStore.LookupEntry(AhBot::auctionIds[index]);
    return entry ? sAuctionMgr.GetAuctionsMap(entry) : nullptr;
}
} // namespace
void AhBot::Init()
{
    if (!Load())
        sLog.outError("[AhBot] CMaNGOS configuration failed; seller/buyer not started.");
}
bool AhBot::Load()
{
    Config config;
    if (!config.SetSource(SYSCONFDIR "ahbot.conf", "AHBot_"))
        return false;
    if (config.GetStringDefault("AuctionHouseBot.Chance.Sell", "").empty())
    {
        sLog.outError("[AhBot] Requires AuctionHouseBot.* settings; old category configuration is not compatible. See "
                      "ahbot.conf.dist.");
        return false;
    }
    Settings candidate;
    candidate.enabled = config.GetBoolDefault("AhBot.Enabled", true);
    auto bounded = [&](char const* key, uint32 fallback, uint32 min, uint32 max) {
        int32 v = config.GetIntDefault(key, fallback);
        return v >= int64(min) && v <= int64(max) ? uint32(v) : fallback;
    };
    candidate.sell = bounded("AuctionHouseBot.Chance.Sell", 10, 0, 100);
    candidate.buy = bounded("AuctionHouseBot.Chance.Buy", 10, 0, 100);
    candidate.variance = bounded("AuctionHouseBot.Value.Variance", 10, 0, 100);
    candidate.bidMin = bounded("AuctionHouseBot.Bid.Min", 75, 0, 100);
    candidate.bidMax = bounded("AuctionHouseBot.Bid.Max", 90, 0, 100);
    candidate.bidMin = std::min(candidate.bidMin, candidate.bidMax);
    candidate.timeMin = bounded("AuctionHouseBot.Time.Min", 2, 1, 72);
    candidate.timeMax = bounded("AuctionHouseBot.Time.Max", 24, 1, 72);
    candidate.timeMin = std::min(candidate.timeMin, candidate.timeMax);
    candidate.buyValue = bounded("AuctionHouseBot.Buy.Value", 90, 0, 200);
    candidate.requiredLevel = bounded("AuctionHouseBot.Level.MaxRequired", 60, 1, 255);
    candidate.dynamicLevel = config.GetBoolDefault("AuctionHouseBot.Level.DynamicMaxRequired", false);
    candidate.ignoreGm = config.GetBoolDefault("AuctionHouseBot.Level.IgnoreGmAccounts", false);
    candidate.levelRefresh = bounded("AuctionHouseBot.Level.DynamicRefreshInterval", 10, 1, 1440) * 60;
    candidate.vendorValue = config.GetBoolDefault("AuctionHouseBot.Value.Vendor", true);
    candidate.sliceUs = bounded("AuctionHouseBot.Work.BudgetUs", 2000, 100, 10000);
    candidate.sliceOperations = bounded("AuctionHouseBot.Work.MaxOperations", 32, 1, 256);
    char const* quality[] = {"Poor", "Normal", "Uncommon", "Rare", "Epic", "Legendary", "Artifact"};
    for (size_t q = 0; q < 7; ++q)
    {
        std::vector<int64> values;
        if (!ReadNumbers(config.GetStringDefault(std::string("AuctionHouseBot.Value.") + quality[q]), values) ||
            values.size() != 17)
        {
            sLog.outError("[AhBot] Value.%s requires 17 percentages; previous configuration retained.", quality[q]);
            return false;
        }
        for (size_t c = 0; c < 17; ++c)
        {
            if (values[c] < 0 || values[c] > 1000000)
                return false;
            candidate.values[q][c] = uint32(values[c]);
        }
    }
    std::vector<Source> newSources(10);
    char const* names[] = {"Loot.Creature.Normal",
                           "Loot.Creature.Elite",
                           "Loot.Creature.RareElite",
                           "Loot.Creature.WorldBoss",
                           "Loot.Creature.Rare",
                           "Loot.Disenchant",
                           "Loot.Fishing",
                           "Loot.Gameobject",
                           "Loot.Skinning",
                           "Items.Profession"};
    for (size_t i = 0; i < 10; ++i)
    {
        std::vector<int64> range;
        if (!ReadNumbers(config.GetStringDefault(std::string("AuctionHouseBot.") + names[i], "0,0,0,0"), range) ||
            range.size() != 4)
            return false;
        for (size_t n = 0; n < 4; ++n)
        {
            if (range[n] < (n == 0 ? -1000000 : 0) || range[n] > 1000000)
                return false;
            newSources[i].range[n] = int32(range[n]);
        }
        newSources[i].range[0] = std::min(newSources[i].range[0], newSources[i].range[1]);
        newSources[i].range[2] = std::min(newSources[i].range[2], newSources[i].range[3]);
    }
    bool dataOk = true;
    for (uint32 rank = 0; rank < 5; ++rank)
    {
        newSources[rank].store = &LootTemplates_Creature;
        std::string sql =
            "SELECT loot_id FROM creature_template WHERE loot_id<>0 AND `rank`=" + std::to_string(rank);
        // Preserve per-creature-template weighting when Turtle shares a loot ID.
        dataOk = QueryIds(sql.c_str(), newSources[rank].ids) && dataOk;
    }
    newSources[5].store = &LootTemplates_Disenchant;
    newSources[6].store = &LootTemplates_Fishing;
    newSources[7].store = &LootTemplates_Gameobject;
    newSources[8].store = &LootTemplates_Skinning;
    dataOk = QueryIds("SELECT DISTINCT entry FROM disenchant_loot_template", newSources[5].ids) && dataOk;
    dataOk = QueryIds("SELECT DISTINCT entry FROM fishing_loot_template", newSources[6].ids) && dataOk;
    dataOk = QueryIds("SELECT DISTINCT gt.data1 FROM gameobject_template gt JOIN gameobject g ON g.id=gt.entry WHERE gt.type=3 "
             "AND g.spawntimesecsmax>0 AND gt.data1<>0",
             newSources[7].ids) && dataOk;
    dataOk = QueryIds("SELECT DISTINCT entry FROM skinning_loot_template", newSources[8].ids) && dataOk;
    std::set<uint32> crafts;
    for (uint32 id = 0; id < sSpellTemplate.GetMaxEntry(); ++id)
        if (SpellEntry const* spell = sSpellTemplate.LookupEntry<SpellEntry>(id))
            if ((spell->Attributes & 32) && (spell->Attributes & 65536))
                for (uint32 e = 0; e < 3; ++e)
                    if (spell->Effect[e] == SPELL_EFFECT_CREATE_ITEM &&
                        sObjectMgr.GetItemPrototype(spell->EffectItemType[e]))
                        crafts.insert(spell->EffectItemType[e]);
    newSources[9].ids.assign(crafts.begin(), crafts.end());
    std::vector<uint32> vendors;
    dataOk = QueryIds("SELECT item FROM npc_vendor UNION SELECT item FROM npc_vendor_template", vendors) && dataOk;
    if (!dataOk)
    {
        sLog.outError("[AhBot] Source query failed; previous settings retained.");
        return false;
    }
    std::map<uint32, Override> newOverrides;
    std::unique_ptr<QueryResult> result(
        CharacterDatabase.Query("SELECT item,value,add_chance,min_amount,max_amount FROM ahbot_items UNION ALL SELECT 0,0,0,0,0"));
    if (!result)
        return false;
    if (result)
        do
        {
            Field* f = result->Fetch();
            if (!f[0].GetUInt32())
                continue;
            Override d{policy::Money(f[1].GetUInt32()), std::min<uint32>(100, f[2].GetUInt32()), f[3].GetUInt32(),
                       f[4].GetUInt32()};
            d.max = std::max(d.min, d.max);
            newOverrides[f[0].GetUInt32()] = d;
        } while (result->NextRow());
    std::vector<Owner> newOwners;
    result.reset(CharacterDatabase.Query("SELECT guid,account FROM characters"));
    if (result)
        do
        {
            Field* f = result->Fetch();
            uint32 guid = f[0].GetUInt32(), account = f[1].GetUInt32();
            // Never create/spend assets as a human account, even if AhBot.GUID was misconfigured.
            if (sPlayerbotAIConfig.IsInRandomAccountList(account) && !IsPlayerHardcore(guid))
                newOwners.push_back({guid, account});
        } while (result->NextRow());
    if (candidate.enabled && newOwners.empty())
    {
        sLog.outError("[AhBot] No non-hardcore random-bot owners available; previous configuration retained.");
        return false;
    }
    settings = candidate;
    sources.swap(newSources);
    owners.swap(newOwners);
    overrides.swap(newOverrides);
    vendorItems = std::set<uint32>(vendors.begin(), vendors.end());
    maxRequiredLevel = settings.requiredLevel;
    maxItemLevel = policy::ItemLevelCap(maxRequiredLevel);
    nextLevelCheck = 0;
    nextCheck = time(nullptr) + 20;
    sLog.outString("[AhBot] CMaNGOS market enabled=%u sell/buy=%u/%u%% owners=%zu, world slice=%uus/%u attempts.",
                   settings.enabled, settings.sell, settings.buy, owners.size(), settings.sliceUs,
                   settings.sliceOperations);
    for (size_t i = 0; i < sources.size(); ++i)
        sLog.outString("[AhBot] %s: %zu templates; %d,%d,%d,%d", names[i], sources[i].ids.size(), sources[i].range[0],
                       sources[i].range[1], sources[i].range[2], sources[i].range[3]);
    return true;
}
void AhBot::RefreshLevel()
{
    if (!settings.dynamicLevel || time(nullptr) < nextLevelCheck)
        return;
    uint32 highest = 0, fallback = 0;
    for (auto const& pair : sWorld.GetAllSessions())
        if (Player* p = pair.second->GetPlayer())
        {
            fallback = std::max(fallback, p->GetLevel());
            if (!settings.ignoreGm || pair.second->GetSecurity() == SEC_PLAYER)
                highest = std::max(highest, p->GetLevel());
        }
    maxRequiredLevel = highest ? highest : (fallback ? fallback : settings.requiredLevel);
    maxItemLevel = policy::ItemLevelCap(maxRequiredLevel);
    nextLevelCheck = time(nullptr) + settings.levelRefresh;
}
bool AhBot::IsBotOwner(uint32 guid, uint32 account) const
{
    return guid && account && sPlayerbotAIConfig.IsInRandomAccountList(account);
}
uint32 AhBot::Price(ItemPrototype const* p) const
{
    if (!p || p->Quality >= 7 || p->Class >= 17)
        return 0;
    auto it = overrides.find(p->ItemId);
    if (it != overrides.end())
        return it->second.value;
    return policy::Price(p->BuyPrice, p->SellPrice, p->Quality,
                         settings.vendorValue && vendorItems.count(p->ItemId) ? 100
                                                                              : settings.values[p->Quality][p->Class]);
}
uint32 AhBot::Varied(uint32 price) const
{
    // Match CMaNGOS's integer-copper rounding, with overflow-safe arithmetic.
    int64 delta = int64(urand(0, settings.variance * 2 + 1)) - settings.variance;
    return policy::Money(uint64(std::max<int64>(0, int64(price) + delta * (price / 100))));
}
bool AhBot::Eligible(ItemPrototype const* p, bool forced) const
{
    if (!p || !p->Stackable || p->Quality >= 7 || p->Class >= 17)
        return false;
    auto it = overrides.find(p->ItemId);
    if (it != overrides.end() && !it->second.value)
        return false;
    if (forced)
        return true;
    return p->RequiredLevel <= maxRequiredLevel && p->ItemLevel <= maxItemLevel && p->Bonding != BIND_WHEN_PICKED_UP &&
           p->Bonding != BIND_QUEST_ITEM && !(p->Flags & ITEM_FLAG_HAS_LOOT) && settings.values[p->Quality][p->Class];
}
void AhBot::Update()
{
    if (!settings.enabled || sWorld.IsStopped() || sWorld.IsShutdowning())
        return;
    if (phase == Phase::Idle && !pendingRebuild && !rebuildRemaining && time(nullptr) < nextCheck)
        return;
    uint64 start = ClockUs();
    WorkSlice slice(start, settings.sliceOperations, settings.sliceUs);
    while (slice.Take(ClockUs()))
    {
        Step();
        if (phase == Phase::Idle && !pendingRebuild && !rebuildRemaining)
            break;
    }
    lastSliceUs = ClockUs() - start;
    maxSliceUs = std::max(maxSliceUs, lastSliceUs);
}
void AhBot::Step()
{
    switch (phase)
    {
    case Phase::Idle:
        if (pendingRebuild)
        {
            includeBids = pendingRebuild == 2;
            pendingRebuild = 0;
            listed = bought = expired = protectedBids = failed = 0;
            scanHouse = scanCursor = 0;
            page.clear();
            pageIndex = 0;
            phase = Phase::Expire;
            return;
        }
        action = (action + 1) % (rebuildRemaining ? 3 : 6);
        currentHouse = action % 3;
        if (rebuildRemaining)
            --rebuildRemaining;
        if (action < 3 && urand(0, 99) < settings.sell)
        {
            RefreshLevel();
            stock.clear();
            sourceIndex = 0;
            picksRemaining = -1;
            rollsRemaining = 0;
            phase = Phase::Gather;
        }
        else if (action >= 3 && urand(0, 99) < settings.buy)
        {
            scanHouse = currentHouse;
            scanCursor = 0;
            page.clear();
            pageIndex = 0;
            phase = Phase::Buy;
        }
        else
            FinishPass();
        return;
    case Phase::Gather:
        GatherOne();
        return;
    case Phase::Overrides: {
        auto it = overrides.upper_bound(overrideCursor);
        if (it == overrides.end())
        {
            phase = Phase::Post;
            return;
        }
        overrideCursor = it->first;
        if (it->second.chance)
            stock[it->first] = urand(0, 99) < it->second.chance ? urand(it->second.min, it->second.max) : 0;
        return;
    }
    case Phase::Post:
        PostOne();
        return;
    case Phase::Buy:
        ScanOne(false);
        return;
    case Phase::Expire:
        ScanOne(true);
        return;
    }
}
void AhBot::GatherOne()
{
    if (sourceIndex == sources.size())
    {
        overrideCursor = 0;
        phase = Phase::Overrides;
        return;
    }
    Source const& s = sources[sourceIndex];
    if (s.ids.empty() || !s.range[1] || !s.range[3])
    {
        ++sourceIndex;
        picksRemaining = -1;
        return;
    }
    if (picksRemaining < 0)
        picksRemaining = int32(std::max<int64>(0, int64(urand(0, s.range[1] - s.range[0])) + s.range[0]));
    if (!rollsRemaining)
    {
        if (!picksRemaining)
        {
            ++sourceIndex;
            picksRemaining = -1;
            return;
        }
        --picksRemaining;
        selectedTemplate = s.ids[urand(0, s.ids.size() - 1)];
        if (!s.store)
        {
            auto p = sObjectMgr.GetItemPrototype(selectedTemplate);
            if (Eligible(p, false) && p->Quality && !urand(0, (1u << (p->Quality - 1)) - 1))
                stock[selectedTemplate] += std::max<uint64>(
                    1, uint64(std::llround(uint64(p->Stackable) * urand(s.range[2], s.range[3]) / 100.0)));
            return;
        }
        rollsRemaining = urand(s.range[2], s.range[3]);
        if (!rollsRemaining)
            return;
    }
    --rollsRemaining;
    if (auto table = s.store->GetLootFor(selectedTemplate))
    {
        // Fresh container PER roll; otherwise Turtle silently caps accumulated loot.
        Loot loot(nullptr);
        table->Process(loot, *s.store, s.store->IsRatesAllowed());
        ++lootRolls;
        for (auto const& item : loot.items)
            if (Eligible(sObjectMgr.GetItemPrototype(item.itemid), false))
                stock[item.itemid] += item.count;
    }
}
void AhBot::PostOne()
{
    if (stock.empty())
    {
        FinishPass();
        return;
    }
    auto it = stock.begin();
    auto o = overrides.find(it->first);
    auto p = sObjectMgr.GetItemPrototype(it->first);
    if (!it->second || !Eligible(p, o != overrides.end() && o->second.chance))
    {
        stock.erase(it);
        return;
    }
    uint32 price = Varied(Price(p)), count = policy::Stack(it->second, p->Stackable, price);
    if (!count)
    {
        stock.erase(it);
        return;
    }
    if (Publish(it->first, count, price))
        ++listed;
    else
        ++failed;
    it->second -= count;
    if (!it->second)
        stock.erase(it);
}
bool AhBot::Publish(uint32 itemId, uint32 count, uint32 unitValue)
{
    auto entry = sAuctionHouseStore.LookupEntry(auctionIds[currentHouse]);
    auto house = House(currentHouse);
    if (!entry || !house || owners.empty())
        return false;
    Owner owner = owners[urand(0, owners.size() - 1)];
    if (!IsBotOwner(owner.guid, owner.account) || IsPlayerHardcore(owner.guid) ||
        sObjectMgr.GetPlayerAccountIdByGUID(owner.guid) != owner.account)
        return false;
    std::unique_ptr<Item> item(Item::CreateItem(itemId, count));
    if (!item)
        return false;
    item->SetOwnerGuid(ObjectGuid(HIGHGUID_PLAYER, owner.guid));
    if (int32 property = Item::GenerateItemRandomPropertyId(itemId))
        item->SetItemRandomProperties(property);
    item->ClearUpdateMask(false);
    std::unique_ptr<AuctionEntry> auction(new AuctionEntry{});
    auction->Id = sObjectMgr.GenerateAuctionID();
    auction->itemGuidLow = item->GetGUIDLow();
    auction->itemTemplate = itemId;
    auction->itemCount = count;
    auction->itemRandomPropertyId = item->GetItemRandomPropertyId();
    auction->owner = owner.guid;
    auction->ownerAccount = owner.account;
    auction->buyout = policy::Money(uint64(unitValue) * count);
    auction->startbid = std::max<uint32>(1, uint64(auction->buyout) * urand(settings.bidMin, settings.bidMax) / 100);
    auction->depositTime = time(nullptr);
    auction->expireTime = auction->depositTime + urand(settings.timeMin, settings.timeMax) * HOUR;
    auction->auctionHouseEntry = entry;
    CharacterDatabase.BeginTransaction();
    item->SaveToDB();
    auction->SaveToDB();
    CharacterDatabase.CommitTransaction();
    sAuctionMgr.AddAItem(item.release());
    house->AddAuction(auction.release());
    return true;
}
void AhBot::ScanOne(bool expire)
{
    if (scanHouse >= 3)
    {
        rebuildTotal = rebuildRemaining = policy::RebuildCycles(settings.timeMin, settings.timeMax);
        action = 5;
        sLog.outString("[AhBot] Expired %llu bot listings; protected %llu bids. Refilling %u CMaNGOS steps.", expired,
                       protectedBids, rebuildTotal);
        phase = Phase::Idle;
        return;
    }
    auto house = House(scanHouse);
    if (expire && house)
        for (uint32 i = 0; i < scanHouse; ++i)
            if (House(i) == house)
            {
                ++scanHouse;
                scanCursor = 0;
                return;
            }
    if (pageIndex >= page.size())
    {
        page = house ? house->GetAuctionsSnapshotPage(scanCursor, 32) : std::vector<AuctionSnapshot>{};
        pageIndex = 0;
        if (page.empty())
        {
            if (expire)
            {
                ++scanHouse;
                scanCursor = 0;
            }
            else
                FinishPass();
            return;
        }
    }
    AuctionSnapshot snapshot = page[pageIndex++];
    scanCursor = snapshot.Id;
    if (!expire)
    {
        Buy(snapshot);
        return;
    }
    AuctionHouseObject::Guard lock(house->GetLock());
    auto auction = house->GetAuction(snapshot.Id);
    if (!auction || auction->itemGuidLow != snapshot.itemGuidLow || !IsBotOwner(auction->owner, auction->ownerAccount))
        return;
    if (!includeBids && auction->bid)
    {
        ++protectedBids;
        return;
    }
    house->ExpireAuction(auction);
    ++expired;
}
void AhBot::Buy(AuctionSnapshot const& snapshot)
{
    auto house = House(currentHouse);
    if (!house || owners.empty())
        return;
    AuctionHouseObject::Guard lock(house->GetLock());
    auto auction = house->GetAuction(snapshot.Id);
    if (!auction || auction->itemGuidLow != snapshot.itemGuidLow || auction->expireTime <= time(nullptr) ||
        (!auction->lockedIpAddress.empty() && auction->depositTime + 300 >= time(nullptr)))
        return;
    if (IsBotOwner(auction->owner, auction->ownerAccount) && !auction->bid)
        return;
    Owner bidder = owners[urand(0, owners.size() - 1)];
    if (!IsBotOwner(bidder.guid, bidder.account) || IsPlayerHardcore(bidder.guid) ||
        sObjectMgr.GetPlayerAccountIdByGUID(bidder.guid) != bidder.account || auction->ownerAccount == bidder.account ||
        (auction->bidder && IsBotOwner(auction->bidder, sObjectMgr.GetPlayerAccountIdByGUID(auction->bidder))))
        return;
    auto item = sAuctionMgr.GetAItem(auction->itemGuidLow);
    if (!item)
        return;
    uint64 value = uint64(Varied(Price(item->GetProto()))) * item->GetCount() * settings.buyValue / 100;
    uint32 bid = policy::Bid(auction->startbid, auction->bid, auction->GetAuctionOutBid(), auction->buyout, value);
    if (!bid)
        return;
    if (auction->bidder)
        sAuctionMgr.SendAuctionOutbiddedMail(auction);
    auction->bidder = bidder.guid;
    auction->bid = bid;
    if (auction->buyout && bid == auction->buyout)
        house->ExpireAuction(auction);
    else
    {
        CharacterDatabase.PExecute("UPDATE auction SET buyguid='%u',lastbid='%u' WHERE id='%u'", auction->bidder,
                                   auction->bid, auction->Id);
        if (auto owner = sObjectMgr.GetPlayer(ObjectGuid(HIGHGUID_PLAYER, auction->owner)))
            owner->GetSession()->SendAuctionOwnerNotification(auction, false);
    }
    ++bought;
}
void AhBot::FinishPass()
{
    phase = Phase::Idle;
    nextCheck = time(nullptr) + 20;
    if (rebuildTotal && !rebuildRemaining)
    {
        sLog.outString("[AhBot] Rebuild complete: listed=%llu failed=%llu loot rolls=%llu. No category caps.", listed,
                       failed, lootRolls);
        rebuildTotal = 0;
    }
}
void AhBot::Status(ChatHandler* h, bool detailed)
{
    char const* phases[] = {"idle", "rolling loot/crafts", "overrides", "listing", "buying", "expiring"};
    h->PSendSysMessage("AHBot CMaNGOS: %s, %s; rebuild remaining %u/%u, queued %u.",
                       settings.enabled ? "enabled" : "disabled", phases[uint32(phase)], rebuildRemaining, rebuildTotal,
                       pendingRebuild);
    h->PSendSysMessage(
        "Listed %llu, bought/bid %llu, expired %llu, protected bids %llu, failed %llu; slice last/max %llu/%llu us.",
        listed, bought, expired, protectedBids, failed, lastSliceUs, maxSliceUs);
    for (uint32 n = 0; n < 3; ++n)
    {
        auto house = House(n);
        bool duplicate = false;
        for (uint32 i = 0; i < n; ++i)
            if (House(i) == house)
                duplicate = true;
        if (house && !duplicate)
            h->PSendSysMessage("AH %u: %u total auctions (shared markets counted once).", auctionIds[n],
                               house->GetCount());
    }
    if (detailed)
    {
        h->PSendSysMessage("Sell/buy %u/%u%% every 20s, duration %u-%uh, level caps %u/%u; pending stock items %zu. No "
                           "category/per-item listing ceilings.",
                           settings.sell, settings.buy, settings.timeMin, settings.timeMax, maxRequiredLevel,
                           maxItemLevel, stock.size());
        for (size_t i = 0; i < sources.size(); ++i)
            h->PSendSysMessage("Source %zu: %zu templates; %d,%d,%d,%d", i, sources[i].ids.size(), sources[i].range[0],
                               sources[i].range[1], sources[i].range[2], sources[i].range[3]);
    }
}
bool AhBot::HandleCommand(ChatHandler* h, std::string command)
{
    std::istringstream in(command);
    std::string name, option, extra;
    in >> name;
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return char(std::tolower(c)); });
    if (name == "item")
    {
        std::string rest;
        std::getline(in, rest);
        return ItemCommand(h, rest);
    }
    in >> option >> extra;
    if (name == "status" || name == "stats")
    {
        if ((!option.empty() && option != "all") || !extra.empty())
            return false;
        Status(h, option == "all");
        return true;
    }
    if (name == "reload")
    {
        if (!option.empty())
            return false;
        if (phase != Phase::Idle || pendingRebuild || rebuildRemaining)
        {
            h->SendSysMessage("AHBot busy; reload after accepted work finishes. Check .ahbot status.");
            return false;
        }
        bool ok = Load();
        h->SendSysMessage(ok ? "AHBot CMaNGOS settings reloaded."
                             : "AHBot reload failed; previous settings retained. Check the log.");
        return ok;
    }
    if (name == "rebuild" || name == "expire")
    {
        if ((!option.empty() && option != "all") || !extra.empty())
            return false;
        if (!settings.enabled)
        {
            h->SendSysMessage("AHBot disabled; reload a valid configuration first.");
            return false;
        }
        if (phase == Phase::Expire || rebuildTotal)
        {
            h->SendSysMessage("AHBot rebuild already active; not restarting it.");
            return true;
        }
        pendingRebuild = std::max<uint32>(pendingRebuild, option == "all" ? 2 : 1);
        h->SendSysMessage("AHBot rebuild queued. Native expiry/CMaNGOS refill proceed in bounded slices; bids "
                          "protected unless 'all' requested.");
        return true;
    }
    if (name == "update" && option.empty())
    {
        nextCheck = 0;
        h->SendSysMessage("AHBot next normal market check requested.");
        return true;
    }
    h->SendSysMessage(".ahbot reload | rebuild [all] | status [all] | update | item <id> [value [chance [min [max]]]] "
                      "| item <id> reset");
    return false;
}
bool AhBot::ItemCommand(ChatHandler* h, std::string const& args)
{
    std::istringstream in(args);
    uint32 id = 0;
    std::string token;
    if (!(in >> id))
        return false;
    auto p = sObjectMgr.GetItemPrototype(id);
    if (!p)
        return false;
    if (!(in >> token))
    {
        auto it = overrides.find(id);
        h->PSendSysMessage("AHBot item %u: value %u copper; override %s.", id, Price(p),
                           it != overrides.end() ? "yes" : "no");
        if (it != overrides.end())
            h->PSendSysMessage("Chance %u%%, quantity %u-%u split into legal stacks.", it->second.chance,
                               it->second.min, it->second.max);
        return true;
    }
    if (phase != Phase::Idle || pendingRebuild || rebuildRemaining)
    {
        h->SendSysMessage("Wait for AHBot work to finish before editing overrides.");
        return false;
    }
    if (token == "reset")
    {
        if (in >> token)
            return false;
        CharacterDatabase.PExecute("DELETE FROM ahbot_items WHERE item='%u'", id);
        overrides.erase(id);
        h->SendSysMessage("AHBot item override reset.");
        return true;
    }
    std::string rest;
    std::getline(in, rest);
    std::vector<int64> values;
    if (!ReadNumbers(token + rest, values) || values.empty() || values.size() > 4)
        return false;
    for (int64 v : values)
        if (v < 0 || v > policy::MoneyLimit)
            return false;
    Override d;
    d.value = uint32(values[0]);
    d.chance = values.size() > 1 ? std::min<uint32>(100, values[1]) : 0;
    d.min = values.size() > 2 && values[2] ? uint32(values[2]) : std::max<uint32>(1, p->Stackable);
    d.max = values.size() > 3 && values[3] ? uint32(values[3]) : d.min;
    d.max = std::max(d.min, d.max);
    CharacterDatabase.PExecute(
        "REPLACE INTO ahbot_items (item,value,add_chance,min_amount,max_amount) VALUES ('%u','%u','%u','%u','%u')", id,
        d.value, d.chance, d.min, d.max);
    overrides[id] = d;
    h->SendSysMessage("AHBot override saved; value 0 bans buying/new stock. Quantities may span multiple stacks.");
    return true;
}
// End market command implementation.
