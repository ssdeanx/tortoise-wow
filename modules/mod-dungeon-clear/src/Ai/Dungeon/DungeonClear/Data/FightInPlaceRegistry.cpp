/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "FightInPlaceRegistry.h"

namespace
{
    // The rooms, sized off live spawn data (acore_world.creature on map 585):
    //   * Selin Fireheart spawns at X=242.1, Y=0.3; his CanAIAttack plane is X>216.
    //   * His 14 room-guard spawns (24688/24689/24690) sit at X 222.3-231.7,
    //     Y -23.0..+23.8 — the whole occupied room.
    //   * The antechamber Sunblade trash top out at X=182.3 (all X<216), so they
    //     stay normally pullable — the box floor at X=216 (Selin's own gate) keeps
    //     them out.
    //   * The instance's other encounters are far outside: Priestess Delrissa at
    //     X=126.9, and Vexallus at X=231.4 but Y=-214.3 — the Y band [-45,45]
    //     excludes him cleanly.
    // So [216,260] x [-45,45] is exactly Selin's room and nothing else.
    // Azjol-Nerub (601) — Hadronox's shaft. A different reason for the same rule:
    // here the boss does not gate on the party's position, her ADDS gate on their
    // own. The crusher packs are TempSummons of boss_hadronox, and
    // boss_hadronox::SummonedCreatureEvade calls EnterEvadeMode on HER the moment
    // any Anub'ar Crusher / Champion / Crypt Fiend / Necromancer summon evades.
    // Since a pack evading is a full encounter reset — Reset() re-summons pack 1
    // and the three swarm triggers and the whole crusher count starts over — an
    // advanced pull that drags a pack member off its MovePoint home is not a
    // tuning question, it is the run.
    //
    // The box is the whole shaft: the two upper ledges the packs spawn on
    // (493,603) and (567,603), the z~733 platform they walk down to, Hadronox's
    // z~675 ledge and the z~648 pit floor below it. Krik'thir sits at
    // (529.6, 646.2) with his watchers and trash out to y 706, so the y ceiling
    // at 625 keeps his whole encounter normally pullable.
    //
    // KNOWN AND ACCEPTED OVERLAP: a FightInPlaceZone is XY-only, and the lower
    // kingdom lies DIRECTLY BENEATH this shaft, so the box also covers the lake
    // and the bank beside it. Nothing of consequence lives there — the only
    // spawns in that XY footprint below are Anub'ar Brood Keepers (29340, rank 5
    // trivial), the decorative spiders the party walks past. Anub'arak (551,
    // 248), his Prime Guards (y 341) and the exit are all at y < 480 and stay
    // normally pullable, and the drop landing (544.18, 481.26, 288.98) sits 1yd
    // inside the southern edge — the party is out of the box within a step.
    FightInPlaceZone const kZones[] =
    {
        // Zul'Gurub (309) — High Priest Venoxis' snake temple. boss_venoxis
        // evades the encounter when HIS z leaves 27..43, i.e. the moment he is
        // dragged off the temple floor while the room's Razzashi trash
        // (Serpent 11371, Adder 11372, Cobra 11373, spawns x -12078..-11984 /
        // y -1752..-1671 / z 39-40) is pulled out through the doorway with him
        // in tow. Same shape as Selin's room: fight the temple in place. Box
        // sized off the live spawn data (the room's mobs + Venoxis at
        // -12029.8, -1707.9, z 39) with a margin that stays on the temple
        // floor; the entrance corridor and the rest of the wing stay normally
        // pullable.
        { 309, -12080.0f, -11980.0f, -1760.0f, -1650.0f },

        { 585, 216.0f, 260.0f, -45.0f, 45.0f },  // Magisters' Terrace — Selin Fireheart's room
        { 601, 470.0f, 640.0f, 480.0f, 625.0f }, // Azjol-Nerub — Hadronox's shaft
    };
}

bool FightInPlaceRegistry::IsNoPullZone(uint32 mapId, float x, float y)
{
    for (FightInPlaceZone const& z : kZones)
    {
        if (z.mapId != mapId)
            continue;
        if (x >= z.minX && x <= z.maxX && y >= z.minY && y <= z.maxY)
            return true;
    }
    return false;
}
