#include "scriptPCH.h"
#include "frostmane_hollow.h"

namespace
{
bool IsHailarRitualist(Creature const* creature)
{
    if (creature->GetMapId() != MAP_FROSTMANE_HOLLOW)
        return false;

    switch (creature->GetGUIDLow())
    {
        case GUID_HAILAR_RITUALIST_1:
        case GUID_HAILAR_RITUALIST_2:
        case GUID_HAILAR_RITUALIST_3:
        case GUID_HAILAR_RITUALIST_4:
        case GUID_HAILAR_RITUALIST_5:
            return true;
        default:
            return false;
    }
}

struct boss_hailar_the_frigidAI : public ScriptedAI
{
    explicit boss_hailar_the_frigidAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    void Reset() override
    {
        m_usedHalfHealthLine = false;
    }

    void Aggro(Unit* /*who*/) override
    {
        DoScriptText(SAY_HAILAR_AGGRO, m_creature);
    }

    void JustDied(Unit* /*killer*/) override
    {
        DoScriptText(SAY_HAILAR_DEATH, m_creature);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_usedHalfHealthLine && m_creature->IsInCombat() && m_creature->GetHealthPercent() <= 50.0f)
        {
            m_usedHalfHealthLine = true;
            DoScriptText(SAY_HAILAR_HALF_HEALTH, m_creature);
        }

        ScriptedAI::UpdateAI(diff);
    }

private:
    bool m_usedHalfHealthLine;
};

struct npc_frostmane_ritualistAI : public ScriptedAI
{
    explicit npc_frostmane_ritualistAI(Creature* creature) :
        ScriptedAI(creature),
        m_isHailarRitualist(IsHailarRitualist(creature))
    {
        Reset();
    }

    void Reset() override
    {
        if (!m_isHailarRitualist)
            return;

        m_creature->SetReactState(REACT_PASSIVE);
        SetCombatMovement(false);
        SetMeleeAttack(false);
        m_channelRetryTimer = 500;
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (!m_isHailarRitualist)
            ScriptedAI::MoveInLineOfSight(who);
    }

    void AttackedBy(Unit* attacker) override
    {
        if (!m_isHailarRitualist)
            ScriptedAI::AttackedBy(attacker);
    }

    void AttackStart(Unit* who) override
    {
        if (!m_isHailarRitualist)
            ScriptedAI::AttackStart(who);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_isHailarRitualist)
        {
            ScriptedAI::UpdateAI(diff);
            return;
        }

        if (m_creature->IsNonMeleeSpellCasted(false))
            return;

        if (m_channelRetryTimer > diff)
        {
            m_channelRetryTimer -= diff;
            return;
        }

        if (Creature* hailar = m_creature->FindNearestCreature(NPC_HAILAR_THE_FRIGID, 100.0f, true))
        {
            if (DoCastSpellIfCan(hailar, SPELL_FROSTMANE_RITUAL, CF_FORCE_CAST) == CAST_OK)
            {
                m_channelRetryTimer = 5 * IN_MILLISECONDS;
                return;
            }
        }

        m_channelRetryTimer = 1 * IN_MILLISECONDS;
    }

private:
    bool const m_isHailarRitualist;
    uint32 m_channelRetryTimer;
};

CreatureAI* GetAI_boss_hailar_the_frigid(Creature* creature)
{
    return new boss_hailar_the_frigidAI(creature);
}

CreatureAI* GetAI_npc_frostmane_ritualist(Creature* creature)
{
    return new npc_frostmane_ritualistAI(creature);
}

struct boss_battlemaster_ubukazAI : public ScriptedAI
{
    explicit boss_battlemaster_ubukazAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    uint32 m_uiCleaveTimer;
    uint32 m_uiMortalStrikeTimer;

    void Reset() override
    {
        m_uiCleaveTimer = 4000;
        m_uiMortalStrikeTimer = 7000;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiCleaveTimer <= diff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 6000;
        }
        else
            m_uiCleaveTimer -= diff;

