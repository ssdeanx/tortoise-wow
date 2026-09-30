#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class TempleOfAhnQirajDungeonStrategy : public Strategy
    {
    public:
        TempleOfAhnQirajDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "temple of ahnqiraj"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Shared AQ40 boss-fight lifecycle: potions + consumables enabled for the
    // fight, restored when combat stops.
    class TempleOfAhnQirajFightStrategy : public Strategy
    {
    public:
        TempleOfAhnQirajFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}

        void OnStrategyAdded(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyAddedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyRemovedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

    protected:
        RaidFightBotState raidState_;
    };

    // AQ40 nature fights (Bug Trio, Viscidus, Huhuran): the dropper shaman keeps
    // the Nature Resistance Totem down for the group.
    class TempleOfAhnQirajNatureFightStrategy : public TempleOfAhnQirajFightStrategy
    {
    public:
        TempleOfAhnQirajNatureFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajFightStrategy(ai) {}

        void OnStrategyAdded(BotState state) override
        {
            TempleOfAhnQirajFightStrategy::OnStrategyAdded(state);
            if (state == BotState::BOT_STATE_COMBAT)
                OnRaidFightStrategyAddedNatureTotem(ai, ai->GetBot(), raidState_);
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
                OnRaidFightStrategyRemovedNatureTotem(ai, ai->GetBot(), raidState_);
            TempleOfAhnQirajFightStrategy::OnStrategyRemoved(state);
        }
    };

    class SkeramFightStrategy : public TempleOfAhnQirajFightStrategy
    {
    public:
        SkeramFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "skeram"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class BugTrioFightStrategy : public TempleOfAhnQirajNatureFightStrategy
    {
    public:
        BugTrioFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajNatureFightStrategy(ai) {}
        std::string getName() override { return "bug trio"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class SarturaFightStrategy : public TempleOfAhnQirajFightStrategy
    {
    public:
        SarturaFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "sartura"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class FankrissFightStrategy : public TempleOfAhnQirajFightStrategy
    {
    public:
        FankrissFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "fankriss"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class ViscidusFightStrategy : public TempleOfAhnQirajNatureFightStrategy
    {
    public:
        ViscidusFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajNatureFightStrategy(ai) {}
        std::string getName() override { return "viscidus"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class HuhuranFightStrategy : public TempleOfAhnQirajNatureFightStrategy
    {
    public:
        HuhuranFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajNatureFightStrategy(ai) {}
        std::string getName() override { return "huhuran"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class TwinsFightStrategy : public TempleOfAhnQirajFightStrategy
    {
    public:
        TwinsFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "twins"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class OuroFightStrategy : public TempleOfAhnQirajFightStrategy
    {
    public:
        OuroFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "ouro"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class CthunFightStrategy : public TempleOfAhnQirajFightStrategy
    {
    public:
        CthunFightStrategy(PlayerbotAI* ai) : TempleOfAhnQirajFightStrategy(ai) {}
        std::string getName() override { return "cthun"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };
}
