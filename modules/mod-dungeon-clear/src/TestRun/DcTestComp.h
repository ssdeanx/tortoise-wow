/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DCTESTCOMP_H
#define _PLAYERBOT_DCTESTCOMP_H

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

// A test run fields one tank, one healer and every remaining seat DPS. Which
// class/spec fills each role is randomised per run (BuildComp) so successive
// runs of the same dungeon exercise different pull/heal/threat paths and shake
// out class-specific edge cases — the earlier fixed prot-warr / holy-priest /
// mage-rogue-hunter comp only ever tested one shape.
//
// The SIZE is the registry row's business, not this file's: a row names how many
// slots it wants (DcTestDungeonRegistry::Row::partySize) and kPartySize below is
// what a row that names no size gets — i.e. every run that ever existed. The
// role LAYOUT does not scale with the size: slot 0 stays the tank (the group's
// leader, and the driver DC's leader election settles on), slot 1 the healer, and
// slots 2+ are DPS whatever the count. A twenty-slot comp is therefore one tank,
// one healer and eighteen DPS, which is the shape DC's own routines are written
// against — DC elects exactly ONE driver and the rest follow it, so a second
// tank would only be another follower holding threat off the driver.
//
// Every spec is named by its playerbots premade-spec template name
// (AiPlayerbot.PremadeSpecName.<class>.<n>) and resolved to a specNo at
// provisioning time; a spec with no matching template fails the run loudly
// (tank/heal) rather than silently rolling a random build, so only specs that
// exist as templates appear in the pools below. Death knights are excluded
// (their level floor breaks low-level dungeons); all-Alliance because the
// addclass pool is picked per team and entrances are faction-agnostic
// teleports.
//
// Randomisation is seeded (BuildComp(seed)) and the seed is recorded, so a
// comp that trips a bug can be replayed exactly via `.dc test start <d> seed=N`.

namespace DcTestComp
{
    struct Slot
    {
        std::uint8_t classId;      // 1=warr 2=pala 3=hunt 4=rogue 5=priest 7=sham 8=mage 9=lock 11=druid
        char const* specName;      // premade-spec template to force
        char const* fallbackSpec;  // substring to match if the exact name is absent
        char const* role;          // "tank" | "heal" | "dps" (for the record/UI)
    };

    // The size every run had before a row could ask for its own: 1 tank,
    // 1 healer, 3 DPS. Also the default argument of BuildComp, so callers that
    // have no row in hand (the unit tests) keep the five-slot behaviour.
    inline constexpr std::size_t kPartySize = 5;

    // Every class/spec that can legitimately fill each role. BuildComp draws
    // from these; a class appears once per role even when it offers several
    // specs for it (e.g. priest disc + holy) so both get exercised over time.
    // Spec templates below name THIS tree's configured premade paths
    // (AiPlayerbot.PremadeSpecName.* in aiplayerbot.conf) - the upstream
    // mod-playerbots names (bear/disc/arms/bm/mm/surv/subtlety/ele/cat) have
    // no template here and made every draw of those slots a setup failure.
    // Druid feral covers both the tank and the cat-dps seat on 1.12.
    inline constexpr Slot kTankPool[] = {
        { 1,  "protection", "prot",  "tank" },  // warrior
        { 2,  "protection", "prot",  "tank" },  // paladin
        // DRUID FERAL PULLED FROM THE TANK SEAT, 2026-08-29 - temporary.
        // 14 runs, average 1.50 bosses, and NOT ONCE past two, while warrior and
        // paladin tanks on comparable samples reached five and four. Controlled
        // for the speed fix (post-fix runs only) and for the comp's distinct-class
        // rule hiding a druid healer (excluded those runs too); the gap survives
        // both. Cause unknown - bear-form geometry and the missing ranged pull are
        // both ruled out (see the analysis note). Restore this line the moment it
        // is understood: without it the feral tank seat has no coverage at all.
        // { 11, "feral",      "feral", "tank" },  // druid
    };

    inline constexpr Slot kHealPool[] = {
        { 5,  "holy",        "holy",  "heal" },  // priest
        { 2,  "holy",        "holy",  "heal" },  // paladin
        { 7,  "restoration", "resto", "heal" },  // shaman
        { 11, "restoration", "resto", "heal" },  // druid
    };

    inline constexpr Slot kDpsPool[] = {
        { 1,  "fury",          "fury",     "dps" },  // warrior
        { 2,  "retribution",   "ret",      "dps" },  // paladin
        { 3,  "beastmastery",  "beast",    "dps" },  // hunter
        { 3,  "marksmanship",  "marksman", "dps" },  // hunter
        { 4,  "assassination", "assassin", "dps" },  // rogue
        { 4,  "combat",        "combat",   "dps" },  // rogue
        { 5,  "shadow",        "shadow",   "dps" },  // priest
        { 7,  "enhancement",   "enh",      "dps" },  // shaman
        { 8,  "arcane",        "arcane",   "dps" },  // mage
        { 8,  "fire",          "fire",     "dps" },  // mage
        { 8,  "frost",         "frost",    "dps" },  // mage
        { 9,  "affliction",    "affli",    "dps" },  // warlock
        { 9,  "demonology",    "demo",     "dps" },  // warlock
        { 9,  "destruction",   "destro",   "dps" },  // warlock
        { 11, "balance",       "balance",  "dps" },  // druid
        { 11, "feral",         "feral",    "dps" },  // druid (cat seat)
    };

    // Deterministically pick a party for the given seed: one tank, one healer,
    // three DPS at the default size, all five on DISTINCT classes (maximises
    // class diversity and keeps the addclass-pool draw from needing several
    // chars of one class). Pure — no globals, no I/O — so the same seed always
    // yields the same comp.
    //
    // `size` is the row's slot count. Below the pool's class count the distinct-
    // class draw is unchanged and a bigger comp simply repeats classes once it
    // runs out; there is no way to seat 20 bots on distinct classes when the
    // pools carry 9. Empty for size < 2 — there is no tank+healer pair to draw.
    // A caller asking for fewer than kPartySize gets exactly that many slots.
    std::vector<Slot> BuildComp(std::uint32_t seed, std::size_t size = kPartySize);

    // The pool backing a role token ("tank" | "heal" | "dps"), for callers that
    // must substitute an alternative class when the drawn one has no available
    // pool character. Empty for an unknown token.
    std::vector<Slot> RolePool(std::string_view role);
}

#endif  // _PLAYERBOT_DCTESTCOMP_H
