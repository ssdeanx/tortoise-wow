#pragma once
#include "MovementActions.h"
#include "AttackAction.h"
#include "playerbot/strategy/values/HazardsValue.h"

namespace ai
{
    class MoveAwayFromHazard : public MovementAction
    {
    public:
        MoveAwayFromHazard(PlayerbotAI* ai, std::string name = "move away from hazard") : MovementAction(ai, name) {}
        bool Execute(Event& event) override;
        bool isPossible() override;

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "move away from hazard"; }
        virtual std::string GetHelpDescription()
        {
            return "This action makes the bot move away from hazardous areas in dungeons.\n"
                   "It identifies dangerous positions and navigates to a safer location.";
        }
        virtual std::vector<std::string> GetUsedActions() { return {}; }
        virtual std::vector<std::string> GetUsedValues() { return {"hazards"}; }
#endif 

    private:
        bool IsHazardNearby(const WorldPosition& point, const std::list<HazardPosition>& hazards) const;
    };

    class MoveAwayFromCreature : public MovementAction
    {
    public:
        MoveAwayFromCreature(PlayerbotAI* ai, std::string name, uint32 creatureID, float range) : MovementAction(ai, name), creatureID(creatureID), range(range) {}
        bool Execute(Event& event) override;
        bool isPossible() override;

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "move away from creature"; }
        virtual std::string GetHelpDescription()
        {
            return "This action makes the bot move away from a specific creature in dungeons.\n"
                   "It maintains a safe distance from the specified creature ID within a defined range.";
        }
        virtual std::vector<std::string> GetUsedActions() { return {}; }
        virtual std::vector<std::string> GetUsedValues() { return {"hazards"}; }
#endif 

    private:
        bool IsValidPoint(const WorldPosition& point, const std::list<Creature*>& creatures, const std::list<HazardPosition>& hazards);
        bool HasCreaturesNearby(const WorldPosition& point, const std::list<Creature*>& creatures) const;
        bool IsHazardNearby(const WorldPosition& point, const std::list<HazardPosition>& hazards) const;

    protected:
        uint32 creatureID;
        float range;
    };

    class MoveAwayFromSpecificCreatures : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromSpecificCreatures(PlayerbotAI* ai, float range, bool ignoreVictim = true, std::string name = "move away from specific creatures") : MoveAwayFromCreature(ai, name, 0, range) {}
        bool Execute(Event& event) override;

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "move away from specific creatures"; }
        virtual std::string GetHelpDescription()
        {
            return "This action makes the bot move away from specific creatures defined by the avoid creature list.\n"
                   "It maintains a safe distance from the specified creature IDs within a defined range.";
        }
        virtual std::vector<std::string> GetUsedActions() { return {}; }
        virtual std::vector<std::string> GetUsedValues() { return {"hazards"}; }
#endif
    };

    // Moves to a point at least N yards away from every raid member (group
    // members + master). Run-out mechanics: Geddon Living Bomb, Vael Burning
    // Adrenaline. Qualified with the distance: "move away from raid::25".
    class MoveAwayFromRaidAction : public MovementAction, public Qualified
    {
    public:
        MoveAwayFromRaidAction(PlayerbotAI* ai, std::string name = "move away from raid") : MovementAction(ai, name), Qualified() {}
        bool Execute(Event& event) override;
        bool isPossible() override;

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "move away from raid"; }
        virtual std::string GetHelpDescription()
        {
            return "This action moves the bot away from all raid members to a safe distance.\n"
                   "The minimum distance comes from the qualifier (default 20 yards).";
        }
        virtual std::vector<std::string> GetUsedActions() { return {}; }
        virtual std::vector<std::string> GetUsedValues() { return {"hazards"}; }
#endif

    private:
        bool IsRaidNearby(const WorldPosition& point, const std::list<WorldPosition>& raidPositions, float minDistance) const;
    };

    // Attacks the nearest alive raid-visible creature of the entry in the
    // qualifier ("attack entry::12099" or "attack entry::12099,40" for a custom
    // max range, default 60). Add-priority kill orders.
    class AttackNearestEntryAction : public AttackAction, public Qualified
    {
    public:
        AttackNearestEntryAction(PlayerbotAI* ai, std::string name = "attack entry") : AttackAction(ai, name), Qualified() {}

        bool Execute(Event& event) override;
        bool isUseful() override;

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "attack entry"; }
        virtual std::string GetHelpDescription()
        {
            return "This action makes the bot attack the nearest creature of the entry given in the qualifier.\n"
                   "Used to focus priority adds during raid encounters.";
        }
        virtual std::vector<std::string> GetUsedActions() { return {}; }
        virtual std::vector<std::string> GetUsedValues() { return {"possible attack targets"}; }
#endif

    protected:
        Unit* FindTarget();
    };

    // Same target rule, but only tanks use it - offtank pickup for adds that
    // must be held away from the raid (Core Ragers, Sons of Ragnaros, drakonids).
    // Qualifier identical to "attack entry".
    class OfftankNearestEntryAction : public AttackNearestEntryAction
    {
    public:
        OfftankNearestEntryAction(PlayerbotAI* ai, std::string name = "offtank entry") : AttackNearestEntryAction(ai, name) {}

        bool isUseful() override;
    };

    // Moves to the nearest pathable point that is OUT of line of sight of the
    // given creature (ranged/melee hide behind a pillar so area buffs fall off:
    // Firemaw Flame Buffet). Constructor-based, like MoveAwayFromCreature.
    class HideFromCreatureAction : public MovementAction
    {
    public:
        HideFromCreatureAction(PlayerbotAI* ai, std::string name, uint32 creatureID) : MovementAction(ai, name), creatureID(creatureID) {}

        bool Execute(Event& event) override;
        bool isPossible() override;
        bool isUseful() override;

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "hide from creature"; }
        virtual std::string GetHelpDescription()
        {
            return "This action makes the bot move to a nearby spot that breaks line of sight to the given creature,\n"
                   "so stacking area buffs can fall off while the fight continues.";
        }
        virtual std::vector<std::string> GetUsedActions() { return {}; }
        virtual std::vector<std::string> GetUsedValues() { return {"hazards"}; }
