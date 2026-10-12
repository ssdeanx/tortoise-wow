#include "scriptPCH.h"
#include "frostmane_hollow.h"

struct instance_frostmane_hollow : public ScriptedInstance
{
    explicit instance_frostmane_hollow(Map* pMap) : ScriptedInstance(pMap)
    {
        Initialize();
    }

    uint32 m_auiEncounter[DATA_MAX_ENCOUNTER];
    uint64 m_uiGhostGateGUID;

    void Initialize() override
    {
        for (uint8 i = 0; i < DATA_MAX_ENCOUNTER; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_uiGhostGateGUID = 0;
    }

    void OnObjectCreate(GameObject* pGo) override
    {
        if (pGo->GetEntry() == GO_GHOST_GATE)
        {
            m_uiGhostGateGUID = pGo->GetGUID();
            if (m_auiEncounter[DATA_HAILAR] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
        }
    }

    void OnCreatureDeath(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_HAILAR_THE_FRIGID:
                SetData(DATA_HAILAR, DONE);
                break;
            case NPC_BATTLEMASTER_UBUKAZ:
                SetData(DATA_UBUKAZ, DONE);
                break;
            case NPC_HANDLER_OBOKA:
                SetData(DATA_OBOKA, DONE);
                break;
            case NPC_TANSHA_THE_SLEEK:
                SetData(DATA_TANSHA, DONE);
                break;
            case NPC_KANZA_THE_SEER:
                SetData(DATA_KANZA, DONE);
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData) override
    {
        if (uiType < DATA_MAX_ENCOUNTER)
        {
            m_auiEncounter[uiType] = uiData;

            if (uiData == DONE && uiType == DATA_HAILAR)
            {
                if (GameObject* pGate = instance->GetGameObject(m_uiGhostGateGUID))
                    pGate->SetGoState(GO_STATE_ACTIVE);
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

InstanceData* GetInstanceData_instance_frostmane_hollow(Map* pMap)
{
    return new instance_frostmane_hollow(pMap);
}

void AddSC_instance_frostmane_hollow()
{
    Script* pNewscript = new Script;
    pNewscript->Name = "instance_frostmane_hollow";
    pNewscript->GetInstanceData = &GetInstanceData_instance_frostmane_hollow;
    pNewscript->RegisterSelf();
}
