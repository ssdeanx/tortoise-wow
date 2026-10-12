#include "scriptPCH.h"
#include "timbermaw_hold.h"

// -----------------------------------------------------------------------------
// 1. Karrsh the Sentinel (62934) - Front Gatekeeper
// -----------------------------------------------------------------------------
enum KarrshSpells
{
    SPELL_SENTINEL_STRIKE   = 23128,
    SPELL_INTIMIDATING_ROAR = 23511,
    SPELL_FURBOLG_FRENZY    = 8269
};

struct boss_karrshAI : public ScriptedAI
{
    explicit boss_karrshAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiStrikeTimer;
    uint32 m_uiRoarTimer;

    void Reset() override
    {
        m_uiStrikeTimer = 6000;
        m_uiRoarTimer   = 15000;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KARRSH, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("Intruders at the gate! Sound the horns!");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KARRSH, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("The hold... is breached...");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KARRSH, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiStrikeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SENTINEL_STRIKE);
            m_uiStrikeTimer = urand(6000, 9000);
        }
        else
            m_uiStrikeTimer -= uiDiff;

        if (m_uiRoarTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_INTIMIDATING_ROAR);
            m_uiRoarTimer = urand(15000, 20000);
        }
        else
            m_uiRoarTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 2. Rotgrowl (62936) & Kodiak (62937) - Paired Dire Bear Encounter
// -----------------------------------------------------------------------------
enum BearSpells
{
    SPELL_FEROCIOUS_BITE  = 22568,
    SPELL_MAUL            = 23128,
    SPELL_ROTTING_BITE    = 28622,
    SPELL_SWIPE           = 22543
};

struct boss_rotgrowlAI : public ScriptedAI
{
    explicit boss_rotgrowlAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiBiteTimer;
    uint32 m_uiSwipeTimer;

    void Reset() override
    {
        m_uiBiteTimer  = 5000;
        m_uiSwipeTimer = 8000;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BEAR_DUO, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterTextEmote("Rotgrowl snarls furiously, calling Kodiak to his side!", nullptr, true);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BEAR_DUO, IN_PROGRESS);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiBiteTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_ROTTING_BITE);
            m_uiBiteTimer = urand(7000, 10000);
        }
        else
            m_uiBiteTimer -= uiDiff;

        if (m_uiSwipeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SWIPE);
            m_uiSwipeTimer = urand(8000, 12000);
        }
        else
            m_uiSwipeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct boss_kodiakAI : public ScriptedAI
{
    explicit boss_kodiakAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiMaulTimer;

    void Reset() override
    {
        m_uiMaulTimer = 6000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiMaulTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_MAUL);
            m_uiMaulTimer = urand(5000, 8000);
        }
        else
            m_uiMaulTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 3. Ormanos the Cracked (62935) - Giant Furbolg Earthbreaker
// -----------------------------------------------------------------------------
enum OrmanosSpells
{
    SPELL_TREMORS_OF_ORMANOS = 49027,
    SPELL_SPAWN_TREMOR_ADD   = 49028,
    SPELL_RAMPAGING_EARTH    = 49029,
    SPELL_RAMPAGE_CHARGE     = 49030
};

struct boss_ormanosAI : public ScriptedAI
{
    explicit boss_ormanosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiTremorTimer;
    uint32 m_uiChargeTimer;

    void Reset() override
    {
        m_uiTremorTimer = 10000;
        m_uiChargeTimer = 22000;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ORMANOS, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("The earth shatters! You crumble to dust!");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ORMANOS, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("The stones... split asunder...");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ORMANOS, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiTremorTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, SPELL_TREMORS_OF_ORMANOS);
                m_creature->SummonCreature(NPC_SON_OF_ORMANOS, pTarget->GetPositionX(), 
                    pTarget->GetPositionY(), pTarget->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 45000);
            }
            m_uiTremorTimer = urand(14000, 18000);
        }
        else
            m_uiTremorTimer -= uiDiff;

        if (m_uiChargeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                m_creature->MonsterTextEmote("Ormanos prepares a Rampaging Earth charge!", nullptr, true);
                DoCastSpellIfCan(pTarget, SPELL_RAMPAGE_CHARGE);
            }
            m_uiChargeTimer = urand(20000, 25000);
        }
        else
            m_uiChargeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 4. Archdruid Kronn (62938) - Corrupted Ancient Druid
