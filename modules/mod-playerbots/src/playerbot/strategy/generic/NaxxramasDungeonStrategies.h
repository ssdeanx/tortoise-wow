#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class NaxxramasDungeonStrategy : public Strategy
    {
    public:
        NaxxramasDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "naxxramas"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Shared Naxxramas boss-fight lifecycle: potions + consumables enabled for
    // the fight, restored when combat stops.
    class NaxxramasFightStrategy : public Strategy
    {
    public:
        NaxxramasFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}

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

    class AnubRekhanFightStrategy : public NaxxramasFightStrategy
    {
    public:
        AnubRekhanFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "anubrekhan"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class FaerlinaFightStrategy : public NaxxramasFightStrategy
    {
    public:
        FaerlinaFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "faerlina"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class MaexxnaFightStrategy : public NaxxramasFightStrategy
    {
    public:
        MaexxnaFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "maexxna"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class NothFightStrategy : public NaxxramasFightStrategy
    {
    public:
        NothFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "noth"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class HeiganFightStrategy : public NaxxramasFightStrategy
    {
    public:
        HeiganFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "heigan"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class LoathebFightStrategy : public NaxxramasFightStrategy
    {
    public:
        LoathebFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "loatheb"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class GrobbulusFightStrategy : public NaxxramasFightStrategy
    {
    public:
        GrobbulusFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "grobbulus"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class GluthFightStrategy : public NaxxramasFightStrategy
    {
    public:
        GluthFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "gluth"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class ThaddiusFightStrategy : public NaxxramasFightStrategy
    {
    public:
        ThaddiusFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "thaddius"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class SapphironFightStrategy : public NaxxramasFightStrategy
    {
    public:
        SapphironFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "sapphiron"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class KelThuzadFightStrategy : public NaxxramasFightStrategy
    {
    public:
        KelThuzadFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "kelthuzad"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class FourHorsemanFightStrategy : public NaxxramasFightStrategy
    {
    public:
        FourHorsemanFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "four horseman"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };
}
