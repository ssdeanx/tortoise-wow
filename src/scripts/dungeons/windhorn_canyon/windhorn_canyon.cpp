#include "scriptPCH.h"
#include "windhorn_canyon.h"

namespace
{
struct boss_chieftain_shalk_blackwindAI : public ScriptedAI
{
    explicit boss_chieftain_shalk_blackwindAI(Creature* creature) :
        ScriptedAI(creature),
        m_pollForFlameAura(false)
    {
        Reset();
    }

    void Reset() override
    {
        m_summonedBloodguard = false;
        m_underwaterCheckTimer = 250;
    }

    void Aggro(Unit* /*who*/) override
    {
        DoScriptText(SAY_SHALK_AGGRO, m_creature);
    }

    void JustDied(Unit* /*killer*/) override
    {
        DoScriptText(SAY_SHALK_DEATH, m_creature);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_summonedBloodguard && m_creature->IsInCombat() && m_creature->GetHealthPercent() <= 50.0f)
        {
            DoScriptText(SAY_SHALK_HALF_HEALTH, m_creature);

            if (Creature* guard = m_creature->SummonCreature(
                    NPC_BLACKWIND_BLOODGUARD,
                    -7546.968750f, -3762.458008f, 282.898224f, 5.498519f,
                    TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000, true))
            {
                DoScriptText(SAY_SHALK_BLOODGUARD_SPAWN, guard);

                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    guard->AI()->AttackStart(target);
            }

            m_summonedBloodguard = true;
        }

        if (m_creature->IsInCombat())
            m_pollForFlameAura = true;

        if (m_pollForFlameAura && m_underwaterCheckTimer <= diff)
        {
            m_pollForFlameAura = false;

            Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
            for (const auto& itr : players)
            {
                Player* player = itr.getSource();
                if (!player || !player->HasAura(SPELL_FLAME_OF_SHALK))
                    continue;

                if (player->IsUnderwater())
                    player->RemoveAurasDueToSpell(SPELL_FLAME_OF_SHALK);
                else
                    m_pollForFlameAura = true;
            }

            m_underwaterCheckTimer = 500;
        }
        else if (m_pollForFlameAura)
            m_underwaterCheckTimer -= diff;

        ScriptedAI::UpdateAI(diff);
    }

private:
    bool m_summonedBloodguard;
    bool m_pollForFlameAura;
    uint32 m_underwaterCheckTimer;
};

struct npc_windhorn_storm_guardianAI : public ScriptedAI
{
    explicit npc_windhorn_storm_guardianAI(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override {}

    void JustDied(Unit* killer) override
    {
        Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
        if (!target && killer && killer->IsAlive())
            target = killer;

        static constexpr float offsets[3][2] =
        {
            { 1.50f,  0.00f },
            {-0.75f,  1.30f },
            {-0.75f, -1.30f },
        };

        for (const auto& offset : offsets)
        {
            if (Creature* residue = m_creature->SummonCreature(
                    NPC_STORM_RESIDUE,
                    m_creature->GetPositionX() + offset[0],
                    m_creature->GetPositionY() + offset[1],
                    m_creature->GetPositionZ(),
                    m_creature->GetOrientation(),
                    TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000, true))
            {
                if (target)
                    residue->AI()->AttackStart(target);
            }
        }
    }
};

struct boss_pathun_duskhideAI : public ScriptedAI
{
    explicit boss_pathun_duskhideAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    uint32 m_uiMultiShotTimer;
    uint32 m_uiConcussiveShotTimer;
    uint32 m_uiRaptorStrikeTimer;
    bool m_bUsedHalfHealth;

    void Reset() override
    {
        m_uiMultiShotTimer = 6000;
        m_uiConcussiveShotTimer = 4000;
        m_uiRaptorStrikeTimer = 8000;
        m_bUsedHalfHealth = false;
    }

    void Aggro(Unit* /*who*/) override
    {
        DoScriptText(SAY_PATHUN_AGGRO, m_creature);
    }

