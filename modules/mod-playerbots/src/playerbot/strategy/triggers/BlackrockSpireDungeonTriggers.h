#pragma once
#include "DungeonTriggers.h"

namespace ai
{
    class BlackrockSpireEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        BlackrockSpireEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter blackrock spire", "blackrock spire", 229) {}
    };

    class BlackrockSpireLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        BlackrockSpireLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave blackrock spire", "blackrock spire", 229) {}
    };

    // Boss 9816: Pyroguard Emberseer
    class PyroguardEmberseerStartFightTrigger : public StartBossFightTrigger
    {
    public:
        PyroguardEmberseerStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start pyroguard emberseer fight", "pyroguard emberseer", 9816) {}
    };

    class PyroguardEmberseerEndFightTrigger : public EndBossFightTrigger
    {
    public:
        PyroguardEmberseerEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end pyroguard emberseer fight", "pyroguard emberseer", 9816) {}
    };

    // Boss 10429: Warchief Rend Blackhand
    class RendBlackhandStartFightTrigger : public StartBossFightTrigger
    {
    public:
        RendBlackhandStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start rend blackhand fight", "rend blackhand", 10429) {}
    };

    class RendBlackhandEndFightTrigger : public EndBossFightTrigger
    {
    public:
        RendBlackhandEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end rend blackhand fight", "rend blackhand", 10429) {}
    };

    // Boss 10430: The Beast
    class TheBeastStartFightTrigger : public StartBossFightTrigger
    {
    public:
        TheBeastStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start the beast fight", "the beast", 10430) {}
    };

    class TheBeastEndFightTrigger : public EndBossFightTrigger
    {
    public:
        TheBeastEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end the beast fight", "the beast", 10430) {}
    };

    // Boss 10363: General Drakkisath
    class GeneralDrakkisathStartFightTrigger : public StartBossFightTrigger
    {
    public:
        GeneralDrakkisathStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start general drakkisath fight", "general drakkisath", 10363) {}
    };

    class GeneralDrakkisathEndFightTrigger : public EndBossFightTrigger
    {
    public:
        GeneralDrakkisathEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end general drakkisath fight", "general drakkisath", 10363) {}
    };
}
