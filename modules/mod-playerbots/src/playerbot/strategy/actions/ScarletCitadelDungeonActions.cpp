#include "playerbot/playerbot.h"
#include "ScarletCitadelDungeonActions.h"
#include "playerbot/PlayerbotAI.h"
#include "playerbot/strategy/values/PositionValue.h"
#include "playerbot/strategy/AiObjectContext.h"
#include "Maps/GridNotifiers.h"
#include "Maps/GridNotifiersImpl.h"
#include "Maps/CellImpl.h"
#include "Objects/Creature.h"
#include "Objects/Player.h"
#include "Spells/SpellAuras.h"

using namespace ai;

// ---------------------------------------------------------------------------
// Boss 1: Sacred Fist Daelus (2000000) Actions
// ---------------------------------------------------------------------------

bool DaelusCloseTetherAction::Execute(Event& event)
{
    Unit* boss = GetRaidTargetByEntry(ai, 2000000);
    if (!boss)
        return false;

    if (bot->GetDistance2d(boss) <= 2.0f)
        return false;

    return MoveTo(bot->GetMapId(), boss->GetPositionX(), boss->GetPositionY(), boss->GetPositionZ(), false, false, false, true);
}

bool DaelusCloseTetherAction::isPossible()
{
    return MovementAction::isPossible() && ai->CanMove();
}

bool AttackDaelusSpiritsAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    Unit* target = FindClosestNormalSpirit();
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

bool AttackDaelusSpiritsAction::isUseful()
{
    return (!ai->ContainsStrategy(STRATEGY_TYPE_HEAL) || ai->HasStrategy("offdps", BotState::BOT_STATE_COMBAT)) && (FindClosestNormalSpirit() != nullptr);
}

Unit* AttackDaelusSpiritsAction::FindClosestNormalSpirit()
{
    Unit* best = nullptr;
    float bestDist = FLT_MAX;
    for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
    {
        Creature* c = ai->GetCreature(guid);
        if (!c || !c->IsAlive() || c->GetEntry() != 2000013)
            continue;

        // Never attack the Chosen One! (The spirit with red aura 26235)
        if (ai->HasAura(26235, c))
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

// ---------------------------------------------------------------------------
// Boss 2: Grand Sorcerer Ardaeus (2000001) Actions
// ---------------------------------------------------------------------------

bool AttackArdaeusSunAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    Unit* sun = GetRaidTargetByEntry(ai, 2000019);
    if (sun && sun->IsInWorld() && sun->IsAlive() && sun->GetMapId() == bot->GetMapId())
    {
        if (Attack(requester, sun))
        {
            SET_AI_VALUE(ObjectGuid, "attack target", sun->GetObjectGuid());
            return true;
        }
    }
    return false;
}

bool AttackArdaeusSunAction::isUseful()
{
    Unit* sun = GetRaidTargetByEntry(ai, 2000019);
    return sun && sun->IsAlive();
}

bool TauntArdaeusStatueAction::Execute(Event& event)
{
    for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
    {
        Creature* c = ai->GetCreature(guid);
        if (c && c->IsAlive() && c->GetEntry() >= 2000020 && c->GetEntry() <= 2000030)
        {
            if (c->GetVictim() != bot)
            {
                ai->CastSpell("taunt", c);
                ai->CastSpell("growl", c);
                return true;
            }
        }
    }
    return false;
}

bool TauntArdaeusStatueAction::isUseful()
{
    if (!ai->IsTank(bot))
        return false;

    for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
    {
        Creature* c = ai->GetCreature(guid);
        if (c && c->IsAlive() && c->GetEntry() >= 2000020 && c->GetEntry() <= 2000030)
            if (c->GetVictim() != bot)
                return true;
    }
    return false;
}

bool AttackArdaeusStatueAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    Unit* target = FindStatueAdd();
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

bool AttackArdaeusStatueAction::isUseful()
{
    return FindStatueAdd() != nullptr;
}

Unit* AttackArdaeusStatueAction::FindStatueAdd()
{
    for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
    {
        Creature* c = ai->GetCreature(guid);
        if (c && c->IsAlive() && c->GetEntry() >= 2000020 && c->GetEntry() <= 2000030)
            return c;
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// Boss 3: High Inquisitor Mariella (2000002) Actions
// ---------------------------------------------------------------------------

bool MoveOutOfMariellaVoidZoneAction::Execute(Event& event)
{
    for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
    {
        Creature* c = ai->GetCreature(guid);
        if (c && c->IsAlive() && (c->GetEntry() == 2000016 || c->GetEntry() == 2000018))
        {
            if (bot->GetDistance(c) < 6.0f)
            {
                float angle = bot->GetAngle(c) + M_PI;
                float destX = bot->GetPositionX() + 8.0f * cos(angle);
                float destY = bot->GetPositionY() + 8.0f * sin(angle);
                return MoveTo(bot->GetMapId(), destX, destY, bot->GetPositionZ(), false, false, false, true);
            }
        }
    }
    return false;
}

bool MoveOutOfMariellaVoidZoneAction::isPossible()
{
    return MovementAction::isPossible() && ai->CanMove();
}

bool AttackMariellaFelhoundsAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    Unit* target = FindFelhound();
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

bool AttackMariellaFelhoundsAction::isUseful()
{
    return (!ai->ContainsStrategy(STRATEGY_TYPE_HEAL) || ai->HasStrategy("offdps", BotState::BOT_STATE_COMBAT)) && (FindFelhound() != nullptr);
}

Unit* AttackMariellaFelhoundsAction::FindFelhound()
{
    Unit* best = nullptr;
    float bestDist = FLT_MAX;
    for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
    {
        Creature* c = ai->GetCreature(guid);
        if (!c || !c->IsAlive() || c->GetEntry() != 2000017)
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

// ---------------------------------------------------------------------------
// Mini-Boss: Brother Eric Vesper (2000004) Actions
// ---------------------------------------------------------------------------

bool AttackEricVesperAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    Unit* eric = GetRaidTargetByEntry(ai, 2000004);
    if (eric && eric->IsInWorld() && eric->IsAlive() && eric->GetMapId() == bot->GetMapId())
    {
        if (Attack(requester, eric))
        {
            SET_AI_VALUE(ObjectGuid, "attack target", eric->GetObjectGuid());
            return true;
        }
    }
    return false;
}

bool AttackEricVesperAction::isUseful()
{
    Unit* eric = GetRaidTargetByEntry(ai, 2000004);
    return eric && eric->IsAlive();
}

// ---------------------------------------------------------------------------
// Boss 4: High General Abbendis (2000003) Actions
// ---------------------------------------------------------------------------

bool TankAbbendisAction::Execute(Event& event)
{
    if (bot->GetDistance(421.21f, -8.98f, 37.70f) <= 3.0f)
        return false;

    return MoveTo(bot->GetMapId(), 421.21f, -8.98f, 37.70f, false, false, false, true);
}

bool TankAbbendisAction::isPossible()
{
    return MovementAction::isPossible() && ai->CanMove();
}
