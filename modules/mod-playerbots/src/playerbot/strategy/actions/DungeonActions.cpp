#include "DungeonActions.h"
#include "playerbot/playerbot.h"
#include "playerbot/strategy/values/PositionValue.h"
#include "playerbot/strategy/AiObjectContext.h"
#include "playerbot/PlayerbotAI.h"
#include "playerbot/ServerFacade.h"
#include "Group/Group.h"
#include "Maps/GridNotifiers.h"
#include "Maps/GridNotifiersImpl.h"
#include "Maps/CellImpl.h"

using namespace ai;

bool MoveAwayFromHazard::Execute(Event& event)
{
    const std::list<HazardPosition>& hazards = AI_VALUE(std::list<HazardPosition>, "hazards");

    // Get the closest hazard to move away from
    const HazardPosition* closestHazard = nullptr;
    float closestHazardDistance = 9999.0f;
    for (const HazardPosition& hazard : hazards)
    {
        const WorldPosition& hazardPosition = hazard.first;
        const float distance = bot->GetDistance(hazardPosition.getX(), hazardPosition.getY(), hazardPosition.getZ());
        if (distance < closestHazardDistance)
        {
            closestHazardDistance = distance;
            closestHazard = &hazard;
        }
    }

    if (closestHazard)
    {
        // Check if the bot is inside the closest hazard
        const float hazardRadius = closestHazard->second;
        if (closestHazardDistance <= hazardRadius)
        {
            float angle = 0.0f;
            const WorldPosition initialPosition(closestHazard->first);
            const float distance = frand(hazardRadius, hazardRadius * 1.5f);

            Unit* currentTarget = AI_VALUE(Unit*, "current target");
            if (currentTarget)
            {
                const int8 startDir = urand(0, 1) * 2 - 1;
                const WorldPosition targetPosition(currentTarget);
                angle = targetPosition.getAngleTo(initialPosition) + (0.5 * M_PI_F * startDir);
            }
            else
            {
                angle = frand(0, M_PI_F * 2.0f);
            }

            const uint8 attempts = 10;
            float angleIncrement = (float)((2 * M_PI) / attempts);

            for (uint8 i = 0; i < attempts; i++)
            {
                WorldPosition point = initialPosition + WorldPosition(0, distance * cos(angle), distance * sin(angle), 1.0f);
                point.setZ(point.getHeight());

                // Check if the point is not near other hazards
                if (!IsHazardNearby(point, hazards))
                {
                    if (bot->IsWithinLOS(point.getX(), point.getY(), point.getZ() + bot->GetCollisionHeight()) && initialPosition.canPathTo(point, bot))
                    {
                        if (ai->HasStrategy("debug move", BotState::BOT_STATE_COMBAT))
                        {
                            bot->SummonCreature(15631, point.getX(), point.getY(), point.getZ(), 0.0f, TEMPSPAWN_TIMED_DESPAWN, 5000.0f);
                        }

                        if (MoveTo(bot->GetMapId(), point.getX(), point.getY(), point.getZ(), false, IsReaction(), false, true))
                        {
                            if (IsReaction())
                            {
                                WaitForReach(point.distance(initialPosition));
                            }

                            return true;
                        }
                    }
                }

                if (ai->HasStrategy("debug move", BotState::BOT_STATE_COMBAT))
                {
                    bot->SummonCreature(1, point.getX(), point.getY(), point.getZ(), 0.0f, TEMPSPAWN_TIMED_DESPAWN, 5000.0f);
                }

                angle += angleIncrement;
            }
        }
    }

    return false;
}

bool MoveAwayFromHazard::isPossible()
{
    if (MovementAction::isPossible())
    {
        return ai->CanMove();
    }

    return false;
}

bool MoveAwayFromHazard::IsHazardNearby(const WorldPosition& point, const std::list<HazardPosition>& hazards) const
{
    for (const HazardPosition& hazard : hazards)
    {
        const float hazardRange = hazard.second;
        const WorldPosition& hazardPosition = hazard.first;
        const float distance = point.distance(hazardPosition);
        if (distance < hazardRange)
        {
            return true;
        }
    }

    return false;
}

