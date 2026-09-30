#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class ScarletCitadelDungeonStrategy : public Strategy
    {
    public:
        ScarletCitadelDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "scarlet citadel"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // ---------------------------------------------------------------------------
    // Boss 1: Sacred Fist Daelus (2000000)
    // ---------------------------------------------------------------------------
    class DaelusFightStrategy : public Strategy
    {
    public:
        DaelusFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "daelus"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // ---------------------------------------------------------------------------
    // Boss 2: Grand Sorcerer Ardaeus (2000001)
    // ---------------------------------------------------------------------------
    class ArdaeusFightStrategy : public Strategy
    {
    public:
        ArdaeusFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "ardaeus"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // ---------------------------------------------------------------------------
    // Boss 3: High Inquisitor Mariella (2000002)
    // ---------------------------------------------------------------------------
    class MariellaFightStrategy : public Strategy
    {
    public:
        MariellaFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "mariella"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };

    // ---------------------------------------------------------------------------
    // Mini-Boss: Brother Eric Vesper (2000004)
    // ---------------------------------------------------------------------------
    class EricVesperFightStrategy : public Strategy
    {
    public:
        EricVesperFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "eric vesper"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // ---------------------------------------------------------------------------
    // Mini-Boss: Darkcaller Rayn (2000037)
    // ---------------------------------------------------------------------------
    class DarkcallerRaynFightStrategy : public Strategy
    {
    public:
        DarkcallerRaynFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "darkcaller rayn"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // ---------------------------------------------------------------------------
    // Boss 4: High General Abbendis (2000003)
    // ---------------------------------------------------------------------------
    class AbbendisFightStrategy : public Strategy
    {
    public:
        AbbendisFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "abbendis"; }

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

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;

        RaidFightBotState raidState_;
    };
}
