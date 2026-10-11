#include "playerbot/playerbot.h"
#include "KarazhanDungeonStrategies.h"
#include "DungeonMultipliers.h"

using namespace ai;

void KarazhanDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"start netherspite fight",
		NextAction::array(0, new NextAction("enable netherspite fight strategy", 100.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"start prince malchezaar fight",
		NextAction::array(0, new NextAction("enable prince malchezaar fight strategy", 100.0f), NULL)));

	// Lower Karazhan Halls Bosses
	triggers.push_back(new TriggerNode(
		"start araxxna fight",
		NextAction::array(0, new NextAction("enable araxxna fight strategy", 100.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"start howlfang fight",
		NextAction::array(0, new NextAction("enable howlfang fight strategy", 100.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"start lord blackwald fight",
		NextAction::array(0, new NextAction("enable lord blackwald fight strategy", 100.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"start grizikil fight",
		NextAction::array(0, new NextAction("enable grizikil fight strategy", 100.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"start lkh moroes fight",
		NextAction::array(0, new NextAction("enable lkh moroes fight strategy", 100.0f), NULL)));
}

void NetherspiteFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"void zone too close",
		NextAction::array(0, new NextAction("move away from void zone", 100.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"add nether portal - perseverence for tank",
		NextAction::array(0, new NextAction("add nether portal - perseverence for tank", 101.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"remove nether portal buffs from netherspite",
		NextAction::array(0, new NextAction("remove nether portal buffs from netherspite", 101.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"nether portal - perseverence",
		NextAction::array(0, new NextAction("remove nether portal - perseverence", 101.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"nether portal - serenity",
		NextAction::array(0, new NextAction("remove nether portal - serenity", 101.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"nether portal - dominance",
		NextAction::array(0, new NextAction("remove nether portal - dominance", 101.0f), NULL)));
}

void NetherspiteFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end netherspite fight",
		NextAction::array(0, new NextAction("disable netherspite fight strategy", 100.0f), NULL)));
}

void NetherspiteFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end netherspite fight",
		NextAction::array(0, new NextAction("disable netherspite fight strategy", 100.0f), NULL)));
}

void PrinceMalchezaarFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"netherspite infernal too close",
		NextAction::array(0, new NextAction("move away from netherspite infernal", 100.0f), NULL)));
}

void PrinceMalchezaarFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end prince malchezaar fight",
		NextAction::array(0, new NextAction("disable prince malchezaar fight strategy", 100.0f), NULL)));
}

void PrinceMalchezaarFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end prince malchezaar fight",
		NextAction::array(0, new NextAction("disable prince malchezaar fight strategy", 100.0f), NULL)));
}

void KarazhanDungeonStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"ahnqiraj supplies needed",
		NextAction::array(0, new NextAction("give ahnqiraj supplies", 90.0f), NULL)));
}

// --- Lower Karazhan Halls Boss Fight Strategies ---------------------------
void AraxxnaFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
	Player* bot = ai->GetBot();

	// Priority targets: Skitterweb Eggs (30008) before they hatch into spiderlings
	triggers.push_back(new TriggerNode(
		"attackers entry::30008",
		NextAction::array(0, new NextAction("attack entry::30008", 85.0f), NULL)));

	if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
	{
		triggers.push_back(new TriggerNode(
			"nature resistance aura",
			NextAction::array(0, new NextAction("nature resistance aura", 10.0f), NULL)));
	}

	triggers.push_back(new TriggerNode(
		"nature protection potion ready",
		NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void AraxxnaFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end araxxna fight",
		NextAction::array(0, new NextAction("disable araxxna fight strategy", 100.0f), NULL)));
}
void AraxxnaFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end araxxna fight",
		NextAction::array(0, new NextAction("disable araxxna fight strategy", 100.0f), NULL)));
}

void HowlfangFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"shadow protection potion ready",
		NextAction::array(0, new NextAction("shadow protection potion", 90.0f), NULL)));
}

void HowlfangFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end howlfang fight",
		NextAction::array(0, new NextAction("disable howlfang fight strategy", 100.0f), NULL)));
}
void HowlfangFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end howlfang fight",
		NextAction::array(0, new NextAction("disable howlfang fight strategy", 100.0f), NULL)));
}

void LordBlackwaldFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
	// Priority targets: Call for Help adds (61192, 61193, 61194, 61211)
	triggers.push_back(new TriggerNode(
		"attackers entry::61192",
		NextAction::array(0, new NextAction("attack entry::61192", 80.0f), NULL)));
	triggers.push_back(new TriggerNode(
		"attackers entry::61193",
		NextAction::array(0, new NextAction("attack entry::61193", 80.0f), NULL)));
	triggers.push_back(new TriggerNode(
		"attackers entry::61194",
		NextAction::array(0, new NextAction("attack entry::61194", 80.0f), NULL)));
	triggers.push_back(new TriggerNode(
		"attackers entry::61211",
		NextAction::array(0, new NextAction("attack entry::61211", 80.0f), NULL)));

	triggers.push_back(new TriggerNode(
		"shadow protection potion ready",
		NextAction::array(0, new NextAction("shadow protection potion", 90.0f), NULL)));
}

void LordBlackwaldFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end lord blackwald fight",
		NextAction::array(0, new NextAction("disable lord blackwald fight strategy", 100.0f), NULL)));
}
void LordBlackwaldFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end lord blackwald fight",
		NextAction::array(0, new NextAction("disable lord blackwald fight strategy", 100.0f), NULL)));
}

void GrizikilFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
	Player* bot = ai->GetBot();

	// Priority targets: Grellkin adds (61195, 61196, 61197)
	triggers.push_back(new TriggerNode(
		"attackers entry::61195",
		NextAction::array(0, new NextAction("attack entry::61195", 80.0f), NULL)));
	triggers.push_back(new TriggerNode(
		"attackers entry::61196",
		NextAction::array(0, new NextAction("attack entry::61196", 80.0f), NULL)));
	triggers.push_back(new TriggerNode(
		"attackers entry::61197",
		NextAction::array(0, new NextAction("attack entry::61197", 80.0f), NULL)));

	if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
	{
		triggers.push_back(new TriggerNode(
			"fire resistance aura",
			NextAction::array(0, new NextAction("fire resistance aura", 10.0f), NULL)));
	}

	triggers.push_back(new TriggerNode(
		"fire protection potion ready",
		NextAction::array(0, new NextAction("fire protection potion", 100.0f), NULL)));
}

void GrizikilFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end grizikil fight",
		NextAction::array(0, new NextAction("disable grizikil fight strategy", 100.0f), NULL)));
}
void GrizikilFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end grizikil fight",
		NextAction::array(0, new NextAction("disable grizikil fight strategy", 100.0f), NULL)));
}

void LkhMoroesFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
	Player* bot = ai->GetBot();

	if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
	{
		triggers.push_back(new TriggerNode(
			"shadow resistance aura",
			NextAction::array(0, new NextAction("shadow resistance aura", 10.0f), NULL)));
	}

	triggers.push_back(new TriggerNode(
		"shadow protection potion ready",
		NextAction::array(0, new NextAction("shadow protection potion", 90.0f), NULL)));
}

void LkhMoroesFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end lkh moroes fight",
		NextAction::array(0, new NextAction("disable lkh moroes fight strategy", 100.0f), NULL)));
}
void LkhMoroesFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
	triggers.push_back(new TriggerNode(
		"end lkh moroes fight",
		NextAction::array(0, new NextAction("disable lkh moroes fight strategy", 100.0f), NULL)));
}

