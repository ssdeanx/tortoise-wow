#include "scriptPCH.h"
#include "upper_karazhan_halls.h"

struct instance_upper_karazhan_halls : public ScriptedInstance
{
    explicit instance_upper_karazhan_halls(Map* pMap) : ScriptedInstance(pMap)
    {
        Initialize();
    }

    uint32 m_auiEncounter[DATA_MAX_ENCOUNTER];
    uint64 m_uiDiningDoor1GUID;
    uint64 m_uiDiningDoor2GUID;
    uint64 m_uiLordaeronDoorGUID;

    void Initialize() override
    {
        for (uint8 i = 0; i < DATA_MAX_ENCOUNTER; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_uiDiningDoor1GUID = 0;
        m_uiDiningDoor2GUID = 0;
        m_uiLordaeronDoorGUID = 0;
    }

    void OnObjectCreate(GameObject* pGo) override
    {
        switch (pGo->GetEntry())
        {
            case GO_KARA_DINING_DOORS:
                m_uiDiningDoor1GUID = pGo->GetGUID();
                if (m_auiEncounter[DATA_KINGS_COUNCIL] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_KARA_DINING_RUIN_DOOR:
                m_uiDiningDoor2GUID = pGo->GetGUID();
                if (m_auiEncounter[DATA_SANV_TASDAL] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_PVP_LORDAERON_DOOR:
                m_uiLordaeronDoorGUID = pGo->GetGUID();
                if (m_auiEncounter[DATA_KRUUL] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
        }
    }

    void OnCreatureDeath(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_RUPTURAN_THE_BROKEN:
                SetData(DATA_RUPTURAN, DONE);
                break;
            case NPC_KARA_KING:
                SetData(DATA_KINGS_COUNCIL, DONE);
                break;
            case NPC_SANV_TASDAL:
                SetData(DATA_SANV_TASDAL, DONE);
                break;
            case NPC_KRUUL:
                SetData(DATA_KRUUL, DONE);
                break;
            case NPC_KEEPER_GNARLMOON:
                SetData(DATA_KEEPER_GNARLMOON, DONE);
                break;
            case NPC_LEY_WATCHER_INCANTAGOS:
                SetData(DATA_INCANTAGOS, DONE);
                break;
            case NPC_ANOMALUS:
                SetData(DATA_ANOMALUS, DONE);
                break;
            case NPC_ECHO_OF_MEDIVH:
                SetData(DATA_ECHO_OF_MEDIVH, DONE);
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData) override
    {
        if (uiType < DATA_MAX_ENCOUNTER)
        {
            m_auiEncounter[uiType] = uiData;

            if (uiData == DONE)
            {
                if (uiType == DATA_KINGS_COUNCIL)
                {
                    if (GameObject* pDoor = instance->GetGameObject(m_uiDiningDoor1GUID))
                        pDoor->SetGoState(GO_STATE_ACTIVE);
                }
                else if (uiType == DATA_SANV_TASDAL)
                {
                    if (GameObject* pDoor = instance->GetGameObject(m_uiDiningDoor2GUID))
                        pDoor->SetGoState(GO_STATE_ACTIVE);
                }
                else if (uiType == DATA_KRUUL)
                {
                    if (GameObject* pDoor = instance->GetGameObject(m_uiLordaeronDoorGUID))
                        pDoor->SetGoState(GO_STATE_ACTIVE);
                }
            }
        }
    }

    uint32 GetData(uint32 uiType) override
    {
        if (uiType < DATA_MAX_ENCOUNTER)
            return m_auiEncounter[uiType];

        return 0;
    }
};

InstanceData* GetInstanceData_instance_upper_karazhan_halls(Map* pMap)
{
    return new instance_upper_karazhan_halls(pMap);
}

void AddSC_instance_upper_karazhan_halls()
{
    Script* pNewscript = new Script;
    pNewscript->Name = "instance_upper_karazhan_halls";
    pNewscript->GetInstanceData = &GetInstanceData_instance_upper_karazhan_halls;
    pNewscript->RegisterSelf();
}
