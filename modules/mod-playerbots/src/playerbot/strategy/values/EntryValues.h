#pragma once

#include "playerbot/playerbot.h"
#include "playerbot/strategy/Value.h"
#include "ScriptedInstance.h"

namespace ai
{
    class MCRunesValue : public StringCalculatedValue, public Qualified
    {
    public:
        MCRunesValue(PlayerbotAI* ai, std::string name = "mc runes") : StringCalculatedValue(ai, name, 1), Qualified() {}

        virtual std::string Calculate() override { return "176951,176952,176953,176954,176955,176956,176957"; };

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "mc runes"; } //Must equal iternal name
        virtual std::string GetHelpTypeName() { return "entry"; }
        virtual std::string GetHelpDescription()
        {
            return "This value contains the entries of the Molten Core runes.";
        }
        virtual std::vector<std::string> GetUsedValues() { return { }; }
#endif 
    };

    // Rune GO entries whose boss encounter is DONE. The core's GOHello_go_rune_MC
    // blocks dousing a live rune (src/scripts/dungeons/molten_core/instance_molten_core.cpp),
    // but without this filter bots walked to and camped on undouseable runes all run.
    // Rune entry -> molten_core.h TYPE_* index mapping matches the instance's
    // OnObjectCreate switch exactly.
    class MCDouseableRunesValue : public StringCalculatedValue, public Qualified
    {
    public:
        MCDouseableRunesValue(PlayerbotAI* ai, std::string name = "mc douseable runes") : StringCalculatedValue(ai, name, 1), Qualified() {}

        virtual std::string Calculate() override
        {
            // molten_core.h TYPE_* indices (not on the module include path):
            // SULFURON=0, GEDDON=1, SHAZZRAH=2, GOLEMAGG=3, GARR=4, MAGMADAR=5, GEHENNAS=6
            static const std::pair<uint32, uint32> runeBossTypes[] =
            {
                {176951, 0}, // Sulfuron
                {176952, 1}, // Geddon
                {176953, 2}, // Shazzrah
                {176954, 3}, // Golemagg
                {176955, 4}, // Garr
                {176956, 5}, // Magmadar
                {176957, 6}, // Gehennas
            };

            Map* map = bot->GetMap();
            if (!map || map->GetId() != 409)
                return "";

            InstanceData* instance = map->GetInstanceData();
            if (!instance)
                return "";

            std::string result;
            for (auto const& runeBossType : runeBossTypes)
            {
                if (instance->GetData(runeBossType.second) == DONE)
                {
                    if (!result.empty())
                        result += ",";

                    result += std::to_string(runeBossType.first);
                }
            }

            return result;
        }

#ifdef GenerateBotHelp
        virtual std::string GetHelpName() { return "mc douseable runes"; } //Must equal iternal name
        virtual std::string GetHelpTypeName() { return "entry"; }
        virtual std::string GetHelpDescription()
        {
            return "This value contains the entries of the Molten Core runes that can currently be doused (their boss is dead).";
        }
        virtual std::vector<std::string> GetUsedValues() { return { }; }
#endif 
    };
}

class SuppressionDevicesValue : public StringCalculatedValue, public Qualified
{
public:
    SuppressionDevicesValue(PlayerbotAI* ai, std::string name = "suppression devices") : StringCalculatedValue(ai, name, 1), Qualified() {}

    virtual std::string Calculate() override { return "179784"; }

#ifdef GenerateBotHelp
    virtual std::string GetHelpName() { return "suppression devices"; }
    virtual std::string GetHelpTypeName() { return "entry"; }
    virtual std::string GetHelpDescription()
    {
        return "This value contains the entries of the Blackwing Lair Suppression Devices.";
    }
    virtual std::vector<std::string> GetUsedValues() { return { }; }
#endif 
};