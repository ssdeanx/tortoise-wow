#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"
#include "Group/Group.h"
#include "Maps/GridNotifiers.h"
#include "Maps/GridNotifiersImpl.h"
#include "Maps/CellImpl.h"

#include <algorithm>
#include <vector>

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

    // -----------------------------------------------------------------------
    // Razorgore (12435) - 4-corner room defense and add management
    // -----------------------------------------------------------------------
    struct RazorgoreCornerDef
    {
        const char* name;
        float x;
        float y;
        float z;
    };

    static const RazorgoreCornerDef RAZORGORE_CORNERS[4] = {
        { "North", -7539.0f, -1052.0f, 407.2f },
        { "South", -7651.0f, -1054.0f, 407.2f },
        { "East",  -7615.0f, -1105.0f, 407.2f },
        { "West",  -7576.0f, -1001.0f, 407.2f }
    };

    inline int RazorgoreCornerIndex(PlayerbotAI* ai)
    {
        Player* bot = ai->GetBot();
        Group* group = bot->GetGroup();
        if (!group)
            return bot->GetGUIDLow() % 4;

        uint8 maxSubgroup = 0;
        uint8 minSubgroup = 255;
        for (Group::member_citerator itr = group->GetMemberSlots().begin(); itr != group->GetMemberSlots().end(); ++itr)
        {
            if (itr->group > maxSubgroup)
                maxSubgroup = itr->group;
            if (itr->group < minSubgroup)
                minSubgroup = itr->group;
        }

        if (maxSubgroup >= 3 && (maxSubgroup - minSubgroup) >= 3)
        {
            uint8 subgroup = group->GetMemberGroup(bot->GetObjectGuid());
            if (maxSubgroup >= 4)
                return (subgroup / 2) % 4;
            else
                return subgroup % 4;
        }

        std::vector<ObjectGuid> tanks;
        std::vector<ObjectGuid> healers;
        std::vector<ObjectGuid> dps;

        for (Group::member_citerator itr = group->GetMemberSlots().begin(); itr != group->GetMemberSlots().end(); ++itr)
        {
            Unit* u = ai->GetUnit(itr->guid);
            Player* p = (u && u->GetTypeId() == TYPEID_PLAYER) ? (Player*)u : nullptr;
            if (!p)
                continue;

            PlayerbotAI* memberAi = GetBotAI(p);
            if (memberAi && memberAi->IsTank(p))
                tanks.push_back(itr->guid);
            else if (memberAi && memberAi->IsHeal(p))
                healers.push_back(itr->guid);
            else
                dps.push_back(itr->guid);
        }

        std::sort(tanks.begin(), tanks.end());
        std::sort(healers.begin(), healers.end());
        std::sort(dps.begin(), dps.end());

        for (size_t i = 0; i < tanks.size(); ++i)
            if (tanks[i] == bot->GetObjectGuid())
                return (int)(i % 4);

        for (size_t i = 0; i < healers.size(); ++i)
            if (healers[i] == bot->GetObjectGuid())
                return (int)(i % 4);

        for (size_t i = 0; i < dps.size(); ++i)
            if (dps[i] == bot->GetObjectGuid())
                return (int)(i % 4);

        return bot->GetGUIDLow() % 4;
    }

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

    // Death Talon Dragonspawn (12422): Druids put them to sleep with Hibernate (2637/8952/8953).
    // Slept dragonkin must be left undisturbed while the raid focuses on Mages and Legionnaires.
    class RazorgoreNeedSleepDragonkinTrigger : public Trigger
    {
    public:
        RazorgoreNeedSleepDragonkinTrigger(PlayerbotAI* ai) : Trigger(ai, "razorgore need sleep dragonkin", 1) {}

        bool IsActive() override
        {
            if (bot->getClass() != CLASS_DRUID)
                return false;

            if (!bot->HasSpell(2637) && !bot->HasSpell(8952) && !bot->HasSpell(8953))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 12435);
            if (boss && ai->HasAura(23040, boss))
                return false;

            std::list<Unit*> creatures;
            MaNGOS::AllCreaturesOfEntryInRangeCheck u_check(bot, 12422, 30.0f);
            MaNGOS::UnitListSearcher<MaNGOS::AllCreaturesOfEntryInRangeCheck> searcher(creatures, u_check);
            Cell::VisitAllObjects(bot, searcher, 30.0f);

            for (Unit* unit : creatures)
            {
                Creature* c = (Creature*)unit;
                if (c && c->IsAlive() && !ai->HasAura("hibernate", c) && !ai->HasAura(2637, c) &&
                    !ai->HasAura(8952, c) && !ai->HasAura(8953, c) && !c->IsPolymorphed())
                {
                    return true;
                }
            }

            return false;
        }
    };

    // Razorgore Phase 1: the raid spreads across the 4 corners of the room
    // (North, South, East, West) to catch add waves as they spawn from each corner.
    class RazorgoreNeedMoveToCornerTrigger : public Trigger
    {
    public:
        RazorgoreNeedMoveToCornerTrigger(PlayerbotAI* ai) : Trigger(ai, "razorgore need move to corner", 2) {}

        bool IsActive() override
        {
            if (!ai->CanMove())
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 12435);
            if (boss && ai->HasAura(23040, boss))
                return false;

            const int corner = RazorgoreCornerIndex(ai);
            const RazorgoreCornerDef& c = RAZORGORE_CORNERS[corner];

            const float dist = bot->GetDistance(c.x, c.y, c.z);
            if (dist <= 12.0f)
                return false;

            if (bot->IsInCombat() && dist < 25.0f)
            {
                Unit* target = AI_VALUE(Unit*, "current target");
                if (target && target->IsAlive() && bot->GetDistance(target) <= 10.0f)
                    return false;
            }

            return true;
        }
    };

    // True when an add (Mage 12420, Legionnaire 12416, unslept Dragonspawn 12422) is near the bot's assigned corner or approaching it.
    class RazorgoreHasCornerAddTrigger : public Trigger
    {
    public:
        RazorgoreHasCornerAddTrigger(PlayerbotAI* ai) : Trigger(ai, "razorgore has corner add", 1) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 12435);
            if (boss && ai->HasAura(23040, boss))
                return false;

            const int corner = RazorgoreCornerIndex(ai);
            const RazorgoreCornerDef& c = RAZORGORE_CORNERS[corner];

            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* creature = ai->GetCreature(guid);
                if (!creature || !creature->IsAlive())
                    continue;

                const uint32 entry = creature->GetEntry();
                if (entry != 12420 && entry != 12416 && entry != 12422 && entry != 12557 && entry != 14456)
                    continue;

                if (entry == 12422 && (ai->HasAura("hibernate", creature) || ai->HasAura(2637, creature) ||
                                       ai->HasAura(8952, creature) || ai->HasAura(8953, creature)))
                    continue;

                const float distToCorner = creature->GetDistance(c.x, c.y, c.z);
                const float distToBot = bot->GetDistance(creature);
                if (distToCorner <= 38.0f || distToBot <= 22.0f)
                    return true;
            }

            return false;
        }
    };

    // Razorgore Phase 2: Conflagration (23023) disorients the active tank.
    // Offtanks must immediately taunt Razorgore so he does not turn and kill non-tanks.
    class RazorgoreConflagrationTrigger : public Trigger
    {
    public:
        RazorgoreConflagrationTrigger(PlayerbotAI* ai) : Trigger(ai, "razorgore conflagration tank switch", 1) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 12435);
            if (!boss || !boss->IsInCombat())
                return false;

            Unit* victim = boss->GetVictim();
            if (!victim || victim == bot)
                return false;

            return ai->HasAura(23023, victim);
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

    // When the current tank gets Burning Adrenaline (23620), they must run away
    // and sacrifice; offtanks taunt Vaelastrasz immediately to prevent raid cleave.
    class VaelTankSwitchTrigger : public Trigger
    {
    public:
        VaelTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "vael tank switch", 2) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 13020);
            return boss && boss->IsInCombat() && boss->GetVictim() &&
                   boss->GetVictim() != bot &&
                   ai->HasAura(23620, boss->GetVictim());
        }
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

    // Ebonroc: Wing Buffet (23339) threat wipe OR Shadow of Ebonroc (23340) debuff.
    // If the boss strikes a target with Shadow of Ebonroc, it heals 25,000 HP per hit!
    class EbonrocTankSwitchTrigger : public Trigger
    {
    public:
        EbonrocTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "ebonroc tank switch", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 14601);
            if (!boss || !boss->IsInCombat() || !boss->GetVictim())
                return false;

            return ai::GetSpellStacks(boss->GetVictim(), 23339) >= 1 ||
                   ai->HasAura(23340, boss->GetVictim());
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

    // Broodlord Lashlayer (12017)
    class BroodlordStartFightTrigger : public StartBossFightTrigger
    {
    public:
        BroodlordStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start broodlord fight", "broodlord", 12017) {}
    };

    class BroodlordEndFightTrigger : public EndBossFightTrigger
    {
    public:
        BroodlordEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end broodlord fight", "broodlord", 12017) {}
    };

    // Knock Away (18670) wipes 50% threat on the victim. Offtanks taunt.
    class BroodlordTankSwitchTrigger : public Trigger
    {
    public:
        BroodlordTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "broodlord tank switch", 2) {}

        bool IsActive() override
        {
            if (!ai->IsTank(bot))
                return false;

            Unit* boss = GetRaidTargetByEntry(ai, 12017);
            return boss && boss->IsInCombat() && boss->GetVictim() &&
                   boss->GetVictim() != bot &&
                   (ai::GetSpellStacks(boss->GetVictim(), 18670) >= 1 || ai->HasAura(18670, boss->GetVictim()));
        }
    };

    // Shadow Flame DoT (22682) - applied by Firemaw, Ebonroc, Flamegor, Nefarian
    // when target lacks Onyxia Scale Cloak (22683). Auto-cleared like Bronze Affliction.
    class ShadowFlameDotTrigger : public Trigger
    {
    public:
        ShadowFlameDotTrigger(PlayerbotAI* ai) : Trigger(ai, "shadow flame dot", 1) {}

        bool IsActive() override { return ai->HasAura(22682, bot); }
    };
}
