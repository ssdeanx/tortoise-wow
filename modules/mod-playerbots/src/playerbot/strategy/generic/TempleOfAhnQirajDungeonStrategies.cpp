
#include "playerbot/playerbot.h"
#include "TempleOfAhnQirajDungeonStrategies.h"

using namespace ai;

// ---------------------------------------------------------------------------
// Temple of Ahn'Qiraj (AQ40, map 531). NPC and spell ids verified from
// src/scripts/dungeons/temple_of_ahnqiraj/*.cpp:
//   The Prophet Skeram 15263 (splits into 2 copies at 75/50/25%; Eye Tentacle
//     15726, Claw Tentacle 15725; True Fulfillment 785 mind-controls the
//     closest player)
//   Bug Trio: Kri 15511 / Yauj 15543 / Vem 15544 - a dead bug is devoured by
//     the survivors (full heal), so all three must die together; Yauj Brood
//     15621; leash if y < 2060 and x > -8600, no run-outs
//   Battleguard Sartura 15516 + Royal Guards 15984 (Whirlwind 26083 / 26038)
//   Fankriss 15510 (Mortal Wound 25646 stacks, Spawn of Fankriss 15630,
//     Vekniss Hatchling 15962; leash above y 1400)
//   Viscidus 15299 (freeze after 200 frost hits: 25926/26034/26036/25937;
//     Globs 15667; Toxin cloud trigger 15922)
//   Princess Huhuran 15509 (Frenzy 26051, Noxious Poison 26053)
//   Twin Emperors Vek'nilash 15275 (melee) / Vek'lor 15276 (caster) - swap
//     positions every 30-40 s, heal each other within 60 yd
//   Ouro 15517 (spawned from Ouro Spawner 15957; Dirt Mound 15712, Ouro
//     Scarab 15718; submerges for 90 s)
//   C'Thun: Eye of C'Thun 15589 -> body 15727; Eye/Claw Tentacles 15726/15725,
//     Giant Claw/Giant Eye 15728/15334, Flesh Tentacles 15802 in the stomach
//     (killing both opens the weakened burn window)
// ---------------------------------------------------------------------------

void TempleOfAhnQirajDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start skeram fight",
        NextAction::array(0, new NextAction("enable skeram fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start bug trio fight",
        NextAction::array(0, new NextAction("enable bug trio fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start sartura fight",
        NextAction::array(0, new NextAction("enable sartura fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start fankriss fight",
        NextAction::array(0, new NextAction("enable fankriss fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start viscidus fight",
        NextAction::array(0, new NextAction("enable viscidus fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start huhuran fight",
        NextAction::array(0, new NextAction("enable huhuran fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start twins fight",
        NextAction::array(0, new NextAction("enable twins fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start ouro fight",
        NextAction::array(0, new NextAction("enable ouro fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start cthun fight",
        NextAction::array(0, new NextAction("enable cthun fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Skeram: the eye and claw tentacles that keep spawning are burned while the
// raid fights the boss and its two images (all copies share entry 15263, so
// the default assist already covers them).
// ---------------------------------------------------------------------------

void SkeramFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // The boss and its two images share entry 15263 - re-acquire them after
    // every unison blink.
    triggers.push_back(new TriggerNode(
        "attackers entry::15263",
        NextAction::array(0, new NextAction("attack entry::15263,50", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15726",
        NextAction::array(0, new NextAction("attack entry::15726,50", 62.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15725",
        NextAction::array(0, new NextAction("attack entry::15725,50", 58.0f), NULL)));

    // True Fulfillment mind-controlled raid member: CC it, do not kill it.
    triggers.push_back(new TriggerNode(
        "fulfilled player",
        NextAction::array(0, new NextAction("cc fulfilled player", 95.0f), NULL)));

    // Arcane Explosion is arcane.
    triggers.push_back(new TriggerNode(
        "arcane protection potion ready",
        NextAction::array(0, new NextAction("arcane protection potion", 100.0f), NULL)));
}

void SkeramFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end skeram fight",
        NextAction::array(0, new NextAction("disable skeram fight strategy", 100.0f), NULL)));
}

void SkeramFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end skeram fight",
        NextAction::array(0, new NextAction("disable skeram fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Bug Trio: the raid spreads damage over all three so the survivors' devour
// does not undo the kill; Yauj Brood adds are cleaned up close by. No
// run-outs - the encounter leashes out of the room.
// ---------------------------------------------------------------------------

void BugTrioFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::15621",
        NextAction::array(0, new NextAction("attack entry::15621,25", 50.0f), NULL)));

    // Kri's death clouds pulse Toxic Vapors - walk out of them. Poison DoTs
    // are handled by the class cure strategies / anti-venom from "potions".
    triggers.push_back(new TriggerNode(
        "kri poison cloud",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));

    // Toxic Volley / Toxic Vapors are nature.
    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void BugTrioFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "kri poison cloud",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

void BugTrioFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end bug trio fight",
        NextAction::array(0, new NextAction("disable bug trio fight strategy", 100.0f), NULL)));
}

void BugTrioFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end bug trio fight",
        NextAction::array(0, new NextAction("disable bug trio fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Sartura: the Royal Guards are picked up by the tanks; melee steps out while
// Sartura or a guard is whirlwinding (the 15 yd search range leaves ranged
// bots where they are).
// ---------------------------------------------------------------------------

void SarturaFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // The raid kills the Royal Guards whenever they are not spinning.
    triggers.push_back(new TriggerNode(
        "attackers entry::15984",
        NextAction::array(0, new NextAction("attack entry::15984,40", 62.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "sartura whirlwind",
        NextAction::array(0, new NextAction("move away from sartura", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "sartura guard whirlwind",
        NextAction::array(0, new NextAction("move away from sartura guard", 100.0f), NULL)));
}

void SarturaFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end sartura fight",
        NextAction::array(0, new NextAction("disable sartura fight strategy", 100.0f), NULL)));
}

void SarturaFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end sartura fight",
        NextAction::array(0, new NextAction("disable sartura fight strategy", 100.0f), NULL)));
}

void SarturaFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "sartura whirlwind",
        NextAction::array(0, new NextAction("move away from sartura", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "sartura guard whirlwind",
        NextAction::array(0, new NextAction("move away from sartura guard", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Fankriss: Mortal Wound stacks force a tank swap; Spawn of Fankriss enrage
// quickly and are killed first; Hatchlings from the webs are cleaned up.
// ---------------------------------------------------------------------------

void FankrissFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "fankriss tank switch",
        NextAction::array(0, new NextAction("taunt fankriss", 95.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15630",
        NextAction::array(0, new NextAction("attack entry::15630,40", 65.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15962",
        NextAction::array(0, new NextAction("attack entry::15962,30", 50.0f), NULL)));
}

void FankrissFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end fankriss fight",
        NextAction::array(0, new NextAction("disable fankriss fight strategy", 100.0f), NULL)));
}

void FankrissFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end fankriss fight",
        NextAction::array(0, new NextAction("disable fankriss fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Viscidus: mages and shamans keep frost spells on him (200 frost hits freeze
// him, frost hits shatter him while frozen), everyone burns the globs after
// the shatter and walks out of the toxin clouds.
// ---------------------------------------------------------------------------

void ViscidusFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "viscidus frost assist",
        NextAction::array(0, new NextAction("viscidus frost assist", 65.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15667",
        NextAction::array(0, new NextAction("attack entry::15667,40", 80.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "viscidus toxin cloud",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));

    // Poison Shock / Poisonbolt Volley / Toxin Cloud are nature.
    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void ViscidusFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end viscidus fight",
        NextAction::array(0, new NextAction("disable viscidus fight strategy", 100.0f), NULL)));
}

void ViscidusFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end viscidus fight",
        NextAction::array(0, new NextAction("disable viscidus fight strategy", 100.0f), NULL)));
}

void ViscidusFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "viscidus toxin cloud",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Huhuran: Frenzy is stripped by the generic dispel-enrage reaction, Noxious
// Poison is healed through. No positioning mechanics in this core's script.
// ---------------------------------------------------------------------------

void HuhuranFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Wyvern Sting sleeps the tank - the offtank picks her up until it breaks.
    triggers.push_back(new TriggerNode(
        "huhuran wyvern stung tank",
        NextAction::array(0, new NextAction("taunt huhuran", 95.0f), NULL)));

    // Noxious Poison / Acid Spit / Wyvern Sting are nature.
    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void HuhuranFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end huhuran fight",
        NextAction::array(0, new NextAction("disable huhuran fight strategy", 100.0f), NULL)));
}

void HuhuranFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end huhuran fight",
        NextAction::array(0, new NextAction("disable huhuran fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Twin Emperors: melee burns Vek'nilash, ranged burns Vek'lor; the 120 yd
// range re-acquires the assigned twin after each swap.
// ---------------------------------------------------------------------------

void TwinsFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Fixed tank posts (two melee tanks + two warlocks, one group per side):
    // they hold their side and never chase a twin across the room. Every
    // position swap drops all threat, so the twin that lands on their side is
    // grabbed fresh.
    triggers.push_back(new TriggerNode(
        "twins tank post",
        NextAction::array(0, new NextAction("twins tank post", 70.0f), NULL)));

    // Vek'nilash is immune to magic: melee/physical tank + DPS only. Vek'lor is
    // the caster twin: a non-melee tank holds it at range (melee never enter its
    // 10 yd Arcane Burst range). The rest of the raid swaps sides with the
    // twins - melee with Vek'nilash, casters with Vek'lor.
    triggers.push_back(new TriggerNode(
        "attackers entry::15275",
        NextAction::array(0, new NextAction("attack veknilash::15275,120", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15276",
        NextAction::array(0, new NextAction("attack veklor::15276,120", 60.0f), NULL)));

    // Warlock tank: Searing Pain threat keeps Vek'lor on the warlock.
    if (bot->getClass() == CLASS_WARLOCK)
    {
        triggers.push_back(new TriggerNode(
            "attackers entry::15276",
            NextAction::array(0, new NextAction("searing pain veklor", 62.0f), NULL)));
    }

    // Shadowbolts: paladin shadow resist duty (one per subgroup) + shadow
    // protection potions for the whole raid.
    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "shadow resistance aura",
            NextAction::array(0, new NextAction("shadow resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "shadow protection potion ready",
        NextAction::array(0, new NextAction("shadow protection potion", 100.0f), NULL)));

    // The room's Qiraji bugs (15316/15317) are mutated and sent at the raid -
    // they are killed once they turn hostile.
    triggers.push_back(new TriggerNode(
        "attackers entry::15316",
        NextAction::array(0, new NextAction("attack entry::15316,30", 50.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15317",
        NextAction::array(0, new NextAction("attack entry::15317,30", 50.0f), NULL)));
}

void TwinsFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end twins fight",
        NextAction::array(0, new NextAction("disable twins fight strategy", 100.0f), NULL)));
}

void TwinsFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end twins fight",
        NextAction::array(0, new NextAction("disable twins fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Ouro: while submerged, Dirt Mounds and Ouro Scarabs are burned; Ouro is the
// default target once he re-emerges.
// ---------------------------------------------------------------------------

void OuroFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // The Dirt Mounds are not selectable - they follow a player, spawn scarabs
    // and despawn. The scarabs are the adds to kill (AoE is unlocked by this
    // fight strategy).
    triggers.push_back(new TriggerNode(
        "attackers entry::15718",
        NextAction::array(0, new NextAction("attack entry::15718,30", 55.0f), NULL)));

    // Sand Blast (26102) is nature.
    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void OuroFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ouro fight",
        NextAction::array(0, new NextAction("disable ouro fight strategy", 100.0f), NULL)));
}

void OuroFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ouro fight",
        NextAction::array(0, new NextAction("disable ouro fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// C'Thun: tentacles are the priority in both phases; a bot pulled into the
// stomach kills the Flesh Tentacles, which opens the weakened burn window for
// the rest of the raid.
// ---------------------------------------------------------------------------

void CthunFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // A swallowed bot runs to the punt platform; once it is standing there the
    // ranged bots keep hitting the two Flesh Tentacles until the punt takes
    // everyone back out.
    triggers.push_back(new TriggerNode(
        "cthun in stomach",
        NextAction::array(0, new NextAction("move to cthun stomach exit", 90.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15802",
        NextAction::array(0, new NextAction("attack flesh tentacle ranged::15802,50", 80.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15726",
        NextAction::array(0, new NextAction("attack entry::15726,50", 62.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15725",
        NextAction::array(0, new NextAction("attack entry::15725,50", 58.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15728",
        NextAction::array(0, new NextAction("attack entry::15728,50", 62.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15334",
        NextAction::array(0, new NextAction("attack entry::15334,50", 58.0f), NULL)));

    // Dark Glare / Mind Flay are shadow: paladin shadow resist duty + shadow
    // protection potions for the raid (priests' prayer of shadow protection is
    // part of their default buff rotation).
    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "shadow resistance aura",
            NextAction::array(0, new NextAction("shadow resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "shadow protection potion ready",
        NextAction::array(0, new NextAction("shadow protection potion", 100.0f), NULL)));
}

void CthunFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "cthun in stomach",
        NextAction::array(0, new NextAction("move to cthun stomach exit", 90.0f), NULL)));
}

void CthunFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end cthun fight",
        NextAction::array(0, new NextAction("disable cthun fight strategy", 100.0f), NULL)));
}

void CthunFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end cthun fight",
        NextAction::array(0, new NextAction("disable cthun fight strategy", 100.0f), NULL)));
}