bool MoveAwayFromCreature::Execute(Event& event)
{
    // Get the active attacking creatures
    std::list<Creature*> creatures;
    size_t closestCreatureIdx = 0;
    float closestCreatureDistance = 9999.0f;

    // Iterate through the near creatures
    std::list<Unit*> units;
    MaNGOS::AllCreaturesOfEntryInRangeCheck u_check(bot, creatureID, range);
    MaNGOS::UnitListSearcher<MaNGOS::AllCreaturesOfEntryInRangeCheck> searcher(units, u_check);
    Cell::VisitAllObjects(bot, searcher, range);
    for (Unit* unit : units)
    {
        Creature* creature = (Creature*)unit;
        if (creature)
        {
            creatures.push_back(creature);

            // Get the closest creature to the bot
            const float distance = bot->GetDistance(creature);
            if (distance < closestCreatureDistance)
            {
                closestCreatureDistance = distance;
                closestCreatureIdx = creatures.size() - 1;
            }
        }
    }

    if (creatures.empty())
    {
        return false;
    }

    const std::list<HazardPosition>& hazards = AI_VALUE(std::list<HazardPosition>, "hazards");

    // Get the closest creature reference
    auto it = creatures.begin();
    advance(it, closestCreatureIdx);
    Creature* closestCreature = *it;
    // Remove the closest creature from the list to prevent checking it twice
    creatures.erase(it);

    // Generate the initial angle directly behind the bot looking at the closest creature
    const WorldPosition botPosition(bot);
    const WorldPosition creaturePosition(closestCreature);
    float angleLeft = creaturePosition.getAngleTo(botPosition);
    float angleRight = angleLeft;

    const uint8 attempts = 20;
    const uint8 halfAtempts = (uint8)(attempts * 0.5f);
    float angleIncrement = (float)((M_PI) / halfAtempts);

    const float sizeFactor = bot->GetCombatReach() + closestCreature->GetCombatReach();
    const float distance = (range + sizeFactor);

    for (uint8 i = 0; i < halfAtempts; i++)
    {
        WorldPosition* validPoint = nullptr;

        // Calculate a point to the left and right
        WorldPosition pointLeft = creaturePosition + WorldPosition(0, distance * cos(angleLeft), distance * sin(angleLeft), 1.0f);
        pointLeft.setZ(pointLeft.getHeight());
        WorldPosition pointRight = creaturePosition + WorldPosition(0, distance * cos(angleRight), distance * sin(angleRight), 1.0f);
        pointRight.setZ(pointRight.getHeight());

        if (IsValidPoint(pointLeft, creatures, hazards))
        {
            validPoint = &pointLeft;
        }
        else if (IsValidPoint(pointRight, creatures, hazards))
        {
            validPoint = &pointRight;
        }

        if (validPoint)
        {
            if (ai->HasStrategy("debug move", BotState::BOT_STATE_COMBAT))
            {
                bot->SummonCreature(15631, validPoint->getX(), validPoint->getY(), validPoint->getZ(), 0.0f, TEMPSPAWN_TIMED_DESPAWN, 5000.0f);
            }

            if (MoveTo(bot->GetMapId(), validPoint->getX(), validPoint->getY(), validPoint->getZ(), false, IsReaction(), false, true))
            {
                if (IsReaction())
                {
                    WaitForReach(validPoint->distance(botPosition));
                }

                return true;
            }
        }

        if (ai->HasStrategy("debug move", BotState::BOT_STATE_COMBAT))
        {
            bot->SummonCreature(1, pointLeft.getX(), pointLeft.getY(), pointLeft.getZ(), 0.0f, TEMPSPAWN_TIMED_DESPAWN, 5000.0f);
            bot->SummonCreature(1, pointRight.getX(), pointRight.getY(), pointRight.getZ(), 0.0f, TEMPSPAWN_TIMED_DESPAWN, 5000.0f);
        }

        angleLeft += angleIncrement;
        angleRight -= angleIncrement;
    }

    return false;
}

