/*
 * Farraki Arena - Zul'Farrak
 */

#include "scriptPCH.h"
#include "zulfarrak.h"
#include "Group.h"
#include "GroupReference.h"

namespace
{
    enum FarrakiArenaActions
    {
        ACTION_ARENA_START = 1,
        ACTION_KATHZEN_DIED,
        ACTION_JUTHZA_DIED,
        ACTION_ARENA_RESET,
    };

    enum FarrakiArenaTexts
    {
        SAY_KATHZEN_AGGRO = 6249601,
        SAY_JUTHZA_AGGRO  = 6249701,
        SAY_RAZJAL_INTRO  = 6249803,
        SAY_RAZJAL_AGGRO  = 6249804,
        SAY_RAZJAL_SANDS  = 6249805,
        SAY_RAZJAL_FINAL  = 6249806,
    };

    enum FarrakiArenaSpells
    {
        SPELL_ENRAGE             = 15716,
        SPELL_NATURE_CHANNELING  = 13236,
        SPELL_HELLFIRE           = 11684,
    };

    enum FarrakiArenaNpcs
    {
        NPC_SANDFURY_SCORPID = 62499,
    };

    enum FarrakiArenaFactions
    {
        FACTION_HOSTILE = 14,
    };

    enum FarrakiArenaPhase
    {
        PHASE_IDLE,
        PHASE_INTRO,
        PHASE_KATHZEN,
        PHASE_JUTHZA,
        PHASE_RAZJAL,
        PHASE_SCORPIDS,
    };

    struct ArenaSpawnPosition
    {
        float x;
        float y;
        float z;
        float o;
    };

    ArenaSpawnPosition const RazjalChannelPosition = { 1512.2099609375f, 1016.0499877929688f, 11.678000450134277f, 2.899270534515381f };

    ArenaSpawnPosition const ScorpidSpawnPositions[] =
    {
        { 1498.650024f, 1030.937012f, 11.6f,    0.0f },
        { 1534.144043f, 1030.718018f, 11.826f,  0.0f },
        { 1536.583008f, 1005.965027f, 11.6277f, 0.0f },
        { 1499.567017f,  998.440002f, 11.756f,  0.0f },
    };

    uint32 const RazjalImmunityMechanics[] =
    {
        MECHANIC_CHARM,
        MECHANIC_DISORIENTED,
        MECHANIC_DISARM,
        MECHANIC_FEAR,
        MECHANIC_ROOT,
        MECHANIC_SILENCE,
        MECHANIC_SLEEP,
        MECHANIC_SNARE,
        MECHANIC_STUN,
        MECHANIC_FREEZE,
        MECHANIC_KNOCKOUT,
        MECHANIC_POLYMORPH,
        MECHANIC_BANISH,
        MECHANIC_SHACKLE,
        MECHANIC_TURN,
        MECHANIC_HORROR,
        MECHANIC_DAZE,
        MECHANIC_SAPPED,
    };

    void ApplyRazjalIntermissionImmunity(Creature* creature, bool apply)
    {
        creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_ALL, apply);
        creature->ApplySpellImmune(0, IMMUNITY_DISPEL, DISPEL_MAGIC, apply);
        creature->ApplySpellImmune(0, IMMUNITY_DISPEL, DISPEL_CURSE, apply);
        creature->ApplySpellImmune(0, IMMUNITY_DISPEL, DISPEL_DISEASE, apply);
        creature->ApplySpellImmune(0, IMMUNITY_DISPEL, DISPEL_POISON, apply);

