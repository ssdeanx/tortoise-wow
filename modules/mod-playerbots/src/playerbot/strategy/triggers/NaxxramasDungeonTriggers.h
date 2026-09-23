#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"

namespace ai
{
    class NaxxramasEnterDungeonTrigger : public EnterDungeonTrigger
    {
    public:
        NaxxramasEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter naxxramas", "naxxramas", 533) {}
    };

    class NaxxramasLeaveDungeonTrigger : public LeaveDungeonTrigger
    {
    public:
        NaxxramasLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave naxxramas", "naxxramas", 533) {}
    };

    // Four Horsemen (16062-16065): any of the four starts the fight.
    class FourHorsemanStartFightTrigger : public Trigger
    {
    public:
        FourHorsemanStartFightTrigger(PlayerbotAI* ai) : Trigger(ai, "start four horseman fight", 5) {}

        bool IsActive() override
        {
            if (ai->HasStrategy("four horseman", BotState::BOT_STATE_COMBAT))
                return false;

            for (const ObjectGuid& attackerGuid : AI_VALUE(std::list<ObjectGuid>, "attackers"))
            {
                Unit* attacker = ai->GetUnit(attackerGuid);
                if (attacker)
                {
                    const uint32 entry = attacker->GetEntry();
                    if (entry == 16062 || entry == 16063 || entry == 16064 || entry == 16065)
                        return true;
                }
            }

            return false;
        }
    };

    class FourHorsemanEndFightTrigger : public EndBossFightTrigger
    {
    public:
        FourHorsemanEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end four horseman fight", "four horseman", 16062) {}
    };

    // Anub'Rekhan (15956): Locust Swarm 28785 = melee run out of the swarm.
    class AnubRekhanStartFightTrigger : public StartBossFightTrigger
    {
    public:
        AnubRekhanStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start anubrekhan fight", "anubrekhan", 15956) {}
    };

    class AnubRekhanEndFightTrigger : public EndBossFightTrigger
    {
    public:
        AnubRekhanEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end anubrekhan fight", "anubrekhan", 15956) {}
    };

