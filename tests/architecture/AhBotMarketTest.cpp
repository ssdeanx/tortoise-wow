// Executes the production market scheduler, loot/craft gathering, listing,
// buying and commands. Native services are deterministic test doubles.
#include <algorithm>
#include <array>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "MarketPolicy.h"
#include "WorkSlice.h"
using uint32=uint32_t; using int32=int32_t; using uint64=uint64_t; using int64=int64_t;
#define CHECK(x) do { if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);} }while(0)
constexpr uint32 HIGHGUID_PLAYER=0, HOUR=3600, BIND_WHEN_PICKED_UP=1, BIND_QUEST_ITEM=4, ITEM_FLAG_HAS_LOOT=4;
uint32 randomValue=0; uint64 clockNow=0;
uint32 urand(uint32 min,uint32 max){CHECK(min<=max);return min+randomValue%(max-min+1);}
uint64 ClockUs(){return clockNow++;}
struct ObjectGuid { uint32 low; ObjectGuid(uint32,uint32 id):low(id){} };
class ItemPrototype {
public: uint32 ItemId=1,Quality=1,Class=0,Stackable=20,RequiredLevel=1,ItemLevel=1,Bonding=0,Flags=0,BuyPrice=100,SellPrice=25;
};
struct Session {void SendAuctionOwnerNotification(void*,bool){}};
class Player {public: Session session; Session* GetSession(){return &session;}};
struct Item {
    uint32 id,count,guid; static uint32 next; static bool fail;
    Item(uint32 i,uint32 c):id(i),count(c),guid(++next){}
    static Item* CreateItem(uint32 id,uint32 count){return fail?nullptr:new Item(id,count);}
    static int32 GenerateItemRandomPropertyId(uint32){return 0;}
    void SetOwnerGuid(ObjectGuid){} void SetItemRandomProperties(int32){} void ClearUpdateMask(bool){}
    uint32 GetGUIDLow()const{return guid;} uint32 GetCount()const{return count;}
    int32 GetItemRandomPropertyId()const{return 0;} void SaveToDB(){}
    ItemPrototype const* GetProto()const;
};
uint32 Item::next=0; bool Item::fail=false;
struct AuctionHouseEntry {uint32 houseId;};
struct AuctionEntry {
    uint32 Id=0,itemGuidLow=0,itemTemplate=0,owner=0,ownerAccount=0,startbid=0,bid=0,buyout=0,bidder=0,deposit=0;
    int32 itemRandomPropertyId=0;uint32 itemCount=1;time_t depositTime=0,expireTime=0;
    std::string lockedIpAddress;AuctionHouseEntry const* auctionHouseEntry=nullptr;
    uint32 GetAuctionOutBid()const{return bid?std::max<uint32>(1,bid/20):1;} void SaveToDB(){}
};
struct AuctionSnapshot {uint32 Id,itemGuidLow,itemTemplate,owner,ownerAccount,startbid,bid,buyout,bidder,houseId,itemCount;time_t expireTime;};
struct AuctionHouseObject {
    using Guard=std::lock_guard<std::recursive_mutex>;
    mutable std::recursive_mutex mutex; std::map<uint32,AuctionEntry*> entries; uint32 expired=0;
    ~AuctionHouseObject(){for(auto const& p:entries)delete p.second;}
    std::recursive_mutex& GetLock()const{return mutex;}
    AuctionEntry* GetAuction(uint32 id){auto i=entries.find(id);return i==entries.end()?nullptr:i->second;}
    void AddAuction(AuctionEntry* a){CHECK(a->ownerAccount==20);CHECK(a->deposit==0);entries[a->Id]=a;}
    void ExpireAuction(AuctionEntry* a){++expired;entries.erase(a->Id);delete a;}
    std::vector<AuctionSnapshot> GetAuctionsSnapshotPage(uint32 after,uint32 limit){
        std::vector<AuctionSnapshot> out;
        for(auto i=entries.upper_bound(after);i!=entries.end()&&out.size()<limit;++i){auto a=i->second;out.push_back({a->Id,a->itemGuidLow,a->itemTemplate,a->owner,a->ownerAccount,a->startbid,a->bid,a->buyout,a->bidder,a->auctionHouseEntry->houseId,a->itemCount,a->expireTime});}return out;
    }
};
AuctionHouseObject market; AuctionHouseEntry houseEntry{1};
AuctionHouseObject* House(uint32){return &market;}
struct HouseStore {AuctionHouseEntry const* LookupEntry(uint32){return &houseEntry;}}sAuctionHouseStore;
struct Manager {
    std::map<uint32,Item*> items;uint32 refunds=0;
    ~Manager(){for(auto const& p:items)delete p.second;}
    void AddAItem(Item* i){items[i->guid]=i;}
    Item* GetAItem(uint32 id){auto i=items.find(id);return i==items.end()?nullptr:i->second;}
    void SendAuctionOutbiddedMail(AuctionEntry*){++refunds;}
}sAuctionMgr;
struct ObjectManager {
    std::map<uint32,ItemPrototype> protos;uint32 next=0;
    ItemPrototype const* GetItemPrototype(uint32 id){auto i=protos.find(id);return i==protos.end()?nullptr:&i->second;}
    uint32 GetPlayerAccountIdByGUID(uint32 id){return id==10?20:30;}
    uint32 GenerateAuctionID(){return ++next;}
    Player* GetPlayer(ObjectGuid){return nullptr;}
}sObjectMgr;
ItemPrototype const* Item::GetProto()const{return sObjectMgr.GetItemPrototype(id);}
bool IsPlayerHardcore(uint32){return false;}
struct BotConfig{bool IsInRandomAccountList(uint32 account)const{return account==20;}}sPlayerbotAIConfig;
struct World{bool stopped=false;bool IsStopped(){return stopped;}bool IsShutdowning(){return false;}}sWorld;
struct Database {
    uint32 writes=0;void BeginTransaction(){}void CommitTransaction(){}
    template<class... T> void PExecute(char const*,T...){++writes;}
}CharacterDatabase;
struct Log{template<class... T>void outString(char const*,T...){}}sLog;
struct LootItem {uint32 itemid,count;};
struct Loot{std::vector<LootItem> items;Loot(void*){}};
struct LootTemplate {
    mutable uint32 calls=0;uint32 item=1;
    template<class T>void Process(Loot& l,T const&,bool)const{++calls;CHECK(l.items.empty());l.items.push_back({item,1});}
};
class LootStore {
public:LootTemplate loot;bool missing=false;
    LootTemplate const* GetLootFor(uint32)const{return missing?nullptr:&loot;}
    bool IsRatesAllowed()const{return true;}
};
class ChatHandler {public:std::vector<std::string> messages;
    void SendSysMessage(char const* s){messages.push_back(s);}
    template<class... T>void PSendSysMessage(char const* s,T...){messages.push_back(s);}
};
#define private public
#include "NativeMarketDeclarations.inc"
#undef private
using ahbot::AhBot;
uint32 AhBot::auctionIds[3]={1,6,7};
void AhBot::RefreshLevel(){} bool AhBot::Load(){return true;} void AhBot::Status(ChatHandler*,bool){}
#include "NativeMarketParser.inc"
#include "NativeMarketWork.inc"
#include "NativeMarketCommands.inc"

