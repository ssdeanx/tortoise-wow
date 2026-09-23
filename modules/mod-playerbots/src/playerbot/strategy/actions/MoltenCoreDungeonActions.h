#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"
#include "UseItemAction.h"
#include "Chat/Chat.h"

namespace ai
{
    class MoltenCoreEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MoltenCoreEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable molten core strategy", "+molten core") {}
    };

    class MoltenCoreDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MoltenCoreDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable molten core strategy", "-molten core") {}
    };

    class MagmadarEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MagmadarEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable magmadar fight strategy", "+magmadar") {}
    };

    class MagmadarDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MagmadarDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable magmadar fight strategy", "-magmadar") {}
    };

    class MagmadarMoveAwayFromLavaBombAction : public MoveAwayFromHazard
    {
    public:
        MagmadarMoveAwayFromLavaBombAction(PlayerbotAI* ai) : MoveAwayFromHazard(ai, "move away from magmadar lava bomb") {}
    };

    class MagmadarMoveAwayAction : public MoveAwayFromCreature
    {
    public:
        MagmadarMoveAwayAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from magmadar", 11982, 31.0f) {}
    };

    // Fight-strategy enable/disable for every MC boss.
    class LucifronEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        LucifronEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable lucifron fight strategy", "+lucifron") {}
    };

    class LucifronDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        LucifronDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable lucifron fight strategy", "-lucifron") {}
    };

    class GehennasEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GehennasEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable gehennas fight strategy", "+gehennas") {}
    };

    class GehennasDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GehennasDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable gehennas fight strategy", "-gehennas") {}
    };

    class GarrEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GarrEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable garr fight strategy", "+garr") {}
    };

    class GarrDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GarrDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable garr fight strategy", "-garr") {}
    };

    class GeddonEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GeddonEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable geddon fight strategy", "+geddon") {}
    };

    class GeddonDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GeddonDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable geddon fight strategy", "-geddon") {}
    };

    class ShazzrahEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ShazzrahEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable shazzrah fight strategy", "+shazzrah") {}
    };

    class ShazzrahDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ShazzrahDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable shazzrah fight strategy", "-shazzrah") {}
    };

    class GolemaggEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GolemaggEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable golemagg fight strategy", "+golemagg") {}
    };

    class GolemaggDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        GolemaggDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable golemagg fight strategy", "-golemagg") {}
    };

    class SulfuronEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SulfuronEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable sulfuron fight strategy", "+sulfuron") {}
    };

    class SulfuronDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        SulfuronDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable sulfuron fight strategy", "-sulfuron") {}
    };

    class DomoEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        DomoEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable domo fight strategy", "+domo") {}
    };

    class DomoDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        DomoDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable domo fight strategy", "-domo") {}
    };

    class RagnarosEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RagnarosEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable ragnaros fight strategy", "+ragnaros") {}
    };

    class RagnarosDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        RagnarosDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable ragnaros fight strategy", "-ragnaros") {}
    };

    // Baron Geddon (12056): run out of Armageddon blast radius / Inferno channel.
    class MoveAwayFromGeddonAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromGeddonAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from geddon", 12056, 35.0f) {}
    };

    // Shazzrah (12264): ranged/healers hold max range from arcane explosion.
    class MoveAwayFromShazzrahAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromShazzrahAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from shazzrah", 12264, 28.0f) {}
    };

    // Ragnaros (11502): ranged/healers stay clear of Wrath knockback + Magma Blast.
    class MoveAwayFromRagnarosAction : public MoveAwayFromCreature
    {
    public:
        MoveAwayFromRagnarosAction(PlayerbotAI* ai) : MoveAwayFromCreature(ai, "move away from ragnaros", 11502, 25.0f) {}
    };

    // Every raid bot needs a douse item for the rune chain. Grants one Aqual
    // Quintessence (17333) when the bot enters MC without either douse item,
    // via the core's ".additem" chat command (same path the test framework
    // uses, strategy/tests/TestAction.cpp). Eternal Quintessence (22754) is the
    // upgradable replacement and is accepted too.
    class AddMCDouseItemAction : public Action
    {
    public:
        AddMCDouseItemAction(PlayerbotAI* ai) : Action(ai, "add mc douse item") {}

        bool Execute(Event& event) override
        {
            if (bot->HasItemCount(17333, 1) || bot->HasItemCount(22754, 1))
                return false;

            return ChatHandler(bot).ParseCommands(".additem 17333 1") != 0;
        }

        bool isUseful() override
        {
            return bot->GetMapId() == 409 && !bot->HasItemCount(17333, 1) && !bot->HasItemCount(22754, 1);
        }
    };

    class MoveToMCRuneAction : public MoveToAction
    {
    public:
        MoveToMCRuneAction(PlayerbotAI* ai) : MoveToAction(ai, "move to mc rune") { qualifier = "entry filter::{gos in sight,mc douseable runes}"; }
    };

    class DouseMCRuneActionAqual : public UseItemIdAction
    {
    public:
        DouseMCRuneActionAqual(PlayerbotAI* ai) : UseItemIdAction(ai, "douse mc rune aqual") { qualifier = "{17333,entry filter::{gos close,mc douseable runes}}"; }
    };

    class DouseMCRuneActionEternal : public UseItemIdAction
    {
    public:
        DouseMCRuneActionEternal(PlayerbotAI* ai) : UseItemIdAction(ai, "douse mc rune eternal") { qualifier = "{22754,entry filter::{gos close,mc douseable runes}}"; }
    };
}
