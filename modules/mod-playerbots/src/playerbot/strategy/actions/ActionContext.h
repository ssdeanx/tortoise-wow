#pragma once

#include "GenericActions.h"
#include "EmoteAction.h"
#include "AddLootAction.h"
#include "LootAction.h"
#include "AddLootAction.h"
#include "LootRollAction.h"
#include "StayActions.h"
#include "FollowActions.h"
#include "ChangeStrategyAction.h"
#include "ChooseTargetActions.h"
#include "SuggestWhatToDoAction.h"
#include "PositionAction.h"
#include "AttackAction.h"
#include "CheckMailAction.h"
#include "CheckValuesAction.h"
#include "ChooseRpgTargetAction.h"
#include "ChooseTravelTargetAction.h"
#include "DelayAction.h"
#include "GiveItemAction.h"
#include "GreetAction.h"
#include "ImbueAction.h"
#include "MovementActions.h"
#include "MoveToRpgTargetAction.h"
#include "MoveToTravelTargetAction.h"
#include "OutfitAction.h"
#include "RevealGatheringItemAction.h"
#include "SayAction.h"
#include "OutfitAction.h"
#include "RandomBotUpdateAction.h"
#include "RemoveAuraAction.h"
#include "RpgAction.h"
#include "TravelAction.h"
#include "RtiAction.h"
#include "BattleGroundTactics.h"
#include "CheckMountStateAction.h"
#include "ChangeTalentsAction.h"
#include "AutoLearnSpellAction.h"
#include "XpGainAction.h"
#include "HonorGainAction.h"
#include "InviteToGroupAction.h"
#include "LeaveGroupAction.h"
#include "ReleaseSpiritAction.h"
#include "CombatActions.h"
#include "WorldBuffAction.h"
#include "CastCustomSpellAction.h"
#include "BattleGroundJoinAction.h"
#include "DestroyItemAction.h"
#include "ResetInstancesAction.h"
#include "BuyAction.h"
#include "GuildCreateActions.h"
#include "GuildManagementActions.h"
#include "GuildAcceptAction.h"
#include "GuildAcceptQuestOrderAction.h"
#include "GuildShareItemAction.h"
#include "GuildShareAhBuyAction.h"
#include "RpgSubActions.h"
#include "VehicleActions.h"
#include "UseTrinketAction.h"
#include "BotStateActions.h"
#include "WaitForAttackAction.h"
#include "PullActions.h"
#include "ResetAiAction.h"
#include "ShareQuestAction.h"
#include "UpdateGearAction.h"
#include "SetAvoidAreaAction.h"
#include "GlyphAction.h"
#include "FishAction.h"
#include "AutoCompleteQuestAction.h"
#include "UnstuckAction.h"
#include "RangeAction.h"
#include "UseConsumableAction.h"
#include "WorldBuffTravelActions.h"

#include "OnyxiasLairDungeonActions.h"
#include "MoltenCoreDungeonActions.h"
#include "BlackwingLairDungeonActions.h"
#include "KarazhanDungeonActions.h"
#include "NaxxramasDungeonActions.h"
#include "RuinsOfAhnQirajDungeonActions.h"
#include "TempleOfAhnQirajDungeonActions.h"
#include "ZulGurubDungeonActions.h"
#include "ScarletCitadelDungeonActions.h"
#include "BlackrockSpireDungeonActions.h"
#include "EmeraldSanctumDungeonActions.h"

#ifdef GenerateBotTests
#include "../tests/TestAction.h"
#endif