bool MoveAwayFromSpecificCreatures::Execute(Event& event)
{
    std::set<uint32>&creatureIDList = AI_VALUE(std::set<uint32>&, "avoid creature list");
    for (const uint32 creatureToCheck : creatureIDList)
    {
        creatureID = creatureToCheck;
        bool result = MoveAwayFromCreature::Execute(event);
        if (result)
            return result;
    }
    return false;
}

bool MoveAwayFromCreature::isPossible()
{
    if (MovementAction::isPossible())
    {
        return ai->CanMove();
    }

    return false;
}

bool MoveAwayFromCreature::IsValidPoint(const WorldPosition& point, const std::list<Creature*>& creatures, const std::list<HazardPosition>& hazards)
{
    // Check if the point is not near other game objects
    if (!HasCreaturesNearby(point, creatures) && !IsHazardNearby(point, hazards))
    {
        if (bot->IsWithinLOS(point.getX(), point.getY(), point.getZ() + bot->GetCollisionHeight()))
        {
            const WorldPosition botPosition(bot);
            return botPosition.canPathTo(point, bot);
        }
    }

    return false;
}

bool MoveAwayFromCreature::HasCreaturesNearby(const WorldPosition& point, const std::list<Creature*>& creatures) const
{
    for (const Creature* creature : creatures)
    {
        const float distance = creature->GetDistance(point.getX(), point.getY(), point.getZ());
        if (distance <= range)
        {
            return true;
        }
    }

    return false;
}

bool MoveAwayFromCreature::IsHazardNearby(const WorldPosition& point, const std::list<HazardPosition>& hazards) const
{
    for (const HazardPosition& hazard : hazards)
    {
        const float hazardRange = hazard.second;
        const WorldPosition& hazardPosition = hazard.first;
        const float distance = point.distance(hazardPosition);
        if (distance < hazardRange)
        {
            return true;
        }
    }

    return false;
}

bool MoveAwayFromRaidAction::Execute(Event& event)
{
    float minDistance = 20.0f;
    if (!getQualifier().empty())
    {
        try { minDistance = stof(getQualifier()); }
        catch (...) { minDistance = 20.0f; }
    }

    if (minDistance < 5.0f)
        minDistance = 5.0f;

    WorldPosition botPos(bot);

    // Collect raid member positions: group members + master.
    std::list<WorldPosition> raidPositions;
    Player* master = GetMaster();
    if (master && master != bot && master->IsInWorld() &&
        master->GetMapId() == bot->GetMapId() && sServerFacade.IsAlive(master))
    {
        raidPositions.push_back(WorldPosition(master));
    }

    Group* group = bot->GetGroup();
    if (group && !bot->InBattleGround())
    {
        Group::MemberSlotList const& groupSlot = group->GetMemberSlots();
        for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
        {
            Player* member = sObjectMgr.GetPlayer(itr->guid);
            if (!member || member == bot || !sServerFacade.IsAlive(member) ||
                !member->IsInWorld() || member->IsBeingTeleported() ||
                member->GetMapId() != bot->GetMapId())
                continue;

            raidPositions.push_back(WorldPosition(member));
        }
    }

    // Already clear of the raid - nothing to do.
    bool alreadyClear = true;
    for (const WorldPosition& memberPos : raidPositions)
    {
        if (botPos.distance(memberPos) < minDistance)
        {
            alreadyClear = false;
            break;
        }
    }

    if (alreadyClear)
        return false;

    const std::list<HazardPosition>& hazards = AI_VALUE(std::list<HazardPosition>, "hazards");

    // Sample 16 directions and pick the pathable point that ends up farthest
    // from the whole raid. Mirrors MoveAwayFromHazard's escape search.
    WorldPosition bestPoint;
    float bestScore = -1.0f;

    const uint8 attempts = 16;
    const float distance = minDistance;
    for (uint8 i = 0; i < attempts; i++)
    {
        const float angle = static_cast<float>(i) * ((2.0f * M_PI_F) / attempts);
        WorldPosition point = botPos + WorldPosition(0, distance * cos(angle), distance * sin(angle), 1.0f);
        point.setZ(point.getHeight());

        if (IsRaidNearby(point, raidPositions, minDistance))
            continue;

        // Reject points inside a hazard unless the bot is standing in one already.
        bool inHazard = false;
        for (const HazardPosition& hazard : hazards)
        {
            if (point.distance(hazard.first) < hazard.second)
            {
                inHazard = true;
                break;
            }
        }
        if (inHazard)
            continue;

        if (!bot->IsWithinLOS(point.getX(), point.getY(), point.getZ() + bot->GetCollisionHeight()))
            continue;

        if (!botPos.canPathTo(point, bot))
            continue;

        float minDistToRaid = FLT_MAX;
        for (const WorldPosition& memberPos : raidPositions)
            minDistToRaid = std::min(minDistToRaid, point.distance(memberPos));

        if (minDistToRaid > bestScore)
        {
            bestScore = minDistToRaid;
            bestPoint = point;
        }
    }

    if (bestScore < minDistance)
        return false;

    if (MoveTo(bot->GetMapId(), bestPoint.getX(), bestPoint.getY(), bestPoint.getZ(), false, IsReaction(), false, true))
    {
        if (IsReaction())
            WaitForReach(bestPoint.distance(botPos));

        return true;
    }

    return false;
}

