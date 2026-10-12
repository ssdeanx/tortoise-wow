#include "scriptPCH.h"
#include "timbermaw_hold.h"

enum PerotharnSpells
{
    SPELL_CORRUPTING_TOUCH       = 28732, // Stacking 10% damage taken debuff
    SPELL_FEL_DECAY              = 28622, // Curse DoT with healing reflection
    SPELL_FLAMES_OF_PURGATION    = 36825, // Persistent fire pool
    SPELL_DRAIN_ESSENCE          = 27053, // Raid stun transition
    SPELL_STEALTH                = 1784,  // Invisibility / stealth
    SPELL_EASY_PREY              = 24375, // 8-second stun on detection
    SPELL_FEL_QUICKENING         = 28747, // +100% attack speed enrage
    SPELL_ENFEEBLED              = 18108, // Stunned + 25% damage taken
    SPELL_ENDLESS_FRENZY         = 23537, // +25% damage soft enrage at 20%
    SPELL_LEGENDARY_DAGGER_BUFF  = 23505  // Haste & damage boost for player
};

enum PerotharnPhases
{
    PHASE_CORRUPTION = 1,
    PHASE_EVASION    = 2,
    PHASE_NIGHTMARE  = 3
};

struct boss_perotharnAI : public ScriptedAI
{
    explicit boss_perotharnAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint8  m_uiPhase;
    uint32 m_uiCorruptingTouchTimer;
    uint32 m_uiFelDecayTimer;
    uint32 m_uiFlamesTimer;
    uint32 m_uiEvasionTimer;
    bool   m_bFrenzyActive;
    bool   m_bWeaponClaimed;

    void Reset() override
    {
        m_uiPhase = PHASE_CORRUPTION;
        m_uiCorruptingTouchTimer = 6000;
        m_uiFelDecayTimer        = 20000;
        m_uiFlamesTimer          = 12000;
        m_uiEvasionTimer         = 0;
        m_bFrenzyActive          = false;
        m_bWeaponClaimed         = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PEROTHARN, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("You dare awaken the Herald? The nightmare shall feast on your souls!");
        DoPlaySoundToSet(m_creature, 60664);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PEROTHARN, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("The Master... will still... awaken...");

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PEROTHARN, DONE);
    }

    void DamageDeal(Unit* pDoneTo, uint32& uiDamage) override
    {
        // Corrupting Touch passive on-hit application
        if (pDoneTo && m_uiPhase != PHASE_EVASION && urand(0, 100) < 35)
            DoCastSpellIfCan(pDoneTo, SPELL_CORRUPTING_TOUCH);
    }

    void EnterPhaseTwo()
    {
        m_uiPhase = PHASE_EVASION;
        m_uiEvasionTimer = 45000; // 45 seconds evasion phase

        m_creature->MonsterYell("Tremble in the dark! Your eyes cannot pierce the nightmare!");
        DoCastSpellIfCan(m_creature, SPELL_DRAIN_ESSENCE);

        // Spawn dropped legendary dagger on the floor
        m_creature->SummonGameObject(GO_LEGENDARY_DAGGER, m_creature->GetPositionX(), 
            m_creature->GetPositionY(), m_creature->GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 120, true);

        // Drop threat and stealth
        m_creature->AttackStop();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        DoCastSpellIfCan(m_creature, SPELL_STEALTH);

        // Spawn 3 Eye of Peroth'arn detector nodes
        for (uint8 i = 0; i < 3; ++i)
        {
            float fX, fY, fZ;
            m_creature->GetNearPoint(m_creature, fX, fY, fZ, 0.0f, 20.0f, i * 2.0f);
            m_creature->SummonCreature(NPC_EYE_OF_PEROTHARN, fX, fY, fZ, 0.0f, 
                TEMPSUMMON_TIMED_DESPAWN, 50000);
        }
    }

    void EndPhaseTwo(bool bDetected)
    {
        m_uiPhase = PHASE_NIGHTMARE;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveAurasDueToSpell(SPELL_STEALTH);

        // Check weapon disarm status
        if (m_pInstance && m_pInstance->GetData(DATA_DAGGER_CLAIMED) == 1)
            m_bWeaponClaimed = true;

        if (bDetected)
        {
            m_creature->MonsterYell("Found you! Die, worms!");
            DoCastSpellIfCan(m_creature, SPELL_FEL_QUICKENING);
        }
        else
        {
            m_creature->MonsterTextEmote("Peroth'arn falters, returning enfeebled!", nullptr, true);
            DoCastSpellIfCan(m_creature, SPELL_ENFEEBLED);
        }

        if (!m_bWeaponClaimed)
        {
            m_creature->MonsterTextEmote("Peroth'arn reclaims his deadly weapon with lethal fury!", nullptr, true);
            m_creature->SetMeleeDamageSchool(SPELL_SCHOOL_SHADOW);
        }

        m_uiCorruptingTouchTimer = 5000;
        m_uiFelDecayTimer        = 15000;
        m_uiFlamesTimer          = 10000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (m_uiPhase == PHASE_EVASION)
        {
            // Check evasion timeout (45s elapsed without detection -> Enfeebled)
            if (m_uiEvasionTimer < uiDiff)
            {
                EndPhaseTwo(false);
            }
            else
                m_uiEvasionTimer -= uiDiff;
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        // Transition to Phase 2 at 70% HP
        if (m_uiPhase == PHASE_CORRUPTION && m_creature->GetHealthPercent() <= 70.0f)
        {
            EnterPhaseTwo();
            return;
        }

        // Endless Frenzy soft enrage at < 20% HP in Phase 3
        if (m_uiPhase == PHASE_NIGHTMARE && !m_bFrenzyActive && m_creature->GetHealthPercent() < 20.0f)
        {
            m_bFrenzyActive = true;
            m_creature->MonsterYell("The nightmare collapses upon you! NONE SHALL SURVIVE!");
            DoCastSpellIfCan(m_creature, SPELL_ENDLESS_FRENZY);
        }

        // Fel Decay (targets non-tank)
        if (m_uiFelDecayTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                m_creature->MonsterTextEmote("Peroth'arn afflicts a target with Fel Decay!", nullptr, true);
                DoCastSpellIfCan(pTarget, SPELL_FEL_DECAY);
            }
            m_uiFelDecayTimer = urand(18000, 22000);
        }
        else
            m_uiFelDecayTimer -= uiDiff;

        // Flames of Purgation
        if (m_uiFlamesTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_FLAMES_OF_PURGATION);
            m_uiFlamesTimer = urand(12000, 16000);
        }
        else
            m_uiFlamesTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Eye of Peroth'arn proximity detector AI