// -----------------------------------------------------------------------------
enum KronnSpells
{
    SPELL_ENTANGLING_ROOTS = 22415,
    SPELL_MOONFIRE         = 20690,
    SPELL_INSECT_SWARM     = 24968,
    SPELL_STARFIRE         = 20692
};

struct boss_archdruid_kronnAI : public ScriptedAI
{
    explicit boss_archdruid_kronnAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiRootsTimer;
    uint32 m_uiMoonfireTimer;
    uint32 m_uiSwarmTimer;

    void Reset() override
    {
        m_uiRootsTimer    = 8000;
        m_uiMoonfireTimer = 5000;
        m_uiSwarmTimer    = 12000;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRONN, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("Nature will tear the flesh from your bones!");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRONN, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("Ursol... forgive me...");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRONN, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiMoonfireTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_MOONFIRE);
            m_uiMoonfireTimer = urand(5000, 7000);
        }
        else
            m_uiMoonfireTimer -= uiDiff;

        if (m_uiRootsTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCastSpellIfCan(pTarget, SPELL_ENTANGLING_ROOTS);
            m_uiRootsTimer = urand(10000, 14000);
        }
        else
            m_uiRootsTimer -= uiDiff;

        if (m_uiSwarmTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_INSECT_SWARM);
            m_uiSwarmTimer = urand(12000, 16000);
        }
        else
            m_uiSwarmTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 5. Loktanag the Vile (2139) - Deep Cave Shadow Warlock
// -----------------------------------------------------------------------------
enum LoktanagSpells
{
    SPELL_SHADOW_BOLT_VOLLEY = 52806,
    SPELL_CORRUPT_SIPHON     = 52813,
    SPELL_CURSE_OF_AGONY     = 52812,
    SPELL_DRAIN_SOUL         = 52808
};

struct boss_loktanagAI : public ScriptedAI
{
    explicit boss_loktanagAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiVolleyTimer;
    uint32 m_uiAgonyTimer;

    void Reset() override
    {
        m_uiVolleyTimer = 10000;
        m_uiAgonyTimer  = 6000;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOKTANAG, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("The darkness claims all! Your suffering begins!");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOKTANAG, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("The shadows... take me...");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOKTANAG, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiVolleyTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SHADOW_BOLT_VOLLEY);
            m_uiVolleyTimer = urand(10000, 14000);
        }
        else
            m_uiVolleyTimer -= uiDiff;

        if (m_uiAgonyTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_CURSE_OF_AGONY);
            m_uiAgonyTimer = urand(8000, 12000);
        }
        else
            m_uiAgonyTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 6. Trioch the Devourer (62946) - Cavern Beast Hydra
// -----------------------------------------------------------------------------
enum TriochSpells
{
    SPELL_ACID_SPIT     = 23511,
    SPELL_POISON_CLOUD  = 24317,
    SPELL_THRASH        = 3391
};

struct boss_triochAI : public ScriptedAI
{
    explicit boss_triochAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiSpitTimer;
    uint32 m_uiCloudTimer;

    void Reset() override
    {
        m_uiSpitTimer  = 6000;
        m_uiCloudTimer = 14000;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TRIOCH, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterTextEmote("Trioch roars with primordial hunger!", nullptr, true);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TRIOCH, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterTextEmote("Trioch collapses heavily to the cavern floor.", nullptr, true);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TRIOCH, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiSpitTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCastSpellIfCan(pTarget, SPELL_ACID_SPIT);
            m_uiSpitTimer = urand(6000, 9000);
        }
        else
            m_uiSpitTimer -= uiDiff;

        if (m_uiCloudTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_POISON_CLOUD);
            m_uiCloudTimer = urand(14000, 18000);
        }
        else
            m_uiCloudTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 7. Selenaxx Foulheart (62940) - Satyr Lord in the Glade
// -----------------------------------------------------------------------------
enum SelenaxxSpells
{
    SPELL_SHADOWSTEP   = 24314,
    SPELL_CRIPPLE      = 20812,
    SPELL_IMMOLATE     = 20294
};

struct boss_selenaxxAI : public ScriptedAI
{
    explicit boss_selenaxxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiCrippleTimer;
    uint32 m_uiImmolateTimer;

