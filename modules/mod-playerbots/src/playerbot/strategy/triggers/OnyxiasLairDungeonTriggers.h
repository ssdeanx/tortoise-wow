#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"
#include "playerbot/BotSlots.h"

namespace ai
{
    class OnyxiasLairEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        OnyxiasLairEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter onyxia's lair", "onyxia's lair", 249) {}
    };

    class OnyxiasLairLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        OnyxiasLairLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave onyxia's lair", "onyxia's lair", 249) {}
    };

    class OnyxiaStartFightTrigger : public StartBossFightTrigger
    {
    public:
        OnyxiaStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start onyxia fight", "onyxia", 10184) {}
    };

    class OnyxiaEndFightTrigger : public EndBossFightTrigger
    {
    public:
        OnyxiaEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end onyxia fight", "onyxia", 10184) {}
    };

    // Phase 2 marker: Onyxia is airborne (HOVER 17131, boss_onyxia.cpp).
    class OnyxiaAirborneTrigger : public Trigger
    {
    public:
        OnyxiaAirborneTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia airborne", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 10184);
            return boss && boss->IsInCombat() && ai->HasAura(17131, boss);
        }
    };

    // Grounded phases (1 and 3): phased DPS target switch back to the boss.
    class OnyxiaGroundedTrigger : public Trigger
    {
    public:
        OnyxiaGroundedTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia grounded", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 10184);
            return boss && boss->IsInCombat() && !ai->HasAura(17131, boss);
        }
    };

    // Deep Breath (23461) cast is the cue to scatter - the breath lines cover
    // the room quadrants, so moving off the boss's lane is the survival play.
    class OnyxiaDeepBreathTrigger : public Trigger
    {
    public:
        OnyxiaDeepBreathTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia deep breath", 1) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 10184);
            return boss && boss->IsInCombat() && (boss->FindCurrentSpellBySpellId(23461) != nullptr || boss->IsNonMeleeSpellCasted(false));
        }
    };

    // Phase 3 Landing Threat Reset: When Onyxia enters Phase 3 (health <= 40% and lands),
    // her threat table is completely reset (DoResetThreat in boss_onyxia.cpp).
    // Tanks must immediately taunt and establish maximum threat to prevent boss turning on raid.
    class OnyxiaPhase3LandingThreatTrigger : public Trigger
    {
    public:
        OnyxiaPhase3LandingThreatTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia phase 3 landing threat", 1) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 10184);
            if (!boss || !boss->IsAlive() || !boss->IsInCombat())
                return false;

            // Grounded and Phase 3 health threshold
            if (ai->HasAura(17131, boss) || boss->GetHealthPercent() > 40.0f)
                return false;

            // Trigger if tank does not currently hold aggro
            Unit* victim = boss->GetVictim();
            return !victim || victim != bot;
        }
    };

    // North Wall Tank Anchor: In Phase 1 and Phase 3, the tank must pull and anchor
    // Onyxia against the North wall (-8.86f, -212.75f, -88.54f) with back to the wall,
    // facing her away from the southern entrance and raid flanks to contain Flame Breath and Wing Buffet.
    class OnyxiaNeedTankNorthTrigger : public Trigger
    {
    public:
        OnyxiaNeedTankNorthTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia need tank north", 2) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 10184);
            if (!boss || !boss->IsAlive() || !boss->IsInCombat())
                return false;

            if (ai->HasAura(17131, boss))
                return false; // Airborne

            // Check if tank is not within 6 yards of the North anchor position
            const float dist = bot->GetDistance(-8.86f, -212.75f, -88.54f);
            return dist > 6.0f;
        }
    };

    // Onyxian Whelps (11262): Summoned in large waves during Phase 2 from side egg caves
    // and continuing in Phase 3. DPS and offtanks burn them down with high priority.
    class OnyxiaHasWhelpsTrigger : public Trigger
    {
    public:
        OnyxiaHasWhelpsTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia has whelps", 1) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* c = ai->GetCreature(guid);
                if (c && c->IsAlive() && c->GetEntry() == 11262)
                    return true;
            }
            return false;
        }
    };

    // Onyxian Warders (12129): Heavy melee dragonkin spawning at the entrance tunnel in Phase 2.
    // Offtanks must intercept, taunt, and turn them away from the raid.
    class OnyxiaHasWarderTrigger : public Trigger
    {
    public:
        OnyxiaHasWarderTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia has warder", 1) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* c = ai->GetCreature(guid);
                if (c && c->IsAlive() && c->GetEntry() == 12129)
                    return true;
            }
            return false;
        }
    };

    // Bellowing Roar (18431): Cast in Phase 3 every 15-30s. Fear counter-measures
    // (Berserker Rage, Tremor Totem, Fear Ward, Will of the Forsaken).
    class OnyxiaBellowingRoarTrigger : public Trigger
    {
    public:
        OnyxiaBellowingRoarTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia bellowing roar", 1) {}

        bool IsActive() override
        {
            if (bot->HasAura(18431) || bot->HasAuraType(SPELL_AURA_MOD_FEAR))
                return true;

            Unit* boss = GetRaidTargetByEntry(ai, 10184);
            if (!boss || !boss->IsAlive() || !boss->IsInCombat())
                return false;

            return boss->FindCurrentSpellBySpellId(18431) != nullptr;
        }
    };

    // Phase 3 Landing Threat Throttle: When Onyxia enters Phase 3 (health <= 40% and lands),
    // her threat table is wiped (DoResetThreat in boss_onyxia.cpp). Non-tank DPS bots must throttle
    // attacks or focus on whelps until the tank re-establishes aggro, preventing turn-cleave/breath wipes.
    class OnyxiaPhase3DpsThrottleTrigger : public Trigger
    {
    public:
        OnyxiaPhase3DpsThrottleTrigger(PlayerbotAI* ai) : Trigger(ai, "onyxia phase 3 dps throttle", 1) {}

        bool IsActive() override
        {
            if (ai->IsTank(bot) || ai->IsHeal(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 10184);
            if (!boss || !boss->IsAlive() || !boss->IsInCombat())
                return false;

            // Only applies during grounded Phase 3
            if (ai->HasAura(17131, boss) || boss->GetHealthPercent() > 40.0f)
                return false;

            Unit* victim = boss->GetVictim();
            if (!victim || !victim->IsPlayer())
                return true;

            Player* pVictim = static_cast<Player*>(victim);
            PlayerbotAI* victimAi = GetBotAI(pVictim);
            if (victimAi && !victimAi->IsTank(pVictim))
                return true;

            return false;
        }
    };
}

