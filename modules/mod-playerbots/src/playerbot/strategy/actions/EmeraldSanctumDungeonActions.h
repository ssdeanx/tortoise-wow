#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"

namespace ai
{
    class EmeraldSanctumEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        EmeraldSanctumEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable emerald sanctum strategy", "+emerald sanctum") {}
    };

    class EmeraldSanctumDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        EmeraldSanctumDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable emerald sanctum strategy", "-emerald sanctum") {}
    };

    class ErenniusEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ErenniusEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable erennius fight strategy", "+erennius") {}
    };

    class ErenniusDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ErenniusDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable erennius fight strategy", "-erennius") {}
    };

    class SolniusEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SolniusEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable solnius fight strategy", "+solnius") {}
    };

    class SolniusDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SolniusDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable solnius fight strategy", "-solnius") {}
    };
}