namespace ai
{
    class ActionContext : public NamedObjectContext<Action>
    {
    public:
        ActionContext()
        {
            creators["mark rti"] = [](PlayerbotAI* ai) { return new MarkRtiAction(ai); };
            creators["set return position"] = [](PlayerbotAI* ai) { return new SetReturnPositionAction(ai); };
            creators["rpg"] = [](PlayerbotAI* ai) { return new RpgAction(ai); };
            creators["crpg"] = [](PlayerbotAI* ai) { return new CRpgAction(ai); };
            creators["choose rpg target"] = [](PlayerbotAI* ai) { return new ChooseRpgTargetAction(ai); };
            creators["move to rpg target"] = [](PlayerbotAI* ai) { return new MoveToRpgTargetAction(ai); };
			creators["travel"] = [](PlayerbotAI* ai) { return new TravelAction(ai); };
			creators["choose travel target"] = [](PlayerbotAI* ai) { return new ChooseTravelTargetAction(ai); };
            creators["choose group travel target"] = [](PlayerbotAI* ai) { return new ChooseGroupTravelTargetAction(ai); };
            creators["refresh travel target"] = [](PlayerbotAI* ai) { return new RefreshTravelTargetAction(ai); };
            creators["request travel target"] = [](PlayerbotAI* ai) { return new RequestTravelTargetAction(ai); };
            creators["request named travel target"] = [](PlayerbotAI* ai) { return new RequestNamedTravelTargetAction(ai); };
            creators["request quest travel target"] = [](PlayerbotAI* ai) { return new RequestQuestTravelTargetAction(ai); };
            creators["reset travel target"] = [](PlayerbotAI* ai) { return new ResetTargetAction(ai); };
            creators["move to travel target"] = [](PlayerbotAI* ai) { return new MoveToTravelTargetAction(ai); };
            creators["move out of collision"] = [](PlayerbotAI* ai) { return new MoveOutOfCollisionAction(ai); };
            creators["move random"] = [](PlayerbotAI* ai) { return new MoveRandomAction(ai); };
            creators["attack"] = [](PlayerbotAI* ai) { return new MeleeAction(ai); };
            creators["melee"] = [](PlayerbotAI* ai) { return new MeleeAction(ai); };
            creators["switch to melee"] = [](PlayerbotAI* ai) { return new SwitchToMeleeAction(ai); };
            creators["switch to ranged"] = [](PlayerbotAI* ai) { return new SwitchToRangedAction(ai); };
            creators["reach spell"] = [](PlayerbotAI* ai) { return new ReachSpellAction(ai); };
            creators["reach melee"] = [](PlayerbotAI* ai) { return new ReachMeleeAction(ai); };
            creators["reach pull"] = [](PlayerbotAI* ai) { return new ReachPullAction(ai); };
            creators["reach party member to heal"] = [](PlayerbotAI* ai) { return new ReachPartyMemberToHealAction(ai); };
            creators["reach party member for totem"] = [](PlayerbotAI* ai) { return new ReachPartyMemberForTotemAction(ai); };
            creators["flee"] = [](PlayerbotAI* ai) { return new FleeAction(ai); };
            creators["flee with pet"] = [](PlayerbotAI* ai) { return new FleeWithPetAction(ai); };
            creators["wait for attack keep safe distance"] = [](PlayerbotAI* ai) { return new WaitForAttackKeepSafeDistanceAction(ai); };
            creators["shoot"] = [](PlayerbotAI* ai) { return new CastShootAction(ai); };
            creators["whipper root tuber"] = [](PlayerbotAI* ai) { return new UseWhipperRootTuberAction(ai); };
            creators["healthstone"] = [](PlayerbotAI* ai) { return new UseHealthstoneAction(ai); };
            creators["healing potion"] = [](PlayerbotAI* ai) { return new UseHealingPotionAction(ai); };
            creators["mana potion"] = [](PlayerbotAI* ai) { return new UseManaPotionAction(ai); };
            creators["food"] = [](PlayerbotAI* ai) { return new EatAction(ai); };
            creators["drink"] = [](PlayerbotAI* ai) { return new DrinkAction(ai); };
            creators["tank assist"] = [](PlayerbotAI* ai) { return new TankAssistAction(ai); };
            creators["dps assist"] = [](PlayerbotAI* ai) { return new DpsAssistAction(ai); };
            creators["dps aoe"] = [](PlayerbotAI* ai) { return new DpsAoeAction(ai); };
            creators["attack rti target"] = [](PlayerbotAI* ai) { return new AttackRTITargetAction(ai); };
            creators["loot"] = [](PlayerbotAI* ai) { return new LootAction(ai); };
            creators["add loot"] = [](PlayerbotAI* ai) { return new AddLootAction(ai); };
            creators["add gathering loot"] = [](PlayerbotAI* ai) { return new AddGatheringLootAction(ai); };
            creators["add all loot"] = [](PlayerbotAI* ai) { return new AddAllLootAction(ai); };
            creators["release loot"] = [](PlayerbotAI* ai) { return new ReleaseLootAction(ai); };
            creators["auto loot roll"] = [](PlayerbotAI* ai) { return new AutoLootRollAction(ai); };
            creators["follow"] = [](PlayerbotAI* ai) { return new FollowAction(ai); };
            creators["stop follow"] = [](PlayerbotAI* ai) { return new StopFollowAction(ai); };
            creators["flee to master"] = [](PlayerbotAI* ai) { return new FleeToMasterAction(ai); };
            creators["runaway"] = [](PlayerbotAI* ai) { return new RunAwayAction(ai); };
            creators["stay"] = [](PlayerbotAI* ai) { return new StayAction(ai); };
            creators["sit"] = [](PlayerbotAI* ai) { return new SitAction(ai); };
            creators["attack anything"] = [](PlayerbotAI* ai) { return new AttackAnythingAction(ai); };
            creators["attack least hp target"] = [](PlayerbotAI* ai) { return new AttackLeastHpTargetAction(ai); };
            creators["attack enemy player"] = [](PlayerbotAI* ai) { return new AttackEnemyPlayerAction(ai); };
            creators["pull my target"] = [](PlayerbotAI* ai) { return new PullMyTargetAction(ai); };
            creators["pull rti target"] = [](PlayerbotAI* ai) { return new PullRTITargetAction(ai); };
            creators["pull nearest target"] = [](PlayerbotAI* ai) { return new PullNearestTargetAction(ai); };
            creators["pull start"] = [](PlayerbotAI* ai) { return new PullStartAction(ai); };
            creators["pull action"] = [](PlayerbotAI* ai) { return new PullAction(ai); };
            creators["return to pull position"] = [](PlayerbotAI* ai) { return new ReturnToPullPositionAction(ai); };
            creators["pull end"] = [](PlayerbotAI* ai) { return new PullEndAction(ai); };
            creators["emote"] = [](PlayerbotAI* ai) { return new EmoteAction(ai); };
            creators["talk"] = [](PlayerbotAI* ai) { return new TalkAction(ai); };
            creators["mount anim"] = [](PlayerbotAI* ai) { return new MountAnimAction(ai); };
            creators["suggest what to do"] = [](PlayerbotAI* ai) { return new SuggestWhatToDoAction(ai); };
            creators["suggest trade"] = [](PlayerbotAI* ai) { return new SuggestTradeAction(ai); };
            creators["return"] = [](PlayerbotAI* ai) { return new ReturnAction(ai); };
            creators["move to loot"] = [](PlayerbotAI* ai) { return new MoveToLootAction(ai); };
            creators["open loot"] = [](PlayerbotAI* ai) { return new OpenLootAction(ai); };
            creators["guard"] = [](PlayerbotAI* ai) { return new GuardAction(ai); };
            creators["return to stay position"] = [](PlayerbotAI* ai) { return new ReturnToStayPositionAction(ai); };
            creators["move out of enemy contact"] = [](PlayerbotAI* ai) { return new MoveOutOfEnemyContactAction(ai); };
            creators["set facing"] = [](PlayerbotAI* ai) { return new SetFacingTargetAction(ai); };
            creators["set behind"] = [](PlayerbotAI* ai) { return new SetBehindTargetAction(ai); };
            creators["attack duel opponent"] = [](PlayerbotAI* ai) { return new AttackDuelOpponentAction(ai); };
            creators["select new target"] = [](PlayerbotAI* ai) { return new SelectNewTargetAction(ai); };
            creators["check mail"] = [](PlayerbotAI* ai) { return new CheckMailAction(ai); };
            creators["say"] = [](PlayerbotAI* ai) { return new SayAction(ai); };
            creators["reveal gathering item"] = [](PlayerbotAI* ai) { return new RevealGatheringItemAction(ai); };
            creators["outfit"] = [](PlayerbotAI* ai) { return new OutfitAction(ai); };
            creators["random bot update"] = [](PlayerbotAI* ai) { return new RandomBotUpdateAction(ai); };
            creators["delay"] = [](PlayerbotAI* ai) { return new DelayAction(ai); };
            creators["greet"] = [](PlayerbotAI* ai) { return new GreetAction(ai); };
            creators["check values"] = [](PlayerbotAI* ai) { return new CheckValuesAction(ai); };
            creators["set avoid area"] = [](PlayerbotAI* ai) { return new SetAvoidAreaAction(ai); };
            creators["ra"] = [](PlayerbotAI* ai) { return new RemoveAuraAction(ai); };
            creators["remove blessing of salvation"] = [](PlayerbotAI* ai) { return new RemoveBlessingOfSalvationAction(ai); };
            creators["remove greater blessing of salvation"] = [](PlayerbotAI* ai) { return new RemoveGreaterBlessingOfSalvationAction(ai); };
            creators["apply stone"] = [](PlayerbotAI* ai) { return new ImbueWithStoneAction(ai); };
            creators["apply oil"] = [](PlayerbotAI* ai) { return new ImbueWithOilAction(ai); };
            creators["try emergency"] = [](PlayerbotAI* ai) { return new TryEmergencyAction(ai); };
            creators["give food"] = [](PlayerbotAI* ai) { return new GiveFoodAction(ai); };
            creators["give water"] = [](PlayerbotAI* ai) { return new GiveWaterAction(ai); };
            creators["mount"] = [](PlayerbotAI* ai) { return new CastSpellAction(ai, "mount"); };
            creators["auto talents"] = [](PlayerbotAI* ai) { return new AutoSetTalentsAction(ai); };
			creators["auto learn spell"] = [](PlayerbotAI* ai) { return new AutoLearnSpellAction(ai); };
            creators["auto share quest"] = [](PlayerbotAI* ai) { return new AutoShareQuestAction(ai); };
            creators["xp gain"] = [](PlayerbotAI* ai) { return new XpGainAction(ai); };
            creators["honor gain"] = [](PlayerbotAI* ai) { return new HonorGainAction(ai); };
            creators["invite nearby"] = [](PlayerbotAI* ai) { return new InviteNearbyToGroupAction(ai); };
            creators["invite guild"] = [](PlayerbotAI* ai) { return new InviteGuildToGroupAction(ai); };
            creators["leave far away"] = [](PlayerbotAI* ai) { return new LeaveFarAwayAction(ai); };
            creators["move to dark portal"] = [](PlayerbotAI* ai) { return new MoveToDarkPortalAction(ai); };
            creators["move from dark portal"] = [](PlayerbotAI* ai) { return new MoveFromDarkPortalAction(ai); };
            creators["use dark portal azeroth"] = [](PlayerbotAI* ai) { return new DarkPortalAzerothAction(ai); };
            creators["world buff"] = [](PlayerbotAI* ai) { return new WorldBuffAction(ai); };
            creators["world buff travel apply"] = [](PlayerbotAI* ai) { return new WorldBuffTravelApplyAction(ai); };
            creators["world buff travel cast portal"] = [](PlayerbotAI* ai) { return new WorldBuffTravelCastPortalAction(ai); };
            creators["world buff travel take portal"] = [](PlayerbotAI* ai) { return new WorldBuffTravelTakePortalAction(ai); };
            creators["world buff travel finish"] = [](PlayerbotAI* ai) { return new WorldBuffTravelFinishAction(ai); };
            creators["world buff travel set target"] = [](PlayerbotAI* ai) { return new WorldBuffTravelSetTargetAction(ai); };
            creators["world buff travel dm buffed"] = [](PlayerbotAI* ai) { return new WorldBuffTravelDMBuffedAction(ai); };
            creators["world buff travel dm exited"] = [](PlayerbotAI* ai) { return new WorldBuffTravelDMExitedAction(ai); };
            creators["world buff travel dm cast portal"] = [](PlayerbotAI* ai) { return new WorldBuffTravelDMCastPortalAction(ai); };
            creators["world buff travel dm take portal"] = [](PlayerbotAI* ai) { return new WorldBuffTravelDMTakePortalAction(ai); };
            creators["hearthstone"] = [](PlayerbotAI* ai) { return new UseHearthStoneAction(ai); };
            creators["cast random spell"] = [](PlayerbotAI* ai) { return new CastRandomSpellAction(ai); };
            creators["free bg join"] = [](PlayerbotAI* ai) { return new FreeBGJoinAction(ai); };
            creators["use random recipe"] = [](PlayerbotAI* ai) { return new UseRandomRecipeAction(ai); };
            creators["open random item"] = [](PlayerbotAI* ai) { return new OpenRandomItemAction(ai); };
            creators["use random quest item"] = [](PlayerbotAI* ai) { return new UseRandomQuestItemAction(ai); };
            creators["craft random item"] = [](PlayerbotAI* ai) { return new CraftRandomItemAction(ai); };
            creators["smart destroy item"] = [](PlayerbotAI* ai) { return new SmartDestroyItemAction(ai); };
            creators["disenchant random item"] = [](PlayerbotAI* ai) { return new DisenchantRandomItemAction(ai); };
            creators["enchant random item"] = [](PlayerbotAI* ai) { return new EnchantRandomItemAction(ai); };
            creators["reset instances"] = [](PlayerbotAI* ai) { return new ResetInstancesAction(ai); };
            creators["reset raids"] = [](PlayerbotAI* ai) { return new ResetRaidsAction(ai); };
            creators["update gear"] = [](PlayerbotAI* ai) { return new UpdateGearAction(ai); };
            creators["buy petition"] = [](PlayerbotAI* ai) { return new BuyPetitionAction(ai); };
            creators["offer petition"] = [](PlayerbotAI* ai) { return new PetitionOfferAction(ai); };
            creators["offer petition nearby"] = [](PlayerbotAI* ai) { return new PetitionOfferNearbyAction(ai); };
            creators["turn in petition"] = [](PlayerbotAI* ai) { return new PetitionTurnInAction(ai); };
            creators["buy tabard"] = [](PlayerbotAI* ai) { return new BuyTabardAction(ai); };
            creators["guild manage nearby"] = [](PlayerbotAI* ai) { return new GuildManageNearbyAction(ai); };
            creators["guild share item"] = [](PlayerbotAI* ai) { return new GuildShareItemAction(ai); };
            creators["guild ah buy"] = [](PlayerbotAI* ai) { return new GuildShareAhBuyAction(ai); };
            creators["guild accept quest order"] = [](PlayerbotAI* ai) { return new GuildAcceptQuestOrderAction(ai); };
            creators["use trinket"] = [](PlayerbotAI* ai) { return new UseTrinketAction(ai); };
            creators["unstuck"] = [](PlayerbotAI* ai) { return new UnstuckAction(ai); };
            creators["reset"] = [](PlayerbotAI* ai) { return new ResetAction(ai); };
            creators["interrupt current spell"] = [](PlayerbotAI* ai) { return new InterruptCurrentSpellAction(ai); };
            creators["initialize pet"] = [](PlayerbotAI* ai) { return new InitializePetAction(ai); };

            // item helpers
            creators["goblin sapper"] = [](PlayerbotAI* ai) { return new UseGoblinSapperChargeAction(ai); };
            creators["oil of immolation"] = [](PlayerbotAI* ai) { return new UseOilOfImmolationAction(ai); };
            creators["stoneshield potion"] = [](PlayerbotAI* ai) { return new UseStoneshieldPotionAction(ai); };
            creators["dark rune"] = [](PlayerbotAI* ai) { return new UseDarkRuneAction(ai); };
            creators["throw grenade"] = [](PlayerbotAI* ai) { return new ThrowGrenadeAction(ai); };
            creators["bg banner"] = [](PlayerbotAI* ai) { return new UseBgBannerAction(ai); };
            creators["use bandage"] = [](PlayerbotAI* ai) { return new UseBandageAction(ai); };
            creators["rocket boots"] = [](PlayerbotAI* ai) { return new UseRocketBootsAction(ai); };
            creators["fire protection potion"] = [](PlayerbotAI* ai) { return new UseFireProtectionPotionAction(ai); };
            creators["free action potion"] = [](PlayerbotAI* ai) { return new UseFreeActionPotionAction(ai); };
            creators["use consumable"] = [](PlayerbotAI* ai) { return new UseConsumableAction(ai); };
            creators["anti-venom"] = [](PlayerbotAI* ai) { return new UseAntiVenomAction(ai); };

            // BG Tactics
            creators["bg tactics"] = [](PlayerbotAI* ai) { return new BGTactics(ai); };
            creators["bg move to start"] = [](PlayerbotAI* ai) { return new BGTactics(ai, "move to start"); };
            creators["bg move to objective"] = [](PlayerbotAI* ai) { return new BGTactics(ai, "move to objective"); };
            creators["bg select objective"] = [](PlayerbotAI* ai) { return new BGTactics(ai, "select objective"); };
            creators["bg check objective"] = [](PlayerbotAI* ai) { return new BGTactics(ai, "check objective"); };
            creators["bg attack fc"] = [](PlayerbotAI* ai) { return new BGTactics(ai, "attack fc"); };
            creators["bg protect fc"] = [](PlayerbotAI* ai) { return new BGTactics(ai, "protect fc"); };
            creators["bg use buff"] = [](PlayerbotAI* ai) { return new BGTactics(ai, "use buff"); };
            creators["attack enemy flag carrier"] = [](PlayerbotAI* ai) { return new AttackEnemyFlagCarrierAction(ai); };
            creators["bg check flag"] = [](PlayerbotAI* ai) { return new BGTactics(ai, "check flag"); };

            // lightwell
            creators["use lightwell"] = [](PlayerbotAI* ai) { return new UseLightwellAction(ai); };

            // Vehicles
            creators["enter vehicle"] = [](PlayerbotAI* ai) { return new EnterVehicleAction(ai); };
            creators["leave vehicle"] = [](PlayerbotAI* ai) { return new LeaveVehicleAction(ai); };
            creators["hurl boulder"] = [](PlayerbotAI* ai) { return new CastHurlBoulderAction(ai); };
            creators["ram"] = [](PlayerbotAI* ai) { return new CastRamAction(ai); };
            creators["steam rush"] = [](PlayerbotAI* ai) { return new CastSteamRushAction(ai); };
            creators["steam blast"] = [](PlayerbotAI* ai) { return new CastSteamBlastAction(ai); };
            creators["napalm"] = [](PlayerbotAI* ai) { return new CastNapalmAction(ai); };
            creators["fire cannon"] = [](PlayerbotAI* ai) { return new CastFireCannonAction(ai); };
            creators["incendiary rocket"] = [](PlayerbotAI* ai) { return new CastIncendiaryRocketAction(ai); };
            creators["rocket blast"] = [](PlayerbotAI* ai) { return new CastRocketBlastAction(ai); };
            creators["blade salvo"] = [](PlayerbotAI* ai) { return new CastBladeSalvoAction(ai); };
            creators["glaive throw"] = [](PlayerbotAI* ai) { return new CastGlaiveThrowAction(ai); };

            // Quest vehicles
            creators["deliver stolen horse"] = [](PlayerbotAI* ai) { return new CastDeliverStolenHorseAction(ai); };
            creators["horsemans call"] = [](PlayerbotAI* ai) { return new CastHorsemansCallAction(ai); };

            creators["scarlet cannon"] = [](PlayerbotAI* ai) { return new CastScarletCannonAction(ai); };
            creators["electro - magnetic pulse"] = [](PlayerbotAI* ai) { return new CastElectroMagneticPulseAction(ai); };            
            creators["skeletal gryphon escape"] = [](PlayerbotAI* ai) { return new CastSkeletalGryphonEscapeAction(ai); };
            
            creators["frozen deathbolt"] = [](PlayerbotAI* ai) { return new CastFrozenDeathboltAction(ai); };
            creators["devour humanoid"] = [](PlayerbotAI* ai) { return new CastDevourHumanoidAction(ai); };            
            

            //Rpg
            creators["rpg stay"] = [](PlayerbotAI* ai) { return new RpgStayAction(ai); };
            creators["rpg work"] = [](PlayerbotAI* ai) { return new RpgWorkAction(ai); };
            creators["rpg emote"] = [](PlayerbotAI* ai) { return new RpgEmoteAction(ai); };
            creators["rpg cancel"] = [](PlayerbotAI* ai) { return new RpgCancelAction(ai); };
            creators["rpg taxi"] = [](PlayerbotAI* ai) { return new RpgTaxiAction(ai); };
            creators["rpg discover"] = [](PlayerbotAI* ai) { return new RpgDiscoverAction(ai); };
            creators["rpg start quest"] = [](PlayerbotAI* ai) { return new RpgStartQuestAction(ai); };
            creators["rpg end quest"] = [](PlayerbotAI* ai) { return new RpgEndQuestAction(ai); };
            creators["rpg buy"] = [](PlayerbotAI* ai) { return new RpgBuyAction(ai); };
            creators["rpg sell"] = [](PlayerbotAI* ai) { return new RpgSellAction(ai); };
            creators["rpg ah sell"] = [](PlayerbotAI* ai) { return new RpgAHSellAction(ai); };
            creators["rpg ah buy"] = [](PlayerbotAI* ai) { return new RpgAHBuyAction(ai); };
            creators["rpg get mail"] = [](PlayerbotAI* ai) { return new RpgGetMailAction(ai); };
            creators["rpg repair"] = [](PlayerbotAI* ai) { return new RpgRepairAction(ai); };
            creators["rpg train"] = [](PlayerbotAI* ai) { return new RpgTrainAction(ai); };
            creators["rpg heal"] = [](PlayerbotAI* ai) { return new RpgHealAction(ai); };
            creators["rpg home bind"] = [](PlayerbotAI* ai) { return new RpgHomeBindAction(ai); };
            creators["rpg queue bg"] = [](PlayerbotAI* ai) { return new RpgQueueBgAction(ai); };
            creators["rpg buy petition"] = [](PlayerbotAI* ai) { return new RpgBuyPetitionAction(ai); };
            creators["rpg use"] = [](PlayerbotAI* ai) { return new RpgUseAction(ai); };
            creators["rpg ai chat"] = [](PlayerbotAI* ai) { return new RpgAIChatAction(ai); };
            creators["rpg spell"] = [](PlayerbotAI* ai) { return new RpgSpellAction(ai); };
            creators["rpg spell click"] = [](PlayerbotAI* ai) { return new RpgSpellClickAction(ai); };
            creators["rpg craft"] = [](PlayerbotAI* ai) { return new RpgCraftAction(ai); };
            creators["rpg trade useful"] = [](PlayerbotAI* ai) { return new RpgTradeUsefulAction(ai); };
            creators["rpg enchant"] = [](PlayerbotAI* ai) { return new RpgEnchantAction(ai); };
            creators["rpg duel"] = [](PlayerbotAI* ai) { return new RpgDuelAction(ai); };
            creators["rpg item"] = [](PlayerbotAI* ai) { return new RpgItemAction(ai); };
            creators["rpg gossip talk"] = [](PlayerbotAI* ai) { return new RpgGossipTalkAction(ai); };

            creators["auto set glyph"] = [](PlayerbotAI* ai) { return new AutoSetGlyphAction(ai); };
            creators["auto complete quest"] = [](PlayerbotAI* ai) { return new AutoCompleteQuestAction(ai); };

            creators["move to fish"] = [](PlayerbotAI* ai) { return new MoveToFishAction(ai); };
            creators["fish"] = [](PlayerbotAI* ai) { return new FishAction(ai); };
            creators["use fishing bobber"] = [](PlayerbotAI* ai) { return new UseFishingBobberAction(ai); };

            // Bot States
            creators["set combat state"] = [](PlayerbotAI* ai) { return new SetCombatStateAction(ai); };
            creators["set non combat state"] = [](PlayerbotAI* ai) { return new SetNonCombatStateAction(ai); };
            creators["set dead state"] = [](PlayerbotAI* ai) { return new SetDeadStateAction(ai); };

            //racials
            creators["war stomp"] = [](PlayerbotAI* ai) { return new CastWarStompAction(ai); };
            creators["berserking"] = [](PlayerbotAI* ai) { return new CastBerserkingAction(ai); };
            creators["blood fury"] = [](PlayerbotAI* ai) { return new CastBloodFuryAction(ai); };
            creators["cannibalize"] = [](PlayerbotAI* ai) { return new CastCannibalizeAction(ai); };
            creators["escape artist"] = [](PlayerbotAI* ai) { return new CastEscapeArtistAction(ai); };
            creators["shadowmeld"] = [](PlayerbotAI* ai) { return new CastShadowmeldAction(ai); };
            creators["stoneform"] = [](PlayerbotAI* ai) { return new CastStoneformAction(ai); };
            creators["perception"] = [](PlayerbotAI* ai) { return new CastPerceptionAction(ai); };
            creators["will of the forsaken"] = [](PlayerbotAI* ai) { return new CastWillOfTheForsakenAction(ai); };
#ifndef MANGOSBOT_ZERO
            creators["mana tap"] = [](PlayerbotAI* ai) { return new CastManaTapAction(ai); };
            creators["arcane torrent"] = [](PlayerbotAI* ai) { return new CastArcaneTorrentAction(ai); };
            creators["gift of the naaru"] = [](PlayerbotAI* ai) { return new CastGiftOfTheNaaruAction(ai); };
#endif
#ifdef MANGOSBOT_TWO
            creators["every_man_for_himself"] = [](PlayerbotAI* ai) { return new CastEveryManforHimselfAction(ai); };
#endif

            creators["use id"] = [](PlayerbotAI* ai) { return new UseItemIdAction(ai); };
            creators["move to"] = [](PlayerbotAI* ai) { return new MoveToAction(ai); };

            // Raid consumables
            creators["shadow protection potion"] = [](PlayerbotAI* ai) { return new UseShadowProtectionPotionAction(ai); };
            creators["nature protection potion"] = [](PlayerbotAI* ai) { return new UseNatureProtectionPotionAction(ai); };
            creators["arcane protection potion"] = [](PlayerbotAI* ai) { return new UseArcaneProtectionPotionAction(ai); };
            creators["frost protection potion"] = [](PlayerbotAI* ai) { return new UseFrostProtectionPotionAction(ai); };

            // Dungeon Actions
            creators["enable onyxia's lair strategy"] = [](PlayerbotAI* ai) { return new OnyxiasLairEnableDungeonStrategyAction(ai); };
            creators["disable onyxia's lair strategy"] = [](PlayerbotAI* ai) { return new OnyxiasLairDisableDungeonStrategyAction(ai); };
            creators["enable molten core strategy"] = [](PlayerbotAI* ai) { return new MoltenCoreEnableDungeonStrategyAction(ai); };
            creators["disable molten core strategy"] = [](PlayerbotAI* ai) { return new MoltenCoreDisableDungeonStrategyAction(ai); };
            creators["enable blackwing lair strategy"] = [](PlayerbotAI* ai) { return new BlackwingLairEnableDungeonStrategyAction(ai); };
            creators["disable blackwing lair strategy"] = [](PlayerbotAI* ai) { return new BlackwingLairDisableDungeonStrategyAction(ai); };
            creators["enable karazhan strategy"] = [](PlayerbotAI* ai) { return new KarazhanEnableDungeonStrategyAction(ai); };
            creators["disable karazhan strategy"] = [](PlayerbotAI* ai) { return new KarazhanDisableDungeonStrategyAction(ai); };
            creators["enable naxxramas strategy"] = [](PlayerbotAI* ai) { return new NaxxramasEnableDungeonStrategyAction(ai); };
            creators["disable naxxramas strategy"] = [](PlayerbotAI* ai) { return new NaxxramasDisableDungeonStrategyAction(ai); };
            creators["enable ruins of ahnqiraj strategy"] = [](PlayerbotAI* ai) { return new RuinsOfAhnQirajEnableDungeonStrategyAction(ai); };
            creators["disable ruins of ahnqiraj strategy"] = [](PlayerbotAI* ai) { return new RuinsOfAhnQirajDisableDungeonStrategyAction(ai); };
            creators["enable temple of ahnqiraj strategy"] = [](PlayerbotAI* ai) { return new TempleOfAhnQirajEnableDungeonStrategyAction(ai); };
            creators["disable temple of ahnqiraj strategy"] = [](PlayerbotAI* ai) { return new TempleOfAhnQirajDisableDungeonStrategyAction(ai); };
            creators["give ahnqiraj supplies"] = [](PlayerbotAI* ai) { return new GiveAhnQirajSuppliesAction(ai); };
            creators["enable scarlet citadel strategy"] = [](PlayerbotAI* ai) { return new ScarletCitadelEnableDungeonStrategyAction(ai); };
            creators["disable scarlet citadel strategy"] = [](PlayerbotAI* ai) { return new ScarletCitadelDisableDungeonStrategyAction(ai); };

            // Blackrock Spire Actions
            creators["enable blackrock spire strategy"] = [](PlayerbotAI* ai) { return new BlackrockSpireEnableDungeonStrategyAction(ai); };
            creators["disable blackrock spire strategy"] = [](PlayerbotAI* ai) { return new BlackrockSpireDisableDungeonStrategyAction(ai); };
            creators["enable pyroguard emberseer fight strategy"] = [](PlayerbotAI* ai) { return new PyroguardEmberseerEnableFightStrategyAction(ai); };
            creators["disable pyroguard emberseer fight strategy"] = [](PlayerbotAI* ai) { return new PyroguardEmberseerDisableFightStrategyAction(ai); };
            creators["enable rend blackhand fight strategy"] = [](PlayerbotAI* ai) { return new RendBlackhandEnableFightStrategyAction(ai); };
            creators["disable rend blackhand fight strategy"] = [](PlayerbotAI* ai) { return new RendBlackhandDisableFightStrategyAction(ai); };
            creators["enable the beast fight strategy"] = [](PlayerbotAI* ai) { return new TheBeastEnableFightStrategyAction(ai); };
            creators["disable the beast fight strategy"] = [](PlayerbotAI* ai) { return new TheBeastDisableFightStrategyAction(ai); };
            creators["enable general drakkisath fight strategy"] = [](PlayerbotAI* ai) { return new GeneralDrakkisathEnableFightStrategyAction(ai); };
            creators["disable general drakkisath fight strategy"] = [](PlayerbotAI* ai) { return new GeneralDrakkisathDisableFightStrategyAction(ai); };

            // Emerald Sanctum Actions
            creators["enable emerald sanctum strategy"] = [](PlayerbotAI* ai) { return new EmeraldSanctumEnableDungeonStrategyAction(ai); };
            creators["disable emerald sanctum strategy"] = [](PlayerbotAI* ai) { return new EmeraldSanctumDisableDungeonStrategyAction(ai); };
            creators["enable erennius fight strategy"] = [](PlayerbotAI* ai) { return new ErenniusEnableFightStrategyAction(ai); };
            creators["disable erennius fight strategy"] = [](PlayerbotAI* ai) { return new ErenniusDisableFightStrategyAction(ai); };
            creators["enable solnius fight strategy"] = [](PlayerbotAI* ai) { return new SolniusEnableFightStrategyAction(ai); };
            creators["disable solnius fight strategy"] = [](PlayerbotAI* ai) { return new SolniusDisableFightStrategyAction(ai); };

            // Lower Karazhan Halls Actions
            creators["enable araxxna fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "enable araxxna fight strategy", "+araxxna"); };
            creators["disable araxxna fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "disable araxxna fight strategy", "-araxxna"); };
            creators["enable howlfang fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "enable howlfang fight strategy", "+howlfang"); };
            creators["disable howlfang fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "disable howlfang fight strategy", "-howlfang"); };
            creators["enable lord blackwald fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "enable lord blackwald fight strategy", "+lord blackwald"); };
            creators["disable lord blackwald fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "disable lord blackwald fight strategy", "-lord blackwald"); };
            creators["enable grizikil fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "enable grizikil fight strategy", "+grizikil"); };
            creators["disable grizikil fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "disable grizikil fight strategy", "-grizikil"); };
            creators["enable lkh moroes fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "enable lkh moroes fight strategy", "+lkh moroes"); };
            creators["disable lkh moroes fight strategy"] = [](PlayerbotAI* ai) { return new ChangeAllStrategyAction(ai, "disable lkh moroes fight strategy", "-lkh moroes"); };


            // Scarlet Citadel Boss Actions
            creators["enable daelus fight strategy"] = [](PlayerbotAI* ai) { return new DaelusEnableFightStrategyAction(ai); };
            creators["disable daelus fight strategy"] = [](PlayerbotAI* ai) { return new DaelusDisableFightStrategyAction(ai); };
            creators["daelus close tether"] = [](PlayerbotAI* ai) { return new DaelusCloseTetherAction(ai); };
            creators["daelus drop poison cloud"] = [](PlayerbotAI* ai) { return new DaelusDropPoisonCloudAction(ai); };
            creators["move away from poison cloud"] = [](PlayerbotAI* ai) { return new MoveAwayFromPoisonCloudAction(ai); };
            creators["daelus tank swap"] = [](PlayerbotAI* ai) { return new DaelusTankSwapAction(ai); };
            creators["attack daelus spirits"] = [](PlayerbotAI* ai) { return new AttackDaelusSpiritsAction(ai); };

            creators["enable ardaeus fight strategy"] = [](PlayerbotAI* ai) { return new ArdaeusEnableFightStrategyAction(ai); };
            creators["disable ardaeus fight strategy"] = [](PlayerbotAI* ai) { return new ArdaeusDisableFightStrategyAction(ai); };
            creators["attack ardaeus sun"] = [](PlayerbotAI* ai) { return new AttackArdaeusSunAction(ai); };
            creators["taunt ardaeus statue"] = [](PlayerbotAI* ai) { return new TauntArdaeusStatueAction(ai); };
            creators["attack ardaeus statue"] = [](PlayerbotAI* ai) { return new AttackArdaeusStatueAction(ai); };

            creators["enable mariella fight strategy"] = [](PlayerbotAI* ai) { return new MariellaEnableFightStrategyAction(ai); };
            creators["disable mariella fight strategy"] = [](PlayerbotAI* ai) { return new MariellaDisableFightStrategyAction(ai); };
            creators["move out of mariella void zone"] = [](PlayerbotAI* ai) { return new MoveOutOfMariellaVoidZoneAction(ai); };
            creators["attack mariella felhounds"] = [](PlayerbotAI* ai) { return new AttackMariellaFelhoundsAction(ai); };
            creators["taunt mariella felhound"] = [](PlayerbotAI* ai) { return new TauntMariellaFelhoundAction(ai); };

            creators["enable eric vesper fight strategy"] = [](PlayerbotAI* ai) { return new EricVesperEnableFightStrategyAction(ai); };
            creators["disable eric vesper fight strategy"] = [](PlayerbotAI* ai) { return new EricVesperDisableFightStrategyAction(ai); };
            creators["attack eric vesper"] = [](PlayerbotAI* ai) { return new AttackEricVesperAction(ai); };

            creators["enable darkcaller rayn fight strategy"] = [](PlayerbotAI* ai) { return new DarkcallerRaynEnableFightStrategyAction(ai); };
            creators["disable darkcaller rayn fight strategy"] = [](PlayerbotAI* ai) { return new DarkcallerRaynDisableFightStrategyAction(ai); };

            creators["enable abbendis fight strategy"] = [](PlayerbotAI* ai) { return new AbbendisEnableFightStrategyAction(ai); };
            creators["disable abbendis fight strategy"] = [](PlayerbotAI* ai) { return new AbbendisDisableFightStrategyAction(ai); };
            creators["tank abbendis"] = [](PlayerbotAI* ai) { return new TankAbbendisAction(ai); };

            // Dungeon Boss Actions
            creators["enable onyxia fight strategy"] = [](PlayerbotAI* ai) { return new OnyxiaEnableFightStrategyAction(ai); };
            creators["disable onyxia fight strategy"] = [](PlayerbotAI* ai) { return new OnyxiaDisableFightStrategyAction(ai); };
            creators["move away from onyxia"] = [](PlayerbotAI* ai) { return new MoveAwayFromOnyxiaAction(ai); };
            creators["tank onyxia north"] = [](PlayerbotAI* ai) { return new TankOnyxiaNorthAction(ai); };
            creators["taunt onyxia landing"] = [](PlayerbotAI* ai) { return new TauntOnyxiaLandingAction(ai); };
            creators["attack onyxia whelps"] = [](PlayerbotAI* ai) { return new AttackOnyxiaWhelpsAction(ai); };
            creators["taunt onyxian warder"] = [](PlayerbotAI* ai) { return new TauntOnyxianWarderAction(ai); };
            creators["counter onyxia fear"] = [](PlayerbotAI* ai) { return new OnyxiaFearCounterAction(ai); };
            creators["pause dps onyxia landing"] = [](PlayerbotAI* ai) { return new PauseDpsOnyxiaLandingAction(ai); };

            creators["enable magmadar fight strategy"] = [](PlayerbotAI* ai) { return new MagmadarEnableFightStrategyAction(ai); };
            creators["disable magmadar fight strategy"] = [](PlayerbotAI* ai) { return new MagmadarDisableFightStrategyAction(ai); };
            creators["move away from magmadar"] = [](PlayerbotAI* ai) { return new MagmadarMoveAwayAction(ai); };

            creators["enable lucifron fight strategy"] = [](PlayerbotAI* ai) { return new LucifronEnableFightStrategyAction(ai); };
            creators["disable lucifron fight strategy"] = [](PlayerbotAI* ai) { return new LucifronDisableFightStrategyAction(ai); };

            creators["enable gehennas fight strategy"] = [](PlayerbotAI* ai) { return new GehennasEnableFightStrategyAction(ai); };
            creators["disable gehennas fight strategy"] = [](PlayerbotAI* ai) { return new GehennasDisableFightStrategyAction(ai); };

            creators["enable garr fight strategy"] = [](PlayerbotAI* ai) { return new GarrEnableFightStrategyAction(ai); };
            creators["disable garr fight strategy"] = [](PlayerbotAI* ai) { return new GarrDisableFightStrategyAction(ai); };

            creators["enable geddon fight strategy"] = [](PlayerbotAI* ai) { return new GeddonEnableFightStrategyAction(ai); };
            creators["disable geddon fight strategy"] = [](PlayerbotAI* ai) { return new GeddonDisableFightStrategyAction(ai); };
            creators["move away from geddon"] = [](PlayerbotAI* ai) { return new MoveAwayFromGeddonAction(ai); };

            creators["enable shazzrah fight strategy"] = [](PlayerbotAI* ai) { return new ShazzrahEnableFightStrategyAction(ai); };
            creators["disable shazzrah fight strategy"] = [](PlayerbotAI* ai) { return new ShazzrahDisableFightStrategyAction(ai); };
            creators["move away from shazzrah"] = [](PlayerbotAI* ai) { return new MoveAwayFromShazzrahAction(ai); };

            creators["enable golemagg fight strategy"] = [](PlayerbotAI* ai) { return new GolemaggEnableFightStrategyAction(ai); };
            creators["disable golemagg fight strategy"] = [](PlayerbotAI* ai) { return new GolemaggDisableFightStrategyAction(ai); };

            creators["enable sulfuron fight strategy"] = [](PlayerbotAI* ai) { return new SulfuronEnableFightStrategyAction(ai); };
            creators["disable sulfuron fight strategy"] = [](PlayerbotAI* ai) { return new SulfuronDisableFightStrategyAction(ai); };

            creators["enable domo fight strategy"] = [](PlayerbotAI* ai) { return new DomoEnableFightStrategyAction(ai); };
            creators["disable domo fight strategy"] = [](PlayerbotAI* ai) { return new DomoDisableFightStrategyAction(ai); };

            creators["enable ragnaros fight strategy"] = [](PlayerbotAI* ai) { return new RagnarosEnableFightStrategyAction(ai); };
            creators["disable ragnaros fight strategy"] = [](PlayerbotAI* ai) { return new RagnarosDisableFightStrategyAction(ai); };
            creators["move away from thaurissan"] = [](PlayerbotAI* ai) { return new MoveAwayFromThaurissanAction(ai); };
            creators["enable twin golem fight strategy"] = [](PlayerbotAI* ai) { return new TwinGolemEnableFightStrategyAction(ai); };
            creators["disable twin golem fight strategy"] = [](PlayerbotAI* ai) { return new TwinGolemDisableFightStrategyAction(ai); };
            creators["enable thaurissan fight strategy"] = [](PlayerbotAI* ai) { return new ThaurissanEnableFightStrategyAction(ai); };
            creators["disable thaurissan fight strategy"] = [](PlayerbotAI* ai) { return new ThaurissanDisableFightStrategyAction(ai); };
            creators["enable incindis fight strategy"] = [](PlayerbotAI* ai) { return new IncindisEnableFightStrategyAction(ai); };
            creators["disable incindis fight strategy"] = [](PlayerbotAI* ai) { return new IncindisDisableFightStrategyAction(ai); };
            creators["move away from ragnaros"] = [](PlayerbotAI* ai) { return new MoveAwayFromRagnarosAction(ai); };

            creators["move away from hazard"] = [](PlayerbotAI* ai) { return new MoveAwayFromHazard(ai); };
            creators["move away from specific creatures"] = [](PlayerbotAI* ai) { return new MoveAwayFromSpecificCreatures(ai, 10.0f); };
            creators["move away from raid"] = [](PlayerbotAI* ai) { return new MoveAwayFromRaidAction(ai); };
            creators["attack entry"] = [](PlayerbotAI* ai) { return new AttackNearestEntryAction(ai); };
            creators["offtank entry"] = [](PlayerbotAI* ai) { return new OfftankNearestEntryAction(ai); };
            creators["hide from firemaw"] = [](PlayerbotAI* ai) { return new HideFromCreatureAction(ai, "hide from firemaw", 11983); };
            creators["hide from ebonroc"] = [](PlayerbotAI* ai) { return new HideFromCreatureAction(ai, "hide from ebonroc", 14601); };
            creators["hide from flamegor"] = [](PlayerbotAI* ai) { return new HideFromCreatureAction(ai, "hide from flamegor", 11981); };
            creators["taunt vael"] = [](PlayerbotAI* ai) { return new TauntCreatureAction(ai, "taunt vael", 13020); };
            creators["taunt broodlord"] = [](PlayerbotAI* ai) { return new TauntCreatureAction(ai, "taunt broodlord", 12017); };
            creators["taunt firemaw"] = [](PlayerbotAI* ai) { return new TauntCreatureAction(ai, "taunt firemaw", 11983); };
            creators["taunt ebonroc"] = [](PlayerbotAI* ai) { return new TauntCreatureAction(ai, "taunt ebonroc", 14601); };
            creators["taunt flamegor"] = [](PlayerbotAI* ai) { return new TauntCreatureAction(ai, "taunt flamegor", 11981); };
            creators["taunt chromaggus"] = [](PlayerbotAI* ai) { return new TauntCreatureAction(ai, "taunt chromaggus", 14020); };
            creators["move to mc rune"] = [](PlayerbotAI* ai) { return new MoveToMCRuneAction(ai); };
            creators["add mc douse item"] = [](PlayerbotAI* ai) { return new AddMCDouseItemAction(ai); };
            creators["douse mc rune aqual"] = [](PlayerbotAI* ai) { return new DouseMCRuneActionAqual(ai); };
            creators["douse mc rune eternal"] = [](PlayerbotAI* ai) { return new DouseMCRuneActionEternal(ai); };
            creators["attack core hound pack"] = [](PlayerbotAI* ai) { return new AttackCoreHoundPackAction(ai); };

            creators["move to suppression device"] = [](PlayerbotAI* ai) { return new MoveToSuppressionDeviceAction(ai); };
            creators["stealth for suppression device"] = [](PlayerbotAI* ai) { return new StealthForSuppressionDeviceAction(ai); };
            creators["disarm suppression device"] = [](PlayerbotAI* ai) { return new DisarmSuppressionDeviceAction(ai); };
            creators["enable suppression room strategy"] = [](PlayerbotAI* ai) { return new SuppressionRoomEnableStrategyAction(ai); };
            creators["disable suppression room strategy"] = [](PlayerbotAI* ai) { return new SuppressionRoomDisableStrategyAction(ai); };

            creators["enable razorgore fight strategy"] = [](PlayerbotAI* ai) { return new RazorgoreEnableFightStrategyAction(ai); };
            creators["disable razorgore fight strategy"] = [](PlayerbotAI* ai) { return new RazorgoreDisableFightStrategyAction(ai); };
            creators["move to razorgore corner"] = [](PlayerbotAI* ai) { return new MoveToRazorgoreCornerAction(ai); };
            creators["attack razorgore corner add"] = [](PlayerbotAI* ai) { return new AttackRazorgoreCornerAddAction(ai); };
            creators["razorgore sleep dragonkin"] = [](PlayerbotAI* ai) { return new RazorgoreSleepDragonkinAction(ai); };
            creators["taunt razorgore"] = [](PlayerbotAI* ai) { return new TauntRazorgoreAction(ai); };

            creators["enable vael fight strategy"] = [](PlayerbotAI* ai) { return new VaelEnableFightStrategyAction(ai); };
            creators["disable vael fight strategy"] = [](PlayerbotAI* ai) { return new VaelDisableFightStrategyAction(ai); };

            creators["enable broodlord fight strategy"] = [](PlayerbotAI* ai) { return new BroodlordEnableFightStrategyAction(ai); };
            creators["disable broodlord fight strategy"] = [](PlayerbotAI* ai) { return new BroodlordDisableFightStrategyAction(ai); };

            creators["enable chromaggus fight strategy"] = [](PlayerbotAI* ai) { return new ChromaggusEnableFightStrategyAction(ai); };
            creators["disable chromaggus fight strategy"] = [](PlayerbotAI* ai) { return new ChromaggusDisableFightStrategyAction(ai); };
            creators["clear chromaggus bronze"] = [](PlayerbotAI* ai) { return new ClearChromaggusBronzeAction(ai); };
            creators["clear shadow flame dot"] = [](PlayerbotAI* ai) { return new ClearShadowFlameDotAction(ai); };

            creators["enable nefarian fight strategy"] = [](PlayerbotAI* ai) { return new NefarianEnableFightStrategyAction(ai); };
            creators["disable nefarian fight strategy"] = [](PlayerbotAI* ai) { return new NefarianDisableFightStrategyAction(ai); };

            creators["enable firemaw fight strategy"] = [](PlayerbotAI* ai) { return new FiremawEnableFightStrategyAction(ai); };
            creators["disable firemaw fight strategy"] = [](PlayerbotAI* ai) { return new FiremawDisableFightStrategyAction(ai); };

            creators["enable ebonroc fight strategy"] = [](PlayerbotAI* ai) { return new EbonrocEnableFightStrategyAction(ai); };
            creators["disable ebonroc fight strategy"] = [](PlayerbotAI* ai) { return new EbonrocDisableFightStrategyAction(ai); };

            creators["enable flamegor fight strategy"] = [](PlayerbotAI* ai) { return new FlamegorEnableFightStrategyAction(ai); };
            creators["disable flamegor fight strategy"] = [](PlayerbotAI* ai) { return new FlamegorDisableFightStrategyAction(ai); };

            creators["enable netherspite fight strategy"] = [](PlayerbotAI* ai) { return new NetherspiteEnableFightStrategyAction(ai); };
            creators["disable netherspite fight strategy"] = [](PlayerbotAI* ai) { return new NetherspiteDisableFightStrategyAction(ai); };
            creators["move away from void zone"] = [](PlayerbotAI* ai) { return new VoidZoneMoveAwayAction(ai); };
            creators["add nether portal - perseverence for tank"] = [](PlayerbotAI* ai) { return new AddNetherPortalPerseverenceForTankAction(ai); };
            creators["remove nether portal buffs from netherspite"] = [](PlayerbotAI* ai) { return new RemoveNetherPortalBuffsFromNetherspiteAction(ai); };
            creators["remove nether portal - perseverence"] = [](PlayerbotAI* ai) { return new RemoveNetherPortalPerseverenceAction(ai); };
            creators["remove nether portal - serenity"] = [](PlayerbotAI* ai) { return new RemoveNetherPortalSerenityAction(ai); };
            creators["remove nether portal - dominance"] = [](PlayerbotAI* ai) { return new RemoveNetherPortalDominanceAction(ai); };

            creators["enable prince malchezaar fight strategy"] = [](PlayerbotAI* ai) { return new PrinceMalchezaarEnableFightStrategyAction(ai); };
            creators["disable prince malchezaar fight strategy"] = [](PlayerbotAI* ai) { return new PrinceMalchezaarDisableFightStrategyAction(ai); };
            creators["move away from netherspite infernal"] = [](PlayerbotAI* ai) { return new NetherspiteInfernalMoveAwayAction(ai); };

            creators["enable four horseman fight strategy"] = [](PlayerbotAI* ai) { return new FourHorsemanEnableFightStrategyAction(ai); };
            creators["disable four horseman fight strategy"] = [](PlayerbotAI* ai) { return new FourHorsemanDisableFightStrategyAction(ai); };

            creators["enable anubrekhan fight strategy"] = [](PlayerbotAI* ai) { return new AnubRekhanEnableFightStrategyAction(ai); };
            creators["disable anubrekhan fight strategy"] = [](PlayerbotAI* ai) { return new AnubRekhanDisableFightStrategyAction(ai); };
            creators["move away from anubrekhan"] = [](PlayerbotAI* ai) { return new MoveAwayFromAnubRekhanAction(ai); };

            creators["enable faerlina fight strategy"] = [](PlayerbotAI* ai) { return new FaerlinaEnableFightStrategyAction(ai); };
            creators["disable faerlina fight strategy"] = [](PlayerbotAI* ai) { return new FaerlinaDisableFightStrategyAction(ai); };

            creators["enable maexxna fight strategy"] = [](PlayerbotAI* ai) { return new MaexxnaEnableFightStrategyAction(ai); };
            creators["disable maexxna fight strategy"] = [](PlayerbotAI* ai) { return new MaexxnaDisableFightStrategyAction(ai); };

            creators["enable noth fight strategy"] = [](PlayerbotAI* ai) { return new NothEnableFightStrategyAction(ai); };
            creators["disable noth fight strategy"] = [](PlayerbotAI* ai) { return new NothDisableFightStrategyAction(ai); };

            creators["enable heigan fight strategy"] = [](PlayerbotAI* ai) { return new HeiganEnableFightStrategyAction(ai); };
            creators["disable heigan fight strategy"] = [](PlayerbotAI* ai) { return new HeiganDisableFightStrategyAction(ai); };

            creators["enable loatheb fight strategy"] = [](PlayerbotAI* ai) { return new LoathebEnableFightStrategyAction(ai); };
            creators["disable loatheb fight strategy"] = [](PlayerbotAI* ai) { return new LoathebDisableFightStrategyAction(ai); };

            creators["enable grobbulus fight strategy"] = [](PlayerbotAI* ai) { return new GrobbulusEnableFightStrategyAction(ai); };
            creators["disable grobbulus fight strategy"] = [](PlayerbotAI* ai) { return new GrobbulusDisableFightStrategyAction(ai); };

            creators["enable gluth fight strategy"] = [](PlayerbotAI* ai) { return new GluthEnableFightStrategyAction(ai); };
            creators["disable gluth fight strategy"] = [](PlayerbotAI* ai) { return new GluthDisableFightStrategyAction(ai); };
            creators["taunt gluth"] = [](PlayerbotAI* ai) { return new TauntGluthAction(ai); };

            creators["enable thaddius fight strategy"] = [](PlayerbotAI* ai) { return new ThaddiusEnableFightStrategyAction(ai); };
            creators["disable thaddius fight strategy"] = [](PlayerbotAI* ai) { return new ThaddiusDisableFightStrategyAction(ai); };
            creators["join thaddius polarity"] = [](PlayerbotAI* ai) { return new JoinThaddiusPolarityAction(ai); };

            creators["enable sapphiron fight strategy"] = [](PlayerbotAI* ai) { return new SapphironEnableFightStrategyAction(ai); };
            creators["disable sapphiron fight strategy"] = [](PlayerbotAI* ai) { return new SapphironDisableFightStrategyAction(ai); };

            creators["enable kelthuzad fight strategy"] = [](PlayerbotAI* ai) { return new KelThuzadEnableFightStrategyAction(ai); };
            creators["disable kelthuzad fight strategy"] = [](PlayerbotAI* ai) { return new KelThuzadDisableFightStrategyAction(ai); };
            creators["cc chained player"] = [](PlayerbotAI* ai) { return new CcChainedPlayerAction(ai); };
            creators["hide behind iceblock"] = [](PlayerbotAI* ai) { return new HideBehindIceblockAction(ai); };

            creators["enable gothik fight strategy"] = [](PlayerbotAI* ai) { return new GothikEnableFightStrategyAction(ai); };
            creators["disable gothik fight strategy"] = [](PlayerbotAI* ai) { return new GothikDisableFightStrategyAction(ai); };
            creators["move to gothik post"] = [](PlayerbotAI* ai) { return new MoveToGothikPostAction(ai); };

            creators["enable razuvious fight strategy"] = [](PlayerbotAI* ai) { return new RazuviousEnableFightStrategyAction(ai); };
            creators["disable razuvious fight strategy"] = [](PlayerbotAI* ai) { return new RazuviousDisableFightStrategyAction(ai); };
            creators["mind control understudy"] = [](PlayerbotAI* ai) { return new MindControlUnderstudyAction(ai); };
            creators["understudy taunt"] = [](PlayerbotAI* ai) { return new UnderstudyTauntAction(ai); };

            creators["enable patchwerk fight strategy"] = [](PlayerbotAI* ai) { return new PatchwerkEnableFightStrategyAction(ai); };
            creators["disable patchwerk fight strategy"] = [](PlayerbotAI* ai) { return new PatchwerkDisableFightStrategyAction(ai); };

            creators["enable kurinnaxx fight strategy"] = [](PlayerbotAI* ai) { return new KurinnaxxEnableFightStrategyAction(ai); };
            creators["disable kurinnaxx fight strategy"] = [](PlayerbotAI* ai) { return new KurinnaxxDisableFightStrategyAction(ai); };
            creators["taunt kurinnaxx"] = [](PlayerbotAI* ai) { return new TauntKurinnaxxAction(ai); };

            creators["enable rajaxx fight strategy"] = [](PlayerbotAI* ai) { return new RajaxxEnableFightStrategyAction(ai); };
            creators["disable rajaxx fight strategy"] = [](PlayerbotAI* ai) { return new RajaxxDisableFightStrategyAction(ai); };

            creators["enable buru fight strategy"] = [](PlayerbotAI* ai) { return new BuruEnableFightStrategyAction(ai); };
            creators["disable buru fight strategy"] = [](PlayerbotAI* ai) { return new BuruDisableFightStrategyAction(ai); };
            creators["move away from buru"] = [](PlayerbotAI* ai) { return new MoveAwayFromBuruAction(ai); };

            creators["enable moam fight strategy"] = [](PlayerbotAI* ai) { return new MoamEnableFightStrategyAction(ai); };
            creators["disable moam fight strategy"] = [](PlayerbotAI* ai) { return new MoamDisableFightStrategyAction(ai); };

            creators["enable ayamiss fight strategy"] = [](PlayerbotAI* ai) { return new AyamissEnableFightStrategyAction(ai); };
            creators["disable ayamiss fight strategy"] = [](PlayerbotAI* ai) { return new AyamissDisableFightStrategyAction(ai); };

            creators["enable ossirian fight strategy"] = [](PlayerbotAI* ai) { return new OssirianEnableFightStrategyAction(ai); };
            creators["disable ossirian fight strategy"] = [](PlayerbotAI* ai) { return new OssirianDisableFightStrategyAction(ai); };

            creators["enable skeram fight strategy"] = [](PlayerbotAI* ai) { return new SkeramEnableFightStrategyAction(ai); };
            creators["disable skeram fight strategy"] = [](PlayerbotAI* ai) { return new SkeramDisableFightStrategyAction(ai); };
            creators["cc fulfilled player"] = [](PlayerbotAI* ai) { return new CcFulfilledPlayerAction(ai); };

            creators["enable bug trio fight strategy"] = [](PlayerbotAI* ai) { return new BugTrioEnableFightStrategyAction(ai); };
            creators["disable bug trio fight strategy"] = [](PlayerbotAI* ai) { return new BugTrioDisableFightStrategyAction(ai); };

            creators["enable sartura fight strategy"] = [](PlayerbotAI* ai) { return new SarturaEnableFightStrategyAction(ai); };
            creators["disable sartura fight strategy"] = [](PlayerbotAI* ai) { return new SarturaDisableFightStrategyAction(ai); };
            creators["move away from sartura"] = [](PlayerbotAI* ai) { return new MoveAwayFromSarturaAction(ai); };
            creators["move away from sartura guard"] = [](PlayerbotAI* ai) { return new MoveAwayFromSarturaGuardAction(ai); };

            creators["enable fankriss fight strategy"] = [](PlayerbotAI* ai) { return new FankrissEnableFightStrategyAction(ai); };
            creators["disable fankriss fight strategy"] = [](PlayerbotAI* ai) { return new FankrissDisableFightStrategyAction(ai); };
            creators["taunt fankriss"] = [](PlayerbotAI* ai) { return new TauntFankrissAction(ai); };

            creators["enable viscidus fight strategy"] = [](PlayerbotAI* ai) { return new ViscidusEnableFightStrategyAction(ai); };
            creators["disable viscidus fight strategy"] = [](PlayerbotAI* ai) { return new ViscidusDisableFightStrategyAction(ai); };
            creators["viscidus frost assist"] = [](PlayerbotAI* ai) { return new ViscidusFrostAssistAction(ai); };

            creators["enable huhuran fight strategy"] = [](PlayerbotAI* ai) { return new HuhuranEnableFightStrategyAction(ai); };
            creators["disable huhuran fight strategy"] = [](PlayerbotAI* ai) { return new HuhuranDisableFightStrategyAction(ai); };
            creators["taunt huhuran"] = [](PlayerbotAI* ai) { return new TauntHuhuranAction(ai); };

            creators["enable twins fight strategy"] = [](PlayerbotAI* ai) { return new TwinsEnableFightStrategyAction(ai); };
            creators["disable twins fight strategy"] = [](PlayerbotAI* ai) { return new TwinsDisableFightStrategyAction(ai); };
            creators["attack veknilash"] = [](PlayerbotAI* ai) { return new AttackVeknilashAction(ai); };
            creators["attack veklor"] = [](PlayerbotAI* ai) { return new AttackVeklorAction(ai); };
            creators["searing pain veklor"] = [](PlayerbotAI* ai) { return new TwinsSearingPainAction(ai); };
            creators["twins tank post"] = [](PlayerbotAI* ai) { return new TwinsTankPostAction(ai); };

            creators["enable ouro fight strategy"] = [](PlayerbotAI* ai) { return new OuroEnableFightStrategyAction(ai); };
            creators["disable ouro fight strategy"] = [](PlayerbotAI* ai) { return new OuroDisableFightStrategyAction(ai); };

            creators["enable cthun fight strategy"] = [](PlayerbotAI* ai) { return new CthunEnableFightStrategyAction(ai); };
            creators["disable cthun fight strategy"] = [](PlayerbotAI* ai) { return new CthunDisableFightStrategyAction(ai); };
            creators["move to cthun stomach exit"] = [](PlayerbotAI* ai) { return new MoveToCthunStomachExitAction(ai); };
            creators["attack flesh tentacle ranged"] = [](PlayerbotAI* ai) { return new AttackFleshTentacleRangedAction(ai); };

            creators["enable zul'gurub strategy"] = [](PlayerbotAI* ai) { return new ZulGurubEnableDungeonStrategyAction(ai); };
            creators["disable zul'gurub strategy"] = [](PlayerbotAI* ai) { return new ZulGurubDisableDungeonStrategyAction(ai); };

            creators["enable jeklik fight strategy"] = [](PlayerbotAI* ai) { return new JeklikEnableFightStrategyAction(ai); };
            creators["disable jeklik fight strategy"] = [](PlayerbotAI* ai) { return new JeklikDisableFightStrategyAction(ai); };

            creators["enable venoxis fight strategy"] = [](PlayerbotAI* ai) { return new VenoxisEnableFightStrategyAction(ai); };
            creators["disable venoxis fight strategy"] = [](PlayerbotAI* ai) { return new VenoxisDisableFightStrategyAction(ai); };

            creators["enable marli fight strategy"] = [](PlayerbotAI* ai) { return new MarliEnableFightStrategyAction(ai); };
            creators["disable marli fight strategy"] = [](PlayerbotAI* ai) { return new MarliDisableFightStrategyAction(ai); };
            creators["taunt marli"] = [](PlayerbotAI* ai) { return new TauntMarliAction(ai); };

            creators["enable mandokir fight strategy"] = [](PlayerbotAI* ai) { return new MandokirEnableFightStrategyAction(ai); };
            creators["disable mandokir fight strategy"] = [](PlayerbotAI* ai) { return new MandokirDisableFightStrategyAction(ai); };
            creators["mandokir watch"] = [](PlayerbotAI* ai) { return new MandokirWatchAction(ai); };

            creators["enable thekal fight strategy"] = [](PlayerbotAI* ai) { return new ThekalEnableFightStrategyAction(ai); };
            creators["disable thekal fight strategy"] = [](PlayerbotAI* ai) { return new ThekalDisableFightStrategyAction(ai); };
            creators["attack thekal trio"] = [](PlayerbotAI* ai) { return new AttackThekalTrioAction(ai); };

            creators["enable arlokk fight strategy"] = [](PlayerbotAI* ai) { return new ArlokkEnableFightStrategyAction(ai); };
            creators["disable arlokk fight strategy"] = [](PlayerbotAI* ai) { return new ArlokkDisableFightStrategyAction(ai); };

            creators["enable jindo fight strategy"] = [](PlayerbotAI* ai) { return new JindoEnableFightStrategyAction(ai); };
            creators["disable jindo fight strategy"] = [](PlayerbotAI* ai) { return new JindoDisableFightStrategyAction(ai); };
            creators["cc brainwashed player"] = [](PlayerbotAI* ai) { return new CcBrainwashedPlayerAction(ai); };
            creators["attack shade of jindo"] = [](PlayerbotAI* ai) { return new AttackShadeOfJindoAction(ai); };

            creators["enable hakkar fight strategy"] = [](PlayerbotAI* ai) { return new HakkarEnableFightStrategyAction(ai); };
            creators["disable hakkar fight strategy"] = [](PlayerbotAI* ai) { return new HakkarDisableFightStrategyAction(ai); };
            creators["cc insane player"] = [](PlayerbotAI* ai) { return new CcInsanePlayerAction(ai); };

#ifdef GenerateBotTests
            creators["test"] = [](PlayerbotAI* ai) { return new TestAction(ai); };
#endif
        }    
    };
};
