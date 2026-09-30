#pragma once
#include "playerbot/strategy/Strategy.h"
#include "playerbot/strategy/actions/DungeonActions.h"

namespace ai
{
    class BlackwingLairDungeonStrategy : public Strategy
    {
    public:
        BlackwingLairDungeonStrategy(PlayerbotAI* ai) : Strategy(ai) {}
        std::string getName() override { return "blackwing lair"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Shared BWL boss-fight lifecycle: enables the potions + consumables
    // strategies on fight start and resets the shaman resist-totem duty on fight
    // end.
    class BlackwingLairFightStrategy : public Strategy
    {
    public:
        BlackwingLairFightStrategy(PlayerbotAI* ai) : Strategy(ai) {}

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

    class SuppressionRoomStrategy : public BlackwingLairFightStrategy
    {
    public:
        SuppressionRoomStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "suppression room"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitCombatMultipliers(std::list<Multiplier*>& multipliers) override;
        void InitNonCombatMultipliers(std::list<Multiplier*>& multipliers) override;
        void OnStrategyAdded(BotState state) override;
    };

    // Razorgore phase 1: the PLAYER controls Razorgore through the Orb of
    // Domination and destroys the 30 eggs. Bots never touch the orb and never
    // attack Razorgore until Warming Flames is up; tanks peel Grethok and the
    // Blackwing Guards away from the orb user.
    class RazorgoreFightStrategy : public BlackwingLairFightStrategy
    {
    public:
        RazorgoreFightStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "razorgore"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Vaelastrasz: Essence of the Red raid-wide; Burning Adrenaline run-outs.
    class VaelFightStrategy : public BlackwingLairFightStrategy
    {
    public:
        VaelFightStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "vael"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
        void InitReactionTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Broodlord Lashlayer (12017): Knock Away 18670 threat wipe tank switch;
    // Mortal Strike 24573 healing focus.
    class BroodlordFightStrategy : public BlackwingLairFightStrategy
    {
    public:
        BroodlordFightStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "broodlord"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Firemaw: Flame Buffet 23341 (fire, whole raid every ~2 s) + Shadow Flame
    // cone (22539); Wing Buffet resets the tank rotation. Fire resist duty +
    // fire pot.
    class FiremawFightStrategy : public BlackwingLairFightStrategy
    {
    public:
        FiremawFightStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "firemaw"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Ebonroc: Shadow Flame cone + Shadow of Ebonroc (23340, tank curse);
    // Shadow Flame cone. Shadow resist duty + shadow pot.
    class EbonrocFightStrategy : public BlackwingLairFightStrategy
    {
    public:
        EbonrocFightStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "ebonroc"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Flamegor: Frenzy (23342, hunter tranquilizing shot) + Shadow Flame cone;
    // Wing Buffet resets the tank rotation. Fire resist duty + fire pot.
    class FlamegorFightStrategy : public BlackwingLairFightStrategy
    {
    public:
        FlamegorFightStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "flamegor"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Chromaggus: brood afflictions (bronze cleared directly), breath LoS per
    // the local boss script.
    class ChromaggusFightStrategy : public BlackwingLairFightStrategy
    {
    public:
        ChromaggusFightStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "chromaggus"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };

    // Nefarian: drakonid add waves, corrupted totems, phase 3 Bone Constructs.
    class NefarianFightStrategy : public BlackwingLairFightStrategy
    {
    public:
        NefarianFightStrategy(PlayerbotAI* ai) : BlackwingLairFightStrategy(ai) {}
        std::string getName() override { return "nefarian"; }

    private:
        void InitCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitNonCombatTriggers(std::list<TriggerNode*>& triggers) override;
        void InitDeadTriggers(std::list<TriggerNode*>& triggers) override;
    };
}
