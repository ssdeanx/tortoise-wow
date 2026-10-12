#include "scriptPCH.h"
#include "timbermaw_hold.h"

// -----------------------------------------------------------------------------
// 1. Withermaw Shaman (62871)
// -----------------------------------------------------------------------------
enum ShamanSpells
{
    SPELL_LIGHTNING_BOLT    = 20564,
    SPELL_CHAIN_LIGHTNING   = 20698,
    SPELL_HEALING_WAVE      = 20614
};

struct mob_withermaw_shamanAI : public ScriptedAI
{
    explicit mob_withermaw_shamanAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiLightningTimer;
    uint32 m_uiHealTimer;

    void Reset() override
    {
        m_uiLightningTimer = 3000;
        m_uiHealTimer      = 12000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiHealTimer < uiDiff)
        {
            if (Unit* pAlly = m_creature->FindLowestHpFriendlyUnit(30.0f))
            {
                if (pAlly->GetHealthPercent() < 50.0f)
                    DoCastSpellIfCan(pAlly, SPELL_HEALING_WAVE);
            }

            m_uiHealTimer = urand(12000, 15000);
        }
        else
            m_uiHealTimer -= uiDiff;

        if (m_uiLightningTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, urand(0, 1) ? SPELL_LIGHTNING_BOLT : SPELL_CHAIN_LIGHTNING);
            m_uiLightningTimer = urand(4000, 6000);
        }
        else
            m_uiLightningTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 2. Withermaw Defiler (62870)
// -----------------------------------------------------------------------------
enum DefilerSpells
{
    SPELL_SHADOW_BOLT       = 20700,
    SPELL_CURSE_OF_AGONY    = 23140,
    SPELL_SHADOW_SHOCK      = 17399
};

struct mob_withermaw_defilerAI : public ScriptedAI
{
    explicit mob_withermaw_defilerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiBoltTimer;
    uint32 m_uiCurseTimer;

    void Reset() override
    {
        m_uiBoltTimer  = 3000;
        m_uiCurseTimer = 8000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiCurseTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_CURSE_OF_AGONY);
            m_uiCurseTimer = urand(10000, 14000);
        }
        else
            m_uiCurseTimer -= uiDiff;

        if (m_uiBoltTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SHADOW_BOLT);
            m_uiBoltTimer = urand(3500, 5000);
        }
        else
            m_uiBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 3. Withermaw Totemic (62873)
// -----------------------------------------------------------------------------
enum TotemicSpells
{
    SPELL_EARTHGRAB_TOTEM   = 8376,
    SPELL_SEARING_TOTEM     = 10438,
    SPELL_BLOODLUST_TRASH   = 6742
};

struct mob_withermaw_totemicAI : public ScriptedAI
{
    explicit mob_withermaw_totemicAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiTotemTimer;
    uint32 m_uiBloodlustTimer;

    void Reset() override
    {
        m_uiTotemTimer     = 4000;
        m_uiBloodlustTimer = 15000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiTotemTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, urand(0, 1) ? SPELL_EARTHGRAB_TOTEM : SPELL_SEARING_TOTEM);
            m_uiTotemTimer = urand(15000, 20000);
        }
        else
            m_uiTotemTimer -= uiDiff;

        if (m_uiBloodlustTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BLOODLUST_TRASH);
            m_uiBloodlustTimer = urand(25000, 30000);
        }
        else
            m_uiBloodlustTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 4. Withermaw Ursa (62877) & Enraged Withermaw (62867)
// -----------------------------------------------------------------------------
enum UrsaSpells
{
    SPELL_DEMORALIZING_ROAR = 20689,
    SPELL_FURBOLG_ENRAGE    = 8269,
    SPELL_SWIPE_TRASH       = 22543
};

struct mob_withermaw_ursaAI : public ScriptedAI
{
    explicit mob_withermaw_ursaAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiRoarTimer;
    uint32 m_uiSwipeTimer;
    bool   m_bEnraged;

    void Reset() override
    {
        m_uiRoarTimer  = 5000;
        m_uiSwipeTimer = 8000;
        m_bEnraged     = false;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (!m_bEnraged && m_creature->GetHealthPercent() < 30.0f)
        {
            m_bEnraged = true;
            DoCastSpellIfCan(m_creature, SPELL_FURBOLG_ENRAGE);
        }

        if (m_uiRoarTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_DEMORALIZING_ROAR);
            m_uiRoarTimer = urand(15000, 20000);
        }
        else
            m_uiRoarTimer -= uiDiff;

        if (m_uiSwipeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SWIPE_TRASH);
            m_uiSwipeTimer = urand(7000, 10000);
        }
        else
            m_uiSwipeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 5. Vile Skitterer (62874)
