#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"

namespace ai
{
    class RuinsOfAhnQirajEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        RuinsOfAhnQirajEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter ruins of ahnqiraj", "ruins of ahnqiraj", 509) {}
    };

    class RuinsOfAhnQirajLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        RuinsOfAhnQirajLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave ruins of ahnqiraj", "ruins of ahnqiraj", 509) {}
    };

    // Kurinnaxx (15348): Mortal Wound (25646) is a healing debuff that stacks
    // to 10; the offtank taunts at 5 stacks (same pattern as Gluth).
    class KurinnaxxStartFightTrigger : public StartBossFightTrigger
    {
    public:
        KurinnaxxStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start kurinnaxx fight", "kurinnaxx", 15348) {}
    };

    class KurinnaxxEndFightTrigger : public EndBossFightTrigger
    {
    public:
        KurinnaxxEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end kurinnaxx fight", "kurinnaxx", 15348) {}
    };

    class KurinnaxxTankSwitchTrigger : public Trigger
    {
    public:
        KurinnaxxTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "kurinnaxx tank switch", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15348);
            return boss && boss->IsInCombat() && ai::GetSpellStacks(boss->GetVictim(), 25646) >= 5;
        }
    };

    // Sand Trap GO 180647 (trap, 15 yd, spell 25656 = nature damage + slow /
    // silence) is summoned under a random player and self-despawns after 5 s.
    // React one yard before the real damage boundary.
    class KurinnaxxSandTrapTrigger : public CloseToGameObjectHazardTrigger
    {
    public:
        KurinnaxxSandTrapTrigger(PlayerbotAI* ai) : CloseToGameObjectHazardTrigger(ai, "kurinnaxx sand trap", 180647, 16.0f, 10) {}
    };

    // Rajaxx event (Andorov 15471 + Kaldorei Elite 15473): seven officer waves
    // - Qeez 15391, Tuubid 15392, Drenn 15389, Xurrem 15390, Yeggeth 15386,
    // Pakkon 15388, Zerran 15385 - then General Rajaxx 15341. Any of the eight
    // starts the fight strategy; any evade or 180 s wave timeout resets the
    // whole event (generic_scripts success chain).
    class RajaxxStartFightTrigger : public Trigger
    {
    public:
        RajaxxStartFightTrigger(PlayerbotAI* ai) : Trigger(ai, "start rajaxx fight", 5) {}

        bool IsActive() override
        {
            if (ai->HasStrategy("rajaxx", BotState::BOT_STATE_COMBAT))
                return false;

            for (const ObjectGuid& attackerGuid : AI_VALUE(std::list<ObjectGuid>, "attackers"))
            {
                Unit* attacker = ai->GetUnit(attackerGuid);
                if (!attacker)
                    continue;

                const uint32 entry = attacker->GetEntry();
                if (entry == 15391 || entry == 15392 || entry == 15389 || entry == 15390 ||
                    entry == 15386 || entry == 15388 || entry == 15385 || entry == 15341)
                    return true;
            }

            return false;
        }
    };

    class RajaxxEndFightTrigger : public EndBossFightTrigger
    {
    public:
        RajaxxEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end rajaxx fight", "rajaxx", 15341) {}
    };

    // Buru the Gorger (15370) locks one target with 1M threat and chases it;
    // the locked bot kites while Buru is close. Eggs (15514) reset the chase
    // but are popped by the player, not by the bots.
    class BuruStartFightTrigger : public StartBossFightTrigger
    {
    public:
        BuruStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start buru fight", "buru", 15370) {}
    };

    class BuruEndFightTrigger : public EndBossFightTrigger
    {
    public:
        BuruEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end buru fight", "buru", 15370) {}
    };

    class BuruChasingMeTrigger : public Trigger
    {
    public:
        BuruChasingMeTrigger(PlayerbotAI* ai) : Trigger(ai, "buru chasing me", 1) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15370);
            if (!boss || !boss->IsInCombat())
                return false;

            Unit* victim = boss->GetVictim();
            if (!victim || victim->GetObjectGuid() != bot->GetObjectGuid())
                return false;

            return bot->IsWithinDist(boss, 25.0f);
        }
    };

    // Moam (15340): Mana Fiends (15527) drain the raid's mana and are killed
    // on sight.
    class MoamStartFightTrigger : public StartBossFightTrigger
    {
    public:
        MoamStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start moam fight", "moam", 15340) {}
    };

    class MoamEndFightTrigger : public EndBossFightTrigger
    {
    public:
        MoamEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end moam fight", "moam", 15340) {}
    };

    // Ayamiss the Hunter (15369): Paralyze (25725) marks a sacrifice; a
    // Hive'Zara Larva (15555) walks to the player and summons a Hornet (15934)
    // unless it is killed first. Swarmers (15546) spawn every minute.
    class AyamissStartFightTrigger : public StartBossFightTrigger
    {
    public:
        AyamissStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start ayamiss fight", "ayamiss", 15369) {}
    };

    class AyamissEndFightTrigger : public EndBossFightTrigger
    {
    public:
        AyamissEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end ayamiss fight", "ayamiss", 15369) {}
    };

    // Ossirian the Unscarred (15339): Sand Vortex (15428) hazards roam the
    // arena; the crystal clicks stay a player action.
    class OssirianStartFightTrigger : public StartBossFightTrigger
    {
    public:
        OssirianStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start ossirian fight", "ossirian", 15339) {}
    };

    class OssirianEndFightTrigger : public EndBossFightTrigger
    {
    public:
        OssirianEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end ossirian fight", "ossirian", 15339) {}
    };

    class OssirianSandVortexTrigger : public CloseToCreatureHazardTrigger
    {
    public:
        OssirianSandVortexTrigger(PlayerbotAI* ai) : CloseToCreatureHazardTrigger(ai, "ossirian sand vortex", 15428, 10.0f, 30) {}

    private:
        // The Sand Vortex wanders the arena (RANDOM_MOTION_TYPE, 55 yd) and its
        // Sandstorm hits everyone close, not just its melee victim, so the base
        // trigger's "not targeting me" exclusion is lifted.
        bool IsHazardValid(const ObjectGuid& hazzardGuid) override
        {
            Creature* vortex = ai->GetCreature(hazzardGuid);
            return vortex && sServerFacade.IsAlive(vortex);
        }
    };
}