bool MoveAwayFromRaidAction::isPossible()
{
    if (MovementAction::isPossible())
    {
        return ai->CanMove();
    }

    return false;
}

bool MoveAwayFromRaidAction::IsRaidNearby(const WorldPosition& point, const std::list<WorldPosition>& raidPositions, float minDistance) const
{
    for (const WorldPosition& memberPos : raidPositions)
    {
        if (point.distance(memberPos) < minDistance)
            return true;
    }

    return false;
}

Unit* AttackNearestEntryAction::FindTarget()
{
    uint32 entryId = 0;
    std::vector<std::string> params = getMultiQualifiers(getQualifier(), ",");
    if (params.empty())
        return nullptr;

    float maxRange = 60.0f;
    try
    {
        entryId = std::stoul(params[0]);
        if (params.size() > 1)
            maxRange = stof(params[1]);
    }
    catch (...) { return nullptr; }

    if (!entryId || maxRange <= 0.0f)
        return nullptr;

    Unit* best = nullptr;
    float bestDist = FLT_MAX;
    for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
    {
        Creature* creature = ai->GetCreature(guid);
        if (!creature || !sServerFacade.IsAlive(creature) || creature->GetEntry() != entryId)
            continue;

        if (!bot->IsWithinDistInMap(creature, maxRange, false))
            continue;

        const float dist = sServerFacade.GetDistance2d(bot, creature);
        if (dist < bestDist)
        {
            bestDist = dist;
            best = creature;
        }
    }

    return best;
}

bool AttackNearestEntryAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();

    Unit* target = FindTarget();
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

bool AttackNearestEntryAction::isUseful()
{
    return (!ai->ContainsStrategy(STRATEGY_TYPE_HEAL) || ai->HasStrategy("offdps", BotState::BOT_STATE_COMBAT)) && FindTarget();
}

bool OfftankNearestEntryAction::isUseful()
{
    return ai->IsTank(bot) && AttackNearestEntryAction::isUseful();
}

