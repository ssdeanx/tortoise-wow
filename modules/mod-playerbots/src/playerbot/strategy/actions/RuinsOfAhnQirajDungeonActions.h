#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"
#include "UseItemAction.h"

namespace ai
{
    class RuinsOfAhnQirajEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RuinsOfAhnQirajEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable ruins of ahnqiraj strategy", "+ruins of ahnqiraj") {}
    };

    class RuinsOfAhnQirajDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RuinsOfAhnQirajDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable ruins of ahnqiraj strategy", "-ruins of ahnqiraj") {}
    };

    // Fight-strategy enable/disable for every AQ20 encounter.
    class KurinnaxxEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        KurinnaxxEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable kurinnaxx fight strategy", "+kurinnaxx") {}
    };

    class KurinnaxxDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        KurinnaxxDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable kurinnaxx fight strategy", "-kurinnaxx") {}
    };

    class RajaxxEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RajaxxEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable rajaxx fight strategy", "+rajaxx") {}
    };

    class RajaxxDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RajaxxDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable rajaxx fight strategy", "-rajaxx") {}
    };

    class BuruEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BuruEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable buru fight strategy", "+buru") {}
    };

    class BuruDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BuruDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable buru fight strategy", "-buru") {}
    };

    class MoamEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MoamEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable moam fight strategy", "+moam") {}
    };

    class MoamDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MoamDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable moam fight strategy", "-moam") {}
    };

    class AyamissEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        AyamissEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable ayamiss fight strategy", "+ayamiss") {}
    };

    class AyamissDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        AyamissDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable ayamiss fight strategy", "-ayamiss") {}
    };

    class OssirianEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        OssirianEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable ossirian fight strategy", "+ossirian") {}
    };

    class OssirianDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        OssirianDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable ossirian fight strategy", "-ossirian") {}
    };

    class TauntKurinnaxxAction : public TauntCreatureAction
    {
    public:
        TauntKurinnaxxAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt kurinnaxx", 15348) {}
    };

    // Buru's locked target kites while the chase trigger fires; range matches
    // the trigger's 25 yd window.
    class MoveAwayFromBuruAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromBuruAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from buru", 15370, 25.0f) {}
    };
}