    class AnubLocustSwarmTrigger : public Trigger
    {
    public:
        AnubLocustSwarmTrigger(PlayerbotAI* ai) : Trigger(ai, "anub locust swarm", 1) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15956);
            return boss && ai->HasAura(28785, boss);
        }
    };

    // Grand Widow Faerlina (15953): enrage 28798 -> kill a Worshipper (16506)
    // for Widow's Embrace, which strips the enrage.
    class FaerlinaStartFightTrigger : public StartBossFightTrigger
    {
    public:
        FaerlinaStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start faerlina fight", "faerlina", 15953) {}
    };

    class FaerlinaEndFightTrigger : public EndBossFightTrigger
    {
    public:
        FaerlinaEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end faerlina fight", "faerlina", 15953) {}
    };

    class FaerlinaEnragedTrigger : public Trigger
    {
    public:
        FaerlinaEnragedTrigger(PlayerbotAI* ai) : Trigger(ai, "faerlina enraged", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15953);
            return boss && ai->HasAura(28798, boss);
        }
    };

    // Maexxna (15952): Web Wrap web (16486) frees trapped players; Spiderlings
    // (17055) are killed by the melee.
    class MaexxnaStartFightTrigger : public StartBossFightTrigger
    {
    public:
        MaexxnaStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start maexxna fight", "maexxna", 15952) {}
    };

    class MaexxnaEndFightTrigger : public EndBossFightTrigger
    {
    public:
        MaexxnaEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end maexxna fight", "maexxna", 15952) {}
    };

    // Noth the Plaguebringer (15954): add waves - Champions/Warriors killed,
    // Guardians/Constructs held by tanks.
    class NothStartFightTrigger : public StartBossFightTrigger
    {
    public:
        NothStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start noth fight", "noth", 15954) {}
    };

    class NothEndFightTrigger : public EndBossFightTrigger
    {
    public:
        NothEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end noth fight", "noth", 15954) {}
    };

    // Heigan the Unclean (15936): the safety dance - dodge Plague Clouds
    // (533002) spawned by the fissures.
    class HeiganStartFightTrigger : public StartBossFightTrigger
    {
    public:
        HeiganStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start heigan fight", "heigan", 15936) {}
    };

    class HeiganEndFightTrigger : public EndBossFightTrigger
    {
    public:
        HeiganEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end heigan fight", "heigan", 15936) {}
    };

    class HeiganPlagueCloudTrigger : public CloseToCreatureHazardTrigger
    {
    public:
        HeiganPlagueCloudTrigger(PlayerbotAI* ai) : CloseToCreatureHazardTrigger(ai, "heigan plague cloud", 533002, 8.0f, 30) {}
    };

    // Loatheb (16011): Spores (16286) are killed to grant Fungal Bloom (29232)
    // crit windows.
    class LoathebStartFightTrigger : public StartBossFightTrigger
    {
    public:
        LoathebStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start loatheb fight", "loatheb", 16011) {}
    };

    class LoathebEndFightTrigger : public EndBossFightTrigger
    {
    public:
        LoathebEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end loatheb fight", "loatheb", 16011) {}
    };

    class LoathebSporeTrigger : public Trigger
    {
    public:
        LoathebSporeTrigger(PlayerbotAI* ai) : Trigger(ai, "loatheb spore", 1) {}

        bool IsActive() override
        {
            // Kill a spore only when the Fungal Bloom crit window is not already up.
            if (ai->HasAura(29232, bot))
                return false;

            for (const ObjectGuid& attackerGuid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
            {
                Creature* attacker = ai->GetCreature(attackerGuid);
                if (attacker && sServerFacade.IsAlive(attacker) && attacker->GetEntry() == 16286)
                    return true;
            }

            return false;
        }
    };

    // Grobbulus (15931): Mutating Injection 28169 - run away from the raid,
    // drop the cloud, return. Fallout Slimes (16290) are held by tanks.
    class GrobbulusStartFightTrigger : public StartBossFightTrigger
    {
    public:
        GrobbulusStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start grobbulus fight", "grobbulus", 15931) {}
    };

    class GrobbulusEndFightTrigger : public EndBossFightTrigger
    {
    public:
        GrobbulusEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end grobbulus fight", "grobbulus", 15931) {}
    };

    class GrobbulusInjectionTrigger : public Trigger
    {
    public:
        GrobbulusInjectionTrigger(PlayerbotAI* ai) : Trigger(ai, "grobbulus injection", 1) {}

        bool IsActive() override { return ai->HasAura(28169, bot); }
    };

    // Gluth (15932): Zombie Chows (16360) held/killed by offtanks; Mortal Wound
    // (25646) stacks force a tank swap.
    class GluthStartFightTrigger : public StartBossFightTrigger
    {
    public:
        GluthStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start gluth fight", "gluth", 15932) {}
    };

    class GluthEndFightTrigger : public EndBossFightTrigger
    {
    public:
        GluthEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end gluth fight", "gluth", 15932) {}
    };

    class GluthTankSwitchTrigger : public Trigger
    {
    public:
        GluthTankSwitchTrigger(PlayerbotAI* ai) : Trigger(ai, "gluth tank switch", 2) {}

        bool IsActive() override
        {
            Unit* boss = GetRaidTargetByEntry(ai, 15932);
            return boss && boss->IsInCombat() && ai::GetSpellStacks(boss->GetVictim(), 25646) >= 5;
        }
    };

    // Thaddius (15928): polarity charges must stack with the same charge.
    class ThaddiusStartFightTrigger : public StartBossFightTrigger
    {
    public:
        ThaddiusStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start thaddius fight", "thaddius", 15928) {}
    };

    class ThaddiusEndFightTrigger : public EndBossFightTrigger
    {
    public:
        ThaddiusEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end thaddius fight", "thaddius", 15928) {}
    };

    class ThaddiusPolarityTrigger : public Trigger
    {
    public:
        ThaddiusPolarityTrigger(PlayerbotAI* ai) : Trigger(ai, "thaddius polarity", 2) {}

        bool IsActive() override
        {
            return ai->HasAura(28059, bot) || ai->HasAura(28084, bot);
        }
    };

    // Sapphiron (15989): Blizzard (NPC_BLIZZARD 16474) is dodged; frost aura /
    // frost resist duty handled by the fight strategy.
    class SapphironStartFightTrigger : public StartBossFightTrigger
    {
    public:
        SapphironStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start sapphiron fight", "sapphiron", 15989) {}
    };

    class SapphironEndFightTrigger : public EndBossFightTrigger
    {
    public:
        SapphironEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end sapphiron fight", "sapphiron", 15989) {}
    };

    class SapphironBlizzardTrigger : public CloseToCreatureHazardTrigger
    {
    public:
        SapphironBlizzardTrigger(PlayerbotAI* ai) : CloseToCreatureHazardTrigger(ai, "sapphiron blizzard", 16474, 10.0f, 30) {}
    };

    // Kel'Thuzad (15990): Mana Detonation (27819) runs out of the raid before it
    // explodes.
    class KelThuzadStartFightTrigger : public StartBossFightTrigger
    {
    public:
        KelThuzadStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start kelthuzad fight", "kelthuzad", 15990) {}
    };

    class KelThuzadEndFightTrigger : public EndBossFightTrigger
    {
    public:
        KelThuzadEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end kelthuzad fight", "kelthuzad", 15990) {}
    };

    class KelThuzadManaDetonationTrigger : public Trigger
    {
    public:
        KelThuzadManaDetonationTrigger(PlayerbotAI* ai) : Trigger(ai, "kelthuzad mana detonation", 1) {}

        bool IsActive() override { return ai->HasAura(27819, bot); }
    };
}