bool HideFromCreatureAction::Execute(Event& event)
{
    // The creature we need to break LoS from.
    std::list<Unit*> creatures;
    float closestDistance = FLT_MAX;
    Unit* closestCreature = nullptr;

    WorldPosition botPos(bot);
    MaNGOS::AllCreaturesOfEntryInRangeCheck u_check(bot, creatureID, 150.0f);
    MaNGOS::UnitListSearcher<MaNGOS::AllCreaturesOfEntryInRangeCheck> searcher(creatures, u_check);
    Cell::VisitAllObjects(bot, searcher, 150.0f);
    for (Unit* unit : creatures)
    {
        Creature* creature = (Creature*)unit;
        if (!creature || !sServerFacade.IsAlive(creature))
            continue;

        const float distance = botPos.distance(WorldPosition(creature));
        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestCreature = creature;
        }
    }

    if (!closestCreature)
        return false;

    const std::list<HazardPosition>& hazards = AI_VALUE(std::list<HazardPosition>, "hazards");

    // Search outward in 8 directions for the closest pathable point the creature
    // cannot see. Grows the ring until a hide spot is found.
    WorldPosition creaturePos(closestCreature);
    for (uint8 ring = 0; ring < 3; ring++)
    {
        const float distance = 10.0f + ring * 10.0f;
        const uint8 attempts = 12;
        for (uint8 i = 0; i < attempts; i++)
        {
            const float angle = static_cast<float>(i) * ((2.0f * M_PI_F) / attempts);
            WorldPosition point = creaturePos + WorldPosition(0, distance * cos(angle), distance * sin(angle), 1.0f);
            point.setZ(point.getHeight());

            // Already hidden from the creature at this point?
            if (closestCreature->IsWithinLOS(point.getX(), point.getY(), point.getZ() + bot->GetCollisionHeight(), true))
                continue;

            bool inHazard = false;
            for (const HazardPosition& hazard : hazards)
            {
                if (point.distance(hazard.first) < hazard.second)
                {
                    inHazard = true;
                    break;
                }
            }
            if (inHazard)
                continue;

            if (!botPos.canPathTo(point, bot))
                continue;

            if (MoveTo(bot->GetMapId(), point.getX(), point.getY(), point.getZ(), false, IsReaction(), false, true))
            {
                if (IsReaction())
                    WaitForReach(point.distance(botPos));

                return true;
            }
        }
    }

    return false;
}

bool HideFromCreatureAction::isPossible()
{
    if (MovementAction::isPossible())
    {
        return ai->CanMove();
    }

    return false;
}

bool HideFromCreatureAction::isUseful()
{
    // Useful while the bot is still visible to the creature: once hidden, the
    // trigger stays active but the movement is complete.
    std::list<Unit*> creatures;
    MaNGOS::AllCreaturesOfEntryInRangeCheck u_check(bot, creatureID, 150.0f);
    MaNGOS::UnitListSearcher<MaNGOS::AllCreaturesOfEntryInRangeCheck> searcher(creatures, u_check);
    Cell::VisitAllObjects(bot, searcher, 150.0f);
    for (Unit* unit : creatures)
    {
        Creature* creature = (Creature*)unit;
        if (creature && sServerFacade.IsAlive(creature) && !creature->IsWithinLOSInMap(bot, true))
            return false; // already hidden

        if (creature && sServerFacade.IsAlive(creature))
            return true;
    }

    return false;
}

bool ai::IsGroupResistDropper(PlayerbotAI* ai, Player* player)
{
    if (!player || !sServerFacade.IsAlive(player))
        return false;

    // Resist-capable classes: shaman (Horde, totem) and paladin (Alliance, aura).
    const uint32 resistClass = player->getClass();
    if (resistClass != CLASS_SHAMAN && resistClass != CLASS_PALADIN)
        return false;

    Group* group = player->GetGroup();
    if (!group)
        return false;

    // This bot's subgroup (vanilla raid subgroups 0-7).
    uint8 subgroup = 0;
    bool inGroupList = false;
    Group::MemberSlotList const& slots = group->GetMemberSlots();
    for (Group::member_citerator itr = slots.begin(); itr != slots.end(); itr++)
    {
        if (itr->guid == player->GetObjectGuid())
        {
            subgroup = itr->group;
            inGroupList = true;
            break;
        }
    }

    if (!inGroupList)
        return false;

    // Lowest-GUID alive bot of the same class inside the same subgroup does it.
    for (Group::member_citerator itr = slots.begin(); itr != slots.end(); itr++)
    {
        if (itr->group != subgroup || itr->guid == player->GetObjectGuid())
            continue;

        Player* member = sObjectMgr.GetPlayer(itr->guid);
        if (!member || member->getClass() != resistClass || !sServerFacade.IsAlive(member) ||
            !member->IsInWorld() || member->GetMapId() != player->GetMapId())
            continue;

        if (itr->guid < player->GetObjectGuid())
            return false;
    }

    return true;
}