    void Reset() override
    {
        m_uiCrippleTimer  = 7000;
        m_uiImmolateTimer = 11000;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SELENAXX, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("Fools! This glade belongs to the Legion's shadow!");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SELENAXX, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("The flame... will not die...");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SELENAXX, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiCrippleTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_CRIPPLE);
            m_uiCrippleTimer = urand(8000, 12000);
        }
        else
            m_uiCrippleTimer -= uiDiff;

        if (m_uiImmolateTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCastSpellIfCan(pTarget, SPELL_IMMOLATE);
            m_uiImmolateTimer = urand(10000, 14000);
        }
        else
            m_uiImmolateTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 8. Chieftain Partath (62941) - Ruler of the Withermaw Stronghold
// -----------------------------------------------------------------------------
enum PartathSpells
{
    SPELL_WAR_STOMP    = 16727,
    SPELL_CLEAVE       = 20684,
    SPELL_BLOODLUST    = 6742,
    SPELL_EARTH_SHOCK  = 22885
};

struct boss_partathAI : public ScriptedAI
{
    explicit boss_partathAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = static_cast<ScriptedInstance*>(pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiCleaveTimer;
    uint32 m_uiStompTimer;
    uint32 m_uiShockTimer;
    bool   m_bLustActive;

    void Reset() override
    {
        m_uiCleaveTimer = 5000;
        m_uiStompTimer  = 12000;
        m_uiShockTimer  = 8000;
        m_bLustActive   = false;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_PARTATH, NOT_STARTED);
    }

    void Aggro(Unit* /*pWho*/) override
    {
        m_creature->MonsterYell("The Withermaw will never bend! Die before the Chieftain!");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_PARTATH, IN_PROGRESS);
    }

    void JustDied(Unit* /*pKiller*/) override
    {
        m_creature->MonsterYell("The Maw... opens... Ursol awake...");
        if (m_pInstance)
            m_pInstance->SetData(TYPE_PARTATH, DONE);
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (!m_bLustActive && m_creature->GetHealthPercent() < 30.0f)
        {
            m_bLustActive = true;
            m_creature->MonsterYell("RAAAAARGH! Crush them all!");
            DoCastSpellIfCan(m_creature, SPELL_BLOODLUST);
        }

        if (m_uiCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = urand(5000, 8000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        if (m_uiStompTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WAR_STOMP);
            m_uiStompTimer = urand(14000, 18000);
        }
        else
            m_uiStompTimer -= uiDiff;

        if (m_uiShockTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCastSpellIfCan(pTarget, SPELL_EARTH_SHOCK);
            m_uiShockTimer = urand(8000, 11000);
        }
        else
            m_uiShockTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// Script Registration
// -----------------------------------------------------------------------------
CreatureAI* GetAI_boss_karrsh(Creature* pCreature) { return new boss_karrshAI(pCreature); }
CreatureAI* GetAI_boss_rotgrowl(Creature* pCreature) { return new boss_rotgrowlAI(pCreature); }
CreatureAI* GetAI_boss_kodiak(Creature* pCreature) { return new boss_kodiakAI(pCreature); }
CreatureAI* GetAI_boss_ormanos(Creature* pCreature) { return new boss_ormanosAI(pCreature); }
CreatureAI* GetAI_boss_archdruid_kronn(Creature* pCreature) { return new boss_archdruid_kronnAI(pCreature); }
CreatureAI* GetAI_boss_loktanag(Creature* pCreature) { return new boss_loktanagAI(pCreature); }
CreatureAI* GetAI_boss_trioch(Creature* pCreature) { return new boss_triochAI(pCreature); }
CreatureAI* GetAI_boss_selenaxx(Creature* pCreature) { return new boss_selenaxxAI(pCreature); }
CreatureAI* GetAI_boss_partath(Creature* pCreature) { return new boss_partathAI(pCreature); }

void AddSC_bosses_timbermaw_hold()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_karrsh";
    pNewscript->GetAI = &GetAI_boss_karrsh;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_rotgrowl";
    pNewscript->GetAI = &GetAI_boss_rotgrowl;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_kodiak";
    pNewscript->GetAI = &GetAI_boss_kodiak;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_ormanos";
    pNewscript->GetAI = &GetAI_boss_ormanos;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_archdruid_kronn";
    pNewscript->GetAI = &GetAI_boss_archdruid_kronn;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_loktanag";
    pNewscript->GetAI = &GetAI_boss_loktanag;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_trioch";
    pNewscript->GetAI = &GetAI_boss_trioch;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_selenaxx";
    pNewscript->GetAI = &GetAI_boss_selenaxx;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "boss_partath";
    pNewscript->GetAI = &GetAI_boss_partath;
    pNewscript->RegisterSelf();
}
