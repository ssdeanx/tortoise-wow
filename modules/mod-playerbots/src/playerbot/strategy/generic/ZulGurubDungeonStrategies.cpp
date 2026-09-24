
#include "playerbot/playerbot.h"
#include "ZulGurubDungeonStrategies.h"

using namespace ai;

// ---------------------------------------------------------------------------
// Zul'Gurub (ZG, map 309). NPC and spell ids verified from
// src/scripts/dungeons/zulgurub/*.cpp plus event_scripts / gameobject_template
// / spell_template in tw_world:
//   Jeklik 14517 (bat P1 -> troll P2; Bloodseeker Bats 11368, Frenzied
//     Bloodseeker Bats 14965 -> Inferno trap GO 177764)
//   Venoxis 14507 (holy P1 -> snake P2; Parasitic Serpent 14884)
//   Mar'li 14510 (troll <-> spider; Spawn of Mar'li 15041; room trash 14880 /
//     14532 / 11338 / 11370 joins on engage - DC pre-clear in
//     mod-dungeon-clear Data/Events/ZulGurubEvents.cpp)
//   Mandokir 11382 + Ohgan 14988 + Chained Spirits 15117 (gated by the
//     Vilebranch Speaker 11391; Threatening Gaze 24314)
//   Thekal 14599 -> 14509 + Lor'Khan 11347 + Zath 11348 (resurrection window;
//     Zulian Guardians 15068 in P2)
//   Arlokk 14515 (summoned by the Gong of Bethekk 180526 - player action;
//     Zulian Prowlers 15101)
//   Jin'do 11380 (Brain Wash Totem 15112, Healing Ward 14987, Shade 14986,
//     Sacrificed Trolls 14826)
//   Hakkar 14834 (Cause Insanity 24327, Corrupted Blood 24328, Blood Siphon
//     24324, priest Aspects 24686-24690)
//   Gahz'ranka 15114 and the Edge of Madness bosses are deliberately not
//     covered (summon-only via fishing / not spawnable in tw_world).
// ---------------------------------------------------------------------------

void ZulGurubDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start jeklik fight",
        NextAction::array(0, new NextAction("enable jeklik fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start venoxis fight",
        NextAction::array(0, new NextAction("enable venoxis fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start marli fight",
        NextAction::array(0, new NextAction("enable marli fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start mandokir fight",
        NextAction::array(0, new NextAction("enable mandokir fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start thekal fight",
        NextAction::array(0, new NextAction("enable thekal fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start arlokk fight",
        NextAction::array(0, new NextAction("enable arlokk fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start jindo fight",
        NextAction::array(0, new NextAction("enable jindo fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start hakkar fight",
        NextAction::array(0, new NextAction("enable hakkar fight strategy", 100.0f), NULL)));

    // Trash: the Voodoo Spirit (15009) is unselectable, walks to the Hakkari
    // Witch Doctor's killer and explodes (Spirit Burst 24050) on contact.
    triggers.push_back(new TriggerNode(
        "zulgurub voodoo spirit",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));

    // Trash: the Witch Doctor summons Jungle Toads and leaves a Voodoo Spirit
    // on death, so it is burned before the rest of its pack.
    triggers.push_back(new TriggerNode(
        "attackers entry::11831",
        NextAction::array(0, new NextAction("attack entry::11831,30", 55.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Jeklik: the P1 bat waves are the only adds; the P2 Inferno bombs are walked
// out of. Great Heal is a 4 s interruptible cast (no class rotation
// prioritizes enemy heals in v1 - watch item); Curse of Blood is a stock
// decurse; the shadow spells are single-target and healed through.
// ---------------------------------------------------------------------------

void JeklikFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::11368",
        NextAction::array(0, new NextAction("attack entry::11368,30", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "jeklik inferno",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

void JeklikFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end jeklik fight",
        NextAction::array(0, new NextAction("disable jeklik fight strategy", 100.0f), NULL)));
}

void JeklikFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end jeklik fight",
        NextAction::array(0, new NextAction("disable jeklik fight strategy", 100.0f), NULL)));
}

void JeklikFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "jeklik inferno",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Venoxis: P2 parasitic serpents are killed on sight; the P2 poison (and the
// death cloud) is nature - potion + the dropper shaman's nature totem. The
// Poison Cloud is a persistent area aura on the victim with no entity to dodge
// in v1 - heal through, validate. The P2 threat reset is recovered by the
// generic tank `lose aggro` taunt.
// ---------------------------------------------------------------------------

void VenoxisFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::14884",
        NextAction::array(0, new NextAction("attack entry::14884,30", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void VenoxisFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end venoxis fight",
        NextAction::array(0, new NextAction("disable venoxis fight strategy", 100.0f), NULL)));
}

void VenoxisFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end venoxis fight",
        NextAction::array(0, new NextAction("disable venoxis fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Mar'li: the Spawn of Mar'li grow +1 level every 3 s and are burned first;
// the room trash gets kill priorities as a fallback behind the DC pre-clear.
// The web + charge is a scripted threat reset - the other tank taunts her off
// the webbed victim. Nature potion + nature totem.
// ---------------------------------------------------------------------------

void MarliFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::15041",
        NextAction::array(0, new NextAction("attack entry::15041,30", 70.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14880",
        NextAction::array(0, new NextAction("attack entry::14880,30", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14532",
        NextAction::array(0, new NextAction("attack entry::14532,30", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::11338",
        NextAction::array(0, new NextAction("attack entry::11338,30", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::11370",
        NextAction::array(0, new NextAction("attack entry::11370,30", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "marli webbed tank",
        NextAction::array(0, new NextAction("taunt marli", 95.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void MarliFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end marli fight",
        NextAction::array(0, new NextAction("disable marli fight strategy", 100.0f), NULL)));
}

void MarliFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end marli fight",
        NextAction::array(0, new NextAction("disable marli fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Mandokir: the watched bot holds still and stops acting (the gaze kills on
// movement or threat); Ohgan is burned first (Mandokir's Enrage is stripped by
// the generic dispel-enrage reaction); the Chained Spirits are free rezzes.
// ---------------------------------------------------------------------------

void MandokirFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "mandokir watched",
        NextAction::array(0, new NextAction("mandokir watch", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14988",
        NextAction::array(0, new NextAction("attack entry::14988,30", 60.0f), NULL)));
}

void MandokirFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end mandokir fight",
        NextAction::array(0, new NextAction("disable mandokir fight strategy", 100.0f), NULL)));
}

void MandokirFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end mandokir fight",
        NextAction::array(0, new NextAction("disable mandokir fight strategy", 100.0f), NULL)));
}

void MandokirFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "mandokir watched",
        NextAction::array(0, new NextAction("mandokir watch", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Thekal: all three of the trio must die inside the ~7 s resurrection window.
// The spread-damage action targets the alive member with the highest health
// percent so the raid keeps them level and the deaths cluster; the phase-2
// tiger (14509) is the fallback target once the zealots are gone, and the
// Zulian Guardians are burned on sight.
// ---------------------------------------------------------------------------

void ThekalFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "thekal trio",
        NextAction::array(0, new NextAction("attack thekal trio", 75.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14509",
        NextAction::array(0, new NextAction("attack entry::14509,50", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15068",
        NextAction::array(0, new NextAction("attack entry::15068,30", 50.0f), NULL)));
}

void ThekalFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end thekal fight",
        NextAction::array(0, new NextAction("disable thekal fight strategy", 100.0f), NULL)));
}

void ThekalFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end thekal fight",
        NextAction::array(0, new NextAction("disable thekal fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Arlokk: the panther swarm is cleared by the prowler kill priority plus the
// default class AoE rotations (density triggers). The marked player keeps
// fighting in v1 - a kite action only if validation shows the swarm wins.
// The gong stays a player action (no bot GO use in this fight).
// ---------------------------------------------------------------------------

void ArlokkFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::15101",
        NextAction::array(0, new NextAction("attack entry::15101,30", 55.0f), NULL)));
}

void ArlokkFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end arlokk fight",
        NextAction::array(0, new NextAction("disable arlokk fight strategy", 100.0f), NULL)));
}

void ArlokkFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end arlokk fight",
        NextAction::array(0, new NextAction("disable arlokk fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Jin'do: the Brain Wash Totem is AoE-immune and mind-controls a player, so it
// is the top single-target priority; the Healing Ward is next; the brainwashed
// player is CC'd (not killed); the deluded player kills their own shade; the
// Banished player's skeletons are cleaned up. The Delusions curse must NOT be
// dispelled - see the open items (the stock decurse will remove it).
// ---------------------------------------------------------------------------

void JindoFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::15112",
        NextAction::array(0, new NextAction("attack entry::15112,40", 90.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14987",
        NextAction::array(0, new NextAction("attack entry::14987,40", 80.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "brainwashed player",
        NextAction::array(0, new NextAction("cc brainwashed player", 95.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "jindo delusions",
        NextAction::array(0, new NextAction("attack shade of jindo::14986,40", 70.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14826",
        NextAction::array(0, new NextAction("attack entry::14826,30", 50.0f), NULL)));
}

void JindoFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end jindo fight",
        NextAction::array(0, new NextAction("disable jindo fight strategy", 100.0f), NULL)));
}

void JindoFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end jindo fight",
        NextAction::array(0, new NextAction("disable jindo fight strategy", 100.0f), NULL)));
}

void JindoFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "brainwashed player",
        NextAction::array(0, new NextAction("cc brainwashed player", 95.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Hakkar: the insane player is CC'd (Cause Insanity is a charm); Corrupted
// Blood / Blood Siphon are shadow - potion; the priests are killed first (the
// DC order) so the Aspects and Hakkar Power stacks are gone.
// ---------------------------------------------------------------------------

void HakkarFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "insane player",
        NextAction::array(0, new NextAction("cc insane player", 95.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "shadow protection potion ready",
        NextAction::array(0, new NextAction("shadow protection potion", 100.0f), NULL)));
}

void HakkarFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end hakkar fight",
        NextAction::array(0, new NextAction("disable hakkar fight strategy", 100.0f), NULL)));
}

void HakkarFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end hakkar fight",
        NextAction::array(0, new NextAction("disable hakkar fight strategy", 100.0f), NULL)));
}

void HakkarFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "insane player",
        NextAction::array(0, new NextAction("cc insane player", 95.0f), NULL)));
}
