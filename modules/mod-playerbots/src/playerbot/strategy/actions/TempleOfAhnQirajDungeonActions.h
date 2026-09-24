#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"
#include "UseItemAction.h"
#include "AttackAction.h"
#include "playerbot/strategy/triggers/DungeonTriggers.h"
#include "playerbot/strategy/triggers/TempleOfAhnQirajDungeonTriggers.h"

namespace ai
{
    class TempleOfAhnQirajEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        TempleOfAhnQirajEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable temple of ahnqiraj strategy", "+temple of ahnqiraj") {}
    };

    class TempleOfAhnQirajDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        TempleOfAhnQirajDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable temple of ahnqiraj strategy", "-temple of ahnqiraj") {}
    };

    // Fight-strategy enable/disable for every AQ40 encounter.
    class SkeramEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SkeramEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable skeram fight strategy", "+skeram") {}
    };

    class SkeramDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SkeramDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable skeram fight strategy", "-skeram") {}
    };

    class BugTrioEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BugTrioEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable bug trio fight strategy", "+bug trio") {}
    };

    class BugTrioDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        BugTrioDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable bug trio fight strategy", "-bug trio") {}
    };

    class SarturaEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SarturaEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable sartura fight strategy", "+sartura") {}
    };

    class SarturaDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SarturaDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable sartura fight strategy", "-sartura") {}
    };

    class FankrissEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FankrissEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable fankriss fight strategy", "+fankriss") {}
    };

    class FankrissDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FankrissDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable fankriss fight strategy", "-fankriss") {}
    };

    class ViscidusEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ViscidusEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable viscidus fight strategy", "+viscidus") {}
    };

    class ViscidusDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ViscidusDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable viscidus fight strategy", "-viscidus") {}
    };

    class HuhuranEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        HuhuranEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable huhuran fight strategy", "+huhuran") {}
    };

    class HuhuranDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        HuhuranDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable huhuran fight strategy", "-huhuran") {}
    };

    class TwinsEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        TwinsEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable twins fight strategy", "+twins") {}
    };

    class TwinsDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        TwinsDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable twins fight strategy", "-twins") {}
    };

    class OuroEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        OuroEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable ouro fight strategy", "+ouro") {}
    };

    class OuroDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        OuroDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable ouro fight strategy", "-ouro") {}
    };

    class CthunEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        CthunEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable cthun fight strategy", "+cthun") {}
    };

    class CthunDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        CthunDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable cthun fight strategy", "-cthun") {}
    };

    class TauntFankrissAction : public TauntCreatureAction
    {
    public:
        TauntFankrissAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt fankriss", 15510) {}
    };

    // Melee steps out of Sartura's / a Royal Guard's whirlwind; the 15 yd
    // search range keeps ranged bots (already farther away) in place.
    class MoveAwayFromSarturaAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromSarturaAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from sartura", 15516, 15.0f) {}
    };

    class MoveAwayFromSarturaGuardAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromSarturaGuardAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from sartura guard", 15984, 15.0f) {}
    };

    // Viscidus only freezes after 200 frost spell hits and only shatters when
    // hit enough while frozen; mages and shamans cast their frost spell at him
    // instead of their normal rotation for the whole fight.
    class ViscidusFrostAssistAction : public Action
    {
    public:
        ViscidusFrostAssistAction(PlayerbotAI* ai) : Action(ai, "viscidus frost assist") {}

        bool Execute(Event& event) override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15299);
            if (!boss)
                return false;

            if (ai->GetBot()->getClass() == CLASS_MAGE)
                return ai->CastSpell("frostbolt", boss);

            if (ai->GetBot()->getClass() == CLASS_SHAMAN)
                return ai->CastSpell("frost shock", boss);

            return false;
        }

        bool isUseful() override
        {
            if (!GetRaidTargetByEntry(ai, 15299))
                return false;

            const uint32 botClass = ai->GetBot()->getClass();
            return botClass == CLASS_MAGE || botClass == CLASS_SHAMAN;
        }
    };

    // Twin Emperors role split: melee burns Vek'nilash, ranged burns Vek'lor
    // (they swap places every 30-40 s, so the range is the whole room). Both
    // reuse the nearest-entry targeting with qualifier "15275,120"/"15276,120".
    class AttackVeknilashAction : public AttackNearestEntryAction
    {
    public:
        AttackVeknilashAction(PlayerbotAI* ai) : AttackNearestEntryAction(ai, "attack veknilash") {}

        bool isUseful() override
        {
            return ai->IsMelee(ai->GetBot()) && AttackNearestEntryAction::isUseful();
        }
    };

    class AttackVeklorAction : public AttackNearestEntryAction
    {
    public:
        AttackVeklorAction(PlayerbotAI* ai) : AttackNearestEntryAction(ai, "attack veklor") {}

        bool isUseful() override
        {
            return !ai->IsMelee(ai->GetBot()) && AttackNearestEntryAction::isUseful();
        }
    };

    // Vek'lor is the caster twin and must be held at range by a non-melee tank;
    // warlocks use Searing Pain's high threat to hold it (the classic warlock
    // tank), while melee never enter its 10 yd Arcane Burst range because they
    // stay assigned to Vek'nilash.
    class TwinsSearingPainAction : public Action
    {
    public:
        TwinsSearingPainAction(PlayerbotAI* ai) : Action(ai, "searing pain veklor") {}

        bool Execute(Event& event) override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15276);
            if (!boss)
                return false;

            return ai->CastSpell("searing pain", boss);
        }

        bool isUseful() override
        {
            return ai->GetBot()->getClass() == CLASS_WARLOCK && GetRaidTargetByEntry(ai, 15276);
        }
    };

    // Fixed Twin Emperors tank posts: the post holders (first two melee tanks
    // and first two warlocks by GUID - see TwinsTankSideIndex) never chase a
    // twin across the room. They attack the twin of their role while it is on
    // their side (melee tanks -> Vek'nilash, warlocks -> Vek'lor) and hold
    // their post otherwise, walking back if displaced. Every position swap
    // drops threat, so the twin that lands here is grabbed fresh.
    class TwinsTankPostAction : public AttackAction
    {
    public:
        TwinsTankPostAction(PlayerbotAI* ai) : AttackAction(ai, "twins tank post") {}

        bool Execute(Event& event) override
        {
            const int side = TwinsTankSideIndex(ai);
            if (side < 0)
                return false;

            Unit* twin = FindLocalTwin(side);
            if (twin)
                return Attack(event.getOwner() ? event.getOwner() : GetMaster(), twin);

            const float x = PostX(side);
            const float y = PostY(side);
            const float z = PostZ();

            // At the post with nothing to do: hold - tanks do not move.
            if (bot->GetDistance(x, y, z) <= 8.0f)
                return true;

            return MoveTo(bot->GetMapId(), x, y, z, false, false, false, true);
        }

        bool isUseful() override { return TwinsTankSideIndex(ai) >= 0; }

    private:
        static float PostX(int side) { return side == 0 ? -9019.14f : -8874.06f; }
        static float PostY(int side) { return side == 0 ? 1180.07f : 1204.74f; }
        static float PostZ() { return -104.17f; }

        // The twin of this bot's role, only while it is on this side.
        Unit* FindLocalTwin(int side)
        {
            const uint32 entry = bot->getClass() == CLASS_WARLOCK ? 15276 : 15275;
            const float x = PostX(side);
            const float y = PostY(side);
            const float z = PostZ();

            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* twin = ai->GetCreature(guid);
                if (twin && twin->IsAlive() && twin->GetEntry() == entry &&
                    twin->GetDistance(x, y, z) <= 60.0f)
                    return twin;
            }

            return nullptr;
        }
    };

    // Huhuran's Wyvern Sting (26180) sleeps the tank - the offtank picks her up.
    class TauntHuhuranAction : public TauntCreatureAction
    {
    public:
        TauntHuhuranAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt huhuran", 15509) {}
    };

    // C'Thun stomach: the only way out is the punt platform (area trigger 4033
    // at -8546.2, 1987.7, -96.5). Digestive Acid (26476) stacks to 99, so a
    // swallowed bot runs to the platform and is launched back into the room.
    class MoveToCthunStomachExitAction : public MovementAction
    {
    public:
        MoveToCthunStomachExitAction(PlayerbotAI* ai) : MovementAction(ai, "move to cthun stomach exit") {}

        bool Execute(Event& event) override
        {
            const float x = -8546.23f;
            const float y = 1987.65f;
            const float z = -96.52f;

            // Already on the platform - the punt creature takes it from here.
            if (bot->GetDistance(x, y, z) <= 4.0f)
                return false;

            return MoveTo(bot->GetMapId(), x, y, z, false, IsReaction(), false, true);
        }

        bool isPossible() override
        {
            return MovementAction::isPossible() && ai->CanMove();
        }
    };

    // The two Flesh Tentacles (15802) in the stomach are ~21-25 yd from the
    // punt platform, so only ranged bots attack them from the platform; melee
    // bots stay on it and take the punt out.
    class AttackFleshTentacleRangedAction : public AttackNearestEntryAction
    {
    public:
        AttackFleshTentacleRangedAction(PlayerbotAI* ai) : AttackNearestEntryAction(ai, "attack flesh tentacle ranged") {}

        bool isUseful() override
        {
            return ai->IsRanged(ai->GetBot()) && AttackNearestEntryAction::isUseful();
        }
    };

    // Skeram's True Fulfillment (785): the controlled raid member is CC'd, not
    // killed.
    class CcFulfilledPlayerAction : public CcMindControlledAllyAction
    {
    public:
        CcFulfilledPlayerAction(PlayerbotAI* ai) : CcMindControlledAllyAction(ai, "cc fulfilled player", 785) {}
    };
}
