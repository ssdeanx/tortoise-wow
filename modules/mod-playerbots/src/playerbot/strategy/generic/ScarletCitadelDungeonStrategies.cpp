#include "playerbot/playerbot.h"
#include "ScarletCitadelDungeonStrategies.h"

using namespace ai;

void ScarletCitadelDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start daelus fight",
        NextAction::array(0, new NextAction("enable daelus fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start ardaeus fight",
        NextAction::array(0, new NextAction("enable ardaeus fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start mariella fight",
        NextAction::array(0, new NextAction("enable mariella fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start eric vesper fight",
        NextAction::array(0, new NextAction("enable eric vesper fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start darkcaller rayn fight",
        NextAction::array(0, new NextAction("enable darkcaller rayn fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start abbendis fight",
        NextAction::array(0, new NextAction("enable abbendis fight strategy", 100.0f), NULL)));
}

void ScarletCitadelDungeonStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Enable boss fight strategies before engaging when in range
    triggers.push_back(new TriggerNode(
        "start daelus fight",
        NextAction::array(0, new NextAction("enable daelus fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start ardaeus fight",
        NextAction::array(0, new NextAction("enable ardaeus fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start mariella fight",
        NextAction::array(0, new NextAction("enable mariella fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start eric vesper fight",
        NextAction::array(0, new NextAction("enable eric vesper fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start darkcaller rayn fight",
        NextAction::array(0, new NextAction("enable darkcaller rayn fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start abbendis fight",
        NextAction::array(0, new NextAction("enable abbendis fight strategy", 100.0f), NULL)));

    // Restock the protection potions out of combat. Scarlet Citadel (map 45)
    // stocks nature/shadow only - fire is deliberately not stocked.
    triggers.push_back(new TriggerNode(
        "ahnqiraj supplies needed",
        NextAction::array(0, new NextAction("give ahnqiraj supplies", 90.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Boss 1: Sacred Fist Daelus (2000000)
//
// Mechanics:
// 1. Tank tether: distance > 2.5f triggers raidwide 10% max HP/sec drain (21157).
// 2. Poison cloud: Green glow (22577) runs 25yd out to drop cloud safely.
// 3. Fallen Spirits (2000013): Regular spirits must be killed to prevent 5% heal.
//    The "Chosen One" spirit (red aura 26235) MUST be spared so it stuns Daelus!
// 4. Sunder Armor (25051): Tank swap at >= 3 stacks.
// ---------------------------------------------------------------------------
void DaelusFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // 1. Tank tether has highest priority - failure wipes raid (tanks only)
    if (ai->IsTank(bot))
    {
        triggers.push_back(new TriggerNode(
            "daelus need close tether",
            NextAction::array(0, new NextAction("daelus close tether", 100.0f), NULL)));

        // Sunder armor tank swap for offtanks
        triggers.push_back(new TriggerNode(
            "daelus sunder armor high",
            NextAction::array(0, new NextAction("daelus tank swap", 80.0f), NULL)));
    }

    // 2. Poison cloud carrier runs out of raid (any afflicted bot)
    triggers.push_back(new TriggerNode(
        "daelus poison cloud carrier",
        NextAction::array(0, new NextAction("daelus drop poison cloud", 95.0f), NULL)));

    // 3. Evacuate poison cloud
    triggers.push_back(new TriggerNode(
        "daelus in poison cloud",
        NextAction::array(0, new NextAction("move away from poison cloud", 90.0f), NULL)));

    // 4. Kill regular spirits (protecting Chosen One) - DPS focus
    if (!ai->IsTank(bot) && !ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "daelus has spirits",
            NextAction::array(0, new NextAction("attack daelus spirits", 85.0f), NULL)));
    }

    // 5. Attack Daelus
    triggers.push_back(new TriggerNode(
        "attackers entry::2000000",
        NextAction::array(0, new NextAction("attack entry::2000000", 65.0f), NULL)));

    // Nature protection potion for poison clouds
    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 50.0f), NULL)));
}

void DaelusFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end daelus fight",
        NextAction::array(0, new NextAction("disable daelus fight strategy", 100.0f), NULL)));
}

void DaelusFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end daelus fight",
        NextAction::array(0, new NextAction("disable daelus fight strategy", 100.0f), NULL)));
}

void DaelusFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    if (ai->IsTank(bot))
    {
        triggers.push_back(new TriggerNode(
            "daelus need close tether",
            NextAction::array(0, new NextAction("daelus close tether", 100.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "daelus poison cloud carrier",
        NextAction::array(0, new NextAction("daelus drop poison cloud", 95.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Boss 2: Grand Sorcerer Ardaeus (2000001)
//
// Mechanics:
// 1. Descending Sun (2000019): Deals 25k damage to elevate it back up into ceiling.
// 2. Statue echoes (2000020 - 2000030): Offtank picks up, DPS kills.
// ---------------------------------------------------------------------------
void ArdaeusFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // 1. Sun descending: burst DPS to prevent raid wipe (all DPS bots)
    if (!ai->IsTank(bot) && !ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "ardaeus sun descending",
            NextAction::array(0, new NextAction("attack ardaeus sun", 100.0f), NULL)));
    }

    // 2. Statue adds: offtanks taunt, DPS burns down
    if (ai->IsTank(bot))
    {
        triggers.push_back(new TriggerNode(
            "ardaeus has statue add",
            NextAction::array(0, new NextAction("taunt ardaeus statue", 85.0f), NULL)));
    }
    else if (!ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "ardaeus has statue add",
            NextAction::array(0, new NextAction("attack ardaeus statue", 80.0f), NULL)));
    }

    // 3. Attack Ardaeus
    triggers.push_back(new TriggerNode(
        "attackers entry::2000001",
        NextAction::array(0, new NextAction("attack entry::2000001", 65.0f), NULL)));

    // Fire protection potion for sun and solar bursts
    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 50.0f), NULL)));
}

void ArdaeusFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ardaeus fight",
        NextAction::array(0, new NextAction("disable ardaeus fight strategy", 100.0f), NULL)));
}

void ArdaeusFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ardaeus fight",
        NextAction::array(0, new NextAction("disable ardaeus fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Boss 3: High Inquisitor Mariella (2000002)
//
// Mechanics:
// 1. Void Zones (2000016) / Kill Zones (2000018): 3000 dmg / instant kill.
// 2. Felhounds (2000017): Mana drain -1250 per tick.
// 3. Shadow Volley (21341): Raidwide shadow damage.
// ---------------------------------------------------------------------------
void MariellaFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // 1. Evacuate void zones immediately
    triggers.push_back(new TriggerNode(
        "mariella in void zone",
        NextAction::array(0, new NextAction("move out of mariella void zone", 100.0f), NULL)));

    // 2. Felhounds: tanks taunt, DPS burns down
    if (ai->IsTank(bot))
    {
        triggers.push_back(new TriggerNode(
            "mariella has felhounds",
            NextAction::array(0, new NextAction("taunt mariella felhound", 90.0f), NULL)));
    }
    else if (!ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "mariella has felhounds",
            NextAction::array(0, new NextAction("attack mariella felhounds", 85.0f), NULL)));
    }

    // 3. Attack Mariella
    triggers.push_back(new TriggerNode(
        "attackers entry::2000002",
        NextAction::array(0, new NextAction("attack entry::2000002", 65.0f), NULL)));

    // Shadow protection potion for shadow volley
    triggers.push_back(new TriggerNode(
        "shadow protection potion ready",
        NextAction::array(0, new NextAction("shadow protection potion", 50.0f), NULL)));
}

void MariellaFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end mariella fight",
        NextAction::array(0, new NextAction("disable mariella fight strategy", 100.0f), NULL)));
}

void MariellaFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end mariella fight",
        NextAction::array(0, new NextAction("disable mariella fight strategy", 100.0f), NULL)));
}

void MariellaFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "mariella in void zone",
        NextAction::array(0, new NextAction("move out of mariella void zone", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Mini-Boss: Brother Eric Vesper (2000004)
//
// Mechanics:
// 1. Channels Energize (25685). At 100% mana casts DoExplosion wiping the dungeon!
// ---------------------------------------------------------------------------
void EricVesperFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "eric vesper energizing",
        NextAction::array(0, new NextAction("attack eric vesper", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::2000004",
        NextAction::array(0, new NextAction("attack entry::2000004", 65.0f), NULL)));

    // Nature protection potion for lightning waves
    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 50.0f), NULL)));
}

void EricVesperFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end eric vesper fight",
        NextAction::array(0, new NextAction("disable eric vesper fight strategy", 100.0f), NULL)));
}

void EricVesperFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end eric vesper fight",
        NextAction::array(0, new NextAction("disable eric vesper fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Mini-Boss: Darkcaller Rayn (2000037)
// ---------------------------------------------------------------------------
void DarkcallerRaynFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::2000037",
        NextAction::array(0, new NextAction("attack entry::2000037", 65.0f), NULL)));

    // Shadow protection potion for shadow volley
    triggers.push_back(new TriggerNode(
        "shadow protection potion ready",
        NextAction::array(0, new NextAction("shadow protection potion", 50.0f), NULL)));
}

void DarkcallerRaynFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end darkcaller rayn fight",
        NextAction::array(0, new NextAction("disable darkcaller rayn fight strategy", 100.0f), NULL)));
}

void DarkcallerRaynFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end darkcaller rayn fight",
        NextAction::array(0, new NextAction("disable darkcaller rayn fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Boss 4: High General Abbendis (2000003)
// ---------------------------------------------------------------------------
void AbbendisFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Tank positioning to turn boss away from raid
    if (ai->IsTank(bot))
    {
        triggers.push_back(new TriggerNode(
            "abbendis need turn boss",
            NextAction::array(0, new NextAction("tank abbendis", 95.0f), NULL)));
    }

    // Hunter tranquilizing shot on enrage
    if (bot->getClass() == CLASS_HUNTER)
    {
        triggers.push_back(new TriggerNode(
            "dispel enrage",
            NextAction::array(0, new NextAction("tranquilizing shot", 90.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "attackers entry::2000003",
        NextAction::array(0, new NextAction("attack entry::2000003", 65.0f), NULL)));
}

void AbbendisFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    if (ai->IsTank(bot))
    {
        triggers.push_back(new TriggerNode(
            "abbendis need turn boss",
            NextAction::array(0, new NextAction("tank abbendis", 20.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "end abbendis fight",
        NextAction::array(0, new NextAction("disable abbendis fight strategy", 100.0f), NULL)));
}

void AbbendisFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end abbendis fight",
        NextAction::array(0, new NextAction("disable abbendis fight strategy", 100.0f), NULL)));
}

void AbbendisFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    if (ai->IsTank(bot))
    {
        triggers.push_back(new TriggerNode(
            "abbendis need turn boss",
            NextAction::array(0, new NextAction("tank abbendis", 95.0f), NULL)));
    }

    if (bot->getClass() == CLASS_HUNTER)
    {
        triggers.push_back(new TriggerNode(
            "dispel enrage",
            NextAction::array(0, new NextAction("tranquilizing shot", ACTION_INTERRUPT), NULL)));
    }
}
