#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class BlackrockSpireDungeonStrategy : public Strategy
    {
    public:
        BlackrockSpireDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "blackrock spire"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Pyroguard Emberseer (9816) fight: fire damage, incarcerated release.
    class PyroguardEmberseerFightStrategy : public Strategy
    {
    public:
        PyroguardEmberseerFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "pyroguard emberseer"; }

        void OnStrategyAdded(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyAddedFireTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyAddedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyRemovedFireTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyRemovedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // Rend Blackhand (10429) & Gyth (10339) fight.
    class RendBlackhandFightStrategy : public Strategy
    {
    public:
        RendBlackhandFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "rend blackhand"; }

        void OnStrategyAdded(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyAddedFireTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyAddedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyRemovedFireTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyRemovedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // The Beast (10430) fight: fire knockback (Flamebreak) and fear (Terrifying Roar).
    class TheBeastFightStrategy : public Strategy
    {
    public:
        TheBeastFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "the beast"; }

        void OnStrategyAdded(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyAddedFireTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyAddedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyRemovedFireTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyRemovedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // General Drakkisath (10363) fight: Conflagration, Chromatic Elite adds.
    class GeneralDrakkisathFightStrategy : public Strategy
    {
    public:
        GeneralDrakkisathFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "general drakkisath"; }

        void OnStrategyAdded(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyAddedFireTotem(ai, ai->GetBot(), raidState_);
                OnRaidFightStrategyAddedConsumables(ai, raidState_.hadPotions, raidState_.hadConsumables);
            }
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
            {
                OnRaidFightStrategyRemovedFireTotem(ai, ai->GetBot(), raidState_);
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
