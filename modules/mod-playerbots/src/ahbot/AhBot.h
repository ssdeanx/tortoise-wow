#pragma once
#include "AuctionHouse/AuctionHouseMgr.h"
#include "Common.h"
#include "MarketPolicy.h"
#include <array>
#include <map>
#include <set>
#include <vector>
class ChatHandler;
class ItemPrototype;
class LootStore;
namespace ahbot
{
// One CMaNGOS-policy supplier/buyer on the joined world owner. No detached
// worker, live pointer across slices, or second legacy auction engine.
class AhBot
{
  public:
    static AhBot& instance()
    {
        static AhBot bot;
        return bot;
    }
    static bool HandleAhBotCommand(ChatHandler*, char const*);
    void Init();
    void Update();
    bool HandleCommand(ChatHandler*, std::string);
    static uint32 auctionIds[3];

  private:
    struct Override
    {
        uint32 value = 0, chance = 0, min = 0, max = 0;
    };
    struct Source
    {
        LootStore const* store = nullptr;
        std::array<int32, 4> range{};
        std::vector<uint32> ids;
    };
    struct Owner
    {
        uint32 guid, account;
    };
    struct Settings
    {
        bool enabled = false, vendorValue = true, dynamicLevel = false, ignoreGm = false;
        uint32 sell = 10, buy = 10, variance = 10, bidMin = 75, bidMax = 90;
        uint32 timeMin = 2, timeMax = 24, buyValue = 90, requiredLevel = 60;
        uint32 levelRefresh = 600, sliceUs = 2000, sliceOperations = 32;
        std::array<std::array<uint32, 17>, 7> values{};
    } settings;
    enum class Phase
    {
        Idle,
        Gather,
        Overrides,
        Post,
        Buy,
        Expire
    } phase = Phase::Idle;
    bool Load();
    void RefreshLevel();
    void Step();
    void GatherOne();
    void PostOne();
    void ScanOne(bool expire);
    void FinishPass();
    bool IsBotOwner(uint32 guid, uint32 account) const;
    uint32 Price(ItemPrototype const*) const;
    uint32 Varied(uint32) const;
    bool Eligible(ItemPrototype const*, bool forced) const;
    bool Publish(uint32 item, uint32 count, uint32 price);
    void Buy(AuctionSnapshot const&);
    bool ItemCommand(ChatHandler*, std::string const&);
    void Status(ChatHandler*, bool detailed);
    std::vector<Source> sources;
    std::vector<Owner> owners;
    std::set<uint32> vendorItems;
    std::map<uint32, Override> overrides;
    std::map<uint32, uint64> stock;
    std::vector<AuctionSnapshot> page;
    size_t pageIndex = 0, sourceIndex = 0;
    int32 picksRemaining = -1;
    uint32 rollsRemaining = 0, selectedTemplate = 0, overrideCursor = 0;
    uint32 currentHouse = 0, action = 5, scanHouse = 0, scanCursor = 0;
    uint32 maxRequiredLevel = 60, maxItemLevel = 255;
    uint32 rebuildRemaining = 0, rebuildTotal = 0, pendingRebuild = 0;
    bool includeBids = false;
    uint64 listed = 0, bought = 0, expired = 0, protectedBids = 0, failed = 0;
    uint64 lootRolls = 0, lastSliceUs = 0, maxSliceUs = 0;
    time_t nextCheck = 0, nextLevelCheck = 0;
};
} // namespace ahbot
#define auctionbot ahbot::AhBot::instance()
