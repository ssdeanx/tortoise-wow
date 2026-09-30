#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"
#include "playerbot/strategy/triggers/DungeonTriggers.h"

namespace ai
{
    class OnyxiasLairEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        OnyxiasLairEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable onyxia's lair strategy", "+onyxia's lair") {}
    };

    class OnyxiasLairDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        OnyxiasLairDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable onyxia's lair strategy", "-onyxia's lair") {}
    };

    class OnyxiaEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        OnyxiaEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable onyxia fight strategy", "+onyxia") {}
    };

    class OnyxiaDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        OnyxiaDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable onyxia fight strategy", "-onyxia") {}
    };

    // Deep Breath scatter (23461): get off the breath lane.
    class MoveAwayFromOnyxiaAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromOnyxiaAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from onyxia", 10184, 40.0f) {}
    };

    // Anchors the Main Tank at the North wall (-8.86f, -212.75f, -88.54f) so Onyxia faces
    // away from the entrance corridor and raid flanks, preventing knockbacks into whelp pits.
    class TankOnyxiaNorthAction : public MovementAction
    {
    public:
        TankOnyxiaNorthAction(PlayerbotAI* ai) : MovementAction(ai, "tank onyxia north") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
    };

    // Phase 3 Landing Threat Recovery: Main Tank taunts Onyxia immediately upon landing
    // after the threat reset to lock aggro.
    class TauntOnyxiaLandingAction : public TauntCreatureAction
    {
    public:
        TauntOnyxiaLandingAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt onyxia landing", 10184) {}
    };

    // Whelp Burn Action: DPS prioritizes and burns Onyxian Whelps (11262).
    class AttackOnyxiaWhelpsAction : public AttackAction
    {
    public:
        AttackOnyxiaWhelpsAction(PlayerbotAI* ai) : AttackAction(ai, "attack onyxia whelps") {}
        bool Execute(Event& event) override;
        bool isUseful() override;
        static Unit* FindClosestWhelp(PlayerbotAI* ai, Player* bot);
    };

    // Offtank picks up and turns Onyxian Warders (12129) away from the raid.
    class TauntOnyxianWarderAction : public TauntCreatureAction
    {
    public:
        TauntOnyxianWarderAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt onyxian warder", 12129) {}
    };

    // Counter Bellowing Roar fear (Berserker Rage, Tremor Totem, Fear Ward, Will of the Forsaken).
    class OnyxiaFearCounterAction : public Action
    {
    public:
        OnyxiaFearCounterAction(PlayerbotAI* ai) : Action(ai, "counter onyxia fear") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
    };

    // Phase 3 landing throttle: non-tank DPS halts burst on Onyxia post-wipe until tank secures threat.
    class PauseDpsOnyxiaLandingAction : public AttackAction
    {
    public:
        PauseDpsOnyxiaLandingAction(PlayerbotAI* ai) : AttackAction(ai, "pause dps onyxia landing") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
    };
}
