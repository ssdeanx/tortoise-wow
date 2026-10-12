#include "scriptPCH.h"
#include "timbermaw_hold.h"

enum UrsolSpells
{
    SPELL_CORRUPTED_MAUL          = 23128, // Striking melee + armor debuff
    SPELL_NIGHTMARE_ROAR          = 23511, // AoE physical roar + knockback + interrupt
    SPELL_CORRUPTED_GROUND        = 24317, // Void fissure ground zone (800 nature/sec)
    SPELL_SIPHON_CHANNEL          = 27053, // Leech siphon tether
    SPELL_PRIMAL_FRENZY           = 23537  // +40% attack speed, +30% physical damage
};

struct boss_ursolAI : public ScriptedAI
{
    explicit boss_ursolAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiMaulTimer;
    uint32 m_uiRoarTimer;
    uint32 m_uiGroundTimer;
    uint32 m_uiTotemTimer;
    bool   m_bFrenzyActive;

    void Reset() override
    {
        m_uiMaulTimer   = 8000;
        m_uiRoarTimer   = 25000;
        m_uiGroundTimer = 15000;
        m_uiTotemTimer  = 40000;
        m_bFrenzyActive = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_URSOL, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("The nightmares consume you! None shall desecrate the Maw!");
        DoPlaySoundToSet(m_creature, 60663);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_URSOL, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("The corruption... finally fades...");

        if (m_pInstance)
            m_pInstance->SetData(TYPE_URSOL, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        // Primal Frenzy Soft Enrage at < 20% HP
        if (!m_bFrenzyActive && m_creature->GetHealthPercent() < 20.0f)
        {
            m_bFrenzyActive = true;
            m_creature->MonsterYell("Ursol enters a primal frenzy!");
            DoCastSpellIfCan(m_creature, SPELL_PRIMAL_FRENZY);
        }

        // Corrupted Maul (Tank swap mechanic: stacking 20% armor reduction)
        if (m_uiMaulTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_CORRUPTED_MAUL);
            m_uiMaulTimer = urand(8000, 10000);
        }
        else
            m_uiMaulTimer -= uiDiff;

        // Nightmare Roar (2.0s cast, raid-wide physical knockback + spell school lock)
        if (m_uiRoarTimer < uiDiff)
        {
            m_creature->MonsterTextEmote("Ursol prepares a deafening Nightmare Roar!", nullptr, true);
            DoCastSpellIfCan(m_creature, SPELL_NIGHTMARE_ROAR);
            m_uiRoarTimer = urand(25000, 30000);
        }
        else
            m_uiRoarTimer -= uiDiff;

        // Corrupted Ground (Void fissures under raid clusters)
        if (m_uiGroundTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCastSpellIfCan(pTarget, SPELL_CORRUPTED_GROUND);
            m_uiGroundTimer = urand(15000, 18000);
        }
        else
            m_uiGroundTimer -= uiDiff;

        // Corrupted Siphon Totem Add (~40s timer)
        if (m_uiTotemTimer < uiDiff)
        {
            float fX, fY, fZ;
            m_creature->GetNearPoint(m_creature, fX, fY, fZ, 0.0f, 18.0f, frand(0.0f, 2.0f * M_PI_F));

            if (Creature* pTotem = m_creature->SummonCreature(NPC_SIPHON_TOTEM, fX, fY, fZ, 0.0f,
                TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
            {
                pTotem->SetMaxHealth(18500);
                pTotem->SetHealth(18500);
                pTotem->CastSpell(m_creature, SPELL_SIPHON_CHANNEL, false);
                m_creature->MonsterTextEmote("A Corrupted Siphon Totem begins draining life to Ursol!", nullptr, true);
            }
            m_uiTotemTimer = 40000;
        }
        else
            m_uiTotemTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ursol(Creature* pCreature)
{
    return new boss_ursolAI(pCreature);
}

void AddSC_boss_ursol()
{
    Script* pNewscript = new Script;
    pNewscript->Name = "boss_ursol";
    pNewscript->GetAI = &GetAI_boss_ursol;
    pNewscript->RegisterSelf();
}
