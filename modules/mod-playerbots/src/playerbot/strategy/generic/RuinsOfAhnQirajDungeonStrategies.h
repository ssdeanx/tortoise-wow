#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class RuinsOfAhnQirajDungeonStrategy : public Strategy
    {
    public:
        RuinsOfAhnQirajDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "ruins of ahnqiraj"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Shared AQ20 boss-fight lifecycle: potions + consumables enabled for the
    // fight, restored when combat stops.
    class RuinsOfAhnQirajFightStrategy : public Strategy
    {
    public:
        RuinsOfAhnQirajFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}

        void OnStrategyAdded(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyAddedNatureTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyAddedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyRemovedNatureTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyRemovedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

    protected:
        RaidFightBotState raidState_;
    };

    class KurinnaxxFightStrategy : public RuinsOfAhnQirajFightStrategy
    {
    public:
        KurinnaxxFightStrategy(PlayerbotAI* ai) : RuinsOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "kurinnaxx"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class RajaxxFightStrategy : public RuinsOfAhnQirajFightStrategy
    {
    public:
        RajaxxFightStrategy(PlayerbotAI* ai) : RuinsOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "rajaxx"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class BuruFightStrategy : public RuinsOfAhnQirajFightStrategy
    {
    public:
        BuruFightStrategy(PlayerbotAI* ai) : RuinsOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "buru"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class MoamFightStrategy : public RuinsOfAhnQirajFightStrategy
    {
    public:
        MoamFightStrategy(PlayerbotAI* ai) : RuinsOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "moam"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class AyamissFightStrategy : public RuinsOfAhnQirajFightStrategy
    {
    public:
        AyamissFightStrategy(PlayerbotAI* ai) : RuinsOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "ayamiss"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class OssirianFightStrategy : public RuinsOfAhnQirajFightStrategy
    {
    public:
        OssirianFightStrategy(PlayerbotAI* ai) : RuinsOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "ossirian"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };
}
