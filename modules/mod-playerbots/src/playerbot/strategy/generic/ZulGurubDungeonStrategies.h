#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class ZulGurubDungeonStrategy : public Strategy
    {
    public:
        ZulGurubDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "zul'gurub"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Shared ZG boss-fight lifecycle: potions + consumables enabled for the
    // fight, restored when combat stops.
    class ZulGurubFightStrategy : public Strategy
    {
    public:
        ZulGurubFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}

        void OnStrategyAdded(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
                OnRaidFightStrategyAddedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
                OnRaidFightStrategyRemovedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
        }

    protected:
        RaidFightBotState raidState_;
    };

    // Same lifecycle plus the dropper shaman's Nature Resistance Totem - the
    // nature fights (Venoxis' P2 poison, Mar'li's poison volley / corrosive
    // poison).
    class ZulGurubNatureFightStrategy : public ZulGurubFightStrategy
    {
    public:
        ZulGurubNatureFightStrategy(PlayerbotAI* ai) : ZulGurubFightStrategy(ai) {}

        void OnStrategyAdded(BotState state) override
        {
            ZulGurubFightStrategy::OnStrategyAdded(state);
            if (state == BotState::BOT_STATE_COMBAT)
                OnRaidFightStrategyAddedNatureTotem(ai, ai->GetBot(), raidState_);
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
                OnRaidFightStrategyRemovedNatureTotem(ai, ai->GetBot(), raidState_);
            ZulGurubFightStrategy::OnStrategyRemoved(state);
        }
    };

    class JeklikFightStrategy : public ZulGurubFightStrategy
    {
    public:
        JeklikFightStrategy(PlayerbotAI* ai) : ZulGurubFightStrategy(ai) {}
        std::string getName() override { return "jeklik"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class VenoxisFightStrategy : public ZulGurubNatureFightStrategy
    {
    public:
        VenoxisFightStrategy(PlayerbotAI* ai) : ZulGurubNatureFightStrategy(ai) {}
        std::string getName() override { return "venoxis"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class MarliFightStrategy : public ZulGurubNatureFightStrategy
    {
    public:
        MarliFightStrategy(PlayerbotAI* ai) : ZulGurubNatureFightStrategy(ai) {}
        std::string getName() override { return "marli"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class MandokirFightStrategy : public ZulGurubFightStrategy
    {
    public:
        MandokirFightStrategy(PlayerbotAI* ai) : ZulGurubFightStrategy(ai) {}
        std::string getName() override { return "mandokir"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class ThekalFightStrategy : public ZulGurubFightStrategy
    {
    public:
        ThekalFightStrategy(PlayerbotAI* ai) : ZulGurubFightStrategy(ai) {}
        std::string getName() override { return "thekal"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class ArlokkFightStrategy : public ZulGurubFightStrategy
    {
    public:
        ArlokkFightStrategy(PlayerbotAI* ai) : ZulGurubFightStrategy(ai) {}
        std::string getName() override { return "arlokk"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class JindoFightStrategy : public ZulGurubFightStrategy
    {
    public:
        JindoFightStrategy(PlayerbotAI* ai) : ZulGurubFightStrategy(ai) {}
        std::string getName() override { return "jindo"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class HakkarFightStrategy : public ZulGurubFightStrategy
    {
    public:
        HakkarFightStrategy(PlayerbotAI* ai) : ZulGurubFightStrategy(ai) {}
        std::string getName() override { return "hakkar"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };
}
