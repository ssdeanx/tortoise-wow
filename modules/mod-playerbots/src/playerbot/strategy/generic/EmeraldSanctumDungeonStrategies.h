#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class EmeraldSanctumDungeonStrategy : public Strategy
    {
    public:
        EmeraldSanctumDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "emerald sanctum"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Erennius (60747) fight: Acid Breath, Curse of Erennius.
    class ErenniusFightStrategy : public Strategy
    {
    public:
        ErenniusFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "erennius"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // Solnius (60748) fight: Acid Breath, Corrosive Bolt, adds waves.
    class SolniusFightStrategy : public Strategy
    {
    public:
        SolniusFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "solnius"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };
}
