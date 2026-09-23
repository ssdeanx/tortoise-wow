#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"

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
            return boss && boss->IsInCombat() && boss->FindCurrentSpellBySpellId(23461) != nullptr;
        }
    };
}
