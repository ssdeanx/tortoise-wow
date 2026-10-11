#pragma once
#include "DungeonTriggers.h"
#include "GenericTriggers.h"

namespace ai
{
	class KarazhanEnterDungeonTrigger : public EnterDungeonTrigger
	{
	public:
		KarazhanEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter karazhan", "karazhan", 532) {}
	};

	class KarazhanLeaveDungeonTrigger : public LeaveDungeonTrigger
	{
	public:
		KarazhanLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave karazhan", "karazhan", 532) {}
	};

	class KarazhanCryptEnterDungeonTrigger : public EnterDungeonTrigger
	{
	public:
		KarazhanCryptEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter karazhan crypt", "karazhan", 800) {}
	};

	class KarazhanCryptLeaveDungeonTrigger : public LeaveDungeonTrigger
	{
	public:
		KarazhanCryptLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave karazhan crypt", "karazhan", 800) {}
	};

	class UpperKarazhanEnterDungeonTrigger : public EnterDungeonTrigger
	{
	public:
		UpperKarazhanEnterDungeonTrigger(PlayerbotAI* ai) : EnterDungeonTrigger(ai, "enter upper karazhan", "karazhan", 814) {}
	};

	class UpperKarazhanLeaveDungeonTrigger : public LeaveDungeonTrigger
	{
	public:
		UpperKarazhanLeaveDungeonTrigger(PlayerbotAI* ai) : LeaveDungeonTrigger(ai, "leave upper karazhan", "karazhan", 814) {}
	};

	// Lower Karazhan Halls Boss Triggers
	// 61221: Brood Queen Araxxna
	class AraxxnaStartFightTrigger : public StartBossFightTrigger
	{
	public:
		AraxxnaStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start araxxna fight", "araxxna", 61221) {}
	};

	class AraxxnaEndFightTrigger : public EndBossFightTrigger
	{
	public:
		AraxxnaEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end araxxna fight", "araxxna", 61221) {}
	};

	// 61223: Clawlord Howlfang
	class HowlfangStartFightTrigger : public StartBossFightTrigger
	{
	public:
		HowlfangStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start howlfang fight", "howlfang", 61223) {}
	};

	class HowlfangEndFightTrigger : public EndBossFightTrigger
	{
	public:
		HowlfangEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end howlfang fight", "howlfang", 61223) {}
	};

	// 61222: Lord Blackwald II
	class LordBlackwaldStartFightTrigger : public StartBossFightTrigger
	{
	public:
		LordBlackwaldStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start lord blackwald fight", "lord blackwald", 61222) {}
	};

	class LordBlackwaldEndFightTrigger : public EndBossFightTrigger
	{
	public:
		LordBlackwaldEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end lord blackwald fight", "lord blackwald", 61222) {}
	};

	// 61224: Grizikil
	class GrizikilStartFightTrigger : public StartBossFightTrigger
	{
	public:
		GrizikilStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start grizikil fight", "grizikil", 61224) {}
	};

	class GrizikilEndFightTrigger : public EndBossFightTrigger
	{
	public:
		GrizikilEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end grizikil fight", "grizikil", 61224) {}
	};

	// 61225: Moroes (LKH)
	class LkhMoroesStartFightTrigger : public StartBossFightTrigger
	{
	public:
		LkhMoroesStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start lkh moroes fight", "lkh moroes", 61225) {}
	};

	class LkhMoroesEndFightTrigger : public EndBossFightTrigger
	{
	public:
		LkhMoroesEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end lkh moroes fight", "lkh moroes", 61225) {}
	};

	class NetherspiteStartFightTrigger : public StartBossFightTrigger
	{
	public:
		NetherspiteStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start netherspite fight", "netherspite", 15689) {}
	};

	class NetherspiteEndFightTrigger : public EndBossFightTrigger
	{
	public:
		NetherspiteEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end netherspite fight", "netherspite", 15689) {}
	};

	class VoidZoneTooCloseTrigger : public CloseToCreatureHazardTrigger
	{
	public:
		VoidZoneTooCloseTrigger(PlayerbotAI* ai) : CloseToCreatureHazardTrigger(ai, "void zone too close", 16697, 5.0f, 99999999.0f) {}
	};

	class NetherspiteBeamsCheatNeedRefreshTrigger : public Trigger
	{
	public:
		NetherspiteBeamsCheatNeedRefreshTrigger(PlayerbotAI* ai, int checkInteval = 2) : Trigger(ai, "netherspite beams cheat need refresh", checkInteval) {}
		bool IsActive() override;
	};

	class RemoveNetherPortalDominanceTrigger : public HasAuraTrigger
	{
	public:
		RemoveNetherPortalDominanceTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "remove nether portal - dominance", 20) {}
	};

	class RemoveNetherPortalPerseverenceTrigger : public Trigger
	{
	public:
		RemoveNetherPortalPerseverenceTrigger(PlayerbotAI* ai) : Trigger(ai, "remove nether portal - perseverence", 1) {}
		bool IsActive() override
		{
			//Bot is not tank with aggro
			if (ai->IsTank(bot) && AI_VALUE2(bool, "has aggro", "current target"))
				return false;

			return ai->HasAura(30421, bot);
		}
	};

	class RemoveNetherPortalSerenityTrigger : public HasAuraTrigger
	{
	public:
		RemoveNetherPortalSerenityTrigger(PlayerbotAI* ai) : HasAuraTrigger(ai, "remove nether portal - serenity", 5) {}
	};

	class PrinceMalchezaarStartFightTrigger : public StartBossFightTrigger
	{
	public:
		PrinceMalchezaarStartFightTrigger(PlayerbotAI* ai) : StartBossFightTrigger(ai, "start prince malchezaar fight", "prince malchezaar", 15690) {}
	};

	class PrinceMalchezaarEndFightTrigger : public EndBossFightTrigger
	{
	public:
		PrinceMalchezaarEndFightTrigger(PlayerbotAI* ai) : EndBossFightTrigger(ai, "end prince malchezaar fight", "prince malchezaar", 15690) {}
	};

	class NetherspiteInfernalTooCloseTrigger : public CloseToCreatureTrigger
	{
	public:
		NetherspiteInfernalTooCloseTrigger(PlayerbotAI* ai) : CloseToCreatureTrigger(ai, "netherspite infernal too close", 17646, 20.0f) {}
	};
}