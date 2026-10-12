#include "scriptPCH.h"
#include "timbermaw_hold.h"

struct instance_timbermaw_hold : public ScriptedInstance
{
    explicit instance_timbermaw_hold(Map* pMap) : ScriptedInstance(pMap)
    {
        Initialize();
    }

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint8  m_uiBraziersCleansed;
    bool   m_bLegendaryDaggerClaimed;

    uint64 m_uiBarrierOfUrsolGUID;
    uint64 m_uiBrazierCorruptionGUID;
    uint64 m_uiBrazierDefilementGUID;
    uint64 m_uiBrazierDecayGUID;
    uint64 m_uiBrazierPutrificationGUID;

    uint64 m_uiUrsolGUID;
    uint64 m_uiPerotharnGUID;

    void Initialize() override
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_uiBraziersCleansed = 0;
        m_bLegendaryDaggerClaimed = false;

        m_uiBarrierOfUrsolGUID = 0;
        m_uiBrazierCorruptionGUID = 0;
        m_uiBrazierDefilementGUID = 0;
        m_uiBrazierDecayGUID = 0;
        m_uiBrazierPutrificationGUID = 0;

        m_uiUrsolGUID = 0;
        m_uiPerotharnGUID = 0;
    }

    void OnObjectCreate(GameObject* pGo) override
    {
        switch (pGo->GetEntry())
        {
            case GO_BARRIER_OF_URSOL:
                m_uiBarrierOfUrsolGUID = pGo->GetGUID();
                if (m_uiBraziersCleansed >= 4 || m_auiEncounter[TYPE_PARTATH] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_BRAZIER_CORRUPTION:
                m_uiBrazierCorruptionGUID = pGo->GetGUID();
                break;
            case GO_BRAZIER_DEFILEMENT:
                m_uiBrazierDefilementGUID = pGo->GetGUID();
                break;
            case GO_BRAZIER_DECAY:
                m_uiBrazierDecayGUID = pGo->GetGUID();
                break;
            case GO_BRAZIER_PUTRIFICATION:
                m_uiBrazierPutrificationGUID = pGo->GetGUID();
                break;
        }
    }

    void OnCreatureCreate(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_URSOL:
                m_uiUrsolGUID = pCreature->GetGUID();
                break;
            case NPC_PEROTHARN:
                m_uiPerotharnGUID = pCreature->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData) override
    {
        switch (uiType)
        {
            case TYPE_KARRSH:
            case TYPE_BEAR_DUO:
            case TYPE_ORMANOS:
            case TYPE_KRONN:
            case TYPE_LOKTANAG:
            case TYPE_TRIOCH:
            case TYPE_SELENAXX:
            case TYPE_PARTATH:
            case TYPE_URSOL:
            case TYPE_PEROTHARN:
                m_auiEncounter[uiType] = uiData;
                break;
            case DATA_BRAZIER_COUNT:
                m_uiBraziersCleansed = static_cast<uint8>(uiData);
                if (m_uiBraziersCleansed >= 4)
                {
                    if (GameObject* pBarrier = instance->GetGameObject(m_uiBarrierOfUrsolGUID))
                        pBarrier->SetGoState(GO_STATE_ACTIVE);
                }
                break;
            case DATA_DAGGER_CLAIMED:
                m_bLegendaryDaggerClaimed = (uiData == 1);
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;
            std::ostringstream saveStream;
            for (uint8 i = 1; i < MAX_ENCOUNTER; ++i)
                saveStream << m_auiEncounter[i] << " ";
            saveStream << static_cast<uint32>(m_uiBraziersCleansed);
            m_strInstData = saveStream.str();
            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    std::string m_strInstData;
    const char* Save() override { return m_strInstData.c_str(); }


    uint32 GetData(uint32 uiType) override
    {
        switch (uiType)
        {
            case TYPE_KARRSH:
            case TYPE_BEAR_DUO:
            case TYPE_ORMANOS:
            case TYPE_KRONN:
            case TYPE_LOKTANAG:
            case TYPE_TRIOCH:
            case TYPE_SELENAXX:
            case TYPE_PARTATH:
            case TYPE_URSOL:
            case TYPE_PEROTHARN:
                return m_auiEncounter[uiType];
            case DATA_BRAZIER_COUNT:
                return m_uiBraziersCleansed;
            case DATA_DAGGER_CLAIMED:
                return m_bLegendaryDaggerClaimed ? 1 : 0;
            default:
                return 0;
        }
    }

    void OnCreatureDeath(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_KARRSH_THE_SENTINEL:
                SetData(TYPE_KARRSH, DONE);
                break;
            case NPC_ROTGROWL:
            case NPC_KODIAK:
            {
                // Check if both bears are dead
                Creature* pOther = pCreature->FindNearestCreature(
                    pCreature->GetEntry() == NPC_ROTGROWL ? NPC_KODIAK : NPC_ROTGROWL, 100.0f, false);
                if (!pOther || !pOther->IsAlive())
                    SetData(TYPE_BEAR_DUO, DONE);
                break;
            }
            case NPC_ORMANOS_THE_CRACKED:
                SetData(TYPE_ORMANOS, DONE);
                break;
            case NPC_ARCHDRUID_KRONN:
                SetData(TYPE_KRONN, DONE);
                break;
            case NPC_LOKTANAG_THE_VILE:
                SetData(TYPE_LOKTANAG, DONE);
                break;
            case NPC_TRIOCH_THE_DEVOURER:
                SetData(TYPE_TRIOCH, DONE);
                break;
            case NPC_SELENAXX_FOULHEART:
                SetData(TYPE_SELENAXX, DONE);
                break;
            case NPC_CHIEFTAIN_PARTATH:
                SetData(TYPE_PARTATH, DONE);
                // Opening Barrier of Ursol upon Partath's fall
                if (GameObject* pBarrier = instance->GetGameObject(m_uiBarrierOfUrsolGUID))
                    pBarrier->SetGoState(GO_STATE_ACTIVE);
                break;
            case NPC_URSOL:
                SetData(TYPE_URSOL, DONE);
                break;
            case NPC_PEROTHARN:
                SetData(TYPE_PEROTHARN, DONE);
                break;
        }
    }

    void Load(char const* chrIn) override
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);
        std::istringstream loadStream(chrIn);
        for (uint8 i = 1; i < MAX_ENCOUNTER; ++i)
        {
            loadStream >> m_auiEncounter[i];
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }
        uint32 braziers = 0;
        loadStream >> braziers;
        m_uiBraziersCleansed = static_cast<uint8>(braziers);
        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_timbermaw_hold(Map* pMap)
{
    return new instance_timbermaw_hold(pMap);
}

// Brazier cleansing interaction
bool GOHello_go_brazier_timbermaw(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = static_cast<ScriptedInstance*>(pGo->GetInstanceData());
    if (!pInstance)
        return false;

    if (pGo->GetGoState() != GO_STATE_ACTIVE)
    {
        pGo->SetGoState(GO_STATE_ACTIVE);
        uint32 currentCount = pInstance->GetData(DATA_BRAZIER_COUNT);
        pInstance->SetData(DATA_BRAZIER_COUNT, currentCount + 1);
        ChatHandler(pPlayer).PSendSysMessage("%s has cleansed the corrupted brazier! (%u/4)", 
            pPlayer->GetName(), currentCount + 1);
    }
    return true;
}

void AddSC_instance_timbermaw_hold()
{
    Script* pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "instance_timbermaw_hold";
    pNewscript->GetInstanceData = &GetInstanceData_instance_timbermaw_hold;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "go_brazier_timbermaw";
    pNewscript->pGOHello = &GOHello_go_brazier_timbermaw;
    pNewscript->RegisterSelf();
}