        for (uint32 mechanic : RazjalImmunityMechanics)
            creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, mechanic, apply);
    }

    void ArenaYell(Creature* creature, uint32 textId)
    {
        DoScriptText(textId, creature);
    }

    void CastZeroCostHellfire(Creature* creature)
    {
        SpellEntry const* hellfire = sSpellMgr.GetSpellEntry(SPELL_HELLFIRE);
        if (!hellfire)
            return;

        SpellEntry* customHellfire = new SpellEntry(*hellfire);
        customHellfire->Internal |= SPELL_INTERNAL_CUSTOM;
        customHellfire->manaCost = 0;
        customHellfire->manaCostPerlevel = 0;
        customHellfire->ManaCostPercentage = 0;
        customHellfire->manaPerSecond = 0;
        customHellfire->manaPerSecondPerLevel = 0;

        creature->InterruptNonMeleeSpells(false);
        creature->CastCustomSpell(creature, customHellfire);
    }

    struct farraki_arena_combatantAI : public ScriptedAI
    {
        farraki_arena_combatantAI(Creature* creature) : ScriptedAI(creature)
        {
            m_instance = creature->GetInstanceData();
            Reset();
        }

        InstanceData* m_instance;
        bool m_enraged;

        void Reset() override
        {
            m_enraged = false;
            m_creature->RestoreFaction();
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SPAWNING | UNIT_FLAG_IMMUNE_TO_PLAYER | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE_2);
        }

        Creature* GetRazjal() const
        {
            if (!m_instance)
                return nullptr;

            return m_creature->GetMap()->GetCreature(m_instance->GetData64(ENTRY_RAZJAL));
        }

        void NotifyRazjal(uint32 action)
        {
            if (Creature* razjal = GetRazjal())
                razjal->AI()->DoAction(action);
        }

        void EnterEvadeMode() override
        {
            NotifyRazjal(ACTION_ARENA_RESET);
            ScriptedAI::EnterEvadeMode();
        }
    };

    struct npc_kathzen_the_brutalAI : public farraki_arena_combatantAI
    {
        npc_kathzen_the_brutalAI(Creature* creature) : farraki_arena_combatantAI(creature) {}

        void JustDied(Unit* /*killer*/) override
        {
            NotifyRazjal(ACTION_KATHZEN_DIED);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
                return;

            if (!m_CreatureSpells.empty())
                UpdateSpellsList(diff);

            if (!m_enraged && m_creature->GetHealthPercent() <= 50.0f)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
                    m_enraged = true;
            }

            DoMeleeAttackIfReady();
        }
    };

    struct npc_juthza_the_cunningAI : public farraki_arena_combatantAI
    {
        npc_juthza_the_cunningAI(Creature* creature) : farraki_arena_combatantAI(creature) {}

        void JustDied(Unit* /*killer*/) override
        {
            NotifyRazjal(ACTION_JUTHZA_DIED);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
                return;

            if (!m_CreatureSpells.empty())
                UpdateSpellsList(diff);

            DoMeleeAttackIfReady();
        }
    };

    struct npc_champion_razjal_the_quickAI : public ScriptedAI
    {
        npc_champion_razjal_the_quickAI(Creature* creature) : ScriptedAI(creature)
        {
            m_instance = creature->GetInstanceData();
            m_originalNpcFlags = creature->GetUInt32Value(UNIT_NPC_FLAGS);
            m_resetting = false;
            Reset();
        }

        InstanceData* m_instance;
        uint64 m_starterGuid;
        uint32 m_originalNpcFlags;
        uint32 m_phase;
        uint32 m_introTimer;
        uint32 m_nextPhaseTimer;
        uint32 m_scorpidStartTimer;
        uint32 m_scorpidSpawnTimer;
        uint8 m_scorpidsSpawned;
        uint8 m_lastScorpidSpawn;
        bool m_sandsStarted;
        bool m_finalStarted;
        bool m_resetting;
        std::vector<uint64> m_scorpidGuids;

        void Reset() override
        {
            DespawnScorpids();
            m_starterGuid = 0;
            m_phase = PHASE_IDLE;
            m_introTimer = 0;
            m_nextPhaseTimer = 0;
            m_scorpidStartTimer = 0;
            m_scorpidSpawnTimer = 0;
            m_scorpidsSpawned = 0;
            m_lastScorpidSpawn = 255;
            m_sandsStarted = false;
            m_finalStarted = false;
            m_resetting = false;
            ApplyRazjalIntermissionImmunity(m_creature, false);
            m_creature->RemoveAurasDueToSpell(SPELL_NATURE_CHANNELING);
            m_creature->SetReactState(REACT_AGGRESSIVE);
            m_creature->RestoreFaction();
            m_creature->SetUInt32Value(UNIT_NPC_FLAGS, m_originalNpcFlags);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SPAWNING | UNIT_FLAG_IMMUNE_TO_PLAYER | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE_2);
            m_scorpidGuids.clear();
        }

        bool CanStartEvent() const
        {
            return m_phase == PHASE_IDLE;
        }

        Creature* GetArenaCreature(uint32 entry) const
        {
            if (!m_instance)
                return nullptr;

            return m_creature->GetMap()->GetCreature(m_instance->GetData64(entry));
        }

        Player* GetStarter() const
        {
            if (Unit* unit = m_starterGuid ? Unit::GetUnit(*m_creature, m_starterGuid) : nullptr)
                return unit->ToPlayer();

            return nullptr;
        }

        void AddThreatToParty(Creature* creature, Player* player)
        {
            if (!creature || !player)
                return;

            if (Group* group = player->GetGroup())
            {
                for (GroupReference* ref = group->GetFirstMember(); ref != nullptr; ref = ref->next())
                {
                    Player* member = ref->getSource();
                    if (member && member->IsAlive() && member->GetMap() == creature->GetMap())
                        creature->AddThreat(member, 1.0f);
                }
            }
            else
                creature->AddThreat(player, 1.0f);
        }

        void EngageCreature(Creature* creature, Player* player, uint32 textId)
        {
            if (!creature || !creature->IsAlive() || !player)
                return;

            creature->SetFactionTemporary(FACTION_HOSTILE, TEMPFACTION_RESTORE_RESPAWN | TEMPFACTION_RESTORE_COMBAT_STOP);
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SPAWNING | UNIT_FLAG_IMMUNE_TO_PLAYER | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE_2);
            AddThreatToParty(creature, player);
            creature->AI()->AttackStart(player);
            ArenaYell(creature, textId);
        }

        bool AreKathzenAndJuthzaDead() const
        {
            Creature* kathzen = GetArenaCreature(ENTRY_KATHZEN);
            Creature* juthza = GetArenaCreature(ENTRY_JUTHZA);
            return (!kathzen || !kathzen->IsAlive()) && (!juthza || !juthza->IsAlive());
        }

        void StartKathzen()
        {
            m_phase = PHASE_KATHZEN;
            m_nextPhaseTimer = 30 * IN_MILLISECONDS;
            EngageCreature(GetArenaCreature(ENTRY_KATHZEN), GetStarter(), SAY_KATHZEN_AGGRO);
        }

        void StartJuthza()
        {
            if (m_phase >= PHASE_JUTHZA)
                return;

            m_phase = PHASE_JUTHZA;
            m_nextPhaseTimer = 30 * IN_MILLISECONDS;
            EngageCreature(GetArenaCreature(ENTRY_JUTHZA), GetStarter(), SAY_JUTHZA_AGGRO);
        }

        void StartRazjal()
        {
            if (m_phase >= PHASE_RAZJAL)
                return;

            m_phase = PHASE_RAZJAL;
            EngageCreature(m_creature, GetStarter(), SAY_RAZJAL_AGGRO);
        }

        void StartSandsIntermission()
        {
            m_sandsStarted = true;
            m_phase = PHASE_SCORPIDS;
            m_scorpidStartTimer = 5 * IN_MILLISECONDS;
            m_scorpidSpawnTimer = 0;
            m_scorpidsSpawned = 0;
            m_lastScorpidSpawn = 255;

            ApplyRazjalIntermissionImmunity(m_creature, true);
            m_creature->SetReactState(REACT_PASSIVE);
            m_creature->NearTeleportTo(RazjalChannelPosition.x, RazjalChannelPosition.y, RazjalChannelPosition.z, RazjalChannelPosition.o);
            m_creature->SetFacingTo(RazjalChannelPosition.o);
            m_creature->AttackStop(true);
            m_creature->ClearTarget();
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->CastSpell(m_creature, SPELL_NATURE_CHANNELING, true);
            ArenaYell(m_creature, SAY_RAZJAL_SANDS);
        }

        void EndSandsIntermission()
        {
            m_creature->InterruptNonMeleeSpells(false, SPELL_NATURE_CHANNELING);
            m_creature->RemoveAurasDueToSpell(SPELL_NATURE_CHANNELING);
            ApplyRazjalIntermissionImmunity(m_creature, false);
            m_creature->SetReactState(REACT_AGGRESSIVE);
            m_phase = PHASE_RAZJAL;

            if (Player* player = GetStarter())
            {
                AddThreatToParty(m_creature, player);
                AttackStart(player);
            }
        }

        void SpawnScorpid()
        {
            uint8 spawnIndex = m_lastScorpidSpawn < 4 ? urand(0, 2) : urand(0, 3);
            if (m_lastScorpidSpawn < 4 && spawnIndex >= m_lastScorpidSpawn)
                ++spawnIndex;

            ArenaSpawnPosition const& position = ScorpidSpawnPositions[spawnIndex];
            if (Creature* scorpid = m_creature->SummonCreature(NPC_SANDFURY_SCORPID, position.x, position.y, position.z, position.o, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 2 * MINUTE * IN_MILLISECONDS))
            {
                m_scorpidGuids.push_back(scorpid->GetGUID());
                scorpid->SetFactionTemplateId(FACTION_HOSTILE);
                if (Player* player = GetStarter())
                {
                    AddThreatToParty(scorpid, player);
                    scorpid->AI()->AttackStart(player);
                }
            }

            m_lastScorpidSpawn = spawnIndex;
            ++m_scorpidsSpawned;

            if (m_scorpidsSpawned == 10)
                EndSandsIntermission();
        }

        void DespawnScorpids(uint32 delay = 0)
        {
            for (uint64 guid : m_scorpidGuids)
            {
                if (Creature* scorpid = m_creature->GetMap()->GetCreature(guid))
                {
                    if (delay)
                    {
                        scorpid->InterruptNonMeleeSpells(false);
                        scorpid->DeleteThreatList();
                        scorpid->CombatStop(true);
                        scorpid->AI()->EnterEvadeMode();
                    }

                    scorpid->ForcedDespawn(delay);
                }
            }

            m_scorpidGuids.clear();
        }

        void ResetArenaCreature(Creature* creature)
        {
            if (!creature)
                return;

            creature->ForcedDespawn();
            creature->Respawn();
            creature->InterruptNonMeleeSpells(false);
            creature->RemoveAllAuras();
            creature->DeleteThreatList();
            creature->CombatStop(true);
            creature->SetHealth(creature->GetMaxHealth());
            creature->RestoreFaction();
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SPAWNING | UNIT_FLAG_IMMUNE_TO_PLAYER | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NON_ATTACKABLE_2);
            creature->AI()->EnterEvadeMode();
        }

        void ResetArena()
        {
            if (m_resetting)
                return;

            m_resetting = true;
            DespawnScorpids(8 * IN_MILLISECONDS);
            ApplyRazjalIntermissionImmunity(m_creature, false);
            m_creature->RemoveAurasDueToSpell(SPELL_NATURE_CHANNELING);
            ResetArenaCreature(GetArenaCreature(ENTRY_KATHZEN));
            ResetArenaCreature(GetArenaCreature(ENTRY_JUTHZA));
            ResetArenaCreature(m_creature);
            m_resetting = false;
            Reset();
        }

        void AttackedBy(Unit* attacker) override
        {
            if (m_phase == PHASE_SCORPIDS)
                return;

            ScriptedAI::AttackedBy(attacker);
        }

        void AttackStart(Unit* who) override
        {
            if (m_phase == PHASE_SCORPIDS)
                return;

            ScriptedAI::AttackStart(who);
        }

        void DoAction(uint32 action) override
        {
            switch (action)
            {
                case ACTION_ARENA_START:
                    if (m_phase != PHASE_IDLE)
                        return;

                    m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    m_phase = PHASE_INTRO;
                    m_introTimer = 5 * IN_MILLISECONDS;
                    break;
                case ACTION_KATHZEN_DIED:
                    if (m_phase == PHASE_KATHZEN)
                        StartJuthza();
                    else if (m_phase == PHASE_JUTHZA && AreKathzenAndJuthzaDead())
                        StartRazjal();
                    break;
                case ACTION_JUTHZA_DIED:
                    if (m_phase == PHASE_JUTHZA && AreKathzenAndJuthzaDead())
                        StartRazjal();
                    break;
                case ACTION_ARENA_RESET:
                    if (m_phase != PHASE_IDLE)
                        ResetArena();
                    break;
            }
        }

        void EnterEvadeMode() override
        {
            if (m_phase != PHASE_IDLE)
                ResetArena();

            ScriptedAI::EnterEvadeMode();
        }

        void JustDied(Unit* /*killer*/) override
        {
            ApplyRazjalIntermissionImmunity(m_creature, false);
        }

        void UpdateAI(uint32 const diff) override
        {
            if (m_phase == PHASE_INTRO)
            {
                if (m_introTimer <= diff)
                {
                    ArenaYell(m_creature, SAY_RAZJAL_INTRO);
                    m_introTimer = 8 * IN_MILLISECONDS;
                    m_phase = PHASE_KATHZEN;
                }
                else
                    m_introTimer -= diff;

                return;
            }

            if (m_phase == PHASE_KATHZEN && m_introTimer)
            {
                if (m_introTimer <= diff)
                {
                    m_introTimer = 0;
                    StartKathzen();
                }
                else
                    m_introTimer -= diff;

                return;
            }

            if (m_phase == PHASE_KATHZEN)
            {
                if (m_nextPhaseTimer <= diff)
                    StartJuthza();
                else
                    m_nextPhaseTimer -= diff;

                return;
            }

            if (m_phase == PHASE_JUTHZA)
            {
                if (AreKathzenAndJuthzaDead() || m_nextPhaseTimer <= diff)
                    StartRazjal();
                else
                    m_nextPhaseTimer -= diff;

                return;
            }

            if (m_phase == PHASE_SCORPIDS)
            {
                if (m_scorpidStartTimer)
                {
                    if (m_scorpidStartTimer <= diff)
                        m_scorpidStartTimer = 0;
                    else
                    {
                        m_scorpidStartTimer -= diff;
                        return;
                    }
                }

                if (m_scorpidsSpawned < 10)
                {
                    if (m_scorpidSpawnTimer <= diff)
                    {
                        SpawnScorpid();
                        m_scorpidSpawnTimer = 2 * IN_MILLISECONDS;
                    }
                    else
                        m_scorpidSpawnTimer -= diff;
                }
                else
                    EndSandsIntermission();

                return;
            }

            if (m_phase != PHASE_RAZJAL)
                return;

            if (!m_sandsStarted && m_creature->GetHealthPercent() <= 50.0f)
            {
                StartSandsIntermission();
                return;
            }

            if (m_finalStarted && m_creature->IsNonMeleeSpellCasted(false))
                return;

            if (!m_finalStarted && m_creature->GetHealthPercent() <= 10.0f)
            {
                m_finalStarted = true;
                ArenaYell(m_creature, SAY_RAZJAL_FINAL);
                CastZeroCostHellfire(m_creature);
                return;
            }

            if (!m_creature->SelectHostileTarget() || !m_creature->GetVictim())
                return;

            if (!m_CreatureSpells.empty())
                UpdateSpellsList(diff);

            DoMeleeAttackIfReady();
        }
    };
}

