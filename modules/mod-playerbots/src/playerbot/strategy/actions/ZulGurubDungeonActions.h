#pragma once
#include "DungeonActions.h"
#include "ChangeStrategyAction.h"

namespace ai
{
    class ZulGurubEnableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ZulGurubEnableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable zul'gurub strategy", "+zul'gurub") {}
    };

    class ZulGurubDisableDungeonStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ZulGurubDisableDungeonStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable zul'gurub strategy", "-zul'gurub") {}
    };

    // Fight-strategy enable/disable for every ZG encounter.
    class JeklikEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        JeklikEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable jeklik fight strategy", "+jeklik") {}
    };

    class JeklikDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        JeklikDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable jeklik fight strategy", "-jeklik") {}
    };

    class VenoxisEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        VenoxisEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable venoxis fight strategy", "+venoxis") {}
    };

    class VenoxisDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        VenoxisDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable venoxis fight strategy", "-venoxis") {}
    };

    class MarliEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MarliEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable marli fight strategy", "+marli") {}
    };

    class MarliDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MarliDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable marli fight strategy", "-marli") {}
    };

    class MandokirEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MandokirEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable mandokir fight strategy", "+mandokir") {}
    };

    class MandokirDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        MandokirDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable mandokir fight strategy", "-mandokir") {}
    };

    class ThekalEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ThekalEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable thekal fight strategy", "+thekal") {}
    };

    class ThekalDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ThekalDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable thekal fight strategy", "-thekal") {}
    };

    class ArlokkEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ArlokkEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable arlokk fight strategy", "+arlokk") {}
    };

    class ArlokkDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        ArlokkDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable arlokk fight strategy", "-arlokk") {}
    };

    class JindoEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        JindoEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable jindo fight strategy", "+jindo") {}
    };

    class JindoDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        JindoDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable jindo fight strategy", "-jindo") {}
    };

    class HakkarEnableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        HakkarEnableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "enable hakkar fight strategy", "+hakkar") {}
    };

    class HakkarDisableFightStrategyAction : public ChangeAllStrategyAction
    {
    public:
        HakkarDisableFightStrategyAction(PlayerbotAI* ai) : ChangeAllStrategyAction(ai, "disable hakkar fight strategy", "-hakkar") {}
    };

    // Mandokir's Threatening Gaze (24314): the watched player must not move and
    // must not act (movement > 2 yd or > 500 threat over the gaze snapshot =
    // decapitation). The engine runs ONE action per tick, so a top-relevance
    // hold wins it and suppresses every ability for as long as the aura is up;
    // the first tick also stops the current movement and the auto-attack, both
    // of which generate threat on their own.
    class MandokirWatchAction : public Action
    {
    public:
        MandokirWatchAction(PlayerbotAI* ai) : Action(ai, "mandokir watch") {}

        bool Execute(Event& /*event*/) override
        {
            if (!bot->HasAura(24314))
                return false;

            // Kill the auto-attack: melee swings are threat the gaze punishes.
            if (bot->GetVictim())
                bot->AttackStop();

            // Stand still: the gaze snapshot allows 2 yd (8 yd while feared).
            ai->StopMoving();

            return true;
        }

        bool isUseful() override { return bot->HasAura(24314); }
    };

    // Thekal trio: spread the raid's damage so all three die inside the ~7 s
    // resurrection window. Target the alive member with the HIGHEST health
    // percent - with the whole raid on that rule the three stay level, and at
    // low health the deaths cluster inside the window. Once the tiger form
    // (14509) is the only thing left this action stops firing (it is not in the
    // trio list) and the phase-2 fallback node takes over.
    class AttackThekalTrioAction : public AttackAction
    {
    public:
        AttackThekalTrioAction(PlayerbotAI* ai) : AttackAction(ai, "attack thekal trio") {}

        bool Execute(Event& event) override
        {
            Unit* target = FindHighestHealthMember();
            if (!target)
                return false;

            return Attack(event.getOwner() ? event.getOwner() : GetMaster(), target);
        }

        bool isUseful() override
        {
            return (!ai->IsHeal(bot) || ai->HasStrategy("offdps", BotState::BOT_STATE_COMBAT))
                && FindHighestHealthMember();
        }

    private:
        Unit* FindHighestHealthMember()
        {
            Unit* best = nullptr;
            float bestHealth = -1.0f;

            for (uint32 entry : { 14599u, 11347u, 11348u })
            {
                Unit* member = GetRaidTargetByEntry(ai, entry);
                if (!member || !member->IsAlive())
                    continue;

                const float health = member->GetHealthPercent();
                if (health > bestHealth)
                {
                    bestHealth = health;
                    best = member;
                }
            }

            return best;
        }
    };

    // Marli's webs root the active tank and the follow-up charge resets threat:
    // the other tank taunts her off the webbed victim (BWL Wing-Buffet shape).
    // Taunt fails harmlessly on a taunt-immune target.
    class TauntMarliAction : public TauntCreatureAction
    {
    public:
        TauntMarliAction(PlayerbotAI* ai) : TauntCreatureAction(ai, "taunt marli", 14510) {}
    };

    // Jin'do's Brain Wash (24261): CC the controlled raid member, do not kill.
    class CcBrainwashedPlayerAction : public CcMindControlledAllyAction
    {
    public:
        CcBrainwashedPlayerAction(PlayerbotAI* ai) : CcMindControlledAllyAction(ai, "cc brainwashed player", 24261) {}
    };

    // Hakkar's Cause Insanity (24327): same handling as Skeram/KT.
    class CcInsanePlayerAction : public CcMindControlledAllyAction
    {
    public:
        CcInsanePlayerAction(PlayerbotAI* ai) : CcMindControlledAllyAction(ai, "cc insane player", 24327) {}
    };

    // Only the player carrying Delusions of Jin'do (24306) can damage their
    // Shade (14986) - the damage gate is in the shade's own AI. The action is
    // gated on the aura by JindoDelusionsTrigger.
    class AttackShadeOfJindoAction : public AttackNearestEntryAction
    {
    public:
        AttackShadeOfJindoAction(PlayerbotAI* ai) : AttackNearestEntryAction(ai, "attack shade of jindo") {}

        bool isUseful() override
        {
            return bot->HasAura(24306) && AttackNearestEntryAction::isUseful();
        }
    };
}
