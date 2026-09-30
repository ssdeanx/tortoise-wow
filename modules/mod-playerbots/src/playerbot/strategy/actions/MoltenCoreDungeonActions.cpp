/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "playerbot/playerbot.h"
#include "MoltenCoreDungeonActions.h"

using namespace ai;

bool AddMCDouseItemAction::Execute(Event& event)
{
    if (bot->HasItemCount(17333, 1) || bot->HasItemCount(22754, 1))
        return false;

    return ChatHandler(bot).ParseCommands(".additem 17333 1") != 0;
}

bool AddMCDouseItemAction::isUseful()
{
    return bot->GetMapId() == 409 && !bot->HasItemCount(17333, 1) && !bot->HasItemCount(22754, 1);
}

Unit* AttackCoreHoundPackAction::FindHighestHpCoreHound()
{
    Unit* best = nullptr;
    float highestHpPct = -1.0f;
    for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
    {
        Creature* c = ai->GetCreature(guid);
        if (!c || !c->IsAlive() || c->GetEntry() != 11671 || c->HasFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD))
            continue;

        float hpPct = c->GetHealthPercent();
        if (hpPct > highestHpPct)
        {
            highestHpPct = hpPct;
            best = c;
        }
    }
    return best;
}

bool AttackCoreHoundPackAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    Unit* target = FindHighestHpCoreHound();
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

bool AttackCoreHoundPackAction::isUseful()
{
    return (!ai->ContainsStrategy(STRATEGY_TYPE_HEAL) || ai->HasStrategy("offdps", BotState::BOT_STATE_COMBAT)) && (FindHighestHpCoreHound() != nullptr);
}