    void JustDied(Unit* /*killer*/) override
    {
        DoScriptText(SAY_PATHUN_DEATH, m_creature);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (!m_bUsedHalfHealth && m_creature->GetHealthPercent() <= 50.0f)
        {
            m_bUsedHalfHealth = true;
            DoScriptText(SAY_PATHUN_HALF_HEALTH, m_creature);
        }

        if (m_uiConcussiveShotTimer <= diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, SPELL_CONCUSSIVE_SHOT);
            m_uiConcussiveShotTimer = 10000;
        }
        else
            m_uiConcussiveShotTimer -= diff;

        if (m_uiMultiShotTimer <= diff)
        {
            if (Unit* target = m_creature->GetVictim())
                DoCastSpellIfCan(target, SPELL_MULTI_SHOT);
            m_uiMultiShotTimer = 8000;
        }
        else
            m_uiMultiShotTimer -= diff;

        if (m_uiRaptorStrikeTimer <= diff)
        {
            if (Unit* target = m_creature->GetVictim())
                DoCastSpellIfCan(target, SPELL_RAPTOR_STRIKE);
            m_uiRaptorStrikeTimer = 6000;
        }
        else
            m_uiRaptorStrikeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct boss_ambassador_vortalusAI : public ScriptedAI
{
    explicit boss_ambassador_vortalusAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    uint32 m_uiShadowBoltTimer;
    uint32 m_uiImmolateTimer;
    uint32 m_uiFearTimer;
    bool m_bUsedHalfHealth;

    void Reset() override
    {
        m_uiShadowBoltTimer = 2000;
        m_uiImmolateTimer = 5000;
        m_uiFearTimer = 12000;
        m_bUsedHalfHealth = false;
    }

    void Aggro(Unit* /*who*/) override
    {
        DoScriptText(SAY_VORTALUS_AGGRO, m_creature);
    }

    void JustDied(Unit* /*killer*/) override
    {
        DoScriptText(SAY_VORTALUS_DEATH, m_creature);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (!m_bUsedHalfHealth && m_creature->GetHealthPercent() <= 50.0f)
        {
            m_bUsedHalfHealth = true;
            DoScriptText(SAY_VORTALUS_HALF_HEALTH, m_creature);
        }

        if (m_uiFearTimer <= diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, SPELL_FEAR);
            m_uiFearTimer = 15000;
        }
        else
            m_uiFearTimer -= diff;

        if (m_uiImmolateTimer <= diff)
        {
            if (Unit* target = m_creature->GetVictim())
                DoCastSpellIfCan(target, SPELL_IMMOLATE);
            m_uiImmolateTimer = 10000;
        }
        else
            m_uiImmolateTimer -= diff;

        if (m_uiShadowBoltTimer <= diff)
        {
            if (Unit* target = m_creature->GetVictim())
                DoCastSpellIfCan(target, SPELL_SHADOW_BOLT);
            m_uiShadowBoltTimer = 4000;
        }
        else
            m_uiShadowBoltTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct boss_walgan_bloodcallerAI : public ScriptedAI
{
    explicit boss_walgan_bloodcallerAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    uint32 m_uiMortalStrikeTimer;
    uint32 m_uiWhirlwindTimer;
    bool m_bUsedBloodlust;
    bool m_bUsedHalfHealth;

    void Reset() override
    {
        m_uiMortalStrikeTimer = 4000;
        m_uiWhirlwindTimer = 8000;
        m_bUsedBloodlust = false;
        m_bUsedHalfHealth = false;
    }

    void Aggro(Unit* /*who*/) override
    {
        DoScriptText(SAY_WALGAN_AGGRO, m_creature);
    }

    void JustDied(Unit* /*killer*/) override
    {
        DoScriptText(SAY_WALGAN_DEATH, m_creature);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (!m_bUsedHalfHealth && m_creature->GetHealthPercent() <= 50.0f)
        {
            m_bUsedHalfHealth = true;
            DoScriptText(SAY_WALGAN_HALF_HEALTH, m_creature);
        }

        if (!m_bUsedBloodlust && m_creature->GetHealthPercent() <= 30.0f)
        {
            m_bUsedBloodlust = true;
            DoCastSpellIfCan(m_creature, SPELL_BLOODLUST);
        }

        if (m_uiMortalStrikeTimer <= diff)
        {
            if (Unit* target = m_creature->GetVictim())
                DoCastSpellIfCan(target, SPELL_MORTAL_STRIKE);
            m_uiMortalStrikeTimer = 7000;
        }
        else
            m_uiMortalStrikeTimer -= diff;

        if (m_uiWhirlwindTimer <= diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND);
            m_uiWhirlwindTimer = 12000;
        }
        else
            m_uiWhirlwindTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct boss_prophet_stormhoofAI : public ScriptedAI
{
    explicit boss_prophet_stormhoofAI(Creature* creature) : ScriptedAI(creature)
    {
        Reset();
    }

    uint32 m_uiLightningBoltTimer;
    uint32 m_uiEarthShockTimer;
    uint32 m_uiChainLightningTimer;
    bool m_bUsedHalfHealth;

    void Reset() override
    {
        m_uiLightningBoltTimer = 2000;
        m_uiEarthShockTimer = 5000;
        m_uiChainLightningTimer = 9000;
        m_bUsedHalfHealth = false;
    }

    void Aggro(Unit* /*who*/) override
    {
        DoScriptText(SAY_STORMHOOF_AGGRO, m_creature);
    }

    void JustDied(Unit* /*killer*/) override
    {
        DoScriptText(SAY_STORMHOOF_DEATH, m_creature);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
            return;

        if (!m_bUsedHalfHealth && m_creature->GetHealthPercent() <= 50.0f)
        {
            m_bUsedHalfHealth = true;
            DoScriptText(SAY_STORMHOOF_HALF_HEALTH, m_creature);
        }

        if (m_uiEarthShockTimer <= diff)
        {
            if (Unit* target = m_creature->GetVictim())
                DoCastSpellIfCan(target, SPELL_EARTH_SHOCK);
            m_uiEarthShockTimer = 6000;
        }
        else
            m_uiEarthShockTimer -= diff;

        if (m_uiChainLightningTimer <= diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target, SPELL_CHAIN_LIGHTNING);
            m_uiChainLightningTimer = 11000;
        }
        else
            m_uiChainLightningTimer -= diff;

        if (m_uiLightningBoltTimer <= diff)
        {
            if (Unit* target = m_creature->GetVictim())
                DoCastSpellIfCan(target, SPELL_LIGHTNING_BOLT);
            m_uiLightningBoltTimer = 4000;
        }
        else
            m_uiLightningBoltTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_chieftain_shalk_blackwind(Creature* creature)
{
    return new boss_chieftain_shalk_blackwindAI(creature);
}

CreatureAI* GetAI_npc_windhorn_storm_guardian(Creature* creature)
{
    return new npc_windhorn_storm_guardianAI(creature);
}

CreatureAI* GetAI_boss_pathun_duskhide(Creature* creature)
{
    return new boss_pathun_duskhideAI(creature);
}

CreatureAI* GetAI_boss_ambassador_vortalus(Creature* creature)
{
    return new boss_ambassador_vortalusAI(creature);
}

CreatureAI* GetAI_boss_walgan_bloodcaller(Creature* creature)
{
    return new boss_walgan_bloodcallerAI(creature);
}

CreatureAI* GetAI_boss_prophet_stormhoof(Creature* creature)
{
    return new boss_prophet_stormhoofAI(creature);
}
}

void AddSC_windhorn_canyon()
{
    Script* script;

    script = new Script;
    script->Name = "boss_chieftain_shalk_blackwind";
    script->GetAI = &GetAI_boss_chieftain_shalk_blackwind;
    script->RegisterSelf();

    script = new Script;
    script->Name = "npc_windhorn_storm_guardian";
    script->GetAI = &GetAI_npc_windhorn_storm_guardian;
    script->RegisterSelf();

    script = new Script;
    script->Name = "boss_pathun_duskhide";
    script->GetAI = &GetAI_boss_pathun_duskhide;
    script->RegisterSelf();

    script = new Script;
    script->Name = "boss_ambassador_vortalus";
    script->GetAI = &GetAI_boss_ambassador_vortalus;
    script->RegisterSelf();

    script = new Script;
    script->Name = "boss_walgan_bloodcaller";
    script->GetAI = &GetAI_boss_walgan_bloodcaller;
    script->RegisterSelf();

    script = new Script;
    script->Name = "boss_prophet_stormhoof";
    script->GetAI = &GetAI_boss_prophet_stormhoof;
    script->RegisterSelf();
}
