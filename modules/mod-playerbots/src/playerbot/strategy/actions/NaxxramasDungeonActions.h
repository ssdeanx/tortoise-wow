#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"
#include "UseItemAction.h"

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
}
