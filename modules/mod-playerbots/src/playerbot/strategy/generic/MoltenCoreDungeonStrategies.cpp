
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

    // Turtle's custom MC bosses (Phase 13).
    triggers.push_back(new TriggerNode(
        "start twin golem fight",
        NextAction::array(0, new NextAction("enable twin golem fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start twin golem fight 2",
        NextAction::array(0, new NextAction("enable twin golem fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start thaurissan fight",
        NextAction::array(0, new NextAction("enable thaurissan fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start incindis fight",
        NextAction::array(0, new NextAction("enable incindis fight strategy", 100.0f), NULL)));

    // Core Hound pack (11671): balance health evenly and burn together so all 5 die within the 10s window.
    triggers.push_back(new TriggerNode(
        "mc core hound pack",
        NextAction::array(0, new NextAction("attack core hound pack", 85.0f), NULL)));
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

    // Restock the protection potions out of combat. Molten Core (map 409)
    // stocks fire/shadow/arcane - the three damage schools the instance
    // actually uses, so no bot ever runs dry mid-raid.
    triggers.push_back(new TriggerNode(
        "ahnqiraj supplies needed",
        NextAction::array(0, new NextAction("give ahnqiraj supplies", 90.0f), NULL)));
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

    // Owner's kill order: the offtank picks up ONE Flamewaker Protector while
    // the other is CC'd (the mage sheep rides the generic `cc target` +
    // polymorph machinery, or a human RTI-CC mark), then the picked-up one
    // dies, then the CC'd one, then Lucifron - he reportedly gains a buff per
    // Protector death (NOT found in tw_world data; verify live). Protectors
    // also cast Dominate Mind 20740 (CC the mind-controlled ally via the
    // existing CcMindControlledAllyAction) and Cleave 20677.
    triggers.push_back(new TriggerNode(
        "attackers entry::12119",
        NextAction::array(0, new NextAction("offtank entry::12119", 65.0f),
                             new NextAction("attack entry::12119", 60.0f), NULL)));

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

    // The 2 Flamewaker adds (11661) are the threat: tanked APART (offtank
    // pickup, tanks only) and DPS-bursted down. Fist of Ragnaros 20277 stuns
    // the tank; Rain of Fire 19717 is a 32s ground DoT the generic
    // "has area debuff" avoidance already steps bots out of.
    triggers.push_back(new TriggerNode(
        "attackers entry::11661",
        NextAction::array(0, new NextAction("offtank entry::11661", 65.0f),
                             new NextAction("attack entry::11661", 60.0f), NULL)));

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
    // All 8 Firesworn (12099) are tanked and kept SEPARATED (offtank pickup,
    // tanks only) and killed ONE at a time - each death fires Eruption 19497
    // (~3500 AoE) and stacks Garr's Enrage 19516. Never drag one >45yd from
    // Garr (Separation Anxiety 23492). AoE is locked out in OnStrategyAdded.
    // The warlock's banish (the generic "banish" on the cc target) is the
    // control that works: mob_fireswornAI/mob_garr's forced Massive Eruption
    // 20483 SKIPS banished adds (the on-death Eruption still fires).
    triggers.push_back(new TriggerNode(
        "attackers entry::12099",
        NextAction::array(0, new NextAction("offtank entry::12099", 65.0f),
                             new NextAction("attack entry::12099", 55.0f), NULL)));
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

    // Arcane Explosion 19712 / Shazzrah's Curse 19713 / Deaden Magic 19714 /
    // Counterspell 19715 are arcane.
    triggers.push_back(new TriggerNode(
        "arcane protection potion ready",
        NextAction::array(0, new NextAction("arcane protection potion", 100.0f), NULL)));
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
    // The 4 Flamewaker Priests (11662) heal Sulfuron with Dark Mending 19775
    // (~27.7k): tanks pull them OUT of his line of sight (offtank pickup) and
    // the raid kills them; the class rotations' "on enemy healer" interrupts
    // (mage counterspell / warrior pummel / shaman wind shear / hunter
    // silencing shot / druid bash) cover Dark Mending. Sulfuron himself buffs
    // self + allies with Inspire 19779 and uses Dark Strike / Knockdown on the
    // tank.
    triggers.push_back(new TriggerNode(
        "attackers entry::11662",
        NextAction::array(0, new NextAction("offtank entry::11662", 65.0f),
                             new NextAction("attack entry::11662", 60.0f), NULL)));
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

// ---------------------------------------------------------------------------
// Turtle's custom MC bosses (raid plan Phase 13) -----------------------------
// Mechanics verified in tw_world (creature_template / creature_spells /
// creature_movement) and src/scripts/dungeons/molten_core/.
// ---------------------------------------------------------------------------

// Twin Golems: Basalthar 65020 + Smoldaris 65021 (boss_twin_golem.cpp).
// Aggro links both; damage to one mirrors the same PERCENT to the other;
// heals mirror; killing one kills the twin. ~1.0M HP each, melee 2.0-3.1k.
void TwinGolemFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Split tanks: the offtank holds Smoldaris (tanks only) while the raid
    // burns Basalthar. DPS must NOT split - the % mirror kills the twin when
    // one dies, so splitting only slows the kill.
    triggers.push_back(new TriggerNode(
        "attackers entry::65021",
        NextAction::array(0, new NextAction("offtank entry::65021", 70.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::65020",
        NextAction::array(0, new NextAction("attack entry::65020", 60.0f), NULL)));

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

void TwinGolemFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end twin golem fight",
        NextAction::array(0, new NextAction("disable twin golem fight strategy", 100.0f), NULL)));
}

void TwinGolemFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end twin golem fight",
        NextAction::array(0, new NextAction("disable twin golem fight strategy", 100.0f), NULL)));
}

// Sorcerer-Thane Thaurissan 57642 (boss_thane.cpp). 1.2M HP, melee 2.0-2.7k.
// Arcane Rupture 52722 on the tank (6s), Arcane Barrage 52723 on a random
// target (16s), Rune of Power 52724/52729 (self dynamic-object buff, re-cast
// at 5s and 75/50/25% HP), Mirror Image 52727 at 50% -> Image of Thane 57643,
// and alternating Rune of Detonation 52725 (1149/tick) / Rune of Combustion
// 52726 (4499/tick) every 20s WITH RAID WARNINGS.
void ThaurissanFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // Rune of Detonation 52725 / Combustion 52726: an 18yd enemy AoE at
    // THAURISSAN's location (TARGET_LOCATION_CASTER_SRC + ENEMY_AOE_AT_SRC_LOC,
    // radius index 12 = 18yd) - everyone inside steps out (Combustion ticks
    // 4499, lethal to non-tanks). Also in the reaction set so it runs in every
    // combat state.
    triggers.push_back(new TriggerNode(
        "thaurissan rune debuff",
        NextAction::array(0, new NextAction("move away from thaurissan", 100.0f), NULL)));

    // Mirror Image: offtank the Image and burn it down.
    triggers.push_back(new TriggerNode(
        "attackers entry::57643",
        NextAction::array(0, new NextAction("offtank entry::57643", 65.0f),
                             new NextAction("attack entry::57643", 62.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::57642",
        NextAction::array(0, new NextAction("attack entry::57642", 55.0f), NULL)));

    // Arcane Rupture 52722 / Arcane Barrage 52723 are arcane.
    triggers.push_back(new TriggerNode(
        "arcane protection potion ready",
        NextAction::array(0, new NextAction("arcane protection potion", 100.0f), NULL)));
}

void ThaurissanFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end thaurissan fight",
        NextAction::array(0, new NextAction("disable thaurissan fight strategy", 100.0f), NULL)));
}

void ThaurissanFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end thaurissan fight",
        NextAction::array(0, new NextAction("disable thaurissan fight strategy", 100.0f), NULL)));
}

void ThaurissanFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    // Rune of Detonation / Combustion: an 18yd enemy AoE centred on Thaurissan
    // - step out of his radius (see the combat-trigger comment).
    triggers.push_back(new TriggerNode(
        "thaurissan rune debuff",
        NextAction::array(0, new NextAction("move away from thaurissan", 100.0f), NULL)));
}

// Incindis 52145 (boss_incindis.cpp). 988k HP, melee 2.5-3.4k. Molten Bite
// 42040 on the tank (applies Molten Armor 42039), Quaking Stomp 42036 then
// Fire Nova 42037 one second later, and at 50% HP spawns 1 large + 2 small
// Incendic Eggs (52147/52146) that hatch into 52149/52148 adds.
void IncindisFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // The hatched adds are the mid-fight threat: pick them up and kill them,
    // then back to the boss.
    triggers.push_back(new TriggerNode(
        "attackers entry::52148",
        NextAction::array(0, new NextAction("offtank entry::52148", 65.0f),
                             new NextAction("attack entry::52148", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::52149",
        NextAction::array(0, new NextAction("offtank entry::52149", 64.0f),
                             new NextAction("attack entry::52149", 59.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::52145",
        NextAction::array(0, new NextAction("attack entry::52145", 55.0f), NULL)));

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

void IncindisFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end incindis fight",
        NextAction::array(0, new NextAction("disable incindis fight strategy", 100.0f), NULL)));
}

void IncindisFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end incindis fight",
        NextAction::array(0, new NextAction("disable incindis fight strategy", 100.0f), NULL)));
}
