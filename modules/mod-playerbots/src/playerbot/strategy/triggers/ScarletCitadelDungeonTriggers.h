#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"
#include "PlayerbotAI.h"

namespace ai
{
    // Dungeon Entry & Exit for Scarlet Citadel (Map 45)
    class ScarletCitadelEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        ScarletCitadelEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter scarlet citadel", "scarlet citadel", 45) {}
    };

    class ScarletCitadelLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        ScarletCitadelLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave scarlet citadel", "scarlet citadel", 45) {}
    };

    // ---------------------------------------------------------------------------
    // Boss 1: Sacred Fist Daelus (2000000)
    // ---------------------------------------------------------------------------
    class DaelusStartFightTrigger : public StartBossFightTrigger
    {
    public:
        DaelusStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start daelus fight", "daelus", 2000000) {}
    };

    class DaelusEndFightTrigger : public EndBossFightTrigger
    {
    public:
        DaelusEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end daelus fight", "daelus", 2000000) {}
    };

    // Tank Tether Trigger: If tank is > 2.5 yards from Daelus (or Daelus channels Life Drain 21157),
    // tank must immediately close distance to prevent raidwide 10% max HP/sec drain wipe.
    class DaelusNeedCloseTetherTrigger : public Trigger
    {
    public:
        DaelusNeedCloseTetherTrigger(PlayerbotAI* ai) : Trigger(ai, "daelus need close tether", 1) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 2000000);
            if (!boss || !boss->IsAlive() || !boss->IsInCombat())
                return false;

            // Trigger if distance > 2.5f or boss is channeling Life Drain (21157)
            return (bot->GetDistance2d(boss) > 2.5f) || (boss->FindCurrentSpellBySpellId(21157) != nullptr);
        }
    };

    // Poison Cloud Carrier: Daelus targets player with Green Glow (22577), 6s later drops Poison Cloud (28240).
    // Afflicted bot runs outward 25 yards from raid.
    class DaelusPoisonCloudCarrierTrigger : public Trigger
    {
    public:
        DaelusPoisonCloudCarrierTrigger(PlayerbotAI* ai) : Trigger(ai, "daelus poison cloud carrier", 1) {}

        bool IsActive() override
        {
            return ai->HasAura(22577, bot);
        }
    };

    // Poison Cloud Hazard: Bot is standing in or near Poison Cloud (16363).
    class DaelusInPoisonCloudTrigger : public Trigger
    {
    public:
        DaelusInPoisonCloudTrigger(PlayerbotAI* ai) : Trigger(ai, "daelus in poison cloud", 1) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* c = ai->GetCreature(guid);
                if (c && c->IsAlive() && c->GetEntry() == 16363 && bot->GetDistance(c) < 8.0f)
                    return true;
            }
            return false;
        }
    };

    // Fallen Spirits (2000013): 6 spawn every 30s. Normal spirits heal Daelus for 5% max HP!
    // The "Chosen One" spirit has red aura (26235) and stuns Daelus if it reaches him.
    // This trigger activates when there are NON-CHOSEN spirits alive that must be killed.
    class DaelusHasSpiritsTrigger : public Trigger
    {
    public:
        DaelusHasSpiritsTrigger(PlayerbotAI* ai) : Trigger(ai, "daelus has spirits", 1) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* c = ai->GetCreature(guid);
                if (c && c->IsAlive() && c->GetEntry() == 2000013)
                {
                    // Check if NOT the Chosen One (Chosen One has aura 26235)
                    if (!ai->HasAura(26235, c))
                        return true;
                }
            }
            return false;
        }
    };

    // Sunder Armor Tank Swap: If main tank has >= 3 stacks of Sunder Armor (25051), offtank taunts.
    class DaelusSunderArmorHighTrigger : public Trigger
    {
    public:
        DaelusSunderArmorHighTrigger(PlayerbotAI* ai) : Trigger(ai, "daelus sunder armor high", 2) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 2000000);
            if (!boss || !boss->IsAlive() || !boss->IsInCombat())
                return false;

            Unit* victim = boss->GetVictim();
            if (!victim)
                return false;

            // If victim has >= 3 stacks and bot is NOT the current victim, offtank taunts
            Aura* sunder = victim->GetAura(25051, EFFECT_INDEX_0);
            if (sunder && sunder->GetStackAmount() >= 3 && victim != bot)
                return true;

            return false;
        }
    };

    // ---------------------------------------------------------------------------
    // Boss 2: Grand Sorcerer Ardaeus (2000001)
    // ---------------------------------------------------------------------------
    class ArdaeusStartFightTrigger : public StartBossFightTrigger
    {
    public:
        ArdaeusStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start ardaeus fight", "ardaeus", 2000001) {}
    };

    class ArdaeusEndFightTrigger : public EndBossFightTrigger
    {
    public:
        ArdaeusEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end ardaeus fight", "ardaeus", 2000001) {}
    };

    // The Sun (2000019): Descending sun that wipes the raid if it reaches Z=18.5f.
    // Deals 25,000 damage to push it up by 5yd.
    class ArdaeusSunDescendingTrigger : public Trigger
    {
    public:
        ArdaeusSunDescendingTrigger(PlayerbotAI* ai) : Trigger(ai, "ardaeus sun descending", 1) {}

        bool IsActive() override
        {
            Unit* sun = GetRaidTargetByEntry(ai, 2000019);
            return sun && sun->IsAlive();
        }
    };

    // Statue Echo Adds (2000020 - 2000030): Spawned every 30s.
    class ArdaeusHasStatueAddTrigger : public Trigger
    {
    public:
        ArdaeusHasStatueAddTrigger(PlayerbotAI* ai) : Trigger(ai, "ardaeus has statue add", 1) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* c = ai->GetCreature(guid);
                if (c && c->IsAlive() && c->GetEntry() >= 2000020 && c->GetEntry() <= 2000030)
                    return true;
            }
            return false;
        }
    };

    // ---------------------------------------------------------------------------
    // Boss 3: High Inquisitor Mariella (2000002)
    // ---------------------------------------------------------------------------
    class MariellaStartFightTrigger : public StartBossFightTrigger
    {
    public:
        MariellaStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start mariella fight", "mariella", 2000002) {}
    };

    class MariellaEndFightTrigger : public EndBossFightTrigger
    {
    public:
        MariellaEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end mariella fight", "mariella", 2000002) {}
    };

    // Void Zone (2000016) / Kill Zone (2000018) hazard detection.
    class MariellaInVoidZoneTrigger : public Trigger
    {
    public:
        MariellaInVoidZoneTrigger(PlayerbotAI* ai) : Trigger(ai, "mariella in void zone", 1) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* c = ai->GetCreature(guid);
                if (c && c->IsAlive())
                {
                    if (c->GetEntry() == 2000016 && bot->GetDistance(c) < 3.5f)
                        return true;
                    if (c->GetEntry() == 2000018 && bot->GetDistance(c) < 6.0f)
                        return true;
                }
            }
            return false;
        }
    };

    // Felhounds (2000017): 4 spawn every 15s and channel Drain Mana (25676) for -1250 mana.
    class MariellaHasFelhoundsTrigger : public Trigger
    {
    public:
        MariellaHasFelhoundsTrigger(PlayerbotAI* ai) : Trigger(ai, "mariella has felhounds", 1) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* c = ai->GetCreature(guid);
                if (c && c->IsAlive() && c->GetEntry() == 2000017)
                    return true;
            }
            return false;
        }
    };

    // ---------------------------------------------------------------------------
    // Mini-Boss: Brother Eric Vesper (2000004)
    // ---------------------------------------------------------------------------
    class EricVesperStartFightTrigger : public StartBossFightTrigger
    {
    public:
        EricVesperStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start eric vesper fight", "eric vesper", 2000004) {}
    };

    class EricVesperEndFightTrigger : public EndBossFightTrigger
    {
    public:
        EricVesperEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end eric vesper fight", "eric vesper", 2000004) {}
    };

    // Energize channel / high mana detection: If mana hits 100%, Eric casts DoExplosion wiping the raid.
    class EricVesperEnergizingTrigger : public Trigger
    {
    public:
        EricVesperEnergizingTrigger(PlayerbotAI* ai) : Trigger(ai, "eric vesper energizing", 1) {}

        bool IsActive() override
        {
            Unit* eric = GetRaidTargetByEntry(ai, 2000004);
            if (!eric || !eric->IsAlive() || !eric->IsInCombat())
                return false;

            return eric->HasAura(25685) || (eric->GetPower(POWER_MANA) > (eric->GetMaxPower(POWER_MANA) * 0.4f));
        }
    };

    // ---------------------------------------------------------------------------
    // Mini-Boss: Darkcaller Rayn (2000037)
    // ---------------------------------------------------------------------------
    class DarkcallerRaynStartFightTrigger : public StartBossFightTrigger
    {
    public:
        DarkcallerRaynStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start darkcaller rayn fight", "darkcaller rayn", 2000037) {}
    };

    class DarkcallerRaynEndFightTrigger : public EndBossFightTrigger
    {
    public:
        DarkcallerRaynEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end darkcaller rayn fight", "darkcaller rayn", 2000037) {}
    };

    // ---------------------------------------------------------------------------
    // Boss 4: High General Abbendis (2000003)
    // ---------------------------------------------------------------------------
    class AbbendisStartFightTrigger : public StartBossFightTrigger
    {
    public:
        AbbendisStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start abbendis fight", "abbendis", 2000003) {}
    };

    class AbbendisEndFightTrigger : public EndBossFightTrigger
    {
    public:
        AbbendisEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end abbendis fight", "abbendis", 2000003) {}
    };

    // Abbendis frontal cleave turn check: Tank must turn boss away from the raid.
    class AbbendisNeedTurnBossTrigger : public Trigger
    {
    public:
        AbbendisNeedTurnBossTrigger(PlayerbotAI* ai) : Trigger(ai, "abbendis need turn boss", 2) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 2000003);
            if (!boss || !boss->IsAlive() || !boss->IsInCombat())
                return false;

            // Tank wants to position at wall (421.21f, -8.98f, 37.70f)
            return bot->GetDistance(421.21f, -8.98f, 37.70f) > 5.0f;
        }
    };
}