struct npc_eye_of_perotharnAI : public ScriptedAI
{
    explicit npc_eye_of_perotharnAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        m_uiScanTimer = 500;
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiScanTimer;

    void Reset() override
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetSpeed(MOVE_WALK, 0.8f);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (m_uiScanTimer < uiDiff)
        {
            // Proximity check: < 6.0 yards
            Player* pNearPlayer = m_creature->FindNearestPlayer(6.0f);
            if (pNearPlayer && pNearPlayer->IsAlive())
            {
                pNearPlayer->CastSpell(pNearPlayer, SPELL_EASY_PREY, true);
                ChatHandler(pNearPlayer).PSendSysMessage("%s has been spotted by an Eye of Peroth'arn!", 
                    pNearPlayer->GetName());

                // Trigger Peroth'arn to break stealth with Fel Quickening
                if (Creature* pBoss = m_creature->FindNearestCreature(NPC_PEROTHARN, 150.0f))
                {
                    if (boss_perotharnAI* pAI = static_cast<boss_perotharnAI*>(pBoss->AI()))
                        pAI->EndPhaseTwo(true);
                }
                m_creature->ForcedDespawn();
                return;
            }
            m_uiScanTimer = 500;
        }
        else
            m_uiScanTimer -= uiDiff;
    }
};

// Legendary Dagger disarm interaction
bool GOHello_go_legendary_dagger(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = static_cast<ScriptedInstance*>(pGo->GetInstanceData());
    if (!pInstance)
        return false;

    pInstance->SetData(DATA_DAGGER_CLAIMED, 1);
    pPlayer->CastSpell(pPlayer, SPELL_LEGENDARY_DAGGER_BUFF, true);
    ChatHandler(pPlayer).PSendSysMessage("%s has claimed Peroth'arn's fallen dagger, disarming the Herald!",
        pPlayer->GetName());

    pGo->SetGoState(GO_STATE_ACTIVE);
    return true;
}

CreatureAI* GetAI_boss_perotharn(Creature* pCreature)
{
    return new boss_perotharnAI(pCreature);
}

CreatureAI* GetAI_npc_eye_of_perotharn(Creature* pCreature)
{
    return new npc_eye_of_perotharnAI(pCreature);
}

void AddSC_boss_perotharn()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_perotharn";
    pNewscript->GetAI = &GetAI_boss_perotharn;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "npc_eye_of_perotharn";
    pNewscript->GetAI = &GetAI_npc_eye_of_perotharn;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "go_legendary_dagger";
    pNewscript->pGOHello = &GOHello_go_legendary_dagger;
    pNewscript->RegisterSelf();
}