void resetMarket(){for(auto p:market.entries)delete p.second;market.entries.clear();market.expired=0;}
AuctionEntry* addAuction(uint32 owner,uint32 account,uint32 bid=0){
    auto a=new AuctionEntry{};a->Id=sObjectMgr.GenerateAuctionID();a->owner=owner;a->ownerAccount=account;
    a->auctionHouseEntry=&houseEntry;a->expireTime=time(nullptr)+10000;a->bid=bid;a->bidder=bid?11:0;
    market.entries[a->Id]=a;return a;
}
int main(){
    using namespace ahbot::policy;
    std::vector<int64> parsed;
    CHECK(ReadNumbers("-10,2,1,1",parsed));CHECK(parsed.size()==4);CHECK(parsed[0]==-10);
    parsed.clear();CHECK(!ReadNumbers("100 9223372036854775808",parsed));
    parsed.clear();CHECK(!ReadNumbers("100 2oops",parsed));
    CHECK(Price(0,25,1,100)==100);CHECK(Price(1000,25,2,200)==250);
    CHECK(Price(100,25,1,0)==0);CHECK(Price(0,MoneyLimit,4,1000000)==MoneyLimit);
    CHECK(RebuildCycles(2,24)==1170);CHECK(ItemLevelCap(59)==64);CHECK(ItemLevelCap(60)==255);
    CHECK(Stack(55,20,100)==20);CHECK(Stack(50,20,MoneyLimit)==1);CHECK(Stack(1,1,0)==0);
    CHECK(Bid(100,0,1,0,200)==100);CHECK(Bid(100,200,10,0,210)==0);
    CHECK(Bid(100,200,10,250,300)==250);CHECK(Bid(1,MoneyLimit,10,0,UINT64_MAX)==0);
    sObjectMgr.protos.emplace(1,ItemPrototype{});
    AhBot b;b.settings.enabled=true;b.settings.values[1][0]=100;b.settings.variance=0;b.owners.push_back({10,20});
    auto& p=sObjectMgr.protos[1];CHECK(b.Eligible(&p,false));
    p.Bonding=BIND_WHEN_PICKED_UP;CHECK(!b.Eligible(&p,false));CHECK(b.Eligible(&p,true));p.Bonding=0;
    p.RequiredLevel=61;CHECK(!b.Eligible(&p,false));p.RequiredLevel=1;
    b.overrides[1]={0,100,20,20};CHECK(!b.Eligible(&p,true));b.overrides.clear();
    LootStore store;AhBot::Source source;source.store=&store;source.ids={9};source.range={1,1,50,50};
    b.sources={source};b.phase=AhBot::Phase::Gather;
    for(int i=0;i<55;++i)b.GatherOne();
    CHECK(store.loot.calls==50);CHECK(b.stock[1]==50); // exceeds one native loot-container capacity
    b.phase=AhBot::Phase::Post;while(!b.stock.empty())b.PostOne();
    CHECK(b.listed==3);uint32 sum=0;for(auto const& e:market.entries)sum+=e.second->itemCount;CHECK(sum==50);
    resetMarket();b.stock[1]=6000*20;uint64 before=b.listed;
    while(!b.stock.empty())b.PostOne();CHECK(b.listed-before==6000);CHECK(market.entries.size()==6000);
    b.stock[1]=40;Item::fail=true;b.PostOne();CHECK(b.failed==1);CHECK(b.stock[1]==20);Item::fail=false;b.stock.clear();
    resetMarket();auto protectedId=addAuction(10,20,100)->Id;auto humanId=addAuction(11,30)->Id;addAuction(10,20);
    b.phase=AhBot::Phase::Idle;b.settings.sell=0;b.settings.buy=100;ChatHandler chat;
    CHECK(b.HandleCommand(&chat,"rebuild"));CHECK(b.HandleCommand(&chat,"rebuild"));CHECK(b.pendingRebuild==1);
    b.Step();CHECK(b.phase==AhBot::Phase::Expire);CHECK(!b.HandleCommand(&chat,"reload"));
    for(int i=0;i<1500&&(b.phase!=AhBot::Phase::Idle||b.rebuildRemaining||i==0);++i)b.Step();
    CHECK(market.GetAuction(protectedId));CHECK(market.GetAuction(humanId));CHECK(market.entries.size()==2);
    CHECK(b.protectedBids==1);CHECK(b.expired==1);CHECK(b.bought==0);CHECK(b.rebuildTotal==0);
    CHECK(b.HandleCommand(&chat,"rebuild all"));b.Step();
    for(int i=0;i<1500&&(b.phase!=AhBot::Phase::Idle||b.rebuildRemaining||i==0);++i)b.Step();
    CHECK(!market.GetAuction(protectedId));CHECK(market.GetAuction(humanId));
    CHECK(b.HandleCommand(&chat,"item 1 100 100 41 61"));CHECK(b.overrides[1].min==41);CHECK(b.overrides[1].max==61);
    CHECK(!b.HandleCommand(&chat,"item 1 -1"));CHECK(!b.HandleCommand(&chat,"rebuild nonsense"));
    CHECK(b.HandleCommand(&chat,"item 1 reset"));CHECK(b.overrides.empty());
    // Native buyer: bid does not prematurely settle; buyout refunds then settles.
    resetMarket();auto a=addAuction(11,30,10);auto item=Item::CreateItem(1,2);sAuctionMgr.AddAItem(item);
    a->itemGuidLow=item->guid;a->itemTemplate=1;a->startbid=10;a->buyout=1000;
    b.settings.buyValue=100;auto snap=market.GetAuctionsSnapshotPage(0,1)[0];
    b.Buy(snap);CHECK(market.GetAuction(a->Id));CHECK(a->bidder==10);CHECK(a->bid==11);CHECK(sAuctionMgr.refunds==1);
    a->bidder=11;a->buyout=50;auto id=a->Id;b.Buy(snap);CHECK(!market.GetAuction(id));CHECK(sAuctionMgr.refunds==2);
    // Bound each tick even when a rebuild has unlimited remaining stock.
    b.phase=AhBot::Phase::Post;b.stock[1]=10000;b.settings.sliceOperations=2;b.settings.sliceUs=100000;
    before=b.listed;b.Update();CHECK(b.listed-before==2);CHECK(b.stock[1]>0);
    std::cout<<"Production CMaNGOS market policy/lifecycle tests passed\n";
}
