/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license; you may redistribute it and/or modify it under version 3 of
 * the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"
#include "Creature.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Playerbots.h"

// --- Tower of Karazhan (map 814) --------------------------------------------
// Custom Turtle WoW 40-man / 10-man raid (Kara 40 / Upper Karazhan Halls).
//
// 1. KING'S COUNCIL (CHESS - 59967)
//    Full chess council: Bishop (59971), Knight (59968), Rook (59970), King (59967).
//    The King has SPELL_INVULNERABILITY (51228) until Bishop, Knight, and Rook
//    are DEAD (IsOnlyPiece() removes aura).
//    Bishop casts Heal (51214) and Redemption (51216) which resurrects dead pieces!
//    Therefore, strict kill order is:
//      Bishop (59971) -> Knight (59968) -> Rook (59970) -> King (59967).
//
// 2. SANV TAS'DAL (59981) - THE SEAT OF DESOLATION
//    Located in the Sanv Tribe Village. At 80% HP, opens three Rift Portals (59973)
//    that summon waves of Lost Draenei (59975-59979) and Rift Elementals (59974).
//    At 50% HP, channels Opening the Rift (51209) and fire/arcane feedback.
//    At 20% HP, enrages with Leader of the Tribe (51207).
//
// 3. KRUUL (59991) - PIT LORD OF THE TWISTING NETHER
//    Guards the Nether Terrace. Casts Mark of the Highlord (51166) mana-drain burn,
//    summons Infernals via Call from the Twisting Nether (51170), and enrages
//    at 25% HP with Wings of Oblivion (51172).
//
// 4. KEEPER GNARLMOON (61939) - CELESTIAL OBSERVATORY
//    Alternates raid between Blue Moon (51080) and Red Moon (51081) astral auras.
//    At health milestones, enters Worgen Dimension (51096 - banishing himself),
//    summoning 2x Blue Owls (59998) and 2x Red Owls (59997). Raid must kill all
//    4 owls to break the dimension and force Gnarlmoon back to reality.
//
// 5. LEY-WATCHER INCANTAGOS (61946) - BLUE DRAGON CONDUIT
//    Surrounded by Dragon Whelps (59988). Summons Ley Seekers (59989), siphons
//    six Ley Affinities (59982-59987), and rains Blizzard (51180).
//
// 6. ANOMALUS (61951) - ARCANE ANOMALY
//    Casts Arcane Overload (51100) and summons Unstable Magic Zones (59996),
//    pulsing arcane hazard fields on the ground that detonate after 8s.
//
// 7. ECHO OF MEDIVH (61958) - PINNACLE END BOSS
//    At 80% HP: Summons Unstoppable Infernal (59995).
//    At 50% HP: Enters Shadowform (51125), summons Unstoppable Infernal, and
//               spawns Shades of Medivh (59994) every 10s.
//    At 20% HP: Enrages (SPELL_ENRAGE 8269 + Medivh's Fury 51110) with third
//               Infernal summon.

