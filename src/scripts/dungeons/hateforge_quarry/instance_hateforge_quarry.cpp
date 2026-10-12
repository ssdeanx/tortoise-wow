#include "scriptPCH.h"
#include "hateforge_quarry.h"

struct instance_hateforge_quarry : public ScriptedInstance
{
    explicit instance_hateforge_quarry(Map* pMap) : ScriptedInstance(pMap)
    {
        Initialize();
    }

    uint32 m_auiEncounter[DATA_MAX_ENCOUNTER];
    std::vector<uint64> m_vWorkDoorsGUID;
    std::vector<uint64> m_vEntryDoorsGUID;
    std::vector<uint64> m_vJusticeDoorsGUID;

    void Initialize() override
    {
        for (uint8 i = 0; i < DATA_MAX_ENCOUNTER; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_vWorkDoorsGUID.clear();
        m_vEntryDoorsGUID.clear();
        m_vJusticeDoorsGUID.clear();
    }

    void OnObjectCreate(GameObject* pGo) override
    {
        switch (pGo->GetEntry())
        {
            case GO_DOOR_NO_ENTRY:
                m_vEntryDoorsGUID.push_back(pGo->GetGUID());
                if (m_auiEncounter[DATA_HIGH_FOREMAN_BARGUL] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_WORK_HARD:
                m_vWorkDoorsGUID.push_back(pGo->GetGUID());
                if (m_auiEncounter[DATA_ENGINEER_FIGGLES] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_JUSTICE:
                m_vJusticeDoorsGUID.push_back(pGo->GetGUID());
                if (m_auiEncounter[DATA_HARGESH_DOOMCALLER] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
        }
    }

    void OnCreatureDeath(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_HIGH_FOREMAN_BARGUL:
                SetData(DATA_HIGH_FOREMAN_BARGUL, DONE);
                break;
            case NPC_ENGINEER_FIGGLES:
                SetData(DATA_ENGINEER_FIGGLES, DONE);
                break;
            case NPC_HARGESH_DOOMCALLER:
                SetData(DATA_HARGESH_DOOMCALLER, DONE);
                break;
            case NPC_HATEREAVER_ANNIHILATOR:
                SetData(DATA_HATEREAVER_ANNIHILATOR, DONE);
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
                if (uiType == DATA_HIGH_FOREMAN_BARGUL)
                {
                    for (uint64 guid : m_vEntryDoorsGUID)
                        if (GameObject* pDoor = instance->GetGameObject(guid))
                            pDoor->SetGoState(GO_STATE_ACTIVE);
                }
                else if (uiType == DATA_ENGINEER_FIGGLES)
                {
                    for (uint64 guid : m_vWorkDoorsGUID)
                        if (GameObject* pDoor = instance->GetGameObject(guid))
                            pDoor->SetGoState(GO_STATE_ACTIVE);
                }
                else if (uiType == DATA_HARGESH_DOOMCALLER)
                {
                    for (uint64 guid : m_vJusticeDoorsGUID)
                        if (GameObject* pDoor = instance->GetGameObject(guid))
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

InstanceData* GetInstanceData_instance_hateforge_quarry(Map* pMap)
{
    return new instance_hateforge_quarry(pMap);
}

void AddSC_instance_hateforge_quarry()
{
    Script* pNewscript = new Script;
    pNewscript->Name = "instance_hateforge_quarry";
    pNewscript->GetInstanceData = &GetInstanceData_instance_hateforge_quarry;
    pNewscript->RegisterSelf();
}
