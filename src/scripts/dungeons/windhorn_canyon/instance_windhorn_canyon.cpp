#include "scriptPCH.h"
#include "windhorn_canyon.h"

struct instance_windhorn_canyon : public ScriptedInstance
{
    explicit instance_windhorn_canyon(Map* pMap) : ScriptedInstance(pMap)
    {
        Initialize();
    }

    uint32 m_auiEncounter[DATA_MAX_ENCOUNTER];
    std::vector<uint64> m_vGhostGatesGUID;

    void Initialize() override
    {
        for (uint8 i = 0; i < DATA_MAX_ENCOUNTER; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_vGhostGatesGUID.clear();
    }

    void OnObjectCreate(GameObject* pGo) override
    {
        if (pGo->GetEntry() == GO_GHOST_GATE)
        {
            m_vGhostGatesGUID.push_back(pGo->GetGUID());
            if (m_auiEncounter[DATA_SHALK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
        }
    }

    void OnCreatureDeath(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_BONESPEAKER_NARLGOM:
                SetData(DATA_NARLGOM, DONE);
                break;
            case NPC_CHIEFTAIN_SHALK:
                SetData(DATA_SHALK, DONE);
                break;
            case NPC_PATHUN_DUSKHIDE:
                SetData(DATA_PATHUN, DONE);
                break;
            case NPC_AMBASSADOR_VORTALUS:
                SetData(DATA_VORTALUS, DONE);
                break;
            case NPC_WALGAN_BLOODCALLER:
                SetData(DATA_WALGAN, DONE);
                break;
            case NPC_PROPHET_STORMHOOF:
                SetData(DATA_STORMHOOF, DONE);
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData) override
    {
        if (uiType < DATA_MAX_ENCOUNTER)
        {
            m_auiEncounter[uiType] = uiData;

            if (uiData == DONE && uiType == DATA_SHALK)
            {
                for (uint64 guid : m_vGhostGatesGUID)
                    if (GameObject* pGate = instance->GetGameObject(guid))
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

InstanceData* GetInstanceData_instance_windhorn_canyon(Map* pMap)
{
    return new instance_windhorn_canyon(pMap);
}

void AddSC_instance_windhorn_canyon()
{
    Script* pNewscript = new Script;
    pNewscript->Name = "instance_windhorn_canyon";
    pNewscript->GetInstanceData = &GetInstanceData_instance_windhorn_canyon;
    pNewscript->RegisterSelf();
}
