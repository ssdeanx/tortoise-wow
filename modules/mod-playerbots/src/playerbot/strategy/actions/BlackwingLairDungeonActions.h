#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"
#include "UseItemAction.h"
#include "RemoveAuraAction.h"
#include "playerbot/strategy/values/GuidPositionValues.h"
#include "playerbot/strategy/triggers/BlackwingLairDungeonTriggers.h"

namespace ai
{
    const uint32 SPELL_DISARM_TRAP = 1842;

    class BlackwingLairEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BlackwingLairEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable blackwing lair strategy", "+blackwing lair") {}
    };

    class BlackwingLairDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BlackwingLairDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable blackwing lair strategy", "-blackwing lair") {}
    };

    class MoveToSuppressionDeviceAction : public MovementAction
    {
    public:
        MoveToSuppressionDeviceAction(PlayerbotAI* ai) : MovementAction(ai, "move to suppression device") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
        bool isUseful() override;
    };

    class StealthForSuppressionDeviceAction : public Action
    {
    public:
        StealthForSuppressionDeviceAction(PlayerbotAI* ai) : Action(ai, "stealth for suppression device") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
        bool isUseful() override;
    };

    class DeactivateSuppressionDeviceAction : public Action
    {
    public:
        DeactivateSuppressionDeviceAction(PlayerbotAI* ai) : Action(ai, "deactivate suppression device") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
    };

    class DisarmSuppressionDeviceAction : public Action
    {
    public:
        DisarmSuppressionDeviceAction(PlayerbotAI* ai) : Action(ai, "disarm suppression device") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
        bool isUseful() override;
    };

    // Suppression room auto-enable/disable.
    class SuppressionRoomEnableStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SuppressionRoomEnableStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable suppression room strategy", "+suppression room") {}
    };

    class SuppressionRoomDisableStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SuppressionRoomDisableStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable suppression room strategy", "-suppression room") {}
    };

    // Boss fight strategy enable/disable.
    class RazorgoreEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RazorgoreEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable razorgore fight strategy", "+razorgore") {}
    };

    class RazorgoreDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RazorgoreDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable razorgore fight strategy", "-razorgore") {}
    };

    // Moves the bot to its assigned corner post in Razorgore's room.
    class MoveToRazorgoreCornerAction : public MovementAction
    {
    public:
        MoveToRazorgoreCornerAction(PlayerbotAI* ai) : MovementAction(ai, "move to razorgore corner") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
    };

    // Attacks the highest priority add in the bot's assigned corner:
    // Mages (12420) > Legionnaires (12416) > unslept Dragonspawn (12422) > gate mobs.
    class AttackRazorgoreCornerAddAction : public AttackAction
    {
    public:
        AttackRazorgoreCornerAddAction(PlayerbotAI* ai) : AttackAction(ai, "attack razorgore corner add") {}
        bool Execute(Event& event) override;
        bool isUseful() override;

    private:
        Unit* FindCornerAdd();
    };

    // Druids cast Hibernate (2637/8952/8953) on unslept Death Talon Dragonspawn (12422).
    class RazorgoreSleepDragonkinAction : public Action
    {
    public:
        RazorgoreSleepDragonkinAction(PlayerbotAI* ai) : Action(ai, "razorgore sleep dragonkin") {}
        bool Execute(Event& event) override;
        bool isPossible() override;
        bool isUseful() override;

    private:
        Creature* FindUnsleptDragonkin();
    };

    // Offtank taunts Razorgore during Conflagration tank swap.
    class TauntRazorgoreAction : public TauntCreatureAction
    {
    public:
        TauntRazorgoreAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt razorgore", 12435) {}
    };

    class VaelEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        VaelEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable vael fight strategy", "+vael") {}
    };

    class VaelDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        VaelDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable vael fight strategy", "-vael") {}
    };

    class ChromaggusEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ChromaggusEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable chromaggus fight strategy", "+chromaggus") {}
    };

    class ChromaggusDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ChromaggusDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable chromaggus fight strategy", "-chromaggus") {}
    };

    class NefarianEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        NefarianEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable nefarian fight strategy", "+nefarian") {}
    };

    class NefarianDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        NefarianDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable nefarian fight strategy", "-nefarian") {}
    };

    class FiremawEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FiremawEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable firemaw fight strategy", "+firemaw") {}
    };

    class FiremawDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FiremawDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable firemaw fight strategy", "-firemaw") {}
    };

    class EbonrocEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        EbonrocEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable ebonroc fight strategy", "+ebonroc") {}
    };

    class EbonrocDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        EbonrocDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable ebonroc fight strategy", "-ebonroc") {}
    };

    class FlamegorEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FlamegorEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable flamegor fight strategy", "+flamegor") {}
    };

    class FlamegorDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FlamegorDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable flamegor fight strategy", "-flamegor") {}
    };

    // Broodlord Lashlayer (12017)
    class BroodlordEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BroodlordEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable broodlord fight strategy", "+broodlord") {}
    };

    class BroodlordDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BroodlordDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable broodlord fight strategy", "-broodlord") {}
    };

    // Brood Affliction: Bronze (23170) clear - afflictions are dispel-immune, so
    // the aura is removed directly (same approach as the upstream scale-cloak /
    // device cheats).
    class ClearChromaggusBronzeAction : public RemoveAuraAction
    {
    public:
        ClearChromaggusBronzeAction(PlayerbotAI* ai) : RemoveAuraAction(ai, "23170") {}
    };

    // Shadow Flame DoT (22682) clear - direct removal like Bronze Affliction 23170,
    // avoiding the need to manually add and equip Onyxia Scale Cloak on 40 bots.
    class ClearShadowFlameDotAction : public RemoveAuraAction
    {
    public:
        ClearShadowFlameDotAction(PlayerbotAI* ai) : RemoveAuraAction(ai, "22682") {}
    };
}