namespace {
constexpr uint32 TOK_MAP = 814;

// Boss entries
constexpr uint32 TOK_KINGS_COUNCIL = 59967; // King
constexpr uint32 TOK_SANV_TASDAL = 59981;
constexpr uint32 TOK_KRUUL = 59991;
constexpr uint32 TOK_GNARLMOON = 61939;
constexpr uint32 TOK_INCANTAGOS = 61946;
constexpr uint32 TOK_ANOMALUS = 61951;
constexpr uint32 TOK_ECHO_OF_MEDIVH = 61958;

// King's Council (Chess) Pieces
constexpr uint32 TOK_CHESS_BISHOP = 59971;
constexpr uint32 TOK_CHESS_KNIGHT = 59968;
constexpr uint32 TOK_CHESS_ROOK = 59970;
constexpr uint32 TOK_CHESS_PAWN = 59972;
constexpr uint32 TOK_CHESS_GHASTLY_HORSEMAN = 59969;

// Sanv Tas'dal Adds
constexpr uint32 TOK_RIFT_PORTAL = 59973;
constexpr uint32 TOK_RIFT_ELEMENTAL = 59974;
constexpr uint32 TOK_RIFT_LOST_DRAENEI = 59975;
constexpr uint32 TOK_DRAENEI_RIFTSTALKER = 59976;
constexpr uint32 TOK_DRAENEI_RIFTWALKER = 59977;
constexpr uint32 TOK_DRAENEI_NETHERWALKER = 59978;
constexpr uint32 TOK_SHADOW_LOST_DRAENEI = 59979;

// Keeper Gnarlmoon Adds
constexpr uint32 TOK_RED_OWL = 59997;
constexpr uint32 TOK_BLUE_OWL = 59998;

// Ley-Watcher Incantagos Adds
constexpr uint32 TOK_LEY_SEEKER = 59989;
constexpr uint32 TOK_DRAGON_WHELP = 59988;

// Anomalus Hazard
constexpr uint32 TOK_UNSTABLE_MAGIC_ZONE = 59996;

// Echo of Medivh Adds
constexpr uint32 TOK_UNSTOPPABLE_INFERNAL = 59995;
constexpr uint32 TOK_SHADE_OF_MEDIVH = 59994;

// Order Indices (key = order - 1)
constexpr uint32 TOK_ORDER_KINGS_COUNCIL = 0; // Order 1 -> key 0
constexpr uint32 TOK_ORDER_SANV_TASDAL = 1;   // Order 2 -> key 1
constexpr uint32 TOK_ORDER_KRUUL = 2;         // Order 3 -> key 2
constexpr uint32 TOK_ORDER_GNARLMOON = 3;     // Order 4 -> key 3
constexpr uint32 TOK_ORDER_INCANTAGOS = 4;    // Order 5 -> key 4
constexpr uint32 TOK_ORDER_ANOMALUS = 5;      // Order 6 -> key 5
constexpr uint32 TOK_ORDER_MEDIVH = 6;        // Order 7 -> key 6

// Event IDs
constexpr uint32 TOK_EVENT_KINGS_COUNCIL = 1;
constexpr uint32 TOK_EVENT_SANV_TASDAL = 2;
constexpr uint32 TOK_EVENT_KRUUL = 3;
constexpr uint32 TOK_EVENT_GNARLMOON = 4;
constexpr uint32 TOK_EVENT_INCANTAGOS = 5;
constexpr uint32 TOK_EVENT_ANOMALUS = 6;
constexpr uint32 TOK_EVENT_MEDIVH = 7;
constexpr uint32 TOK_EVENT_ROOM_PRECLEAR = 8;

// Boss Coordinates
constexpr float TOK_CHESS_X = -11100.0f;
constexpr float TOK_CHESS_Y = -1999.0f;
constexpr float TOK_CHESS_Z = 93.5f;

constexpr float TOK_SANV_X = -6439.15f;
constexpr float TOK_SANV_Y = -2722.25f;
constexpr float TOK_SANV_Z = 18.66f;

constexpr float TOK_KRUUL_X = -11168.0f;
constexpr float TOK_KRUUL_Y = -1635.5f;
constexpr float TOK_KRUUL_Z = 278.23f;

constexpr float TOK_GNARLMOON_X = -11100.0f;
constexpr float TOK_GNARLMOON_Y = -1850.0f;
constexpr float TOK_GNARLMOON_Z = 165.76f;

constexpr float TOK_INCANTAGOS_X = -11111.4f;
constexpr float TOK_INCANTAGOS_Y = -1837.6f;
constexpr float TOK_INCANTAGOS_Z = 230.96f;

constexpr float TOK_ANOMALUS_X = -11233.2f;
constexpr float TOK_ANOMALUS_Y = -1688.2f;
constexpr float TOK_ANOMALUS_Z = 290.42f;

constexpr float TOK_MEDIVH_X = -11038.0f;
constexpr float TOK_MEDIVH_Y = -1999.5f;
constexpr float TOK_MEDIVH_Z = 93.5f;
} // namespace

