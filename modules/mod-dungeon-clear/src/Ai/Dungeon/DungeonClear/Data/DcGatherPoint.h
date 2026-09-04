/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */
#ifndef _PLAYERBOT_DCGATHERPOINT_H
#define _PLAYERBOT_DCGATHERPOINT_H

#include "Common.h"

// A spot the LEADER wants its followers standing on right now - published in
// the leader's context (DcKey::GatherPoint), read by every follower through
// DcLeaderSignal::GetLeaderGatherPoint. Written by the event executor while a
// step needs the party physically at an object (a SUMMONING_RITUAL altar with
// reqParticipants 3: the core counts distinct clickers, and a click needs the
// clicker inside 5 yd). Carries a TTL so a leader that stops refreshing it
// (step done, run off, bot gone) releases the party without a cleanup path.
//
// Why not HopTo from the leader's tick: a follower is nearly always moving,
// so HopTo (which bails on isMoving()) never fired; a forced MovePoint every
// 2 s did fetch them but restarted their splines each time and cost more than
// it won. Here the follower moves ITSELF, once, and then holds - its own
// follow-tank action stands down because this outranks it.
struct DcGatherPoint
{
    uint32 mapId{0};
    float  x{0.0f};
    float  y{0.0f};
    float  z{0.0f};
    float  radius{3.5f};   // "arrived" when inside this; stand and hold
    float  reach{40.0f};   // followers further away than this are not summoned
    uint32 untilMs{0};     // getMSTime() deadline; refreshed by the writer

    bool Live(uint32 nowMs) const { return mapId != 0 && untilMs != 0 && nowMs < untilMs; }
};

#endif