CreatureAI* GetAI_npc_kathzen_the_brutal(Creature* creature)
{
    return new npc_kathzen_the_brutalAI(creature);
}

CreatureAI* GetAI_npc_juthza_the_cunning(Creature* creature)
{
    return new npc_juthza_the_cunningAI(creature);
}

CreatureAI* GetAI_npc_champion_razjal_the_quick(Creature* creature)
{
    return new npc_champion_razjal_the_quickAI(creature);
}

// The world DB may carry no gossip_menu / gossip_menu_option rows for Razjal
// (gossip_menu_id 0 in the base template; the 1.18 update only touches an
// option row that never existed there). Offer the challenge from the script,
// so the menu works with or without those rows (2026-09-05).
bool OnGossipHello_npc_champion_razjal_the_quick(Player* player, Creature* creature)
{
    if (npc_champion_razjal_the_quickAI* ai = dynamic_cast<npc_champion_razjal_the_quickAI*>(creature->AI()))
        if (ai->CanStartEvent())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I accept your challenge, Champion.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetObjectGuid());
    return true;
}

bool OnGossipSelect_npc_champion_razjal_the_quick(Player* player, Creature* creature, uint32 /*sender*/, uint32 /*action*/)
{
    player->CLOSE_GOSSIP_MENU();

    if (npc_champion_razjal_the_quickAI* ai = dynamic_cast<npc_champion_razjal_the_quickAI*>(creature->AI()))
    {
        if (!ai->CanStartEvent())
            return true;

        ai->m_starterGuid = player->GetGUID();
        ai->DoAction(ACTION_ARENA_START);
    }

    return true;
}

void AddSC_farraki_arena()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_kathzen_the_brutal";
    newscript->GetAI = &GetAI_npc_kathzen_the_brutal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_juthza_the_cunning";
    newscript->GetAI = &GetAI_npc_juthza_the_cunning;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_champion_razjal_the_quick";
    newscript->GetAI = &GetAI_npc_champion_razjal_the_quick;
    newscript->pGossipSelect = &OnGossipSelect_npc_champion_razjal_the_quick;
    newscript->pGossipHello = &OnGossipHello_npc_champion_razjal_the_quick;
    newscript->RegisterSelf();
}
