#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class MoltenCoreDungeonStrategy : public Strategy
    {
    public:
        MoltenCoreDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "molten core"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Shared MC boss-fight lifecycle: enables the potions + consumables
    // strategies on fight start, resets the shaman resist-totem duty on fight
    // end, and (for the school the boss deals) puts the resist class of each
    // subgroup on the resist duty.
    class MoltenCoreFightStrategy : public Strategy
    {
    public:
        MoltenCoreFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}

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

    protected:
        RaidFightBotState raidState_;
    };

    class MagmadarFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        MagmadarFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "magmadar"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
        void InitCombatMultipliers(std::list<Multiplier*>& multipliers) override;
    };

    class LucifronFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        LucifronFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "lucifron"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class GehennasFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        GehennasFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "gehennas"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class GarrFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        GarrFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "garr"; }

        void OnStrategyAdded(BotState state) override
        {
            MoltenCoreFightStrategy::OnStrategyAdded(state);

            // Lock out AoE for the whole fight: Firesworn deaths chain-explode.
            if (state == BotState::BOT_STATE_COMBAT)
            {
                hadAoe_ = ai->HasStrategy("aoe", BotState::BOT_STATE_COMBAT);
                if (hadAoe_)
                    ai->ChangeStrategy("-aoe", BotState::BOT_STATE_COMBAT);
            }
        }

        void OnStrategyRemoved(BotState state) override
        {
            MoltenCoreFightStrategy::OnStrategyRemoved(state);

            // Fight over - restore the master's AoE choice.
            if (state == BotState::BOT_STATE_COMBAT && hadAoe_)
            {
                ai->ChangeStrategy("+aoe", BotState::BOT_STATE_COMBAT);
                hadAoe_ = false;
            }
        }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        bool hadAoe_ = false;
    };

    class GeddonFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        GeddonFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "geddon"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class ShazzrahFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        ShazzrahFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "shazzrah"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class SulfuronFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        SulfuronFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "sulfuron"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class GolemaggFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        GolemaggFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "golemagg"; }

        void OnStrategyAdded(BotState state) override
        {
            MoltenCoreFightStrategy::OnStrategyAdded(state);

            // AoE is locked out whole fight: Core Ragers must be held away.
            if (state == BotState::BOT_STATE_COMBAT)
            {
                hadAoe_ = ai->HasStrategy("aoe", BotState::BOT_STATE_COMBAT);
                if (hadAoe_)
                    ai->ChangeStrategy("-aoe", BotState::BOT_STATE_COMBAT);
            }
        }

        void OnStrategyRemoved(BotState state) override
        {
            MoltenCoreFightStrategy::OnStrategyRemoved(state);

            if (state == BotState::BOT_STATE_COMBAT && hadAoe_)
            {
                ai->ChangeStrategy("+aoe", BotState::BOT_STATE_COMBAT);
                hadAoe_ = false;
            }
        }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;

        bool hadAoe_ = false;
    };

    class DomoFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        DomoFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "domo"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    class RagnarosFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        RagnarosFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "ragnaros"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // --- Turtle's custom MC bosses (raid plan Phase 13) ----------------------
    // Twin Golems (Basalthar 65020 + Smoldaris 65021): shared damage mirror +
    // linked death. Split tanks; the raid burns ONE target - the % mirror
    // kills the other, so splitting DPS only slows the kill.
    class TwinGolemFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        TwinGolemFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "twin golem"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Sorcerer-Thane Thaurissan (57642): marked runes run out, Mirror Image at
    // 50% (Image of Thane 57643), Arcane Rupture on the tank.
    class ThaurissanFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        ThaurissanFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "thaurissan"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Incindis (52145): Quaking Stomp -> Fire Nova, and at 50% the Incendic
    // Eggs hatch into 52148/52149 adds.
    class IncindisFightStrategy : public MoltenCoreFightStrategy
    {
    public:
        IncindisFightStrategy(PlayerbotAI* ai) : MoltenCoreFightStrategy(ai) {}
        std::string getName() override { return "incindis"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };
}
