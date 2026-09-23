#pragma once
#include "playerbot/strategy/Trigger.h"
#include "ScriptedInstance.h"

namespace ai
{
    class EnterDungeonTrigger : public Trigger
    {
    public:
        // You can get the mapID from worlddb > instance_template > map column
        // or from here https://wow.tools/dbc/?dbc=map&build=1.12.1.5875
        EnterDungeonTrigger(PlayerbotAI* ai, std::string name, std::string dungeonStrategy, uint32 mapID)
        : Trigger(ai, name, 5)
        , dungeonStrategy(dungeonStrategy)
        , mapID(mapID) {}

        bool IsActive() override;

    private:
        std::string dungeonStrategy;
        uint32 mapID;
    };

    class LeaveDungeonTrigger : public Trigger
    {
    public:
        // You can get the mapID from worlddb > instance_template > map column
        // or from here https://wow.tools/dbc/?dbc=map&build=1.12.1.5875
        LeaveDungeonTrigger(PlayerbotAI* ai, std::string name, std::string dungeonStrategy, uint32 mapID)
        : Trigger(ai, name, 5)
        , dungeonStrategy(dungeonStrategy)
        , mapID(mapID) {}

        bool IsActive() override;

    private:
        std::string dungeonStrategy;
        uint32 mapID;
    };

    class StartBossFightTrigger : public Trigger
    {
    public:
        StartBossFightTrigger(PlayerbotAI* ai, std::string name, std::string bossStrategy, uint64 bossID)
        : Trigger(ai, name, 1)
        , bossStrategy(bossStrategy)
        , bossID(bossID) {}

        bool IsActive() override;

    private:
        std::string bossStrategy;
        uint64 bossID;
    };

    class EndBossFightTrigger : public Trigger
    {
    public:
        EndBossFightTrigger(PlayerbotAI* ai, std::string name, std::string bossStrategy, uint64 bossID)
        : Trigger(ai, name, 5)
        , bossStrategy(bossStrategy)
        , bossID(bossID) {}

        bool IsActive() override;

    private:
        std::string bossStrategy;
        uint64 bossID;
    };

    class CloseToHazardTrigger : public Trigger
    {
    public:
        CloseToHazardTrigger(PlayerbotAI* ai, std::string name, int checkInterval, float hazardRadius, time_t hazardDuration)
        : Trigger(ai, name, checkInterval)
        , hazardRadius(hazardRadius)
        , hazardDuration(hazardDuration) {}

        bool IsActive() override final;

    protected:
        virtual std::list<ObjectGuid> GetPossibleHazards() = 0;
        virtual bool IsHazardValid(const ObjectGuid& hazzardGuid);

    private:
        float GetDistanceToHazard(const ObjectGuid& hazzardGuid);

    protected:
        float hazardRadius;
        time_t hazardDuration;
    };

    class CloseToGameObjectHazardTrigger : public CloseToHazardTrigger
    {
    public:
        CloseToGameObjectHazardTrigger(PlayerbotAI* ai, std::string name, uint32 gameObjectID, float radius, time_t expirationTime)
        : CloseToHazardTrigger(ai, name, 1, radius, expirationTime)
        , gameObjectID(gameObjectID) {}

    private:
        std::list<ObjectGuid> GetPossibleHazards() override;

    private:
        uint32 gameObjectID;
    };

    // Not dungeon-specific despite living in this file - lives here because it wants the
    // same "add hazard"/HazardsValue plumbing CloseToHazardTrigger uses. Detects any nearby
    // GAMEOBJECT_TYPE_TRAP GameObject that auto-fires environmental damage (no owner, spell
    // effect SPELL_EFFECT_ENVIRONMENTAL_DAMAGE - matches the exact condition
    // GameObject::Update uses to hit any player in range, src/game/Objects/GameObject.cpp:459-538),
    // e.g. overworld braziers, generically - no per-entry allowlist needed. Registered on the
    // reaction engine (see ReactionStrategy) so it fires regardless of the bot's
    // combat/non-combat state.
    //
    // Doesn't subclass CloseToHazardTrigger (unlike the other hazard triggers below) because
    // that base class uses one fixed radius for every hazard it tracks - here each GameObject
    // has its own real danger radius (goInfo->trap.radius, the same value the core engine
    // itself uses to decide who to hit), so this reacts at the actual danger boundary instead
    // of a guessed constant, and registers that same accurate radius into "hazards" so
    // pathing (MovementAction::GeneratePathAvoidingHazards) bends around the real zone too.
    class EnvironmentalHazardTrigger : public Trigger
    {
    public:
        EnvironmentalHazardTrigger(PlayerbotAI* ai, std::string name = "environmental hazard nearby", time_t hazardDuration = 60)
        : Trigger(ai, name, 1)
        , hazardDuration(hazardDuration) {}

        bool IsActive() override;

    private:
        time_t hazardDuration;
    };

    class CloseToCreatureHazardTrigger : public CloseToHazardTrigger
    {
    public:
        CloseToCreatureHazardTrigger(PlayerbotAI* ai, std::string name, uint32 creatureID, float radius, time_t expirationTime)
        : CloseToHazardTrigger(ai, name, 1, radius, expirationTime)
        , creatureID(creatureID) {}