void RegisterTowerOfKarazhanEvents(std::vector<DungeonEvent> &out) {
  // 1. King's Council (Chess): Bishop -> Knight -> Rook -> King priority chain.
  out.push_back(EventBuilder(TOK_MAP, TOK_EVENT_KINGS_COUNCIL,
                             "King's Council (Chess) Strategy")
                    .Anchored(TOK_ORDER_KINGS_COUNCIL)
                    .Persistent()
                    // Bishop first to stop heals & resurrection
                    .KillCreatureEngage(TOK_CHESS_BISHOP, 1, 60.0f)
                    .Timeout(120000)
                    // Knight second to stop cleaves & horsemen
                    .KillCreatureEngage(TOK_CHESS_KNIGHT, 1, 60.0f)
                    .Timeout(120000)
                    // Rook third to stop siege charges
                    .KillCreatureEngage(TOK_CHESS_ROOK, 1, 60.0f)
                    .Timeout(120000)
                    // King fourth once invulnerability drops
                    .KillCreatureEngage(TOK_KINGS_COUNCIL, 1, 80.0f)
                    .Timeout(300000)
                    .Build());

  // 2. Sanv Tas'dal: Engage and defeat Sanv Tas'dal after room trash pre-clear.
  out.push_back(EventBuilder(TOK_MAP, TOK_EVENT_SANV_TASDAL,
                             "Sanv Tas'dal & Rift Invasions")
                    .Anchored(TOK_ORDER_SANV_TASDAL)
                    .Persistent()
                    .KillCreatureEngage(TOK_SANV_TASDAL, 1, 75.0f)
                    .Timeout(360000)
                    .Build());

  // 3. Kruul: Engage, burn down, execute through Wings of Oblivion.
  out.push_back(EventBuilder(TOK_MAP, TOK_EVENT_KRUUL,
                             "Kruul & Wings of Oblivion")
                    .Anchored(TOK_ORDER_KRUUL)
                    .Persistent()
                    .KillCreatureEngage(TOK_KRUUL, 1, 75.0f)
                    .Timeout(420000)
                    .Build());

  // 4. Keeper Gnarlmoon: Engage Celestial Keeper and burn through Worgen Dimension.
  out.push_back(EventBuilder(TOK_MAP, TOK_EVENT_GNARLMOON,
                             "Keeper Gnarlmoon & Worgen Dimension")
                    .Anchored(TOK_ORDER_GNARLMOON)
                    .Persistent()
                    .KillCreatureEngage(TOK_GNARLMOON, 1, 75.0f)
                    .Timeout(360000)
                    .Build());

  // 5. Ley-Watcher Incantagos: Clear channeling Ley Seekers to drop Draconic Ley Charge,
  // then execute the blue dragon.
  out.push_back(EventBuilder(TOK_MAP, TOK_EVENT_INCANTAGOS,
                             "Ley-Watcher Incantagos & Ley Seekers")
                    .Anchored(TOK_ORDER_INCANTAGOS)
                    .Persistent()
                    .KillCreatureEngage(TOK_LEY_SEEKER, 0, 60.0f)
                    .Timeout(60000)
                    .KillCreatureEngage(TOK_INCANTAGOS, 1, 80.0f)
                    .Timeout(360000)
                    .Build());

  // 6. Anomalus: Engage and defeat Arcane Anomaly while avoiding Unstable Magic Zones.
  out.push_back(EventBuilder(TOK_MAP, TOK_EVENT_ANOMALUS,
                             "Anomalus & Unstable Magic Zones")
                    .Anchored(TOK_ORDER_ANOMALUS)
                    .Persistent()
                    .KillCreatureEngage(TOK_ANOMALUS, 1, 75.0f)
                    .Timeout(360000)
                    .Build());

  // 7. Echo of Medivh: Pinnacle encounter, survive Shadowform & execute Medivh.
  out.push_back(EventBuilder(TOK_MAP, TOK_EVENT_MEDIVH,
                             "Echo of Medivh, Infernals & Shades")
                    .Anchored(TOK_ORDER_MEDIVH)
                    .Persistent()
                    .KillCreatureEngage(TOK_UNSTOPPABLE_INFERNAL, 0, 80.0f)
                    .KillCreatureEngage(TOK_SHADE_OF_MEDIVH, 0, 80.0f)
                    .KillCreatureEngage(TOK_ECHO_OF_MEDIVH, 1, 80.0f)
                    .Timeout(480000)
                    .Build());

  // 8. Room-aggro pre-clear for Sanv Tas'dal and Ley-Watcher Incantagos
  out.push_back(
      EventBuilder(TOK_MAP, TOK_EVENT_ROOM_PRECLEAR,
                   "Clear the room before the room-aggro boss (Tower of Karazhan)")
          .Conditional(&DcRoomAggroPreClearCondition)
          .KillCreature(/*room trash*/ 0)
          .Build());
}

