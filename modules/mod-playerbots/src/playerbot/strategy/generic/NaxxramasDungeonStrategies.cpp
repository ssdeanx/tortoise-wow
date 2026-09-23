
#include "playerbot/playerbot.h"
#include "NaxxramasDungeonStrategies.h"

using namespace ai;

// ---------------------------------------------------------------------------
// Naxxramas raid strategies (map 533). All spell/NPC ids verified from
// src/scripts/dungeons/naxxramas/*.cpp:
//   Anub'Rekhan 15956 (Crypt Guard 16573, Locust Swarm 28785, Impale 28783)
//   Faerlina 15953 (Worshippers 16506/Followers 16505, Enrage 28798)
//   Maexxna 15952 (Web Wrap web 16486, Spiderling 17055, Necrotic Poison 28776)
//   Noth 15954 (Champion 16983, Warrior 16984, Guardian 16981, Construct 16982)
//   Heigan 15936 (Plague Cloud 533002, Eruption 29371, Decrepit Fever 29998)
//   Loatheb 16011 (Spore 16286, Fungal Bloom 29232)
//   Razuvious 16061 - needs Mind Control; no bot strategy (deferred)
//   Gothik 16060 - two-side gate phase; no bot strategy (deferred)
//   Four Horsemen 16062-16065 (Void Zone 28863; strategy existed, fixed here)
//   Patchwerk 16028 - pure tank'n'spank, nothing needed
//   Grobbulus 15931 (Injection 28169, Fallout Slime 16290)
//   Gluth 15932 (Zombie Chow 16360, Mortal Wound 25646)
//   Thaddius 15928 (positive 28059 / negative 28084 charges)
//   Sapphiron 15989 (Blizzard 16474, Frost Aura 28529)
//   Kel'Thuzad 15990 (Mana Detonation 27819; adds 16427-16429)
// ---------------------------------------------------------------------------

void NaxxramasDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start anubrekhan fight",
        NextAction::array(0, new NextAction("enable anubrekhan fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start faerlina fight",
        NextAction::array(0, new NextAction("enable faerlina fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start maexxna fight",
        NextAction::array(0, new NextAction("enable maexxna fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start noth fight",
        NextAction::array(0, new NextAction("enable noth fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start heigan fight",
        NextAction::array(0, new NextAction("enable heigan fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start loatheb fight",
        NextAction::array(0, new NextAction("enable loatheb fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start four horseman fight",
        NextAction::array(0, new NextAction("enable four horseman fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start grobbulus fight",
        NextAction::array(0, new NextAction("enable grobbulus fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start gluth fight",
        NextAction::array(0, new NextAction("enable gluth fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start thaddius fight",
        NextAction::array(0, new NextAction("enable thaddius fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start sapphiron fight",
        NextAction::array(0, new NextAction("enable sapphiron fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start kelthuzad fight",
        NextAction::array(0, new NextAction("enable kelthuzad fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Anub'Rekhan: Crypt Guards 16573 held by offtanks; Locust Swarm 28785 -
// melee run out of the swarm radius while it channels.
// ---------------------------------------------------------------------------

void AnubRekhanFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    triggers.push_back(new TriggerNode(
        "attackers entry::16573",
        NextAction::array(0, new NextAction("offtank entry::16573", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "anub locust swarm",
        NextAction::array(0, new NextAction("move away from anubrekhan", 100.0f), NULL)));
}

void AnubRekhanFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end anubrekhan fight",
        NextAction::array(0, new NextAction("disable anubrekhan fight strategy", 100.0f), NULL)));
}

void AnubRekhanFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end anubrekhan fight",
        NextAction::array(0, new NextAction("disable anubrekhan fight strategy", 100.0f), NULL)));
}

void AnubRekhanFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "anub locust swarm",
        NextAction::array(0, new NextAction("move away from anubrekhan", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Faerlina: when she enrages (28798), kill a Worshipper (16506) - Widow's
// Embrace strips the enrage. Poison Bolt Volley is handled by class cures.
// ---------------------------------------------------------------------------

void FaerlinaFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "faerlina enraged",
        NextAction::array(0, new NextAction("attack entry::16506", 80.0f), NULL)));
}

void FaerlinaFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end faerlina fight",
        NextAction::array(0, new NextAction("disable faerlina fight strategy", 100.0f), NULL)));
}

void FaerlinaFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end faerlina fight",
        NextAction::array(0, new NextAction("disable faerlina fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Maexxna: Web Wrap webs (16486) trap players - attack the web to free them;
// Spiderlings (17055) are burned by DPS.
// ---------------------------------------------------------------------------

void MaexxnaFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::16486",
        NextAction::array(0, new NextAction("attack entry::16486", 85.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::17055",
        NextAction::array(0, new NextAction("attack entry::17055", 60.0f), NULL)));
}

void MaexxnaFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end maexxna fight",
        NextAction::array(0, new NextAction("disable maexxna fight strategy", 100.0f), NULL)));
}

void MaexxnaFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end maexxna fight",
        NextAction::array(0, new NextAction("disable maexxna fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Noth: Champion/Warrior adds die fast, Guardian/Construct held by tanks
// (they explode/deal heavy damage). Curse of Plaguebringer/Cripple decursed by
// class strategies.
// ---------------------------------------------------------------------------

void NothFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::16983",
        NextAction::array(0, new NextAction("attack entry::16983", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::16984",
        NextAction::array(0, new NextAction("attack entry::16984", 58.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::16981",
        NextAction::array(0, new NextAction("offtank entry::16981", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::16982",
        NextAction::array(0, new NextAction("offtank entry::16982", 55.0f), NULL)));
}

void NothFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end noth fight",
        NextAction::array(0, new NextAction("disable noth fight strategy", 100.0f), NULL)));
}

void NothFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end noth fight",
        NextAction::array(0, new NextAction("disable noth fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Heigan: the safety dance - Plague Clouds (533002) spawned by the fissures
// are dodged with the shared hazard movement.
// ---------------------------------------------------------------------------

void HeiganFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "heigan plague cloud",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

void HeiganFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end heigan fight",
        NextAction::array(0, new NextAction("disable heigan fight strategy", 100.0f), NULL)));
}

void HeiganFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end heigan fight",
        NextAction::array(0, new NextAction("disable heigan fight strategy", 100.0f), NULL)));
}

void HeiganFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "heigan plague cloud",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Loatheb: Spores (16286) are killed to trigger the Fungal Bloom (29232) crit
// window; only one at a time while the buff is not already up.
// ---------------------------------------------------------------------------

void LoathebFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "loatheb spore",
        NextAction::array(0, new NextAction("attack entry::16286", 65.0f), NULL)));
}

void LoathebFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end loatheb fight",
        NextAction::array(0, new NextAction("disable loatheb fight strategy", 100.0f), NULL)));
}

void LoathebFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end loatheb fight",
        NextAction::array(0, new NextAction("disable loatheb fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Grobbulus: Mutating Injection (28169) - run out of the raid, drop the
// poison cloud, return. Fallout Slimes (16290) held by tanks.
// ---------------------------------------------------------------------------

void GrobbulusFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "grobbulus injection",
        NextAction::array(0, new NextAction("move away from raid::30", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::16290",
        NextAction::array(0, new NextAction("offtank entry::16290", 55.0f), NULL)));
}

void GrobbulusFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end grobbulus fight",
        NextAction::array(0, new NextAction("disable grobbulus fight strategy", 100.0f), NULL)));
}

void GrobbulusFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end grobbulus fight",
        NextAction::array(0, new NextAction("disable grobbulus fight strategy", 100.0f), NULL)));
}

void GrobbulusFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "grobbulus injection",
        NextAction::array(0, new NextAction("move away from raid::30", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Gluth: Zombie Chows (16360) held by offtanks away from the boss; Mortal
// Wound (25646) stacks force a tank swap.
// ---------------------------------------------------------------------------

void GluthFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "gluth tank switch",
        NextAction::array(0, new NextAction("taunt gluth", 95.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::16360",
        NextAction::array(0, new NextAction("offtank entry::16360", 55.0f), NULL)));
}

void GluthFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end gluth fight",
        NextAction::array(0, new NextAction("disable gluth fight strategy", 100.0f), NULL)));
}

void GluthFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end gluth fight",
        NextAction::array(0, new NextAction("disable gluth fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Thaddius: polarity charges (positive 28059 / negative 28084) stack with the
// same charge - move next to the nearest same-charge raid member. Feugen
// (15930) and Stalagg (15929) are the phase 1 bosses.
// ---------------------------------------------------------------------------

void ThaddiusFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "thaddius polarity",
        NextAction::array(0, new NextAction("join thaddius polarity", 90.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15929",
        NextAction::array(0, new NextAction("attack entry::15929", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15930",
        NextAction::array(0, new NextAction("attack entry::15930", 60.0f), NULL)));
}

void ThaddiusFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end thaddius fight",
        NextAction::array(0, new NextAction("disable thaddius fight strategy", 100.0f), NULL)));
}

void ThaddiusFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end thaddius fight",
        NextAction::array(0, new NextAction("disable thaddius fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Sapphiron: Blizzard (NPC 16474) is dodged; frost resistance aura covers the
// raid wide frost damage.
// ---------------------------------------------------------------------------

void SapphironFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    triggers.push_back(new TriggerNode(
        "sapphiron blizzard",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "frost resistance aura",
            NextAction::array(0, new NextAction("frost resistance aura", 10.0f), NULL)));
    }
}

void SapphironFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end sapphiron fight",
        NextAction::array(0, new NextAction("disable sapphiron fight strategy", 100.0f), NULL)));
}

void SapphironFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end sapphiron fight",
        NextAction::array(0, new NextAction("disable sapphiron fight strategy", 100.0f), NULL)));
}

void SapphironFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "sapphiron blizzard",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Kel'Thuzad: Mana Detonation (27819) runs out of the raid; the add trio
// (Soldier 16427 / Abomination 16428 / Soul Weaver 16429) is burned as it
// spawns.
// ---------------------------------------------------------------------------

void KelThuzadFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "kelthuzad mana detonation",
        NextAction::array(0, new NextAction("move away from raid::20", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::16428",
        NextAction::array(0, new NextAction("offtank entry::16428", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::16427",
        NextAction::array(0, new NextAction("attack entry::16427", 58.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::16429",
        NextAction::array(0, new NextAction("attack entry::16429", 58.0f), NULL)));
}

void KelThuzadFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end kelthuzad fight",
        NextAction::array(0, new NextAction("disable kelthuzad fight strategy", 100.0f), NULL)));
}

void KelThuzadFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end kelthuzad fight",
        NextAction::array(0, new NextAction("disable kelthuzad fight strategy", 100.0f), NULL)));
}

void KelThuzadFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "kelthuzad mana detonation",
        NextAction::array(0, new NextAction("move away from raid::20", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Four Horsemen: Void Zone (28863) dodge (existing behavior, now with the
// corrected end-fight trigger name).
// ---------------------------------------------------------------------------

void FourHorsemanFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "void zone too close",
        NextAction::array(0, new NextAction("move away from void zone", 100.0f), NULL)));
}

void FourHorsemanFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end four horseman fight",
        NextAction::array(0, new NextAction("disable four horseman fight strategy", 100.0f), NULL)));
}

void FourHorsemanFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end four horseman fight",
        NextAction::array(0, new NextAction("disable four horseman fight strategy", 100.0f), NULL)));
}