#endif

    private:
        uint32 creatureID;
    };

    // Tank swap: taunt the given creature (warrior "taunt", druid "growl" -
    // vanilla 1.12 has no paladin taunt). Tanks only; the bot must NOT be the
    // creature's current victim (the active tank does not re-taunt).
    class TauntCreatureAction : public AttackAction
    {
    public:
        TauntCreatureAction(PlayerbotAI* ai, std::string name, uint32 creatureID) : AttackAction(ai, name), creatureID(creatureID) {}

        bool Execute(Event& event) override;
        bool isUseful() override;

    private:
        uint32 creatureID;
    };

    // Thaddius-style charge stacking: move next to the nearest friendly player
    // carrying the SAME charge aura. Opposite charges near each other take
    // damage; same charges must stand together. Constructor takes the positive
    // and negative aura spell ids.
    class FollowChargedAllyAction : public MovementAction
    {
    public:
        FollowChargedAllyAction(PlayerbotAI* ai, std::string name, uint32 positiveSpellId, uint32 negativeSpellId)
        : MovementAction(ai, name), positiveSpellId(positiveSpellId), negativeSpellId(negativeSpellId) {}

        bool Execute(Event& event) override;
        bool isPossible() override;
        bool isUseful() override;

    private:
        Player* FindChargedAlly(uint32 chargeSpellId);

        uint32 positiveSpellId;
        uint32 negativeSpellId;
    };
}

namespace ai
{
    // Exactly one resist-capable bot per raid SUBGROUP performs the resistance
    // duty. Vanilla resistance totems cover only the dropper's group (so every
    // group wants its own) but two same-class bots sharing a group must not both
    // burn the duty: shaman (Horde) drops the totem, paladin (Alliance) casts the
    // aura. Rule: lowest-GUID alive bot of the same class within the same
    // subgroup. Defined in DungeonActions.cpp.
    bool IsGroupResistDropper(PlayerbotAI* ai, Player* player);

    // Per-bot state for one raid boss fight lifecycle.
    struct RaidFightBotState
    {
        bool hadTotemFireRes = false;
        bool hadPotions = false;
        bool hadConsumables = false;
    };

    // Shaman lifecycle for a raid boss fight: force the fire resistance totem
    // strategy on the group's designated dropper when the fight starts, and
    // reset it back to whatever the master had when combat stops. The flag is
    // only ever set on the dropper shaman, so the reset is safe for everyone
    // else.
    inline void OnRaidFightStrategyAddedFireTotem(PlayerbotAI* ai, Player* bot, RaidFightBotState& state)
    {
        if (bot->getClass() != CLASS_SHAMAN || !IsGroupResistDropper(ai, bot))
            return;

        state.hadTotemFireRes = ai->HasStrategy("totem fire resistance", BotState::BOT_STATE_COMBAT);
        if (!state.hadTotemFireRes)
            ai->ChangeStrategy("+totem fire resistance", BotState::BOT_STATE_COMBAT);
    }

    inline void OnRaidFightStrategyRemovedFireTotem(PlayerbotAI* ai, Player* bot, RaidFightBotState& state)
    {
        if (bot->getClass() != CLASS_SHAMAN || !state.hadTotemFireRes)
            return;

        ai->ChangeStrategy("-totem fire resistance", BotState::BOT_STATE_COMBAT);
        state.hadTotemFireRes = false;
    }

    // Consumable lifecycle for a raid boss fight: enable the "potions" strategy
    // (healthstone, healing potion on low health, dark rune/mana potion on low
    // mana, whipper root tuber, bandage, anti-venom) and the "consumables"
    // strategy (flasks, elixirs, food buffs, scrolls scored per role) for every
    // bot during the fight, then restore the master's choice when it ends.
    inline void OnRaidFightStrategyAddedConsumables(PlayerbotAI* ai, bool& hadPotions, bool& hadConsumables)
    {
        hadPotions = ai->HasStrategy("potions", BotState::BOT_STATE_COMBAT);
        hadConsumables = ai->HasStrategy("consumables", BotState::BOT_STATE_COMBAT);
        if (!hadPotions)
            ai->ChangeStrategy("+potions", BotState::BOT_STATE_COMBAT);
        if (!hadConsumables)
            ai->ChangeStrategy("+consumables", BotState::BOT_STATE_COMBAT);
    }

    inline void OnRaidFightStrategyRemovedConsumables(PlayerbotAI* ai, bool& hadPotions, bool& hadConsumables)
    {
        if (!hadPotions)
            ai->ChangeStrategy("-potions", BotState::BOT_STATE_COMBAT);
        if (!hadConsumables)
            ai->ChangeStrategy("-consumables", BotState::BOT_STATE_COMBAT);
        hadPotions = false;
        hadConsumables = false;
    }
}