    private:
        std::list<ObjectGuid> GetPossibleHazards() override;
        bool IsHazardValid(const ObjectGuid& hazzardGuid) override;

    protected:
        uint32 creatureID;
    };

    class CloseToHostileCreatureHazardTrigger : public CloseToCreatureHazardTrigger
    {
    public:
        CloseToHostileCreatureHazardTrigger(PlayerbotAI* ai, std::string name, uint32 creatureID, float radius, time_t expirationTime)
        : CloseToCreatureHazardTrigger(ai, name, creatureID, radius, expirationTime) {}

    private:
        std::list<ObjectGuid> GetPossibleHazards() override;
    };

    class CloseToCreatureTrigger : public Trigger
    {
    public:
        CloseToCreatureTrigger(PlayerbotAI* ai, std::string name, uint32 creatureID, float range)
        : Trigger(ai, name, 1)
        , creatureID(creatureID)
        , range(range) {}

        bool IsActive() override;

    private:
        uint32 creatureID;
        float range;
    };

    class CloseToSpecificCreaturesTrigger : public Trigger
    {
    public:
        CloseToSpecificCreaturesTrigger(PlayerbotAI* ai, std::string name = "specific creature too close", float range = 10.0f, bool ignoreVictim = true)
        : Trigger(ai, name, 1)
        , range(range)
        , ignoreVictim(ignoreVictim) {}

        bool IsActive() override;

    private:
        float range;
        bool ignoreVictim;
    };

    class ItemReadyTrigger : public Trigger
    {
    public:
        ItemReadyTrigger(PlayerbotAI* ai, std::string name, uint32 itemID)
        : Trigger(ai, name, 1)
        , itemID(itemID) {}

        virtual bool IsActive() override;

    protected:
        uint32 itemID;
    };

    class ItemBuffReadyTrigger : public ItemReadyTrigger
    {
    public:
        ItemBuffReadyTrigger(PlayerbotAI* ai, std::string name, uint32 itemID, uint32 buffID)
        : ItemReadyTrigger(ai, name, itemID)
        , buffID(buffID) {}

        bool IsActive() override;

    private:
        uint32 buffID;
    };

    // Finds an alive raid-visible unit of the given entry among "possible attack
    // targets". Shared helper for raid boss-aura checks (Geddon inferno etc.);
    // defined in DungeonTriggers.cpp.
    Unit* GetRaidTargetByEntry(PlayerbotAI* ai, uint32 entryId);

    // Stack count of a spell aura on a unit (0 when absent). Shared helper for
    // stack-based fight triggers (drake Wing Buffet swaps, Gluth Mortal Wound);
    // defined in DungeonTriggers.cpp.
    uint32 GetSpellStacks(Unit* unit, uint32 spellId);

    // True when any possible attack target (alive, raid-visible) has the given
    // creature entry. Qualified with the entry id: "attackers entry::12099".
    // Drives add-priority kill orders (Garr Firesworn, Lucifron Protectors, ...).
    class AttackersEntryTrigger : public Trigger, public Qualified
    {
    public:
        AttackersEntryTrigger(PlayerbotAI* ai, std::string name = "attackers entry", int checkInterval = 1)
        : Trigger(ai, name, checkInterval), Qualified() {}

        bool IsActive() override;
    };

    // CloseToCreatureTrigger with a runtime qualifier "{entry},{range}".
    // Optional third element "{entry},{range},{ignoreVictim}" - when true the
    // trigger also fires while the creature is targeting this bot (melee run-out
    // cases like Geddon Inferno).
    class EntryProximityTrigger : public Trigger, public Qualified
    {
    public:
        EntryProximityTrigger(PlayerbotAI* ai, std::string name = "entry proximity", int checkInterval = 1)
        : Trigger(ai, name, checkInterval), Qualified() {}

        bool IsActive() override;
    };

    // True while the encounter type is DONE in the map's instance data. Qualified
    // with the instance's TYPE_* index, e.g. "instance encounter done::8" for the
    // Majordomo surrender in Molten Core.
    class InstanceEncounterDoneTrigger : public Trigger, public Qualified
    {
    public:
        InstanceEncounterDoneTrigger(PlayerbotAI* ai, std::string name = "instance encounter done", int checkInterval = 3)
        : Trigger(ai, name, checkInterval), Qualified() {}

        bool IsActive() override;
    };

    // Protection potions (raid-generic): Greater Fire Protection Potion
    // (13457 -> buff 17543) and Greater Shadow Protection Potion
    // (13459 -> buff 17548). IDs verified against tw_world.item_template /
    // spell_template. Used by every raid boss fight of the matching school.
    class FireProtectionPotionReadyTrigger : public ItemBuffReadyTrigger
    {
    public:
        FireProtectionPotionReadyTrigger(PlayerbotAI* ai) : ItemBuffReadyTrigger(ai, "fire protection potion ready", 13457, 17543) {}
    };

    class ShadowProtectionPotionReadyTrigger : public ItemBuffReadyTrigger
    {
    public:
        ShadowProtectionPotionReadyTrigger(PlayerbotAI* ai) : ItemBuffReadyTrigger(ai, "shadow protection potion ready", 13459, 17548) {}
    };
}