bool TauntCreatureAction::Execute(Event& event)
{
    std::list<Unit*> creatures;
    MaNGOS::AllCreaturesOfEntryInRangeCheck u_check(bot, creatureID, 150.0f);
    MaNGOS::UnitListSearcher<MaNGOS::AllCreaturesOfEntryInRangeCheck> searcher(creatures, u_check);
    Cell::VisitAllObjects(bot, searcher, 150.0f);

    Unit* target = nullptr;
    float bestDistance = FLT_MAX;
    for (Unit* unit : creatures)
    {
        Creature* creature = (Creature*)unit;
        if (!creature || !sServerFacade.IsAlive(creature))
            continue;

        const float distance = sServerFacade.GetDistance2d(bot, creature);
        if (distance < bestDistance)
        {
            bestDistance = distance;
            target = creature;
        }
    }

    if (!target)
        return false;

    // Warrior: taunt 355. Druid: growl 6795. Vanilla 1.12 has no paladin taunt.
    if (bot->getClass() == CLASS_WARRIOR)
        return ai->CastSpell("taunt", target);

    if (bot->getClass() == CLASS_DRUID)
        return ai->CastSpell("growl", target);

    return false;
}

bool TauntCreatureAction::isUseful()
{
    if (!ai->IsTank(bot))
        return false;

    // The active tank (the creature's current victim) must not re-taunt.
    std::list<Unit*> creatures;
    MaNGOS::AllCreaturesOfEntryInRangeCheck u_check(bot, creatureID, 150.0f);
    MaNGOS::UnitListSearcher<MaNGOS::AllCreaturesOfEntryInRangeCheck> searcher(creatures, u_check);
    Cell::VisitAllObjects(bot, searcher, 150.0f);
    for (Unit* unit : creatures)
    {
        Creature* creature = (Creature*)unit;
        if (creature && sServerFacade.IsAlive(creature) && creature->GetVictim() == bot)
            return false;
    }

    return !creatures.empty();
}

Player* FollowChargedAllyAction::FindChargedAlly(uint32 chargeSpellId)
{
    Player* best = nullptr;
    float bestDistance = FLT_MAX;

    Player* master = GetMaster();
    std::list<Player*> candidates;
    if (master && master != bot)
        candidates.push_back(master);

    Group* group = bot->GetGroup();
    if (group)
    {
        Group::MemberSlotList const& slots = group->GetMemberSlots();
        for (Group::member_citerator itr = slots.begin(); itr != slots.end(); itr++)
        {
            Player* member = sObjectMgr.GetPlayer(itr->guid);
            if (!member || member == bot || !sServerFacade.IsAlive(member) ||
                !member->IsInWorld() || member->IsBeingTeleported() ||
                member->GetMapId() != bot->GetMapId())
                continue;

            candidates.push_back(member);
        }
    }

    for (Player* candidate : candidates)
    {
        if (!candidate->HasAura(chargeSpellId))
            continue;

        const float distance = sServerFacade.GetDistance2d(bot, candidate);
        if (distance < bestDistance)
        {
            bestDistance = distance;
            best = candidate;
        }
    }

    return best;
}

bool FollowChargedAllyAction::Execute(Event& event)
{
    uint32 chargeSpellId = 0;
    if (bot->HasAura(positiveSpellId))
        chargeSpellId = positiveSpellId;
    else if (bot->HasAura(negativeSpellId))
        chargeSpellId = negativeSpellId;

    if (!chargeSpellId)
        return false;

    Player* ally = FindChargedAlly(chargeSpellId);
    if (!ally)
        return false;

    if (sServerFacade.GetDistance2d(bot, ally) <= 6.0f)
        return false;

    return MoveNear(ally, 4.0f);
}

bool FollowChargedAllyAction::isPossible()
{
    if (MovementAction::isPossible())
    {
        return ai->CanMove();
    }

    return false;
}

bool FollowChargedAllyAction::isUseful()
{
    uint32 chargeSpellId = 0;
    if (bot->HasAura(positiveSpellId))
        chargeSpellId = positiveSpellId;
    else if (bot->HasAura(negativeSpellId))
        chargeSpellId = negativeSpellId;

    if (!chargeSpellId)
        return false;

    Player* ally = FindChargedAlly(chargeSpellId);
    return ally && sServerFacade.GetDistance2d(bot, ally) > 6.0f;
}
