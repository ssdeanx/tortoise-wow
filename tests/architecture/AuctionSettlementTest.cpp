// Production expiry, refund and paged-snapshot functions, isolated from the
// database/network. Covers native hook ordering and single ownership cleanup.
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>
using uint32=uint32_t;
#define CHECK(x) do{if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);}}while(0)
std::vector<std::string> calls;
constexpr uint32 HIGHGUID_PLAYER=0,AUCTION_OUTBIDDED=0,MAIL_CHECK_MASK_COPIED=0;
struct ObjectGuid{uint32 id;ObjectGuid(uint32,uint32 n):id(n){}};
struct AuctionHouseEntry{uint32 houseId=1;};
struct AuctionEntry{
    uint32 Id=0,itemGuidLow=0,itemTemplate=0,owner=0,ownerAccount=0,startbid=0,bid=0,buyout=0,bidder=0,itemCount=0;
    time_t expireTime=0;AuctionHouseEntry const* auctionHouseEntry=nullptr;
    void DeleteFromDB(){calls.push_back("delete row");}
    ~AuctionEntry(){calls.push_back("delete entry");}
};
struct AuctionSnapshot{uint32 Id,itemGuidLow,itemTemplate,owner,ownerAccount,startbid,bid,buyout,bidder,houseId,itemCount;time_t expireTime;};
struct Session{void SendAuctionBidderNotification(AuctionEntry*,bool){calls.push_back("notify old bidder");}};
struct Player{Session session;bool hardcore=false;bool IsHardcore(){return hardcore;}Session* GetSession(){return &session;}};
struct ObjectMgr{Player* online=nullptr;uint32 account=20;Player* GetPlayer(ObjectGuid){return online;}uint32 GetPlayerAccountIdByGUID(ObjectGuid){return account;}}sObjectMgr;
bool offlineHardcore=false;bool IsPlayerHardcore(uint32){return offlineHardcore;}
struct MailReceiver{MailReceiver(Player*,ObjectGuid){}};
struct MailDraft{
    uint32 money=0;MailDraft(std::string){}
    MailDraft& SetMoney(uint32 n){money=n;return *this;}
    void SendMailTo(MailReceiver,AuctionEntry* a,uint32){CHECK(money==a->bid);calls.push_back("refund");}
};
struct Item{uint32 GetCount(){return 20;}};
struct AuctionHouseMgr{
    Item item;
    void SendAuctionOutbiddedMail(AuctionEntry*);
    void SendAuctionExpiredMail(AuctionEntry*){calls.push_back("expired mail");}
    void SendAuctionSuccessfulMail(AuctionEntry*){calls.push_back("seller mail");}
    void SendAuctionWonMail(AuctionEntry*){calls.push_back("winner mail");}
    void RemoveAItem(uint32){calls.push_back("remove item index");}
    Item* GetAItem(uint32){return &item;}
}sAuctionMgr;
struct PlayerTransactionData{
    std::string type;struct Part{uint32 lowGuid,itemsEntries[1],itemsCount[1],itemsGuid[1],money;}parts[2];
};
struct AuctionHouseObject{
    using Guard=std::lock_guard<std::recursive_mutex>;mutable std::recursive_mutex m_auctionsLock;
    std::map<uint32,AuctionEntry*> AuctionsMap;
    AuctionEntry* GetAuction(uint32 id){auto it=AuctionsMap.find(id);return it==AuctionsMap.end()?nullptr:it->second;}
    void RemoveAuction(AuctionEntry* e){AuctionsMap.erase(e->Id);calls.push_back("remove auction indexes");}
    void ExpireAuction(AuctionEntry*);
    std::vector<AuctionSnapshot> GetAuctionsSnapshotPage(uint32,uint32)const;
};
struct AuctionHouseScript{
    void OnAuctionExpire(AuctionHouseObject*,AuctionEntry*){calls.push_back("expire hook");}
    void OnAuctionSuccessful(AuctionHouseObject*,AuctionEntry*){calls.push_back("sale hook");}
};
template<class T>struct ScriptRegistry{template<class F>static void ForEach(F f){T script;f(&script);}};
#include "NativeMarketExpiry.inc"
#include "NativeMarketRefund.inc"
#include "NativeMarketPage.inc"
int main(){
    AuctionHouseObject h;
    auto a=new AuctionEntry{};a->Id=1;h.AuctionsMap[1]=a;h.ExpireAuction(a);
    CHECK((calls==std::vector<std::string>{"expire hook","expired mail","delete row","remove item index","remove auction indexes","delete entry"}));
    CHECK(h.AuctionsMap.empty());calls.clear();
    a=new AuctionEntry{};a->Id=2;a->bid=500;a->bidder=10;h.AuctionsMap[2]=a;h.ExpireAuction(a);
    CHECK((calls==std::vector<std::string>{"sale hook","seller mail","winner mail","delete row","remove item index","remove auction indexes","delete entry"}));
    calls.clear();AuctionEntry refund{};refund.bid=500;refund.bidder=10;
    sAuctionMgr.SendAuctionOutbiddedMail(&refund);CHECK((calls==std::vector<std::string>{"refund"}));
    calls.clear();offlineHardcore=true;sAuctionMgr.SendAuctionOutbiddedMail(&refund);CHECK(calls.empty());offlineHardcore=false;
    sObjectMgr.account=0;sAuctionMgr.SendAuctionOutbiddedMail(&refund);CHECK(calls.empty());
    Player bidder;sObjectMgr.online=&bidder;sAuctionMgr.SendAuctionOutbiddedMail(&refund);
    CHECK((calls==std::vector<std::string>{"notify old bidder","refund"}));calls.clear();
    bidder.hardcore=true;sAuctionMgr.SendAuctionOutbiddedMail(&refund);CHECK(calls.empty());
    for(uint32 i=1;i<=100;++i){a=new AuctionEntry{};a->Id=i;a->itemGuidLow=1000+i;h.AuctionsMap[i]=a;}
    auto page=h.GetAuctionsSnapshotPage(0,32);CHECK(page.size()==32);CHECK(page.front().Id==1);CHECK(page.back().Id==32);
    delete h.AuctionsMap[33];h.AuctionsMap.erase(33);auto next=h.GetAuctionsSnapshotPage(32,32);CHECK(next.front().Id==34);CHECK(next.size()==32);
    CHECK(h.GetAuctionsSnapshotPage(0,0).empty());CHECK(h.GetAuctionsSnapshotPage(100,32).empty());
    CHECK(page.front().itemGuidLow==1001); // copied value survives source removal
    for(auto p:h.AuctionsMap)delete p.second;
    std::cout<<"Native auction expiry/refund/paging tests passed\n";
}
