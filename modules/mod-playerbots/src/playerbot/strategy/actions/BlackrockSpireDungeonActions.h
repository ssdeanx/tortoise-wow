#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"

namespace ai
{
    class BlackrockSpireEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BlackrockSpireEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable blackrock spire strategy", "+blackrock spire") {}
    };

    class BlackrockSpireDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BlackrockSpireDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable blackrock spire strategy", "-blackrock spire") {}
    };

    class PyroguardEmberseerEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        PyroguardEmberseerEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable pyroguard emberseer fight strategy", "+pyroguard emberseer") {}
    };

    class PyroguardEmberseerDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        PyroguardEmberseerDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable pyroguard emberseer fight strategy", "-pyroguard emberseer") {}
    };

    class RendBlackhandEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RendBlackhandEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable rend blackhand fight strategy", "+rend blackhand") {}
    };

    class RendBlackhandDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RendBlackhandDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable rend blackhand fight strategy", "-rend blackhand") {}
    };

    class TheBeastEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        TheBeastEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable the beast fight strategy", "+the beast") {}
    };

    class TheBeastDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        TheBeastDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable the beast fight strategy", "-the beast") {}
    };

    class GeneralDrakkisathEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GeneralDrakkisathEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable general drakkisath fight strategy", "+general drakkisath") {}
    };

    class GeneralDrakkisathDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GeneralDrakkisathDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable general drakkisath fight strategy", "-general drakkisath") {}
    };
}
