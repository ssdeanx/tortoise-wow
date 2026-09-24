/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "gtest/gtest.h"

#include "Ai/Dungeon/DungeonClear/Data/FightInPlaceRegistry.h"

// Zul'Gurub's Venoxis temple (map 309, classic). boss_venoxis evades when HIS z
// leaves 27..43, so an advanced pull that drags the room's Razzashi trash out
// through the doorway with him in tow resets the encounter.

TEST(FightInPlaceTest, ZulGurubVenoxisTempleIsNoPull)
{
    // Venoxis' spawn.
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(309, -12029.8f, -1707.9f));
    // The room's Razzashi spawn extremes (Serpent 11371 / Adder 11372 / Cobra
    // 11373): x -12078..-11984, y -1752..-1671.
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(309, -12078.0f, -1752.0f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(309, -11984.0f, -1671.0f));
    // The room guards' spots (Axe Thrower 11350 / Hakkari Priest 11830).
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(309, -12059.0f, -1707.0f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(309, -12058.0f, -1705.0f));
}

TEST(FightInPlaceTest, ZulGurubKeepsTheRestOfTheWingPullable)
{
    // Jeklik (-12289.7, -1382.2) and Mar'li (-12326.5, -1577.1) keep the
    // ordinary pull.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(309, -12289.7f, -1382.2f));
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(309, -12326.5f, -1577.1f));
    // The instance gate / zone-in.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(309, -11916.6f, -1243.5f));
}

// Selin Fireheart's room (Magisters' Terrace, map 585). The registry must veto the
// advanced pull for anything inside the room (X>216, the boss's own CanAIAttack
// plane) while leaving the antechamber and the instance's other encounters pullable.

TEST(FightInPlaceTest, SelinRoomGuardsAreInTheZone)
{
    // The three room-guard spawn extremes (24688/24689/24690): X 222.3-231.7,
    // Y -23.0..+23.8. Every one must read as no-pull.
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(585, 222.3f, -18.0f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(585, 231.7f, 2.6f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(585, 227.3f, -23.0f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(585, 228.0f, 23.8f));
    // Selin himself and the deep end of his room.
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(585, 242.1f, 0.3f));
}

TEST(FightInPlaceTest, AntechamberStaysPullable)
{
    // Sunblade trash top out at X=182.3 — below Selin's X=216 gate, so a normal
    // pull must still fire there. The camp the stuck runs landed on (X~197) is also
    // outside the room and must not be swallowed.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(585, 182.3f, 0.0f));
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(585, 197.0f, 7.0f));
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(585, 216.0f - 0.01f, 0.0f));
}

TEST(FightInPlaceTest, OtherMgtEncountersAreNotVetoed)
{
    // Priestess Delrissa (X=126.9) — far west, below the gate.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(585, 126.9f, 19.2f));
    // Vexallus (X=231.4 but Y=-214.3) — inside the X band but far outside the Y band.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(585, 231.4f, -214.3f));
}

TEST(FightInPlaceTest, OtherMapsAreNeverVetoed)
{
    // The same coordinates on any other map carry no zone.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(0, 242.0f, 0.0f));
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(560, 230.0f, 0.0f));
}

TEST(FightInPlaceTest, ZoneBoundsAreInclusive)
{
    // The gate plane (X=216) and the Y edges are inside the room (closed interval).
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(585, 216.0f, 0.0f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(585, 260.0f, 45.0f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(585, 240.0f, -45.0f));
    // Just past the far/side walls: out.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(585, 260.01f, 0.0f));
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(585, 240.0f, 45.01f));
}

// Azjol-Nerub's Hadronox shaft (map 601). The crusher packs are TempSummons of
// the boss, and boss_hadronox::SummonedCreatureEvade resets the WHOLE encounter
// the moment one of them evades — so an advanced pull that drags a pack member
// off its home is a run-ender, not a tuning question.

TEST(FightInPlaceTest, HadronoxShaftIsNoPull)
{
    // The z~733 platform the packs walk down to and the fight ends on.
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(601, 530.4f, 560.0f));
    // The two upper ledges packs 2 and 3 spawn on.
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(601, 493.5f, 603.3f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(601, 567.0f, 602.6f));
    // Hadronox's own spawn ledge and the pit floor below it.
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(601, 522.5f, 544.9f));
    EXPECT_TRUE(FightInPlaceRegistry::IsNoPullZone(601, 522.0f, 548.0f));
}

TEST(FightInPlaceTest, AzjolNerubKeepsItsOtherEncountersPullable)
{
    // Krik'thir (529.6, 646.2) and his watcher trash out to y~706 sit above the
    // y=625 ceiling and must keep the ordinary pull.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(601, 529.6f, 646.2f));
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(601, 529.0f, 706.9f));
    // Anub'arak (551, 248) and his Prime Guards (y 341) are below the y=480 floor.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(601, 551.0f, 248.3f));
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(601, 542.0f, 341.4f));
    // The zone-in.
    EXPECT_FALSE(FightInPlaceRegistry::IsNoPullZone(601, 413.3f, 796.0f));
}

