#include "scriptPCH.h"
#include "upper_karazhan_halls.h"

enum
{
    SPELL_FLAME_BREATH      = 16390,
    SPELL_CLEAVE            = 15284,
    SPELL_SLOW              = 13747,
    SPELL_ARCANE_MISSILES   = 22272,
    SPELL_ARCANE_EXPLOSION  = 19712,
    SPELL_SHADOW_BOLT       = 15232,
    SPELL_DRAIN_LIFE        = 17238
};

struct mob_manascale_drakeAI : public ScriptedAI
{
    explicit mob_manascale_drakeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiBreathTimer;
    uint32 m_uiCleaveTimer;

    void Reset() override
    {
        m_uiBreathTimer = 6000;
        m_uiCleaveTimer = 4000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiCleaveTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 6000;
        }
        else
            m_uiCleaveTimer -= uiDiff;

        if (m_uiBreathTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_FLAME_BREATH);
            m_uiBreathTimer = 10000;
        }
        else
            m_uiBreathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct mob_lingering_arcanistAI : public ScriptedAI
{
    explicit mob_lingering_arcanistAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiMissilesTimer;
    uint32 m_uiSlowTimer;
    uint32 m_uiExplosionTimer;

    void Reset() override
    {
        m_uiMissilesTimer = 2000;
        m_uiSlowTimer = 8000;
        m_uiExplosionTimer = 12000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiSlowTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_SLOW);
            m_uiSlowTimer = 15000;
        }
        else
            m_uiSlowTimer -= uiDiff;

        if (m_uiExplosionTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_ARCANE_EXPLOSION);
            m_uiExplosionTimer = 14000;
        }
        else
            m_uiExplosionTimer -= uiDiff;

        if (m_uiMissilesTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_ARCANE_MISSILES);
            m_uiMissilesTimer = 5000;
        }
        else
            m_uiMissilesTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct mob_outcast_souleaterAI : public ScriptedAI
{
    explicit mob_outcast_souleaterAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShadowBoltTimer;
    uint32 m_uiDrainLifeTimer;

    void Reset() override
    {
        m_uiShadowBoltTimer = 2000;
        m_uiDrainLifeTimer = 9000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiDrainLifeTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_DRAIN_LIFE);
            m_uiDrainLifeTimer = 12000;
        }
        else
            m_uiDrainLifeTimer -= uiDiff;

        if (m_uiShadowBoltTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_SHADOW_BOLT);
            m_uiShadowBoltTimer = 4000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_manascale_drake(Creature* pCreature)
{
    return new mob_manascale_drakeAI(pCreature);
}

CreatureAI* GetAI_mob_lingering_arcanist(Creature* pCreature)
{
    return new mob_lingering_arcanistAI(pCreature);
}

CreatureAI* GetAI_mob_outcast_souleater(Creature* pCreature)
{
    return new mob_outcast_souleaterAI(pCreature);
}

void AddSC_upper_karazhan_halls_trash()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "mob_manascale_drake";
    pNewscript->GetAI = &GetAI_mob_manascale_drake;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_lingering_arcanist";
    pNewscript->GetAI = &GetAI_mob_lingering_arcanist;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_outcast_souleater";
    pNewscript->GetAI = &GetAI_mob_outcast_souleater;
    pNewscript->RegisterSelf();
}
