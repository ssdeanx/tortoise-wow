#include "playerbot/playerbot.h"
#include "BlackwingLairDungeonActions.h"
#include "playerbot/PlayerbotAI.h"
#include "playerbot/strategy/values/PositionValue.h"
#include "playerbot/strategy/AiObjectContext.h"
#include "Maps/GridNotifiers.h"
#include "Maps/GridNotifiersImpl.h"
#include "Maps/CellImpl.h"
#include "Objects/Creature.h"
#include "Objects/GameObject.h"
#include "Objects/Player.h"
#include "Spells/SpellAuras.h"
#include "WorldPacket.h"
#include "Opcodes.h"

using namespace ai;

// ---------------------------------------------------------------------------
// Suppression Room Device Actions
// ---------------------------------------------------------------------------

bool MoveToSuppressionDeviceAction::Execute(Event& event)
{
    std::list<GuidPosition> gos = AI_VALUE(std::list<GuidPosition>, "go usable filter::go trapped filter::entry filter::{gos in sight,suppression devices}");
    if (gos.empty())
        return false;

    WorldPosition botPos(bot);
    GuidPosition closest;
    float closestDist = FLT_MAX;

    for (const GuidPosition& gp : gos)
    {
        float dist = botPos.distance(gp);
        if (dist < closestDist)
        {
            closestDist = dist;
            closest = gp;
        }
    }

    if (!closest)
        return false;

    if (ai->HasStrategy("debug move", BotState::BOT_STATE_NON_COMBAT))
    {
        ai->TellPlayerNoFacing(GetMaster(), "Moving to Suppression Device at " + std::to_string((int)closestDist) + " yards");
    }

    return MoveTo(closest.getMapId(), closest.getX(), closest.getY(), closest.getZ());
}

bool MoveToSuppressionDeviceAction::isPossible()
{
    return ai->CanMove();
}

bool MoveToSuppressionDeviceAction::isUseful()
{
    std::list<GuidPosition> gos = AI_VALUE(std::list<GuidPosition>, "go usable filter::go trapped filter::entry filter::{gos in sight,suppression devices}");
    return !gos.empty();
}

bool StealthForSuppressionDeviceAction::Execute(Event& event)
{
    if (bot->getClass() != CLASS_ROGUE)
        return false;

    if (ai->HasAura("stealth", bot))
        return false;

    if (ai->CastSpell("stealth", bot))
    {
        ai->ChangeStrategy("+stealthed", BotState::BOT_STATE_COMBAT);
        ai->ChangeStrategy("+stealthed", BotState::BOT_STATE_NON_COMBAT);
        bot->InterruptSpell(CURRENT_MELEE_SPELL);
        return true;
    }

    return false;
}

bool StealthForSuppressionDeviceAction::isPossible()
{
    return bot->getClass() == CLASS_ROGUE && !ai->HasAura("stealth", bot);
}

bool StealthForSuppressionDeviceAction::isUseful()
{
    if (ai->HasAura("stealth", bot))
        return false;

    return !ai->HasAura(23333, bot) && !ai->HasAura(23335, bot) && !ai->HasAura(34976, bot);
}

bool DeactivateSuppressionDeviceAction::Execute(Event& event)
{
    std::list<GuidPosition> gos = AI_VALUE(std::list<GuidPosition>, "entry filter::{gos close,suppression devices}");
    if (gos.empty())
        return false;

    for (const GuidPosition& guidPos : gos)
    {
        GameObject* go = ai->GetGameObject(guidPos);
        if (!go)
            continue;

        if (go->GetLootState() != GO_READY)
            continue;

        if (!bot->GetGameObjectIfCanInteractWith(go->GetObjectGuid(), GAMEOBJECT_TYPE_TRAP))
            continue;

        std::unique_ptr<WorldPacket> packet(new WorldPacket(CMSG_GAMEOBJ_USE));
        *packet << go->GetObjectGuid();
        bot->GetSession()->QueuePacket(std::move(packet));

        if (ai->HasStrategy("debug move", BotState::BOT_STATE_NON_COMBAT))
        {
            ai->TellPlayerNoFacing(GetMaster(), "Deactivating Suppression Device");
        }

        return true;
    }

    return false;
}

bool DeactivateSuppressionDeviceAction::isPossible()
{
    return ai->CanMove();
}

bool DisarmSuppressionDeviceAction::Execute(Event& event)
{
    if (bot->getClass() != CLASS_ROGUE)
        return false;

    if (!bot->HasSpell(SPELL_DISARM_TRAP))
        return false;

    std::list<GuidPosition> gos = AI_VALUE(std::list<GuidPosition>, "go usable filter::go trapped filter::entry filter::{gos close,suppression devices}");
    if (gos.empty())
        return false;

    WorldPosition botPos(bot);
    GameObject* closestGo = nullptr;
    float closestDist = FLT_MAX;

    for (const GuidPosition& guidPos : gos)
    {
        GameObject* go = ai->GetGameObject(guidPos);
        if (!go)
            continue;

        if (go->GetLootState() != GO_READY)
            continue;

        float dist = botPos.distance(WorldPosition(go));
        if (dist < closestDist)
        {
            closestDist = dist;
            closestGo = go;
        }
    }

    if (!closestGo)
        return false;

    if (ai->HasStrategy("debug move", BotState::BOT_STATE_NON_COMBAT))
    {
        ai->TellPlayerNoFacing(GetMaster(), "Casting Disarm Trap on Suppression Device");
    }

    return ai->CastSpell(SPELL_DISARM_TRAP, closestGo);
}

