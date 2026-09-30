#include "playerbot/playerbot.h"
#include "BlackwingLairDungeonStrategies.h"

using namespace ai;

void BlackwingLairDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start razorgore fight",
        NextAction::array(0, new NextAction("enable razorgore fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start vael fight",
        NextAction::array(0, new NextAction("enable vael fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start broodlord fight",
        NextAction::array(0, new NextAction("enable broodlord fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start firemaw fight",
        NextAction::array(0, new NextAction("enable firemaw fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start ebonroc fight",
        NextAction::array(0, new NextAction("enable ebonroc fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start flamegor fight",
        NextAction::array(0, new NextAction("enable flamegor fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start chromaggus fight",
        NextAction::array(0, new NextAction("enable chromaggus fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start nefarian fight",
        NextAction::array(0, new NextAction("enable nefarian fight strategy", 100.0f), NULL)));

    // Shadow Flame dot (22682) auto-removed across all dragons in BWL (Ebonroc, Flamegor, Nefarian)
    // without needing Onyxia Scale Cloak (15138) on all 40 bots.
    triggers.push_back(new TriggerNode(
        "shadow flame dot",
        NextAction::array(0, new NextAction("clear shadow flame dot", 100.0f), NULL)));

    // Suppression room turns on when a device is in sight, off once none are.
    triggers.push_back(new TriggerNode(
        "suppression room entered",
        NextAction::array(0, new NextAction("enable suppression room strategy", 99.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression device close",
        NextAction::array(0, new NextAction("disarm suppression device", 80.0f), NULL)));
}

void BlackwingLairDungeonStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "suppression room entered",
        NextAction::array(0, new NextAction("enable suppression room strategy", 99.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression room left",
        NextAction::array(0, new NextAction("disable suppression room strategy", 99.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression device need stealth",
        NextAction::array(0, new NextAction("stealth for suppression device", ACTION_HIGH + 3), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression device in sight",
        NextAction::array(0, new NextAction("move to suppression device", ACTION_HIGH + 2), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression device close",
        NextAction::array(0, new NextAction("disarm suppression device", ACTION_HIGH + 4), NULL)));
}

class SuppressionRoomPassiveMultiplier : public Multiplier
{
public:
    SuppressionRoomPassiveMultiplier(PlayerbotAI* ai) : Multiplier(ai, "suppression room passive") {}

    float GetValue(Action* action) override
    {
        if (!action)
            return 1.0f;

        if (ai->GetBot()->getClass() != CLASS_ROGUE)
            return 1.0f;

        const std::string& name = action->getName();

        // Enable only the following strats for suppression room to avoid regular combat breaking logic
        if (name == "stealth for suppression device" ||
            name == "move to suppression device" ||
            name == "disarm suppression device" ||
            name == "deactivate suppression device")
        {
            return 1.0f;
        }

        if (name == "stealth" ||
            name == "unstealth" ||
            name == "check stealth" ||
            name == "sprint" ||
            name == "vanish")
        {
            return 1.0f;
        }

        if (name == "co" ||
            name == "nc" ||
            name == "load ai" ||
            name == "save ai" ||
            name == "list ai" ||
            name == "reset ai" ||
            name == "reset strats" ||
            name == "reset values" ||
            name == "check mount state" ||
            name == "accept invitation" ||
            name == "set combat state" ||
            name == "set non combat state" ||
            name == "set dead state" ||
            name == "update pvp strats" ||
            name == "update pve strats" ||
            name == "update raid strats" ||
            name == "loot roll" ||
            name == "auto loot roll" ||
            name == "follow" ||
            name == "stay" ||
            name == "food" ||
            name == "drink")
        {
            return 1.0f;
        }

        return 0.0f;
    }
};

void SuppressionRoomStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "suppression device need stealth",
        NextAction::array(0, new NextAction("vanish", ACTION_EMERGENCY + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression device in sight",
        NextAction::array(0, new NextAction("move to suppression device", ACTION_HIGH + 8), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression device close",
        NextAction::array(0, new NextAction("disarm suppression device", 90.0f), NULL)));
}

void SuppressionRoomStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "suppression device need stealth",
        NextAction::array(0, new NextAction("stealth for suppression device", ACTION_MOVE), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression device in sight",
        NextAction::array(0, new NextAction("move to suppression device", ACTION_HIGH + 8), NULL)));

    triggers.push_back(new TriggerNode(
        "suppression device close",
        NextAction::array(0, new NextAction("disarm suppression device", ACTION_MOVE + 2), NULL)));
}

void SuppressionRoomStrategy::InitCombatMultipliers(std::list<Multiplier*>& multipliers)
{
    multipliers.push_back(new SuppressionRoomPassiveMultiplier(ai));
}

void SuppressionRoomStrategy::InitNonCombatMultipliers(std::list<Multiplier*>& multipliers)
{
    multipliers.push_back(new SuppressionRoomPassiveMultiplier(ai));
}

void SuppressionRoomStrategy::OnStrategyAdded(BotState state)
{
    // The room freezes the rogue's whole rotation down to the disarm loop.
    if (state == BotState::BOT_STATE_COMBAT && ai->GetBot()->getClass() == CLASS_ROGUE)
    {
        ai->ChangeStrategy("-avoid aoe", BotState::BOT_STATE_COMBAT);
        ai->ChangeStrategy("-avoid aoe", BotState::BOT_STATE_NON_COMBAT);
        ai->ChangeStrategy("-avoid aoe", BotState::BOT_STATE_REACTION);
        ai->ChangeStrategy("-avoid mobs", BotState::BOT_STATE_COMBAT);
        ai->ChangeStrategy("-avoid mobs", BotState::BOT_STATE_NON_COMBAT);
        ai->ChangeStrategy("-avoid mobs", BotState::BOT_STATE_REACTION);
    }
}

// ---------------------------------------------------------------------------
// Razorgore (12435) - full phase 1 add list (boss_razorgore.cpp verified):
// static gate mobs Grethok 12557 + Blackwing Guards 14456, then timed waves of
// Blackwing Legionnaires 12416, Blackwing Mages 12420, and Death Talon
// Dragonspawn 12422. Kill order: Mages first (they channel heavy AoE), then
// Legionnaires (AoE down in packs); Dragonspawn are held away from the eggs by
// offtanks; the PLAYER controls Razorgore through the Orb. Bots never attack
// Razorgore until Warming Flames (23040) marks the burn phase.
// ---------------------------------------------------------------------------

void RazorgoreFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Phase 2: Burning Razorgore once all 30 eggs are destroyed (Warming Flames 23040 active).
    triggers.push_back(new TriggerNode(
        "razorgore warming flames",
        NextAction::array(0, new NextAction("attack entry::12435", 95.0f), NULL)));

    // Phase 2: Conflagration tank swap - offtank taunts when current tank is disoriented.
    triggers.push_back(new TriggerNode(
        "razorgore conflagration tank switch",
        NextAction::array(0, new NextAction("taunt razorgore", 90.0f), NULL)));

    // Phase 1: Druids put Death Talon Dragonspawn (12422) to sleep with Hibernate.
    triggers.push_back(new TriggerNode(
        "razorgore need sleep dragonkin",
        NextAction::array(0, new NextAction("razorgore sleep dragonkin", 75.0f), NULL)));

    // Phase 1: Each corner defends its post and kills corner adds (Mages > Legionnaires > unslept Dragonspawn).
    triggers.push_back(new TriggerNode(
        "razorgore has corner add",
        NextAction::array(0, new NextAction("attack razorgore corner add", 65.0f), NULL)));

    // Phase 1: If out of position and no immediate threats, move to assigned corner post.
    triggers.push_back(new TriggerNode(
        "razorgore need move to corner",
        NextAction::array(0, new NextAction("move to razorgore corner", 40.0f), NULL)));

    // Fallbacks if adds wander outside corner boundaries:
    triggers.push_back(new TriggerNode(
        "attackers entry::12420",
        NextAction::array(0, new NextAction("attack entry::12420,35", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::12557",
        NextAction::array(0, new NextAction("attack entry::12557", 53.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14456",
        NextAction::array(0, new NextAction("attack entry::14456", 52.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::12416",
        NextAction::array(0, new NextAction("attack entry::12416,35", 50.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::12422",
        NextAction::array(0, new NextAction("offtank entry::12422,35", 48.0f), NULL)));
}

void RazorgoreFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Spread to assigned corners before / between waves
    triggers.push_back(new TriggerNode(
        "razorgore need move to corner",
        NextAction::array(0, new NextAction("move to razorgore corner", 20.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "end razorgore fight",
        NextAction::array(0, new NextAction("disable razorgore fight strategy", 100.0f), NULL)));
}

void RazorgoreFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end razorgore fight",
        NextAction::array(0, new NextAction("disable razorgore fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Vaelastrasz (13020) - Burning Adrenaline (23620) run-out in combat + reaction.
// ---------------------------------------------------------------------------

void VaelFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Burning Adrenaline: afflicted bot runs away from the raid before exploding
    triggers.push_back(new TriggerNode(
        "vael burning adrenaline",
        NextAction::array(0, new NextAction("move away from raid::25", 100.0f), NULL)));

    // Burning Adrenaline tank swap: offtank immediately taunts Vaelastrasz
    triggers.push_back(new TriggerNode(
        "vael tank switch",
        NextAction::array(0, new NextAction("taunt vael", 95.0f), NULL)));
}

void VaelFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end vael fight",
        NextAction::array(0, new NextAction("disable vael fight strategy", 100.0f), NULL)));
}

void VaelFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end vael fight",
        NextAction::array(0, new NextAction("disable vael fight strategy", 100.0f), NULL)));
}

void VaelFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "vael burning adrenaline",
        NextAction::array(0, new NextAction("move away from raid::25", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Broodlord Lashlayer (12017) - Knock Away (18670) wipes active tank threat -> offtanks taunt.
// Mortal Strike (16856) requires strong healing; Blast Wave (23331) slows/damages melee.
// ---------------------------------------------------------------------------

void BroodlordFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Knock Away threat wipe -> offtanks taunt the boss.
    triggers.push_back(new TriggerNode(
        "broodlord tank switch",
        NextAction::array(0, new NextAction("taunt broodlord", 95.0f), NULL)));
}

void BroodlordFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end broodlord fight",
        NextAction::array(0, new NextAction("disable broodlord fight strategy", 100.0f), NULL)));
}

void BroodlordFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end broodlord fight",
        NextAction::array(0, new NextAction("disable broodlord fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Firemaw (11983) - Flame Buffet 23341 hits the whole raid; fire resist duty +
// fire pot; Wing Buffet -> offtank re-attacks to take the next tank slot.
// ---------------------------------------------------------------------------

void FiremawFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Wing Buffet threat wipe -> offtanks taunt the boss.
    triggers.push_back(new TriggerNode(
        "firemaw tank switch",
        NextAction::array(0, new NextAction("taunt firemaw", 95.0f), NULL)));

    // Inactive tanks stay out of line of sight (no Flame Buffet stacks while
    // the main tank holds the boss).
    triggers.push_back(new TriggerNode(
        "firemaw inactive tank",
        NextAction::array(0, new NextAction("hide from firemaw", 92.0f), NULL)));

    // Non-tanks hide behind the pillar once they stack up, so Flame Buffet
    // falls off and they return to fight.
    triggers.push_back(new TriggerNode(
        "firemaw flame buffet stacks",
        NextAction::array(0, new NextAction("hide from firemaw", 90.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "fire resistance aura",
            NextAction::array(0, new NextAction("fire resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 100.0f), NULL)));
}

void FiremawFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end firemaw fight",
        NextAction::array(0, new NextAction("disable firemaw fight strategy", 100.0f), NULL)));
}

void FiremawFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end firemaw fight",
        NextAction::array(0, new NextAction("disable firemaw fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Ebonroc (14601) - Shadow Flame cone + Shadow of Ebonroc (23340) tank curse;
// shadow resist duty + shadow pot; Wing Buffet pickup.
// ---------------------------------------------------------------------------

void EbonrocFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Wing Buffet threat wipe -> offtanks taunt the boss.
    triggers.push_back(new TriggerNode(
        "ebonroc tank switch",
        NextAction::array(0, new NextAction("taunt ebonroc", 95.0f), NULL)));

    // Inactive tanks stay out of line of sight.
    triggers.push_back(new TriggerNode(
        "ebonroc inactive tank",
        NextAction::array(0, new NextAction("hide from ebonroc", 92.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "shadow resistance aura",
            NextAction::array(0, new NextAction("shadow resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "shadow protection potion ready",
        NextAction::array(0, new NextAction("shadow protection potion", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "shadow flame dot",
        NextAction::array(0, new NextAction("clear shadow flame dot", 100.0f), NULL)));
}

void EbonrocFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ebonroc fight",
        NextAction::array(0, new NextAction("disable ebonroc fight strategy", 100.0f), NULL)));
}

void EbonrocFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ebonroc fight",
        NextAction::array(0, new NextAction("disable ebonroc fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Flamegor (11981) - Frenzy 23342 (tranq handled by the hunter's dispel enrage
// reaction); fire resist duty + fire pot; Wing Buffet -> offtank re-attacks.
// ---------------------------------------------------------------------------

void FlamegorFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Wing Buffet threat wipe -> offtanks taunt the boss.
    triggers.push_back(new TriggerNode(
        "flamegor tank switch",
        NextAction::array(0, new NextAction("taunt flamegor", 95.0f), NULL)));

    // Inactive tanks stay out of line of sight.
    triggers.push_back(new TriggerNode(
        "flamegor inactive tank",
        NextAction::array(0, new NextAction("hide from flamegor", 92.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "fire resistance aura",
            NextAction::array(0, new NextAction("fire resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "shadow flame dot",
        NextAction::array(0, new NextAction("clear shadow flame dot", 100.0f), NULL)));
}

void FlamegorFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end flamegor fight",
        NextAction::array(0, new NextAction("disable flamegor fight strategy", 100.0f), NULL)));
}

void FlamegorFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end flamegor fight",
        NextAction::array(0, new NextAction("disable flamegor fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Chromaggus (14020) - Brood Affliction: Bronze (23170) direct clear
// (afflictions are dispel-immune); Frenzy handled by the hunter's generic
// "dispel enrage" reaction.
// ---------------------------------------------------------------------------

void ChromaggusFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Time Lapse (23310) wipes the active tank's threat -> offtank taunts.
    triggers.push_back(new TriggerNode(
        "chromaggus time lapse",
        NextAction::array(0, new NextAction("taunt chromaggus", 95.0f), NULL)));

    // Brood Affliction: Bronze (23170) direct clear (afflictions are
    // dispel-immune); Frenzy handled by the hunter's dispel enrage reaction.
    triggers.push_back(new TriggerNode(
        "chromaggus bronze affliction",
        NextAction::array(0, new NextAction("clear chromaggus bronze", 100.0f), NULL)));
}

void ChromaggusFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end chromaggus fight",
        NextAction::array(0, new NextAction("disable chromaggus fight strategy", 100.0f), NULL)));
}

void ChromaggusFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end chromaggus fight",
        NextAction::array(0, new NextAction("disable chromaggus fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Nefarian (11583) - full fight strat: corrupted totems (14662-14666), phase 3
// Bone Constructs (14605), drakonid add waves (14261-14265 + chromatic 14302)
// peeled by offtanks. Shadow resist duty + shadow pot for Shadowflame / Veil.
// ---------------------------------------------------------------------------

void NefarianFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Corrupted totems first (Stoneskin / Healing Stream / Windfury / Fire Nova).
    triggers.push_back(new TriggerNode(
        "attackers entry::14663",
        NextAction::array(0, new NextAction("attack entry::14663", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14664",
        NextAction::array(0, new NextAction("attack entry::14664", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14666",
        NextAction::array(0, new NextAction("attack entry::14666", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14662",
        NextAction::array(0, new NextAction("attack entry::14662", 60.0f), NULL)));

    // Phase 3 Bone Constructs at dead raid members' positions.
    triggers.push_back(new TriggerNode(
        "attackers entry::14605",
        NextAction::array(0, new NextAction("attack entry::14605", 70.0f), NULL)));

    // Drakonid add waves - offtanks peel them.
    triggers.push_back(new TriggerNode(
        "attackers entry::14261",
        NextAction::array(0, new NextAction("offtank entry::14261", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14262",
        NextAction::array(0, new NextAction("offtank entry::14262", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14263",
        NextAction::array(0, new NextAction("offtank entry::14263", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14264",
        NextAction::array(0, new NextAction("offtank entry::14264", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14265",
        NextAction::array(0, new NextAction("offtank entry::14265", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::14302",
        NextAction::array(0, new NextAction("offtank entry::14302", 55.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "shadow resistance aura",
            NextAction::array(0, new NextAction("shadow resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "shadow protection potion ready",
        NextAction::array(0, new NextAction("shadow protection potion", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "shadow flame dot",
        NextAction::array(0, new NextAction("clear shadow flame dot", 100.0f), NULL)));
}

void NefarianFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end nefarian fight",
        NextAction::array(0, new NextAction("disable nefarian fight strategy", 100.0f), NULL)));
}

void NefarianFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end nefarian fight",
        NextAction::array(0, new NextAction("disable nefarian fight strategy", 100.0f), NULL)));
}