// -----------------------------------------------------------------------------
enum SpiderSpells
{
    SPELL_POISON_SPIT = 21067,
    SPELL_WEB_SPRAY   = 24650
};

struct mob_vile_skittererAI : public ScriptedAI
{
    explicit mob_vile_skittererAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiSpitTimer;
    uint32 m_uiWebTimer;

    void Reset() override
    {
        m_uiSpitTimer = 4000;
        m_uiWebTimer  = 10000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiSpitTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_POISON_SPIT);
            m_uiSpitTimer = urand(6000, 9000);
        }
        else
            m_uiSpitTimer -= uiDiff;

        if (m_uiWebTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCastSpellIfCan(pTarget, SPELL_WEB_SPRAY);
            m_uiWebTimer = urand(12000, 16000);
        }
        else
            m_uiWebTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 6. Foulheart Hellcaller (62884) & Satyr (62882)
// -----------------------------------------------------------------------------
enum SatyrSpells
{
    SPELL_RAIN_OF_FIRE   = 20754,
    SPELL_IMMOLATE_TRASH = 20294,
    SPELL_GOUGE          = 1776
};

struct mob_foulheart_hellcallerAI : public ScriptedAI
{
    explicit mob_foulheart_hellcallerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiRainTimer;
    uint32 m_uiImmolateTimer;

    void Reset() override
    {
        m_uiRainTimer     = 6000;
        m_uiImmolateTimer = 3000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiImmolateTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_IMMOLATE_TRASH);
            m_uiImmolateTimer = urand(8000, 12000);
        }
        else
            m_uiImmolateTimer -= uiDiff;

        if (m_uiRainTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_RAIN_OF_FIRE);
            m_uiRainTimer = urand(15000, 20000);
        }
        else
            m_uiRainTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// 7. Son of Ursol (62885)
// -----------------------------------------------------------------------------
enum SonOfUrsolSpells
{
    SPELL_SON_MAUL        = 23128,
    SPELL_THUNDERCLAP     = 23931,
    SPELL_FEROCIOUS_ROAR  = 23511
};

struct mob_son_of_ursolAI : public ScriptedAI
{
    explicit mob_son_of_ursolAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiMaulTimer;
    uint32 m_uiClapTimer;

    void Reset() override
    {
        m_uiMaulTimer = 4000;
        m_uiClapTimer = 9000;
    }

    void UpdateAI(uint32 const uiDiff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (m_uiMaulTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->GetVictim(), SPELL_SON_MAUL);
            m_uiMaulTimer = urand(5000, 7000);
        }
        else
            m_uiMaulTimer -= uiDiff;

        if (m_uiClapTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_THUNDERCLAP);
            m_uiClapTimer = urand(10000, 14000);
        }
        else
            m_uiClapTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// -----------------------------------------------------------------------------
// Script Registrations
// -----------------------------------------------------------------------------
CreatureAI* GetAI_mob_withermaw_shaman(Creature* pCreature) { return new mob_withermaw_shamanAI(pCreature); }
CreatureAI* GetAI_mob_withermaw_defiler(Creature* pCreature) { return new mob_withermaw_defilerAI(pCreature); }
CreatureAI* GetAI_mob_withermaw_totemic(Creature* pCreature) { return new mob_withermaw_totemicAI(pCreature); }
CreatureAI* GetAI_mob_withermaw_ursa(Creature* pCreature) { return new mob_withermaw_ursaAI(pCreature); }
CreatureAI* GetAI_mob_vile_skitterer(Creature* pCreature) { return new mob_vile_skittererAI(pCreature); }
CreatureAI* GetAI_mob_foulheart_hellcaller(Creature* pCreature) { return new mob_foulheart_hellcallerAI(pCreature); }
CreatureAI* GetAI_mob_son_of_ursol(Creature* pCreature) { return new mob_son_of_ursolAI(pCreature); }

void AddSC_timbermaw_hold_trash()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "mob_withermaw_shaman";
    pNewscript->GetAI = &GetAI_mob_withermaw_shaman;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_withermaw_defiler";
    pNewscript->GetAI = &GetAI_mob_withermaw_defiler;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_withermaw_totemic";
    pNewscript->GetAI = &GetAI_mob_withermaw_totemic;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_withermaw_ursa";
    pNewscript->GetAI = &GetAI_mob_withermaw_ursa;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_vile_skitterer";
    pNewscript->GetAI = &GetAI_mob_vile_skitterer;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_foulheart_hellcaller";
    pNewscript->GetAI = &GetAI_mob_foulheart_hellcaller;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_son_of_ursol";
    pNewscript->GetAI = &GetAI_mob_son_of_ursol;
    pNewscript->RegisterSelf();
}
