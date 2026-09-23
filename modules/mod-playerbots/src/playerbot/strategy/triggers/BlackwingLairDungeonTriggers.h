#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"

namespace ai
{
    class BlackwingLairEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        BlackwingLairEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter blackwing lair", "blackwing lair", 469) {}
    };

    class BlackwingLairLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        BlackwingLairLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave blackwing lair", "blackwing lair", 469) {}
    };

    // Suppression device room auto-enable: the rogue stealth/move/disarm loop was
    // only reachable via a manual "co +suppression room" - these triggers turn the
    // strategy on when a device GO (179784) is in sight and off once none are.
    class SuppressionRoomEnteredTrigger : public ValueTrigger
    {
    public:
        SuppressionRoomEnteredTrigger(PlayerbotAI* ai) : ValueTrigger(ai, "suppression room entered", 2)
        {
            qualifier = "has object::go usable filter::go trapped filter::entry filter::{gos in sight,suppression devices}";
        }
    };

    class SuppressionRoomLeftTrigger : public ValueTrigger
    {
    public:
        SuppressionRoomLeftTrigger(PlayerbotAI* ai) : ValueTrigger(ai, "suppression room left", 2)
        {
            qualifier = "not::has object::go usable filter::go trapped filter::entry filter::{gos in sight,suppression devices}";
        }
    };

    class SuppressionDeviceNeedStealthTrigger : public Trigger
    {
    public:
        SuppressionDeviceNeedStealthTrigger(PlayerbotAI* ai) : Trigger(ai, "suppression device need stealth", 1) {}

        bool IsActive() override
        {
            if (bot->getClass() != CLASS_ROGUE)
                return false;

            if (ai->HasAura("stealth", bot))
                return false;

            std::list<GuidPosition> gos = AI_VALUE(std::list<GuidPosition>, "go usable filter::go trapped filter::entry filter::{gos in sight,suppression devices}");
            return !gos.empty();
        }
    };

    class SuppressionDeviceInSightTrigger : public Trigger
    {
    public:
        SuppressionDeviceInSightTrigger(PlayerbotAI* ai) : Trigger(ai, "suppression device in sight", 1) {}

        bool IsActive() override
        {
            if (bot->getClass() != CLASS_ROGUE)
                return false;

            std::list<GuidPosition> gosInSight = AI_VALUE(std::list<GuidPosition>, "go usable filter::go trapped filter::entry filter::{gos in sight,suppression devices}");
            std::list<GuidPosition> gosClose = AI_VALUE(std::list<GuidPosition>, "entry filter::{gos close,suppression devices}");

            return !gosInSight.empty() && gosClose.empty();
        }
    };

    class SuppressionDeviceCloseTrigger : public Trigger
    {
    public:
        SuppressionDeviceCloseTrigger(PlayerbotAI* ai) : Trigger(ai, "suppression device close", 1) {}

        bool IsActive() override
        {
            if (bot->getClass() != CLASS_ROGUE)
                return false;

            std::list<GuidPosition> gos = AI_VALUE(std::list<GuidPosition>, "go usable filter::go trapped filter::entry filter::{gos close,suppression devices}");
            return !gos.empty();
        }
    };

    // Razorgore (12435)
    class RazorgoreStartFightTrigger : public StartBossFightTrigger
    {
    public:
        RazorgoreStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start razorgore fight", "razorgore", 12435) {}
    };

    class RazorgoreEndFightTrigger : public EndBossFightTrigger
    {
    public:
        RazorgoreEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end razorgore fight", "razorgore", 12435) {}
    };

    // Warming Flames (23040, boss_razorgore.cpp via instance egg logic): applied
    // to Razorgore once all 30 eggs are destroyed - he becomes killable and the
    // raid burns him. Before that, bots must never touch him (early kill FAILs
    // the encounter).
    class RazorgoreWarmingTrigger : public Trigger
    {
    public:
        RazorgoreWarmingTrigger(PlayerbotAI* ai) : Trigger(ai, "razorgore warming flames", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 12435);
            return boss && ai->HasAura(23040, boss);
        }
    };

    // Vaelastrasz (13020)
    class VaelStartFightTrigger : public StartBossFightTrigger
    {
    public:
        VaelStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start vael fight", "vael", 13020) {}
    };

    class VaelEndFightTrigger : public EndBossFightTrigger
    {
    public:
        VaelEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end vael fight", "vael", 13020) {}
    };

    // Burning Adrenaline (23620, boss_vaelastrasz.cpp): the affected unit is a
    // ticking bomb - run away from the raid immediately. (The Blackwing
    // Technicians 13996 in this room are pre-event spawns, not part of the
    // encounter.)
    class VaelBurningAdrenalineTrigger : public Trigger
    {
    public:
        VaelBurningAdrenalineTrigger(PlayerbotAI* ai) : Trigger(ai, "vael burning adrenaline", 1) {}

        bool IsActive() override { return ai->HasAura(23620, bot); }
    };

    // Firemaw (11983): Flame Buffet (23341) stacks on the whole room every ~2 s.
    // Non-tanks with enough stacks hide behind the pillar so stacks drop.
    class FiremawFlameBuffetHideTrigger : public Trigger
    {
    public:
        FiremawFlameBuffetHideTrigger(PlayerbotAI* ai) : Trigger(ai, "firemaw flame buffet stacks", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 11983);
            if (!boss || !boss->IsInCombat())
                return false;

            if (ai->IsTank(bot))
                return false;

            return ai::GetSpellStacks(bot, 23341) >= 3;
        }
    };

    // Tank switches - Wing Buffet (23339) wipes the active tank's threat.
    // Trigger: the boss's current victim carries the Wing Buffet aura; the
    // offtanks (tanks that are NOT the current victim) taunt.
    class FiremawTankSwitchTrigger : public Trigger
    {
    public:
        FiremawTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "firemaw tank switch", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 11983);
            return boss && boss->IsInCombat() && ai::GetSpellStacks(boss->GetVictim(), 23339) >= 1;
        }
    };

    class EbonrocTankSwitchTrigger : public Trigger
    {
    public:
        EbonrocTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "ebonroc tank switch", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 14601);
            return boss && boss->IsInCombat() && ai::GetSpellStacks(boss->GetVictim(), 23339) >= 1;
        }
    };

    class FlamegorTankSwitchTrigger : public Trigger
    {
    public:
        FlamegorTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "flamegor tank switch", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 11981);
            return boss && boss->IsInCombat() && ai::GetSpellStacks(boss->GetVictim(), 23339) >= 1;
        }
    };

    // Inactive tanks stay out of line of sight so they take no Flame Buffet
    // stacks while the main tank holds the boss.
    class FiremawInactiveTankTrigger : public Trigger
    {
    public:
        FiremawInactiveTankTrigger(PlayerbotAI* ai) : Trigger(ai, "firemaw inactive tank", 2) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 11983);
            return boss && boss->IsInCombat() && boss->GetVictim() != bot;
        }
    };

    class EbonrocInactiveTankTrigger : public Trigger
    {
    public:
        EbonrocInactiveTankTrigger(PlayerbotAI* ai) : Trigger(ai, "ebonroc inactive tank", 2) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 14601);
            return boss && boss->IsInCombat() && boss->GetVictim() != bot;
        }
    };

    class FlamegorInactiveTankTrigger : public Trigger
    {
    public:
        FlamegorInactiveTankTrigger(PlayerbotAI* ai) : Trigger(ai, "flamegor inactive tank", 2) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 11981);
            return boss && boss->IsInCombat() && boss->GetVictim() != bot;
        }
    };

    // Firemaw (11983), Ebonroc (14601), Flamegor (11981) - entries verified
    // against tw_world.creature_template (Turtle: Firemaw is 11983, Flamegor
    // 11981 - not the other way round).
    class FiremawStartFightTrigger : public StartBossFightTrigger
    {
    public:
        FiremawStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start firemaw fight", "firemaw", 11983) {}
    };

    class FiremawEndFightTrigger : public EndBossFightTrigger
    {
    public:
        FiremawEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end firemaw fight", "firemaw", 11983) {}
    };

    class EbonrocStartFightTrigger : public StartBossFightTrigger
    {
    public:
        EbonrocStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start ebonroc fight", "ebonroc", 14601) {}
    };

    class EbonrocEndFightTrigger : public EndBossFightTrigger
    {
    public:
        EbonrocEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end ebonroc fight", "ebonroc", 14601) {}
    };

    class FlamegorStartFightTrigger : public StartBossFightTrigger
    {
    public:
        FlamegorStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start flamegor fight", "flamegor", 11981) {}
    };

    class FlamegorEndFightTrigger : public EndBossFightTrigger
    {
    public:
        FlamegorEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end flamegor fight", "flamegor", 11981) {}
    };

    // Chromaggus (14020): Time Lapse (23310) wipes the active tank's threat -
    // the offtank taunts. The aura is on the boss's victim.
    class ChromaggusTimeLapseTrigger : public Trigger
    {
    public:
        ChromaggusTimeLapseTrigger(PlayerbotAI* ai) : Trigger(ai, "chromaggus time lapse", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 14020);
            return boss && boss->IsInCombat() && boss->GetVictim() &&
                   boss->GetVictim()->GetTypeId() == TYPEID_PLAYER &&
                   ai->HasAura(23310, boss->GetVictim());
        }
    };

    // Chromaggus (14020)
    class ChromaggusStartFightTrigger : public StartBossFightTrigger
    {
    public:
        ChromaggusStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start chromaggus fight", "chromaggus", 14020) {}
    };

    class ChromaggusEndFightTrigger : public EndBossFightTrigger
    {
    public:
        ChromaggusEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end chromaggus fight", "chromaggus", 14020) {}
    };

    // Brood Affliction: Bronze (23170, boss_chromaggus.cpp) - the stun chain.
    // Brood afflictions are dispel-immune; bots clear the bronze one directly.
    class ChromaggusBronzeAfflictionTrigger : public Trigger
    {
    public:
        ChromaggusBronzeAfflictionTrigger(PlayerbotAI* ai) : Trigger(ai, "chromaggus bronze affliction", 1) {}

        bool IsActive() override { return ai->HasAura(23170, bot); }
    };

    // Nefarian (11583)
    class NefarianStartFightTrigger : public StartBossFightTrigger
    {
    public:
        NefarianStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start nefarian fight", "nefarian", 11583) {}
    };

    class NefarianEndFightTrigger : public EndBossFightTrigger
    {
    public:
        NefarianEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end nefarian fight", "nefarian", 11583) {}
    };
}
