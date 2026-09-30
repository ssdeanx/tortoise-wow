#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"
#include "UseItemAction.h"
#include "playerbot/strategy/triggers/DungeonTriggers.h"

namespace ai
{
    // Dungeon Strategy Enable / Disable
    class ScarletCitadelEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ScarletCitadelEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable scarlet citadel strategy", "+scarlet citadel") {}
    };

    class ScarletCitadelDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ScarletCitadelDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable scarlet citadel strategy", "-scarlet citadel") {}
    };

    // ---------------------------------------------------------------------------
    // Boss 1: Sacred Fist Daelus (2000000)
    // ---------------------------------------------------------------------------
    class DaelusEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        DaelusEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable daelus fight strategy", "+daelus") {}
    };

    class DaelusDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        DaelusDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable daelus fight strategy", "-daelus") {}
    };

    // Tank close tether: tank must maintain <= 2.5 yd distance with Daelus to prevent Life Drain wipe
    class DaelusCloseTetherAction : public MovementAction
    {
    public:
        DaelusCloseTetherAction(PlayerbotAI* ai) : MovementAction(ai, "daelus close tether") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
    };

    // Poison Cloud Run-out: Afflicted player moves 25yd away from the raid to drop the cloud safely
    class DaelusDropPoisonCloudAction : public MoveAwayFromRaidAction
    {
    public:
        DaelusDropPoisonCloudAction(PlayerbotAI* ai) : MoveAwayFromRaidAction(ai, "daelus drop poison cloud")
        {
            qualifier = "25";
        }
    };

    // Evacuate Poison Cloud area
    class MoveAwayFromPoisonCloudAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromPoisonCloudAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from poison cloud", 16363, 10.0f) {}
    };

    // Sunder Armor Tank Swap: Offtank taunts Daelus
    class DaelusTankSwapAction : public TauntCreatureAction
    {
    public:
        DaelusTankSwapAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "daelus tank swap", 2000000) {}
    };

    // Attack Fallen Spirits (2000013): Kills regular spirits that would heal Daelus for 5% max HP.
    // Crucially checks and SKIPS the Chosen One (aura 26235) so it reaches Daelus to stun him!
    class AttackDaelusSpiritsAction : public AttackAction
    {
    public:
        AttackDaelusSpiritsAction(PlayerbotAI* ai) : AttackAction(ai, "attack daelus spirits") {}
        bool Execute(Event& event) override;
        bool isUseful() override;

    private:
        Unit* FindClosestNormalSpirit();
    };

    // ---------------------------------------------------------------------------
    // Boss 2: Grand Sorcerer Ardaeus (2000001)
    // ---------------------------------------------------------------------------
    class ArdaeusEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ArdaeusEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable ardaeus fight strategy", "+ardaeus") {}
    };

    class ArdaeusDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ArdaeusDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable ardaeus fight strategy", "-ardaeus") {}
    };

    // Attack The Sun (2000019): Deals damage to push it upwards by 5yd per 25k damage
    class AttackArdaeusSunAction : public AttackAction
    {
    public:
        AttackArdaeusSunAction(PlayerbotAI* ai) : AttackAction(ai, "attack ardaeus sun") {}
        bool Execute(Event& event) override;
        bool isUseful() override;
    };

    // Statue Echo adds (2000020 - 2000030) offtank pickup
    class TauntArdaeusStatueAction : public AttackAction
    {
    public:
        TauntArdaeusStatueAction(PlayerbotAI* ai) : AttackAction(ai, "taunt ardaeus statue") {}
        bool Execute(Event& event) override;
        bool isUseful() override;
    };

    // Attack Statue Echo adds
    class AttackArdaeusStatueAction : public AttackAction
    {
    public:
        AttackArdaeusStatueAction(PlayerbotAI* ai) : AttackAction(ai, "attack ardaeus statue") {}
        bool Execute(Event& event) override;
        bool isUseful() override;

    private:
        Unit* FindStatueAdd();
    };

    // ---------------------------------------------------------------------------
    // Boss 3: High Inquisitor Mariella (2000002)
    // ---------------------------------------------------------------------------
    class MariellaEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MariellaEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable mariella fight strategy", "+mariella") {}
    };

    class MariellaDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MariellaDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable mariella fight strategy", "-mariella") {}
    };

    // Evacuate Mariella Void Zones (2000016) and Kill Zones (2000018)
    class MoveOutOfMariellaVoidZoneAction : public MovementAction
    {
    public:
        MoveOutOfMariellaVoidZoneAction(PlayerbotAI* ai) : MovementAction(ai, "move out of mariella void zone") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
    };

    // Attack Felhounds (2000017) to prevent mana drain
    class AttackMariellaFelhoundsAction : public AttackAction
    {
    public:
        AttackMariellaFelhoundsAction(PlayerbotAI* ai) : AttackAction(ai, "attack mariella felhounds") {}
        bool Execute(Event& event) override;
        bool isUseful() override;

    private:
        Unit* FindFelhound();
    };

    // Offtank Felhounds
    class TauntMariellaFelhoundAction : public TauntCreatureAction
    {
    public:
        TauntMariellaFelhoundAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt mariella felhound", 2000017) {}
    };

    // ---------------------------------------------------------------------------
    // Mini-Boss: Brother Eric Vesper (2000004)
    // ---------------------------------------------------------------------------
    class EricVesperEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        EricVesperEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable eric vesper fight strategy", "+eric vesper") {}
    };

    class EricVesperDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        EricVesperDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable eric vesper fight strategy", "-eric vesper") {}
    };

    // Attack Eric Vesper (2000004) to beat Energize wipe timer
    class AttackEricVesperAction : public AttackAction
    {
    public:
        AttackEricVesperAction(PlayerbotAI* ai) : AttackAction(ai, "attack eric vesper") {}
        bool Execute(Event& event) override;
        bool isUseful() override;
    };

    // ---------------------------------------------------------------------------
    // Mini-Boss: Darkcaller Rayn (2000037)
    // ---------------------------------------------------------------------------
    class DarkcallerRaynEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        DarkcallerRaynEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable darkcaller rayn fight strategy", "+darkcaller rayn") {}
    };

    class DarkcallerRaynDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        DarkcallerRaynDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable darkcaller rayn fight strategy", "-darkcaller rayn") {}
    };

    // ---------------------------------------------------------------------------
    // Boss 4: High General Abbendis (2000003)
    // ---------------------------------------------------------------------------
    class AbbendisEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        AbbendisEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable abbendis fight strategy", "+abbendis") {}
    };

    class AbbendisDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        AbbendisDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable abbendis fight strategy", "-abbendis") {}
    };

    // Position Abbendis facing away from raid (tank anchors against wall at 421.21f, -8.98f, 37.70f)
    class TankAbbendisAction : public MovementAction
    {
    public:
        TankAbbendisAction(PlayerbotAI* ai) : MovementAction(ai, "tank abbendis") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
    };
}
