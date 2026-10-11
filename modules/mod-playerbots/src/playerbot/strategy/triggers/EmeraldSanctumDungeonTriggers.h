#pragma once
#include "DungeonTriggers.h"

namespace ai
{
    class EmeraldSanctumEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        EmeraldSanctumEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter emerald sanctum", "emerald sanctum", 807) {}
    };

    class EmeraldSanctumLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        EmeraldSanctumLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave emerald sanctum", "emerald sanctum", 807) {}
    };

    // Boss 60747: Erennius
    class ErenniusStartFightTrigger : public StartBossFightTrigger
    {
    public:
        ErenniusStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start erennius fight", "erennius", 60747) {}
    };

    class ErenniusEndFightTrigger : public EndBossFightTrigger
    {
    public:
        ErenniusEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end erennius fight", "erennius", 60747) {}
    };

    // Boss 60748: Solnius
    class SolniusStartFightTrigger : public StartBossFightTrigger
    {
    public:
        SolniusStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start solnius fight", "solnius", 60748) {}
    };

    class SolniusEndFightTrigger : public EndBossFightTrigger
    {
    public:
        SolniusEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end solnius fight", "solnius", 60748) {}
    };
}
