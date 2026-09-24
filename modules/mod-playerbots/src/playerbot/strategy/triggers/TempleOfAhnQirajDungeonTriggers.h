#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"
#include "Group/Group.h"

#include <algorithm>
#include <vector>

namespace ai
{
    class TempleOfAhnQirajEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        TempleOfAhnQirajEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter temple of ahnqiraj", "temple of ahnqiraj", 531) {}
    };

    class TempleOfAhnQirajLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        TempleOfAhnQirajLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave temple of ahnqiraj", "temple of ahnqiraj", 531) {}
    };

    // The Prophet Skeram (15263): splits into two images at 75/50/25%, all
    // copies share the entry; eye/claw tentacles (15726/15725) are burned as
    // they spawn. True Fulfillment (785) mind-controls the closest player -
    // the raid handles the controlled unit, bots assist the leader's target.
    class SkeramStartFightTrigger : public StartBossFightTrigger
    {
    public:
        SkeramStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start skeram fight", "skeram", 15263) {}
    };

    class SkeramEndFightTrigger : public EndBossFightTrigger
    {
    public:
        SkeramEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end skeram fight", "skeram", 15263) {}
    };

    // True Fulfillment (785) mind-controls a raid member; the controlled unit
    // (bot or leader) is CC'd by the raid, not killed (per the server owner).
    class SkeramFulfilledPlayerTrigger : public MindControlledAllyTrigger
    {
    public:
        SkeramFulfilledPlayerTrigger(PlayerbotAI* ai) : MindControlledAllyTrigger(ai, "fulfilled player", 785) {}
    };

    // Bug Trio (Kri 15511 / Yauj 15543 / Vem 15544): when one dies the other
    // two devour the corpse and heal to full, so all three must die together.
    // Yauj Brood (15621) adds are cleaned up. The trio leashes if the fight
    // leaves the room (y < 2060 and x > -8600), so no run-out actions here.
    class BugTrioStartFightTrigger : public Trigger
    {
    public:
        BugTrioStartFightTrigger(PlayerbotAI* ai) : Trigger(ai, "start bug trio fight", 5) {}

        bool IsActive() override
        {
            if (ai->HasStrategy("bug trio", BotState::BOT_STATE_COMBAT))
                return false;

            for (const ObjectGuid& attackerGuid : AI_VALUE(std::list<ObjectGuid>, "attackers"))
            {
                Unit* attacker = ai->GetUnit(attackerGuid);
                if (!attacker)
                    continue;

                const uint32 entry = attacker->GetEntry();
                if (entry == 15511 || entry == 15543 || entry == 15544)
                    return true;
            }

            return false;
        }
    };

    class BugTrioEndFightTrigger : public EndBossFightTrigger
    {
    public:
        BugTrioEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end bug trio fight", "bug trio", 15511) {}
    };

    // Kri's death clouds (Poison Cloud 15933) pulse Toxic Vapors - walked out of.
    class KriPoisonCloudTrigger : public CloseToCreatureHazardTrigger
    {
    public:
        KriPoisonCloudTrigger(PlayerbotAI* ai) : CloseToCreatureHazardTrigger(ai, "kri poison cloud", 15933, 15.0f, 60) {}
    };

    // Battleguard Sartura (15516) and her Royal Guards (15984): melee steps out
    // while either is whirlwinding (26083 on Sartura, 26038 on a guard).
    class SarturaStartFightTrigger : public StartBossFightTrigger
    {
    public:
        SarturaStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start sartura fight", "sartura", 15516) {}
    };

    class SarturaEndFightTrigger : public EndBossFightTrigger
    {
    public:
        SarturaEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end sartura fight", "sartura", 15516) {}
    };

    class SarturaWhirlwindTrigger : public Trigger
    {
    public:
        SarturaWhirlwindTrigger(PlayerbotAI* ai) : Trigger(ai, "sartura whirlwind", 1) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15516);
            return boss && boss->IsInCombat() && ai->HasAura(26083, boss);
        }
    };

    class SarturaGuardWhirlwindTrigger : public Trigger
    {
    public:
        SarturaGuardWhirlwindTrigger(PlayerbotAI* ai) : Trigger(ai, "sartura guard whirlwind", 1) {}

        bool IsActive() override
        {
            for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* guard = ai->GetCreature(guid);
                if (guard && guard->IsAlive() && guard->GetEntry() == 15984 && ai->HasAura(26038, guard))
                    return true;
            }

            return false;
        }
    };

    // Fankriss the Unyielding (15510): Mortal Wound (25646) stacks force a tank
    // swap; Spawn of Fankriss (15630) enrage and are killed fast; Vekniss
    // Hatchlings (15962) come from the webs. The fight leashes above y 1400,
    // so no run-out actions.
    class FankrissStartFightTrigger : public StartBossFightTrigger
    {
    public:
        FankrissStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start fankriss fight", "fankriss", 15510) {}
    };

    class FankrissEndFightTrigger : public EndBossFightTrigger
    {
    public:
        FankrissEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end fankriss fight", "fankriss", 15510) {}
    };

    class FankrissTankSwitchTrigger : public Trigger
    {
    public:
        FankrissTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "fankriss tank switch", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15510);
            return boss && boss->IsInCombat() && ai::GetSpellStacks(boss->GetVictim(), 25646) >= 5;
        }
    };

    // Viscidus (15299): only frost damage advances the freeze count (200 hits),
    // and frost hits also help shatter him while frozen, so mages and shamans
    // switch to their frost spell. Globs (15667) are burned after the shatter.
    // Toxin clouds (Viscidus Trigger 15922) are walked out of.
    class ViscidusStartFightTrigger : public StartBossFightTrigger
    {
    public:
        ViscidusStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start viscidus fight", "viscidus", 15299) {}
    };

    class ViscidusEndFightTrigger : public EndBossFightTrigger
    {
    public:
        ViscidusEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end viscidus fight", "viscidus", 15299) {}
    };

    class ViscidusFrostAssistTrigger : public Trigger
    {
    public:
        ViscidusFrostAssistTrigger(PlayerbotAI* ai) : Trigger(ai, "viscidus frost assist", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15299);
            if (!boss || !boss->IsInCombat())
                return false;

            const uint32 botClass = bot->getClass();
            return botClass == CLASS_MAGE || botClass == CLASS_SHAMAN;
        }
    };

    class ViscidusToxinCloudTrigger : public CloseToCreatureHazardTrigger
    {
    public:
        ViscidusToxinCloudTrigger(PlayerbotAI* ai) : CloseToCreatureHazardTrigger(ai, "viscidus toxin cloud", 15922, 12.0f, 30) {}
    };

    // Princess Huhuran (15509): Frenzy (26051) is stripped by the generic
    // dispel-enrage reaction; Noxious Poison (26053) is a single-target DoT.
    class HuhuranStartFightTrigger : public StartBossFightTrigger
    {
    public:
        HuhuranStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start huhuran fight", "huhuran", 15509) {}
    };

    class HuhuranEndFightTrigger : public EndBossFightTrigger
    {
    public:
        HuhuranEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end huhuran fight", "huhuran", 15509) {}
    };

    // Wyvern Sting (26180) sleeps the tank - the offtank taunts her meanwhile.
    class HuhuranWyvernStingTrigger : public Trigger
    {
    public:
        HuhuranWyvernStingTrigger(PlayerbotAI* ai) : Trigger(ai, "huhuran wyvern stung tank", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15509);
            if (!boss || !boss->IsInCombat())
                return false;

            Unit* victim = boss->GetVictim();
            return victim && ai->HasAura(26180, victim);
        }
    };

    // Twin Emperors (Vek'nilash 15275 / Vek'lor 15276): they swap positions
    // every 30-40 s, heal each other within 60 yd, so melee stays on Vek'nilash
    // and ranged on Vek'lor (role split handled by the fight strategy).
    class TwinsStartFightTrigger : public Trigger
    {
    public:
        TwinsStartFightTrigger(PlayerbotAI* ai) : Trigger(ai, "start twins fight", 5) {}

        bool IsActive() override
        {
            if (ai->HasStrategy("twins", BotState::BOT_STATE_COMBAT))
                return false;

            for (const ObjectGuid& attackerGuid : AI_VALUE(std::list<ObjectGuid>, "attackers"))
            {
                Unit* attacker = ai->GetUnit(attackerGuid);
                if (!attacker)
                    continue;

                const uint32 entry = attacker->GetEntry();
                if (entry == 15275 || entry == 15276)
                    return true;
            }

            return false;
        }
    };

    class TwinsEndFightTrigger : public EndBossFightTrigger
    {
    public:
        TwinsEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end twins fight", "twins", 15275) {}
    };

    // TWIN EMPERORS TANK POSTS (per the server owner): the two sides of the
    // room are held by FIXED tank groups that never move. The first two melee
    // tanks (warrior/druid/paladin) and the first two warlocks by GUID form
    // those groups - side 0 = west (Vek'nilash's spawn), side 1 = east
    // (Vek'lor's). Everyone else (-1) plays normally and swaps sides with the
    // twins. Every position swap drops all threat, so each side's tanks simply
    // grab the twin that lands on their side.
    inline int TwinsTankSideIndex(PlayerbotAI* ai)
    {
        Player* bot = ai->GetBot();
        Group* group = bot->GetGroup();
        if (!group)
            return -1;

        const uint8 botClass = bot->getClass();
        const bool isWarlock = botClass == CLASS_WARLOCK;
        const bool isMeleeTank = PlayerbotAI::IsTank(bot) &&
            (botClass == CLASS_WARRIOR || botClass == CLASS_DRUID || botClass == CLASS_PALADIN);

        if (!isWarlock && !isMeleeTank)
            return -1;

        std::vector<ObjectGuid> candidates;
        Group::MemberSlotList const& slots = group->GetMemberSlots();
        for (Group::member_citerator itr = slots.begin(); itr != slots.end(); itr++)
        {
            Unit* unit = ai->GetUnit(itr->guid);
            if (!unit || unit->GetTypeId() != TYPEID_PLAYER)
                continue;

            Player* member = (Player*)unit;
            if (!member->IsAlive() || !member->IsInWorld() || member->IsBeingTeleported())
                continue;

            if (isWarlock)
            {
                if (member->getClass() != CLASS_WARLOCK)
                    continue;
            }
            else
            {
                const uint8 memberClass = member->getClass();
                if (!PlayerbotAI::IsTank(member) ||
                    (memberClass != CLASS_WARRIOR && memberClass != CLASS_DRUID && memberClass != CLASS_PALADIN))
                    continue;
            }

            candidates.push_back(itr->guid);
        }

        std::sort(candidates.begin(), candidates.end());

        for (size_t i = 0; i < candidates.size() && i < 2; i++)
        {
            if (candidates[i] == bot->GetObjectGuid())
                return (int)i;
        }

        return -1;
    }

    class TwinsTankPostTrigger : public Trigger
    {
    public:
        TwinsTankPostTrigger(PlayerbotAI* ai) : Trigger(ai, "twins tank post", 2) {}

        bool IsActive() override { return TwinsTankSideIndex(ai) >= 0; }
    };

    // Ouro (15517): summoned when the raid approaches the Ouro Spawner
    // (15957). While submerged, Dirt Mounds (15712) and Ouro Scarabs (15718)
    // are burned; he re-emerges on a timer.
    class OuroStartFightTrigger : public StartBossFightTrigger
    {
    public:
        OuroStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start ouro fight", "ouro", 15517) {}
    };

    class OuroEndFightTrigger : public EndBossFightTrigger
    {
    public:
        OuroEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end ouro fight", "ouro", 15517) {}
    };

    // C'Thun (eye 15589, body 15727): phase 1 kills Eye/Claw Tentacles
    // (15726/15725) while the eye is burned; phase 2 alternates between giant
    // tentacles (15728/15334) and the weakened burn window, which opens when
    // the two Flesh Tentacles (15802) inside the stomach are killed.
    class CthunStartFightTrigger : public Trigger
    {
    public:
        CthunStartFightTrigger(PlayerbotAI* ai) : Trigger(ai, "start cthun fight", 5) {}

        bool IsActive() override
        {
            if (ai->HasStrategy("cthun", BotState::BOT_STATE_COMBAT))
                return false;

            for (const ObjectGuid& attackerGuid : AI_VALUE(std::list<ObjectGuid>, "attackers"))
            {
                Unit* attacker = ai->GetUnit(attackerGuid);
                if (!attacker)
                    continue;

                const uint32 entry = attacker->GetEntry();
                if (entry == 15589 || entry == 15727)
                    return true;
            }

            return false;
        }
    };

    // A swallowed bot carries Digestive Acid (26476) while inside the stomach.
    // The fight strategy must stay active for it (the punt platform is its only
    // way out), so the end trigger ignores "not in combat" while the aura is up.
    class CthunStomachTrigger : public Trigger
    {
    public:
        CthunStomachTrigger(PlayerbotAI* ai) : Trigger(ai, "cthun in stomach", 1) {}

        bool IsActive() override { return bot->HasAura(26476); }
    };

    class CthunEndFightTrigger : public Trigger
    {
    public:
        CthunEndFightTrigger(PlayerbotAI* ai) : Trigger(ai, "end cthun fight", 5) {}

        bool IsActive() override
        {
            if (!ai->HasStrategy("cthun", BotState::BOT_STATE_COMBAT))
                return false;

            // Still inside the stomach - keep the strategy until punted out.
            if (bot->HasAura(26476))
                return false;

            return !ai->IsStateActive(BotState::BOT_STATE_COMBAT);
        }
    };
}
