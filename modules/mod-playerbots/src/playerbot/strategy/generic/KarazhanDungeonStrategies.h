#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class KarazhanDungeonStrategy : public Strategy
    {
    public:
        KarazhanDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "karazhan"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class NetherspiteFightStrategy : public Strategy
    {
    public:
        NetherspiteFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "netherspite"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class PrinceMalchezaarFightStrategy : public Strategy
    {
    public:
        PrinceMalchezaarFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "prince malchezaar"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Lower Karazhan Halls: Brood Queen Araxxna (61221)
    class AraxxnaFightStrategy : public Strategy
    {
    public:
        AraxxnaFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "araxxna"; }

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

    // Lower Karazhan Halls: Clawlord Howlfang (61223)
    class HowlfangFightStrategy : public Strategy
    {
    public:
        HowlfangFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "howlfang"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // Lower Karazhan Halls: Lord Blackwald II (61222)
    class LordBlackwaldFightStrategy : public Strategy
    {
    public:
        LordBlackwaldFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "lord blackwald"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // Lower Karazhan Halls: Grizikil (61224)
    class GrizikilFightStrategy : public Strategy
    {
    public:
        GrizikilFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "grizikil"; }

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

    // Lower Karazhan Halls: Moroes (61225)
    class LkhMoroesFightStrategy : public Strategy
    {
    public:
        LkhMoroesFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "lkh moroes"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };
}