#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"

namespace ai
{
    class MoltenCoreEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        MoltenCoreEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter molten core", "molten core", 409) {}
    };

    class MoltenCoreLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        MoltenCoreLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave molten core", "molten core", 409) {}
    };

    // Magmadar (11982)
    class MagmadarStartFightTrigger : public StartBossFightTrigger
    {
    public:
        MagmadarStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start magmadar fight", "magmadar", 11982) {}
    };

    class MagmadarEndFightTrigger : public EndBossFightTrigger
    {
    public:
        MagmadarEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end magmadar fight", "magmadar", 11982) {}
    };

    class MagmadarLavaBombTrigger : public CloseToGameObjectHazardTrigger
    {
    public:
        MagmadarLavaBombTrigger(PlayerbotAI* ai) : CloseToGameObjectHazardTrigger(ai, "magmadar lava bomb", 177704, 5.0f, 60) {}
    };

    class MagmadarTooCloseTrigger : public CloseToCreatureTrigger
    {
    public:
        MagmadarTooCloseTrigger(PlayerbotAI* ai) : CloseToCreatureTrigger(ai, "magmadar too close", 11982, 30.0f) {}
    };

    // Lucifron (12118)
    class LucifronStartFightTrigger : public StartBossFightTrigger
    {
    public:
        LucifronStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start lucifron fight", "lucifron", 12118) {}
    };

    class LucifronEndFightTrigger : public EndBossFightTrigger
    {
    public:
        LucifronEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end lucifron fight", "lucifron", 12118) {}
    };

    // Gehennas (12259)
    class GehennasStartFightTrigger : public StartBossFightTrigger
    {
    public:
        GehennasStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start gehennas fight", "gehennas", 12259) {}
    };

    class GehennasEndFightTrigger : public EndBossFightTrigger
    {
    public:
        GehennasEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end gehennas fight", "gehennas", 12259) {}
    };

    // Garr (12057)
    class GarrStartFightTrigger : public StartBossFightTrigger
    {
    public:
        GarrStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start garr fight", "garr", 12057) {}
    };

    class GarrEndFightTrigger : public EndBossFightTrigger
    {
    public:
        GarrEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end garr fight", "garr", 12057) {}
    };

    // Geddon (12056)
    class GeddonStartFightTrigger : public StartBossFightTrigger
    {
    public:
        GeddonStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start geddon fight", "geddon", 12056) {}
    };

    class GeddonEndFightTrigger : public EndBossFightTrigger
    {
    public:
        GeddonEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end geddon fight", "geddon", 12056) {}
    };

    // Baron Geddon mechanics (spell ids from src/scripts/dungeons/molten_core/boss_baron_geddon.cpp)
    // Living Bomb 20475, Inferno 19695, Armageddon 20478
    class GeddonLivingBombTrigger : public Trigger
    {
    public:
        GeddonLivingBombTrigger(PlayerbotAI* ai) : Trigger(ai, "geddon living bomb", 1) {}

        bool IsActive() override { return ai->HasAura(20475, bot); }
    };

    class GeddonInfernoTrigger : public Trigger
    {
    public:
        GeddonInfernoTrigger(PlayerbotAI* ai) : Trigger(ai, "geddon inferno", 1) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 12056);
            return boss && ai->HasAura(19695, boss);
        }
    };

    class GeddonArmageddonTrigger : public Trigger
    {
    public:
        GeddonArmageddonTrigger(PlayerbotAI* ai) : Trigger(ai, "geddon armageddon", 1) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 12056);
            return boss && ai->HasAura(20478, boss);
        }
    };

    // Shazzrah (12264)
    class ShazzrahStartFightTrigger : public StartBossFightTrigger
    {
    public:
        ShazzrahStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start shazzrah fight", "shazzrah", 12264) {}
    };

    class ShazzrahEndFightTrigger : public EndBossFightTrigger
    {
    public:
        ShazzrahEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end shazzrah fight", "shazzrah", 12264) {}
    };

    class ShazzrahTooCloseTrigger : public CloseToCreatureTrigger
    {
    public:
        ShazzrahTooCloseTrigger(PlayerbotAI* ai) : CloseToCreatureTrigger(ai, "shazzrah too close", 12264, 25.0f) {}
    };

    // Golemagg (11988)
    class GolemaggStartFightTrigger : public StartBossFightTrigger
    {
    public:
        GolemaggStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start golemagg fight", "golemagg", 11988) {}
    };

    class GolemaggEndFightTrigger : public EndBossFightTrigger
    {
    public:
        GolemaggEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end golemagg fight", "golemagg", 11988) {}
    };

    // Sulfuron (12098)
    class SulfuronStartFightTrigger : public StartBossFightTrigger
    {
    public:
        SulfuronStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start sulfuron fight", "sulfuron", 12098) {}
    };

    class SulfuronEndFightTrigger : public EndBossFightTrigger
    {
    public:
        SulfuronEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end sulfuron fight", "sulfuron", 12098) {}
    };

    // Majordomo Executus (12018)
    class DomoStartFightTrigger : public StartBossFightTrigger
    {
    public:
        DomoStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start domo fight", "domo", 12018) {}
    };

    class DomoEndFightTrigger : public EndBossFightTrigger
    {
    public:
        DomoEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end domo fight", "domo", 12018) {}
    };

    // Ragnaros (11502)
    class RagnarosStartFightTrigger : public StartBossFightTrigger
    {
    public:
        RagnarosStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start ragnaros fight", "ragnaros", 11502) {}
    };

    class RagnarosEndFightTrigger : public EndBossFightTrigger
    {
    public:
        RagnarosEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end ragnaros fight", "ragnaros", 11502) {}
    };

    class RagnarosTooCloseTrigger : public CloseToCreatureTrigger
    {
    public:
        RagnarosTooCloseTrigger(PlayerbotAI* ai) : CloseToCreatureTrigger(ai, "ragnaros too close", 11502, 25.0f) {}
    };

    // Every bot entering MC should carry a douse item for the rune chain; the
    // grant runs only on map 409 and only when neither item is present.
    class MCDouseItemMissingTrigger : public Trigger
    {
    public:
        MCDouseItemMissingTrigger(PlayerbotAI* ai) : Trigger(ai, "mc douse item missing", 5) {}

        bool IsActive() override
        {
            if (!bot->IsInWorld() || bot->IsBeingTeleported() || bot->GetMapId() != 409)
                return false;

            return !bot->HasItemCount(17333, 1) && !bot->HasItemCount(22754, 1);
        }
    };

    class MCRuneInSightTrigger : public ValueTrigger
    {
    public:
        MCRuneInSightTrigger(PlayerbotAI* ai) : ValueTrigger(ai, "mc rune in sight", 1)
        {
            qualifier = "and::{"
                "action possible::use id::17333,"
                "has object::go usable filter::go trapped filter::entry filter::{gos in sight,mc douseable runes},"
                "not::has object::entry filter::{gos close,mc douseable runes}"
                "}";
        }
    };

    class MCRuneCloseTrigger : public ValueTrigger
    {
    public:
        MCRuneCloseTrigger(PlayerbotAI* ai) : ValueTrigger(ai, "mc rune close", 1) { qualifier = "has object::go usable filter::entry filter::{gos close,mc douseable runes}"; }
    };
}
