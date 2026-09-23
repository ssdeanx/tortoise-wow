
#include "playerbot/playerbot.h"
#include "MoltenCoreDungeonStrategies.h"
#include "DungeonMultipliers.h"

using namespace ai;

void MoltenCoreDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start magmadar fight",
        NextAction::array(0, new NextAction("enable magmadar fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start lucifron fight",
        NextAction::array(0, new NextAction("enable lucifron fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start gehennas fight",
        NextAction::array(0, new NextAction("enable gehennas fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start garr fight",
        NextAction::array(0, new NextAction("enable garr fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start geddon fight",
        NextAction::array(0, new NextAction("enable geddon fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start shazzrah fight",
        NextAction::array(0, new NextAction("enable shazzrah fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start sulfuron fight",
        NextAction::array(0, new NextAction("enable sulfuron fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start golemagg fight",
        NextAction::array(0, new NextAction("enable golemagg fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start domo fight",
        NextAction::array(0, new NextAction("enable domo fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start ragnaros fight",
        NextAction::array(0, new NextAction("enable ragnaros fight strategy", 100.0f), NULL)));
}

void MoltenCoreDungeonStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Every raid bot carries a douse item for the rune chain.
    triggers.push_back(new TriggerNode(
        "mc douse item missing",
        NextAction::array(0, new NextAction("add mc douse item", 10.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "mc rune in sight",
        NextAction::array(0, new NextAction("move to mc rune", 1.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "mc rune close",
        NextAction::array(0,
            new NextAction("douse mc rune eternal", 2.0f),
            new NextAction("douse mc rune aqual", 1.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Magmadar (11982) - fire fight: Frenzy 19451 (hunter tranquilizing shot via
// the generic "dispel enrage" reaction trigger), Panic 19408 (fear), Lava Bomb
// GO 177704 (all bots run out of the hazard, melee included). Fire resist duty:
// one paladin or one shaman per subgroup.
// ---------------------------------------------------------------------------

void MagmadarFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();
    if (ai->IsRanged(bot) || ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "magmadar too close",
            NextAction::array(0, new NextAction("move away from magmadar", 100.0f), NULL)));
    }

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

void MagmadarFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end magmadar fight",
        NextAction::array(0, new NextAction("disable magmadar fight strategy", 100.0f), NULL)));
}

void MagmadarFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end magmadar fight",
        NextAction::array(0, new NextAction("disable magmadar fight strategy", 100.0f), NULL)));
}

void MagmadarFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "magmadar lava bomb",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

void MagmadarFightStrategy::InitCombatMultipliers(std::list<Multiplier*>& multipliers)
{
    Player* bot = ai->GetBot();
    if (ai->IsRanged(bot) || ai->IsHeal(bot))
    {
        multipliers.push_back(new PreventMoveAwayFromCreatureOnReachToCastMultiplier(ai));
    }
}

// ---------------------------------------------------------------------------
// Lucifron (12118) - shadow fight: Impending Doom 19702 (40y PBAOE), Curse
// 19703, Shadow Shock 19460. Decursing is the existing cure strategies; kill
// order on the Flamewaker Protectors (12119); shadow resistance to the subgroup
// resist class (paladin aura / shadow potion).
// ---------------------------------------------------------------------------

void LucifronFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    triggers.push_back(new TriggerNode(
        "attackers entry::12119",
        NextAction::array(0, new NextAction("attack entry::12119", 60.0f), NULL)));

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

void LucifronFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end lucifron fight",
        NextAction::array(0, new NextAction("disable lucifron fight strategy", 100.0f), NULL)));
}

void LucifronFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end lucifron fight",
        NextAction::array(0, new NextAction("disable lucifron fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Gehennas (12259) - shadow fight: Curse 19716, Rain of Fire 19717, Shadow
// Bolts 19728/19729. Decurse via class cure strategies; priority on Flamewaker
// adds (11661).
// ---------------------------------------------------------------------------

void GehennasFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    triggers.push_back(new TriggerNode(
        "attackers entry::11661",
        NextAction::array(0, new NextAction("attack entry::11661", 60.0f), NULL)));

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

void GehennasFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end gehennas fight",
        NextAction::array(0, new NextAction("disable gehennas fight strategy", 100.0f), NULL)));
}

void GehennasFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end gehennas fight",
        NextAction::array(0, new NextAction("disable gehennas fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Garr (12057) - AoE lockout via the base lifecycle callers; Firesworn 12099
// peeled one at a time. Surge 19196 chains on farthest targets, so the raid
// spreads by default; no extra positioning for tanks.
// ---------------------------------------------------------------------------

void GarrFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::12099",
        NextAction::array(0, new NextAction("attack entry::12099", 55.0f), NULL)));
}

void GarrFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end garr fight",
        NextAction::array(0, new NextAction("disable garr fight strategy", 100.0f), NULL)));
}

void GarrFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end garr fight",
        NextAction::array(0, new NextAction("disable garr fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Baron Geddon (12056) - fire fight: Living Bomb 20475 (run away from the
// raid), Inferno 19695 channel (melee run out of the room), Armageddon 20478
// at <5% (everyone runs far out), Ignite Mana 19659. Fire pot kept.
// ---------------------------------------------------------------------------

void GeddonFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    triggers.push_back(new TriggerNode(
        "geddon living bomb",
        NextAction::array(0, new NextAction("move away from raid::25", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "geddon inferno",
        NextAction::array(0, new NextAction("move away from geddon", 95.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "geddon armageddon",
        NextAction::array(0, new NextAction("move away from geddon", 100.0f), NULL)));

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

void GeddonFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end geddon fight",
        NextAction::array(0, new NextAction("disable geddon fight strategy", 100.0f), NULL)));
}

void GeddonFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end geddon fight",
        NextAction::array(0, new NextAction("disable geddon fight strategy", 100.0f), NULL)));
}

void GeddonFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "geddon living bomb",
        NextAction::array(0, new NextAction("move away from raid::25", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Shazzrah (12264) - Arcane Explosion 19712 (ranged/healers hold max range),
// Curse 19713 (decurse via class strategies), Counterspell 19715.
// ---------------------------------------------------------------------------

void ShazzrahFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();
    if (ai->IsRanged(bot) || ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "shazzrah too close",
            NextAction::array(0, new NextAction("move away from shazzrah", 100.0f), NULL)));
    }
}

void ShazzrahFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end shazzrah fight",
        NextAction::array(0, new NextAction("disable shazzrah fight strategy", 100.0f), NULL)));
}

void ShazzrahFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end shazzrah fight",
        NextAction::array(0, new NextAction("disable shazzrah fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Sulfuron Harbinger (12098) - Flamewaker Priest 11662 must die first (Inspire
// 19779 heals the pack). Priority kill order through the generic entry action.
// ---------------------------------------------------------------------------

void SulfuronFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::11662",
        NextAction::array(0, new NextAction("attack entry::11662", 60.0f), NULL)));
}

void SulfuronFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end sulfuron fight",
        NextAction::array(0, new NextAction("disable sulfuron fight strategy", 100.0f), NULL)));
}

void SulfuronFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end sulfuron fight",
        NextAction::array(0, new NextAction("disable sulfuron fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Golemagg (11988) - Core Ragers 11672 must be pulled away from the boss (their
// Golemagg Trust buff 20553 drops at distance), so only offtanks peel them.
// AoE is locked out whole fight.
// ---------------------------------------------------------------------------

void GolemaggFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::11672",
        NextAction::array(0, new NextAction("offtank entry::11672", 55.0f), NULL)));
}

void GolemaggFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end golemagg fight",
        NextAction::array(0, new NextAction("disable golemagg fight strategy", 100.0f), NULL)));
}

void GolemaggFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end golemagg fight",
        NextAction::array(0, new NextAction("disable golemagg fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Majordomo Executus (12018) - Elites 11664 die first, then Healers 11663; the
// boss yields once the eight adds are down (TYPE_MAJORDOMO == DONE, index 8)
// and bots disable the strategy on that trigger. Fire pot kept for Blast Wave.
// ---------------------------------------------------------------------------

void DomoFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::11664",
        NextAction::array(0, new NextAction("attack entry::11664", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::11663",
        NextAction::array(0, new NextAction("attack entry::11663", 58.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "instance encounter done::8",
        NextAction::array(0, new NextAction("disable domo fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 90.0f), NULL)));
}

void DomoFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end domo fight",
        NextAction::array(0, new NextAction("disable domo fight strategy", 100.0f), NULL)));
}

void DomoFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end domo fight",
        NextAction::array(0, new NextAction("disable domo fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Ragnaros (11502) - fire fight: Sons of Flame 12143 during submerge, ranged/
// healers 25+ yd from Wrath knockback + Magma Blast, fire pot.
// ---------------------------------------------------------------------------

void RagnarosFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();
    if (ai->IsRanged(bot) || ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "ragnaros too close",
            NextAction::array(0, new NextAction("move away from ragnaros", 90.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "attackers entry::12143",
        NextAction::array(0, new NextAction("attack entry::12143", 55.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "fire resistance aura",
            NextAction::array(0, new NextAction("fire resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 90.0f), NULL)));
}

void RagnarosFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ragnaros fight",
        NextAction::array(0, new NextAction("disable ragnaros fight strategy", 100.0f), NULL)));
}

void RagnarosFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ragnaros fight",
        NextAction::array(0, new NextAction("disable ragnaros fight strategy", 100.0f), NULL)));
}
