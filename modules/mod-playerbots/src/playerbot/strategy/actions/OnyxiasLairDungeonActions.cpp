#include "playerbot/playerbot.h"
#include "OnyxiasLairDungeonActions.h"

using namespace ai;

bool TankOnyxiaNorthAction::Execute(Event& event)
{
    if (bot->GetDistance(-8.86f, -212.75f, -88.54f) <= 3.0f)
    {
        // Face Onyxia so tank parries/blocks with back firmly to the North wall
        Unit* boss = GetRaidTargetByEntry(ai, 10184);
        if (boss)
            bot->SetFacingTo(bot->GetAngle(boss));
        return false;
    }

    return MoveTo(bot->GetMapId(), -8.86f, -212.75f, -88.54f, false, false, false, true);
}

bool TankOnyxiaNorthAction::isPossible()
{
    return MovementAction::isPossible() && ai->CanMove();
}

Unit* AttackOnyxiaWhelpsAction::FindClosestWhelp(PlayerbotAI* ai, Player* bot)
{
    Unit* best = nullptr;
    float bestDist = FLT_MAX;
    for (const ObjectGuid& guid : ai->GetAiObjectContext()->GetValue<std::list<ObjectGuid>>("possible attack targets")->Get())
    {
        Creature* c = ai->GetCreature(guid);
        if (!c || !c->IsAlive() || c->GetEntry() != 11262)
            continue;

        float dist = bot->GetDistance(c);
        if (dist < bestDist)
        {
            bestDist = dist;
            best = c;
        }
    }
    return best;
}

bool AttackOnyxiaWhelpsAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    Unit* target = FindClosestWhelp(ai, bot);
    if (target && target->IsInWorld() && target->GetMapId() == bot->GetMapId())
    {
        if (Attack(requester, target))
        {
            SET_AI_VALUE(ObjectGuid, "attack target", target->GetObjectGuid());
            return true;
        }
    }
    return false;
}

bool AttackOnyxiaWhelpsAction::isUseful()
{
    return (!ai->ContainsStrategy(STRATEGY_TYPE_HEAL) || ai->HasStrategy("offdps", BotState::BOT_STATE_COMBAT)) && (FindClosestWhelp(ai, bot) != nullptr);
}

bool OnyxiaFearCounterAction::Execute(Event& event)
{
    // 1. Warrior: Berserker Rage breaks fear and gives fear immunity for 10s
    if (bot->getClass() == CLASS_WARRIOR)
    {
        if (ai->HasSpell("berserker rage"))
        {
            if (bot->GetShapeshiftForm() != FORM_BERSERKERSTANCE && ai->HasSpell("berserker stance"))
                ai->DoSpecificAction("berserker stance", event);

            return ai->DoSpecificAction("berserker rage", event);
        }
    }
    // 2. Shaman: Tremor Totem pulses to break fear on all party members within 30yd
    else if (bot->getClass() == CLASS_SHAMAN)
    {
        if (ai->HasSpell("tremor totem") && !AI_VALUE2(bool, "has totem", "tremor totem"))
        {
            return ai->DoSpecificAction("tremor totem", event);
        }
    }
    // 3. Priest: Fear Ward
    else if (bot->getClass() == CLASS_PRIEST)
    {
        if (ai->HasSpell("fear ward"))
        {
            return ai->DoSpecificAction("fear ward", event);
        }
    }
    // 4. Undead racial: Will of the Forsaken
    if (bot->getRace() == RACE_UNDEAD)
    {
        if ((bot->HasAuraType(SPELL_AURA_MOD_FEAR) || bot->HasAura(18431)) && ai->HasSpell("will of the forsaken"))
        {
            return ai->DoSpecificAction("will of the forsaken", event);
        }
    }
    return false;
}

bool OnyxiaFearCounterAction::isPossible()
{
    if (bot->getClass() == CLASS_WARRIOR)
        return ai->HasSpell("berserker rage");
    if (bot->getClass() == CLASS_SHAMAN)
        return ai->HasSpell("tremor totem");
    if (bot->getClass() == CLASS_PRIEST)
        return ai->HasSpell("fear ward");
    if (bot->getRace() == RACE_UNDEAD)
        return ai->HasSpell("will of the forsaken");
    return false;
}

bool PauseDpsOnyxiaLandingAction::Execute(Event& event)
{
    // If there are living whelps, switch focus to whelps rather than idling
    Unit* whelp = AttackOnyxiaWhelpsAction::FindClosestWhelp(ai, bot);
    if (whelp && whelp->IsInWorld() && whelp->GetMapId() == bot->GetMapId())
    {
        Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
        if (Attack(requester, whelp))
        {
            SET_AI_VALUE(ObjectGuid, "attack target", whelp->GetObjectGuid());
            return true;
        }
    }

    // Stop melee/auto-shot and interrupt current spell on Onyxia
    bot->AttackStop();
    ai->InterruptSpell();
    SET_AI_VALUE(Unit*, "current target", nullptr);
    return true;
}

bool PauseDpsOnyxiaLandingAction::isPossible()
{
    return !ai->IsTank(bot);
}