        if (m_uiMortalStrikeTimer <= diff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_MORTAL_STRIKE);
            m_uiMortalStrikeTimer = 9000;
        }
        else
            m_uiMortalStrikeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct boss_handler_obokaAI : public ScriptedAI
{
    explicit boss_handler_obokaAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    uint32 m_uiShootTimer;
    bool m_bSummonedLeopard;

    void Reset() override
    {
        m_uiShootTimer = 2500;
        m_bSummonedLeopard = false;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (!m_bSummonedLeopard && m_creature->GetHealthPercent() <= 50.0f)
        {
            m_bSummonedLeopard = true;
            m_creature->SummonCreature(NPC_FROSTMANE_LEOPARD, m_creature->GetPositionX() + 2.0f, m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000);
        }

        DoMeleeAttackIfReady();
    }
};

struct boss_tansha_the_sleekAI : public ScriptedAI
{
    explicit boss_tansha_the_sleekAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    uint32 m_uiSinisterStrikeTimer;
    uint32 m_uiGougeTimer;

    void Reset() override
    {
        m_uiSinisterStrikeTimer = 3000;
        m_uiGougeTimer = 8000;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiGougeTimer <= diff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_GOUGE);
            m_uiGougeTimer = 10000;
        }
        else
            m_uiGougeTimer -= diff;

        if (m_uiSinisterStrikeTimer <= diff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SINISTER_STRIKE);
            m_uiSinisterStrikeTimer = 4000;
        }
        else
            m_uiSinisterStrikeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct boss_kanza_the_seerAI : public ScriptedAI
{
    explicit boss_kanza_the_seerAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    uint32 m_uiChainLightningTimer;
    uint32 m_uiHealingWaveTimer;

    void Reset() override
    {
        m_uiChainLightningTimer = 3000;
        m_uiHealingWaveTimer = 10000;
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiHealingWaveTimer <= diff)
        {
            if (m_creature->GetHealthPercent() <= 40.0f)
            {
                DoCastSpellIfCan(m_creature, SPELL_HEALING_WAVE);
                m_uiHealingWaveTimer = 15000;
            }
        }
        else
            m_uiHealingWaveTimer -= diff;

        if (m_uiChainLightningTimer <= diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, SPELL_CHAIN_LIGHTNING);
            m_uiChainLightningTimer = 8000;
        }
        else
            m_uiChainLightningTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_battlemaster_ubukaz(Creature* creature)
{
    return new boss_battlemaster_ubukazAI(creature);
}

CreatureAI* GetAI_boss_handler_oboka(Creature* creature)
{
    return new boss_handler_obokaAI(creature);
}

CreatureAI* GetAI_boss_tansha_the_sleek(Creature* creature)
{
    return new boss_tansha_the_sleekAI(creature);
}

CreatureAI* GetAI_boss_kanza_the_seer(Creature* creature)
{
    return new boss_kanza_the_seerAI(creature);
}
}

void AddSC_boss_hailar_the_frigid()
{
    Script* script;

    script = new Script;
    script->Name = "boss_hailar_the_frigid";
    script->GetAI = &GetAI_boss_hailar_the_frigid;
    script->RegisterSelf();

    script = new Script;
    script->Name = "npc_frostmane_ritualist";
    script->GetAI = &GetAI_npc_frostmane_ritualist;
    script->RegisterSelf();

    script = new Script;
    script->Name = "boss_battlemaster_ubukaz";
    script->GetAI = &GetAI_boss_battlemaster_ubukaz;
    script->RegisterSelf();

    script = new Script;
    script->Name = "boss_handler_oboka";
    script->GetAI = &GetAI_boss_handler_oboka;
    script->RegisterSelf();

    script = new Script;
    script->Name = "boss_tansha_the_sleek";
    script->GetAI = &GetAI_boss_tansha_the_sleek;
    script->RegisterSelf();

    script = new Script;
    script->Name = "boss_kanza_the_seer";
    script->GetAI = &GetAI_boss_kanza_the_seer;
    script->RegisterSelf();
}
