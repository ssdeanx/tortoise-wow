#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"
#include "UseItemAction.h"
#include "playerbot/strategy/values/PositionValue.h"
#include "playerbot/strategy/triggers/NaxxramasDungeonTriggers.h"
#include "Spells/SpellDefines.h"

namespace ai
{
    class NaxxramasEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        NaxxramasEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable naxxramas strategy", "+naxxramas") {}
    };

    class NaxxramasDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        NaxxramasDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable naxxramas strategy", "-naxxramas") {}
    };

    class FourHorsemanEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FourHorsemanEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable four horseman fight strategy", "+four horseman") {}
    };

    class FourHorsemanDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FourHorsemanDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable four horseman fight strategy", "-four horseman") {}
    };

    class AnubRekhanEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        AnubRekhanEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable anubrekhan fight strategy", "+anubrekhan") {}
    };

    class AnubRekhanDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        AnubRekhanDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable anubrekhan fight strategy", "-anubrekhan") {}
    };

    class FaerlinaEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FaerlinaEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable faerlina fight strategy", "+faerlina") {}
    };

    class FaerlinaDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        FaerlinaDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable faerlina fight strategy", "-faerlina") {}
    };

    class MaexxnaEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MaexxnaEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable maexxna fight strategy", "+maexxna") {}
    };

    class MaexxnaDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MaexxnaDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable maexxna fight strategy", "-maexxna") {}
    };

    class NothEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        NothEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable noth fight strategy", "+noth") {}
    };

    class NothDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        NothDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable noth fight strategy", "-noth") {}
    };

    class HeiganEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        HeiganEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable heigan fight strategy", "+heigan") {}
    };

    class HeiganDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        HeiganDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable heigan fight strategy", "-heigan") {}
    };

    class LoathebEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        LoathebEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable loatheb fight strategy", "+loatheb") {}
    };

    class LoathebDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        LoathebDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable loatheb fight strategy", "-loatheb") {}
    };

    class GrobbulusEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GrobbulusEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable grobbulus fight strategy", "+grobbulus") {}
    };

    class GrobbulusDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GrobbulusDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable grobbulus fight strategy", "-grobbulus") {}
    };

    class GluthEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GluthEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable gluth fight strategy", "+gluth") {}
    };

    class GluthDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GluthDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable gluth fight strategy", "-gluth") {}
    };

    class ThaddiusEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ThaddiusEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable thaddius fight strategy", "+thaddius") {}
    };

    class ThaddiusDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ThaddiusDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable thaddius fight strategy", "-thaddius") {}
    };

    class SapphironEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SapphironEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable sapphiron fight strategy", "+sapphiron") {}
    };

    class SapphironDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SapphironDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable sapphiron fight strategy", "-sapphiron") {}
    };

    class KelThuzadEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        KelThuzadEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable kelthuzad fight strategy", "+kelthuzad") {}
    };

    class KelThuzadDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        KelThuzadDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable kelthuzad fight strategy", "-kelthuzad") {}
    };

    class MoveAwayFromAnubRekhanAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromAnubRekhanAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from anubrekhan", 15956, 18.0f) {}
    };

    class TauntGluthAction : public TauntCreatureAction
    {
    public:
        TauntGluthAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt gluth", 15932) {}
    };

    class JoinThaddiusPolarityAction : public FollowChargedAllyAction
    {
    public:
        JoinThaddiusPolarityAction(PlayerbotAI* ai) : FollowChargedAllyAction(ai, "join thaddius polarity", 28059, 28084) {}
    };

    class GothikEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GothikEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable gothik fight strategy", "+gothik") {}
    };

    class GothikDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GothikDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable gothik fight strategy", "-gothik") {}
    };

    class RazuviousEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RazuviousEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable razuvious fight strategy", "+razuvious") {}
    };

    class RazuviousDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RazuviousDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable razuvious fight strategy", "-razuvious") {}
    };

    class PatchwerkEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        PatchwerkEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable patchwerk fight strategy", "+patchwerk") {}
    };

    class PatchwerkDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        PatchwerkDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable patchwerk fight strategy", "-patchwerk") {}
    };

    // Kel'Thuzad's Chains (28410): CC the chained players, do not kill them.
    class CcChainedPlayerAction : public CcMindControlledAllyAction
    {
    public:
        CcChainedPlayerAction(PlayerbotAI* ai) : CcMindControlledAllyAction(ai, "cc chained player", 28410) {}
    };

    // Sapphiron: run behind the nearest Ice Block (GO 181247) relative to the
    // boss so Frost Breath does not land.
    class HideBehindIceblockAction : public MovementAction
    {
    public:
        HideBehindIceblockAction(PlayerbotAI* ai) : MovementAction(ai, "hide behind iceblock") {}

        bool Execute(Event& event) override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15989);
            if (!boss)
                return false;

            GameObject* iceblock = nullptr;
            float bestDistance = FLT_MAX;
            for (const ObjectGuid& guid : AI_VALUE2(std::list<ObjectGuid>, "nearest game objects no los", 181247))
            {
                GameObject* go = ai->GetGameObject(guid);
                if (!go)
                    continue;

                const float distance = bot->GetDistance(go);
                if (distance < bestDistance)
                {
                    bestDistance = distance;
                    iceblock = go;
                }
            }

            if (!iceblock || bestDistance > 40.0f)
                return false;

            // Point on the far side of the block from Sapphiron.
            WorldPosition bossPos(boss);
            WorldPosition blockPos(iceblock);
            const float angle = bossPos.getAngleTo(blockPos);
            WorldPosition hideSpot = blockPos + WorldPosition(0, 3.0f * cos(angle), 3.0f * sin(angle), 1.0f);
            hideSpot.setZ(hideSpot.getHeight());

            if (boss->IsWithinLOS(hideSpot.getX(), hideSpot.getY(), hideSpot.getZ() + bot->GetCollisionHeight(), true))
                return false;

            return MoveTo(bot->GetMapId(), hideSpot.getX(), hideSpot.getY(), hideSpot.getZ(), false, false, false, true);
        }

        bool isPossible() override { return MovementAction::isPossible() && ai->CanMove(); }

        bool isUseful() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15989);
            return boss && boss->IsInCombat() && boss->IsWithinLOSInMap(bot, true);
        }
    };

    // Gothik split: walk to the assigned side post (GothikSideIndex) while
    // idle so each side of the combat gate has enough players.
    class MoveToGothikPostAction : public MovementAction
    {
    public:
        MoveToGothikPostAction(PlayerbotAI* ai) : MovementAction(ai, "move to gothik post") {}

        bool Execute(Event& event) override
        {
            const int side = GothikSideIndex(ai);
            if (side < 0)
                return false;

            const float x = 2692.07f;
            const float y = side == 0 ? -3428.78f : -3322.35f;
            const float z = side == 0 ? 268.6f : 267.77f;

            if (bot->GetDistance(x, y, z) <= 10.0f)
                return false;

            return MoveTo(bot->GetMapId(), x, y, z, false, false, false, true);
        }

        bool isPossible() override { return MovementAction::isPossible() && ai->CanMove(); }
    };

    // Razuvious: a priest mind-controls an Understudy (no Mind Exhaustion).
    class MindControlUnderstudyAction : public Action
    {
    public:
        MindControlUnderstudyAction(PlayerbotAI* ai) : Action(ai, "mind control understudy") {}

        bool Execute(Event& event) override
        {
            Unit* understudy = FindRazuviousUnderstudy(ai);
            if (!understudy)
                return false;

            return ai->CastSpell("mind control", understudy);
        }

        bool isUseful() override
        {
            return ai->GetBot()->getClass() == CLASS_PRIEST && !ai->GetBot()->GetCharm() &&
                FindRazuviousUnderstudy(ai) != nullptr;
        }
    };

    // The mind-controlled Understudy tanks Razuvious: SHIELD WALL FIRST (29061)
    // - otherwise Razuvious kills the understudy before the next taunt cycle -
    // then Taunt (29060) whenever he is not already on the understudy. The
    // understudy is kept attacking him.
    class UnderstudyTauntAction : public Action
    {
    public:
        UnderstudyTauntAction(PlayerbotAI* ai) : Action(ai, "understudy taunt") {}

        bool Execute(Event& event) override
        {
            Unit* charm = bot->GetCharm();
            if (!charm || charm->GetEntry() != 16803 || !charm->IsAlive())
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 16061);
            if (!boss || !boss->IsInCombat())
                return false;

            // Keep the understudy on Razuvious.
            if (charm->GetVictim() != boss)
                charm->Attack(boss, true);

            // Shield Wall before the taunt.
            if (!charm->HasAura(29061))
            {
                if (charm->CastSpell(charm, 29061, false) == SPELL_CAST_OK)
                    return true;
            }

            // Taunt whenever Razuvious is not on the understudy.
            if (boss->GetVictim() != charm)
                return charm->CastSpell(boss, 29060, false) == SPELL_CAST_OK;

            return false;
        }

        bool isUseful() override
        {
            Unit* charm = bot->GetCharm();
            return charm && charm->GetEntry() == 16803 && charm->IsAlive();
        }
    };
}
