#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"

namespace ai
{
    class ZulGurubEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        ZulGurubEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter zul'gurub", "zul'gurub", 309) {}
    };

    class ZulGurubLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        ZulGurubLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave zul'gurub", "zul'gurub", 309) {}
    };

    // High Priestess Jeklik (14517): P1 bat form (Charge 24408, Sonic Burst
    // 23918, Swoop 23919, Pierce Armor 12097, Bloodseeker Bats 11368 every
    // 65 s); P2 (<50%) lands and threat-resets, then shadow spells + Frenzied
    // Bloodseeker Bats (14965) that bomb random players with liquid fire.
    class JeklikStartFightTrigger : public StartBossFightTrigger
    {
    public:
        JeklikStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start jeklik fight", "jeklik", 14517) {}
    };

    class JeklikEndFightTrigger : public EndBossFightTrigger
    {
    public:
        JeklikEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end jeklik fight", "jeklik", 14517) {}
    };

    // The P2 bat bombs (Throw Liquid Fire 23970 -> Inferno trap GO 177764, a
    // 10 yd fire trap, Flames 19628) land under a random player and are walked
    // out of. The generic trap-hazard trigger only covers trap GOs whose spell
    // is environmental damage; this one is a school-damage trap, so it gets a
    // per-fight hazard row.
    class JeklikInfernoTrigger : public CloseToGameObjectHazardTrigger
    {
    public:
        JeklikInfernoTrigger(PlayerbotAI* ai) : CloseToGameObjectHazardTrigger(ai, "jeklik inferno", 177764, 11.0f, 10) {}
    };

    // High Priest Venoxis (14507): P1 holy (Holy Nova 23858, Dispell 23859,
    // Holy Fire 23860, Renew 23895, Holy Wrath 23979); P2 (<50%) snake form +
    // threat reset (Poison Cloud 23861 persistent aura, Venom Spit 23862,
    // Parasitic Serpent 23865 -> Razzashi Serpent 14884). Frenzy 8269 at 20%.
    class VenoxisStartFightTrigger : public StartBossFightTrigger
    {
    public:
        VenoxisStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start venoxis fight", "venoxis", 14507) {}
    };

    class VenoxisEndFightTrigger : public EndBossFightTrigger
    {
    public:
        VenoxisEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end venoxis fight", "venoxis", 14507) {}
    };

    // High Priestess Mar'li (14510): alternates troll <-> spider every 35 s.
    // Troll: Poison Bolt Volley 24099 (nature), Drain Life 24300, Spawn of
    // Mar'li 15041 from the eggs 179985 (they gain +1 level every 3 s).
    // Spider: Enveloping Webs 24110 on the victim (root + silence + threat
    // -100) then Charge 22911 to the top aggro + threat reset, Corrosive Poison
    // 24111. The room trash (Razzashi Skitterer 14880, Razzashi Venombrood
    // 14532, Hakkari Shadowcaster 11338, Razzashi Broodwidow 11370) joins on
    // engage via the instance SetData - the DC pre-clears it (see
    // ZulGurubEvents.cpp) and the fight strategy keeps kill priorities for any
    // that were missed.
    class MarliStartFightTrigger : public StartBossFightTrigger
    {
    public:
        MarliStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start marli fight", "marli", 14510) {}
    };

    class MarliEndFightTrigger : public EndBossFightTrigger
    {
    public:
        MarliEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end marli fight", "marli", 14510) {}
    };

    // Marli's webs root the active tank and her charge resets threat: the
    // other tank taunts her off the webbed victim (the BWL Wing-Buffet shape).
    class MarliWebbedTankTrigger : public Trigger
    {
    public:
        MarliWebbedTankTrigger(PlayerbotAI* ai) : Trigger(ai, "marli webbed tank", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 14510);
            if (!boss || !boss->IsInCombat())
                return false;

            Unit* victim = boss->GetVictim();
            return victim && victim->HasAura(24110);
        }
    };

    // Bloodlord Mandokir (11382): pacified / immune to players until the
    // Vilebranch Speaker 11391 dies, then auto-aggro at 40 yd. Threatening
    // Gaze 24314 on a random player - the watched player must not move > 2 yd
    // or gain > 500 threat over the gaze snapshot, or is decapitated. Charge
    // 24408 + Fear 19134; Whirlwind 13736; Mortal Strike 16856 below 50%;
    // Enrage 24318 when Ohgan 14988 dies. Chained Spirits 15117 rez the dead.
    class MandokirStartFightTrigger : public StartBossFightTrigger
    {
    public:
        MandokirStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start mandokir fight", "mandokir", 11382) {}
    };

    class MandokirEndFightTrigger : public EndBossFightTrigger
    {
    public:
        MandokirEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end mandokir fight", "mandokir", 11382) {}
    };

    // The gaze aura itself: while it is up the watched bot holds position and
    // stops acting (see MandokirWatchAction).
    class MandokirWatchedTrigger : public Trigger
    {
    public:
        MandokirWatchedTrigger(PlayerbotAI* ai) : Trigger(ai, "mandokir watched", 1) {}

        bool IsActive() override { return bot->HasAura(24314); }
    };

    // High Priest Thekal (14599 -> 14509): the trio (Thekal, Lor'Khan 11347,
    // Zath 11348) resurrect each other ~7 s after a death unless all three die
    // inside that window; then Thekal revives in tiger form (entry 14509,
    // P2: Charge, Frenzy 24185, Force Punch 24189, Enrage 8269, Zulian
    // Guardians 15068). Multi-entry start so both forms are recognised.
    class ThekalStartFightTrigger : public Trigger
    {
    public:
        ThekalStartFightTrigger(PlayerbotAI* ai) : Trigger(ai, "start thekal fight", 5) {}

        bool IsActive() override
        {
            if (ai->HasStrategy("thekal", BotState::BOT_STATE_COMBAT))
                return false;

            for (const ObjectGuid& attackerGuid : AI_VALUE(std::list<ObjectGuid>, "attackers"))
            {
                Unit* attacker = ai->GetUnit(attackerGuid);
                if (!attacker)
                    continue;

                const uint32 entry = attacker->GetEntry();
                if (entry == 14599 || entry == 14509)
                    return true;
            }

            return false;
        }
    };

    class ThekalEndFightTrigger : public EndBossFightTrigger
    {
    public:
        ThekalEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end thekal fight", "thekal", 14509) {}
    };

    // True while any of the trio is attackable - drives the spread-damage
    // action (attack the member with the highest health percent so all three
    // fall inside the resurrection window).
    class ThekalTrioTrigger : public Trigger
    {
    public:
        ThekalTrioTrigger(PlayerbotAI* ai) : Trigger(ai, "thekal trio", 2) {}

        bool IsActive() override
        {
            for (uint32 entry : { 14599u, 11347u, 11348u })
            {
                Unit* member = GetRaidTargetByEntry(ai, entry);
                if (member && member->IsAlive())
                    return true;
            }

            return false;
        }
    };

    // High Priestess Arlokk (14515): summoned by the Gong of Bethekk (GO
    // 180526 - player action). Troll: SW:P 24212, Backstab 15582, Mark of
    // Arlokk 24210 (the Zulian Prowlers 15101 fixate the marked player), 2
    // prowlers every 5 s up to 30; vanishes at 35 s for 35-50 s. Panther
    // (45 s): Thrash / Ravage 24213 / Gouge (threat -80%) / Whirlwind 15589.
    class ArlokkStartFightTrigger : public StartBossFightTrigger
    {
    public:
        ArlokkStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start arlokk fight", "arlokk", 14515) {}
    };

    class ArlokkEndFightTrigger : public EndBossFightTrigger
    {
    public:
        ArlokkEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end arlokk fight", "arlokk", 14515) {}
    };

    // Jin'do the Hexxer (11380): Brain Wash Totem 15112 (rooted, AoE-immune,
    // mind-controls a random player with 24261), Powerful Healing Ward 14987
    // (heals him), Hex 17172, Delusions of Jin'do 24306 (curse -> Shade 14986
    // that ONLY the aura carrier can damage), Banish 24466 (teleport + 9
    // Sacrificed Trolls 14826). Needs >= 2 threat entries or he resets.
    class JindoStartFightTrigger : public StartBossFightTrigger
    {
    public:
        JindoStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start jindo fight", "jindo", 11380) {}
    };

    class JindoEndFightTrigger : public EndBossFightTrigger
    {
    public:
        JindoEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end jindo fight", "jindo", 11380) {}
    };

    // The Brain Wash totem's mind control (24261) marks a raid member the same
    // way Skeram's True Fulfillment / KT's Chains do - CC them, do not kill.
    class BrainwashedPlayerTrigger : public MindControlledAllyTrigger
    {
    public:
        BrainwashedPlayerTrigger(PlayerbotAI* ai) : MindControlledAllyTrigger(ai, "brainwashed player", 24261) {}
    };

    // The deluded player is the only one who can damage their Shade 14986; the
    // shade is invisible to everyone else (24307), but being in combat with it
    // is target knowledge, so the bot-specific gate is the curse aura.
    class JindoDelusionsTrigger : public Trigger
    {
    public:
        JindoDelusionsTrigger(PlayerbotAI* ai) : Trigger(ai, "jindo delusions", 2) {}

        bool IsActive() override
        {
            if (!bot->HasAura(24306))
                return false;

            return GetRaidTargetByEntry(ai, 14986) != nullptr;
        }
    };

    // Hakkar the Soulflayer (14834): Blood Siphon 24324 (raid stun + drain),
    // Corrupted Blood 24328, Cause Insanity 24327 (CHARM on the current victim
    // - CC'd, not killed, same handling as Skeram/KT), Berserk 27680 after
    // 10 min, and the five priest Aspects while the matching high priest
    // lives. Hakkar Power 24692 stacks = living priests.
    class HakkarStartFightTrigger : public StartBossFightTrigger
    {
    public:
        HakkarStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start hakkar fight", "hakkar", 14834) {}
    };

    class HakkarEndFightTrigger : public EndBossFightTrigger
    {
    public:
        HakkarEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end hakkar fight", "hakkar", 14834) {}
    };

    class InsanePlayerTrigger : public MindControlledAllyTrigger
    {
    public:
        InsanePlayerTrigger(PlayerbotAI* ai) : MindControlledAllyTrigger(ai, "insane player", 24327) {}
    };

    // Trash: the Hakkari Witch Doctor (11831) summons Jungle Toads 15010 and
    // spawns a Voodoo Spirit 15009 on death; the spirit is unselectable, walks
    // to the doctor's killer and explodes (Spirit Burst 24050) on contact. The
    // base hazard trigger exempts the creature's own victim, which is exactly
    // the chased player here, so lift that exclusion.
    class ZulGurubVoodooSpiritTrigger : public CloseToCreatureHazardTrigger
    {
    public:
        ZulGurubVoodooSpiritTrigger(PlayerbotAI* ai) : CloseToCreatureHazardTrigger(ai, "zulgurub voodoo spirit", 15009, 12.0f, 20) {}

    private:
        bool IsHazardValid(const ObjectGuid& hazardGuid) override
        {
            Creature* spirit = ai->GetCreature(hazardGuid);
            return spirit && sServerFacade.IsAlive(spirit);
        }
    };
}
