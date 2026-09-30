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

    // True while a mind-controlled ally is nearby and not yet CC'd. The aura
    // that marks the control comes from the per-raid subclass: Skeram's True
    // Fulfillment (785) and Kel'Thuzad's Chains of Kel'Thuzad (28410).
    class MindControlledAllyTrigger : public Trigger
    {
    public:
        MindControlledAllyTrigger(PlayerbotAI* ai, std::string name, uint32 auraId)
        : Trigger(ai, name, 2)
        , auraId(auraId) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "attackers"))
            {
                Unit* unit = ai->GetUnit(guid);
                if (!unit || unit->GetTypeId() != TYPEID_PLAYER || !unit->IsAlive() || !unit->IsInWorld())
                    continue;

                if (!unit->HasAura(auraId))
                    continue;

                if (unit->HasAuraType(SPELL_AURA_MOD_CONFUSE) ||
                    unit->HasAuraType(SPELL_AURA_MOD_STUN) ||
                    unit->HasAuraType(SPELL_AURA_MOD_FEAR))
                    continue;

                if (unit->GetDistance(bot) > 40.0f)
                    continue;

                return true;
            }

            return false;
        }

    private:
        uint32 auraId;
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

    // Greater Nature Protection Potion (13458 -> 17546), Greater Arcane
    // Protection Potion (13461 -> 17549) and Greater Frost Protection Potion
    // (13456 -> 17544) - same pattern as fire/shadow; wired on the fights whose
    // damage school matches.
    class NatureProtectionPotionReadyTrigger : public ItemBuffReadyTrigger
    {
    public:
        NatureProtectionPotionReadyTrigger(PlayerbotAI* ai) : ItemBuffReadyTrigger(ai, "nature protection potion ready", 13458, 17546) {}
    };

    class ArcaneProtectionPotionReadyTrigger : public ItemBuffReadyTrigger
    {
    public:
        ArcaneProtectionPotionReadyTrigger(PlayerbotAI* ai) : ItemBuffReadyTrigger(ai, "arcane protection potion ready", 13461, 17549) {}
    };

    class FrostProtectionPotionReadyTrigger : public ItemBuffReadyTrigger
    {
    public:
        FrostProtectionPotionReadyTrigger(PlayerbotAI* ai) : ItemBuffReadyTrigger(ai, "frost protection potion ready", 13456, 17544) {}
    };

    // ---- AQ raid protection-potion supply (map 509 Ruins / 531 Temple) ----
    //
    // The five Greater Protection Potions the AQ fights actually consume. Item
    // ids verified against tw_world.item_template; the shared supply rule keeps
    // a bot from ever running dry mid-raid:
    //
    //   * LOW watermark  - when a stack drops below this, the supply action
    //                      tops it back up (bots always hold at least LOW).
    //   * TOP-UP target  - the action refills to this value, so every top-up
    //                      hands the bot (TOP-UP - LOW) MORE than the amount it
    //                      was short. With LOW=10 / TARGET=20 a bot is always
    //                      given at least 10 more than it needs, and the
    //                      "give 10+ than the amount needed on entry" rule the
    //                      server owner asked for holds on every refill.
    //
    // Potion ids: Arcane 13461, Fire 13457, Frost 13456, Nature 13458,
    // Shadow 13459 (all Greater * Protection Potion; see the buff-id triggers
    // above for the matching ON_USE buffs).
    inline uint32 AhnQirajSupplyLowWatermark() { return 10; }
    inline uint32 AhnQirajSupplyTopUpTarget() { return 20; }

    inline uint32 const* AhnQirajProtectionPotions(uint32& count)
    {
        static uint32 const kItems[] = { 13461, 13457, 13456, 13458, 13459 };
        count = sizeof(kItems) / sizeof(kItems[0]);
        return kItems;
    }

    // True when the bot is inside an AQ raid and any of the five Greater
    // Protection Potions has dropped below the low watermark. Shared by the
    // trigger (IsActive) and the action (isUseful) so they can never disagree.
    bool AhnQirajSuppliesLow(PlayerbotAI* ai);

    class AhnQirajSuppliesNeededTrigger : public Trigger
    {
    public:
        AhnQirajSuppliesNeededTrigger(PlayerbotAI* ai) : Trigger(ai, "ahnqiraj supplies needed", 5) {}
        bool IsActive() override;
    };
}
