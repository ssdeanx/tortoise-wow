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

        // Frost Aura is raid-wide frost damage: the dropper shaman keeps the
        // Frost Resistance Totem down for the fight.
        void OnStrategyAdded(BotState state) override
        {
            NaxxramasFightStrategy::OnStrategyAdded(state);
            if (state == BotState::BOT_STATE_COMBAT)
                OnRaidFightStrategyAddedFrostTotem(ai, ai->GetBot(), raidState_);
        }

        void OnStrategyRemoved(BotState state) override
        {
            if (state == BotState::BOT_STATE_COMBAT)
                OnRaidFightStrategyRemovedFrostTotem(ai, ai->GetBot(), raidState_);
            NaxxramasFightStrategy::OnStrategyRemoved(state);
        }

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

    // Gothik (16060): the raid is split by side (>=10 alive per side of the
    // combat gate before the waves); the wave adds are killed as they spawn.
    class GothikFightStrategy : public NaxxramasFightStrategy
    {
    public:
        GothikFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "gothik"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Razuvious (16061): a priest mind-controls an Understudy, which Shield
    // Walls and then Taunts him. Fallback for MC-less raids: tank and heal.
    class RazuviousFightStrategy : public NaxxramasFightStrategy
    {
    public:
        RazuviousFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "razuvious"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Patchwerk (16028): pure tank'n'spank - the strategy carries the
    // consumable lifecycle and validation only.
    class PatchwerkFightStrategy : public NaxxramasFightStrategy
    {
    public:
        PatchwerkFightStrategy(PlayerbotAI* ai) : NaxxramasFightStrategy(ai) {}
        std::string getName() override { return "patchwerk"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };
}