void RegisterTowerOfKarazhanRoster(std::vector<BossRosterPatch> &t) {
  using namespace DcRoster;

  // Complete 7-boss roster for Tower of Karazhan (map 814).
  {
    BossRosterPatch p;
    p.mapId = TOK_MAP;
    p.add = {
        MakeBoss(TOK_KINGS_COUNCIL, TOK_MAP, "King's Council (Chess)", TOK_CHESS_X,
                 TOK_CHESS_Y, TOK_CHESS_Z,
                 /*completionFrom*/ TOK_KINGS_COUNCIL,
                 /*orderOverride*/ TOK_ORDER_KINGS_COUNCIL),
        MakeBoss(TOK_SANV_TASDAL, TOK_MAP, "Sanv Tas'dal", TOK_SANV_X,
                 TOK_SANV_Y, TOK_SANV_Z,
                 /*completionFrom*/ TOK_SANV_TASDAL,
                 /*orderOverride*/ TOK_ORDER_SANV_TASDAL),
        MakeBoss(TOK_KRUUL, TOK_MAP, "Kruul", TOK_KRUUL_X, TOK_KRUUL_Y,
                 TOK_KRUUL_Z,
                 /*completionFrom*/ TOK_KRUUL,
                 /*orderOverride*/ TOK_ORDER_KRUUL),
        MakeBoss(TOK_GNARLMOON, TOK_MAP, "Keeper Gnarlmoon", TOK_GNARLMOON_X,
                 TOK_GNARLMOON_Y, TOK_GNARLMOON_Z,
                 /*completionFrom*/ TOK_GNARLMOON,
                 /*orderOverride*/ TOK_ORDER_GNARLMOON),
        MakeBoss(TOK_INCANTAGOS, TOK_MAP, "Ley-Watcher Incantagos", TOK_INCANTAGOS_X,
                 TOK_INCANTAGOS_Y, TOK_INCANTAGOS_Z,
                 /*completionFrom*/ TOK_INCANTAGOS,
                 /*orderOverride*/ TOK_ORDER_INCANTAGOS),
        MakeBoss(TOK_ANOMALUS, TOK_MAP, "Anomalus", TOK_ANOMALUS_X,
                 TOK_ANOMALUS_Y, TOK_ANOMALUS_Z,
                 /*completionFrom*/ TOK_ANOMALUS,
                 /*orderOverride*/ TOK_ORDER_ANOMALUS),
        MakeBoss(TOK_ECHO_OF_MEDIVH, TOK_MAP, "Echo of Medivh", TOK_MEDIVH_X,
                 TOK_MEDIVH_Y, TOK_MEDIVH_Z,
                 /*completionFrom*/ TOK_ECHO_OF_MEDIVH,
                 /*orderOverride*/ TOK_ORDER_MEDIVH)};
    t.push_back(std::move(p));
  }
}
