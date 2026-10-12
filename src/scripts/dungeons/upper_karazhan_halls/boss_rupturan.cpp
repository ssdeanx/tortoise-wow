#include "scriptPCH.h"
#include "upper_karazhan_halls.h"

enum
{
    SPELL_CHAIN_LIGHTNING = 16006,
    SPELL_EARTH_SHOCK     = 22885,
    SPELL_WAR_STOMP       = 16727,
    SPELL_ENRAGE          = 8599
};

struct boss_rupturanAI : public ScriptedAI
{
    explicit boss_rupturanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiChainLightningTimer;
    uint32 m_uiEarthShockTimer;
    uint32 m_uiWarStompTimer;
    bool m_bEnraged;

    void Reset() override
    {
        m_uiChainLightningTimer = 8000;
        m_uiEarthShockTimer = 5000;
        m_uiWarStompTimer = 15000;
        m_bEnraged = false;

        if (m_pInstance)
            m_pInstance->SetData(DATA_RUPTURAN, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("The earth shall swallow you whole!");
        if (m_pInstance)
            m_pInstance->SetData(DATA_RUPTURAN, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("The elements... reclaim me...");
        if (m_pInstance)
            m_pInstance->SetData(DATA_RUPTURAN, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (!m_bEnraged && m_creature->GetHealthPercent() < 25.0f)
        {
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            m_bEnraged = true;
            m_creature->MonsterTextEmote("Rupturan becomes enraged!", nullptr, true);
        }

        if (m_uiEarthShockTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->GetVictim())
                DoCastSpellIfCan(pTarget, SPELL_EARTH_SHOCK);
            m_uiEarthShockTimer = 8000;
        }
        else
            m_uiEarthShockTimer -= uiDiff;

        if (m_uiChainLightningTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_CHAIN_LIGHTNING);
            m_uiChainLightningTimer = 12000;
        }
        else
            m_uiChainLightningTimer -= uiDiff;

        if (m_uiWarStompTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WAR_STOMP);
            m_uiWarStompTimer = 18000;
        }
        else
            m_uiWarStompTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_rupturan(Creature* pCreature)
{
    return new boss_rupturanAI(pCreature);
}

void AddSC_boss_rupturan()
{
    Script* pNewscript = new Script;
    pNewscript->Name = "boss_rupturan";
    pNewscript->GetAI = &GetAI_boss_rupturan;
    pNewscript->RegisterSelf();
}