bool DisarmSuppressionDeviceAction::isPossible()
{
    return bot->getClass() == CLASS_ROGUE &&
           bot->HasSpell(SPELL_DISARM_TRAP) &&
           ai->CanMove();
}

bool DisarmSuppressionDeviceAction::isUseful()
{
    std::list<GuidPosition> gos = AI_VALUE(std::list<GuidPosition>, "go usable filter::go trapped filter::entry filter::{gos close,suppression devices}");
    return !gos.empty();
}

// ---------------------------------------------------------------------------
// Razorgore Encounter Actions
// ---------------------------------------------------------------------------

bool MoveToRazorgoreCornerAction::Execute(Event& event)
{
    const int corner = RazorgoreCornerIndex(ai);
    const RazorgoreCornerDef& c = RAZORGORE_CORNERS[corner];

    if (bot->GetDistance(c.x, c.y, c.z) <= 6.0f)
        return false;

    return MoveTo(bot->GetMapId(), c.x, c.y, c.z, false, false, false, true);
}

bool MoveToRazorgoreCornerAction::isPossible()
{
    return MovementAction::isPossible() && ai->CanMove();
}

bool AttackRazorgoreCornerAddAction::Execute(Event& event)
{
    Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
    Unit* target = FindCornerAdd();
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

bool AttackRazorgoreCornerAddAction::isUseful()
{
    return (!ai->ContainsStrategy(STRATEGY_TYPE_HEAL) || ai->HasStrategy("offdps", BotState::BOT_STATE_COMBAT)) && (FindCornerAdd() != nullptr);
}

Unit* AttackRazorgoreCornerAddAction::FindCornerAdd()
{
    const int corner = RazorgoreCornerIndex(ai);
    const RazorgoreCornerDef& c = RAZORGORE_CORNERS[corner];

    static const uint32 priorityEntries[] = { 12420, 12416, 12422, 12557, 14456 };

    for (uint32 entry : priorityEntries)
    {
        Unit* best = nullptr;
        float bestDist = FLT_MAX;

        for (const ObjectGuid& guid : AI_VALUE(std::list<ObjectGuid>, "possible attack targets"))
        {
            Creature* creature = ai->GetCreature(guid);
            if (!creature || !creature->IsAlive() || creature->GetEntry() != entry)
                continue;

            if (entry == 12422 && (ai->HasAura("hibernate", creature) || ai->HasAura(2637, creature) ||
                                   ai->HasAura(8952, creature) || ai->HasAura(8953, creature)))
                continue;

            const float distToCorner = creature->GetDistance(c.x, c.y, c.z);
            const float distToBot = bot->GetDistance(creature);

            if (distToCorner > 38.0f && distToBot > 22.0f)
                continue;

            if (distToBot < bestDist)
            {
                bestDist = distToBot;
                best = creature;
            }
        }

        if (best)
            return best;
    }

    return nullptr;
}

bool RazorgoreSleepDragonkinAction::Execute(Event& event)
{
    if (bot->getClass() != CLASS_DRUID)
        return false;

    Creature* target = FindUnsleptDragonkin();
    if (!target)
        return false;

    if (bot->GetShapeshiftForm() != FORM_NONE)
    {
        bot->RemoveSpellsCausingAura(SPELL_AURA_MOD_SHAPESHIFT);
    }

    return ai->CastSpell("hibernate", target);
}

bool RazorgoreSleepDragonkinAction::isPossible()
{
    return bot->getClass() == CLASS_DRUID &&
           (bot->HasSpell(2637) || bot->HasSpell(8952) || bot->HasSpell(8953));
}

bool RazorgoreSleepDragonkinAction::isUseful()
{
    return FindUnsleptDragonkin() != nullptr;
}

Creature* RazorgoreSleepDragonkinAction::FindUnsleptDragonkin()
{
    std::list<Unit*> creatures;
    MaNGOS::AllCreaturesOfEntryInRangeCheck u_check(bot, 12422, 30.0f);
    MaNGOS::UnitListSearcher<MaNGOS::AllCreaturesOfEntryInRangeCheck> searcher(creatures, u_check);
    Cell::VisitAllObjects(bot, searcher, 30.0f);

    for (Unit* unit : creatures)
    {
        Creature* c = (Creature*)unit;
        if (c && c->IsAlive() && !ai->HasAura("hibernate", c) && !ai->HasAura(2637, c) &&
            !ai->HasAura(8952, c) && !ai->HasAura(8953, c) && !c->IsPolymorphed())
        {
            return c;
        }
    }
    return nullptr;
}
