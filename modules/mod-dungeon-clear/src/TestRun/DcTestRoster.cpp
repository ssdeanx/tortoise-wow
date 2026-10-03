/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "TestRun/DcTestRoster.h"

#include <algorithm>
#include <cctype>

#include "TestRun/DcTestComp.h"

namespace DcTestRoster
{
    namespace
    {
        std::string Trim(std::string const& in)
        {
            std::size_t const b = in.find_first_not_of(" \t");
            if (b == std::string::npos)
                return {};
            std::size_t const e = in.find_last_not_of(" \t");
            return in.substr(b, e - b + 1);
        }

        std::string Lower(std::string s)
        {
            std::transform(s.begin(), s.end(), s.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            return s;
        }

        // The role list a wrong-count refusal quotes: "tank,heal,dps,dps,dps" for
        // the five-slot default every run used, and a summary of scaled roles for
        // longer ones.
        std::string RoleSummary(std::size_t size)
        {
            std::size_t numTanks = 1;
            std::size_t numHealers = 1;

            if (size >= 26)
            {
                numTanks = 3;
                numHealers = 8;
            }
            else if (size >= 16)
            {
                numTanks = 2;
                numHealers = 4;
            }
            else if (size >= 6)
            {
                numTanks = 2;
                numHealers = 2;
            }

            if (numTanks + numHealers >= size)
            {
                numTanks = 1;
                numHealers = 1;
            }

            std::size_t const numDps = size > (numTanks + numHealers) ? size - numTanks - numHealers : 0;
            if (size <= DcTestComp::kPartySize)
            {
                std::string out = "tank,heal";
                for (std::size_t i = 2; i < size; ++i)
                    out += ",dps";
                return out;
            }
            return std::to_string(numTanks) + " tank, " + std::to_string(numHealers) + " heal, " + std::to_string(numDps) + " dps";
        }
    }

    char const* RoleForIndex(std::size_t i, std::size_t totalSize)
    {
        std::size_t numTanks = 1;
        std::size_t numHealers = 1;

        if (totalSize >= 26)
        {
            numTanks = 3;
            numHealers = 8;
        }
        else if (totalSize >= 16)
        {
            numTanks = 2;
            numHealers = 4;
        }
        else if (totalSize >= 6)
        {
            numTanks = 2;
            numHealers = 2;
        }

        if (numTanks + numHealers >= totalSize)
        {
            numTanks = 1;
            numHealers = 1;
        }

        if (i < numTanks)
            return "tank";
        if (i < numTanks + numHealers)
            return "heal";
        return "dps";
    }

    Result Parse(std::string const& spec, std::size_t size)
    {
        // The worked example is the five-slot one people have always typed, plus
        // an ellipsis when the row asks for more. A refusal that says "needs 20
        // names" and then shows five is worse than no example at all.
        std::string const example = size > DcTestComp::kPartySize
                                        ? "party=Tankname,Healname,Dps1,Dps2,Dps3,..."
                                        : "party=Tankname,Healname,Dps1,Dps2,Dps3";

        std::vector<std::string> names;
        std::size_t from = 0;
        while (from <= spec.size())
        {
            std::size_t const comma = spec.find(',', from);
            std::string const piece =
                Trim(comma == std::string::npos ? spec.substr(from) : spec.substr(from, comma - from));
            // Skip empties rather than counting them: a trailing comma or a
            // double separator is a typo in the list, not a sixth member.
            if (!piece.empty())
                names.push_back(piece);
            if (comma == std::string::npos)
                break;
            from = comma + 1;
        }

        if (names.empty())
            return {Kind::Empty, {},
                    "party= needs " + std::to_string(size) + " character names, e.g. " + example};

        // Names the row asked for, and the role each one lands in.
        if (names.size() != size)
            return {Kind::WrongCount, {},
                    "party= needs exactly " + std::to_string(size) +
                        " names (" + RoleSummary(size) + ") — got " + std::to_string(names.size())};

        for (std::size_t i = 0; i < names.size(); ++i)
            for (std::size_t j = i + 1; j < names.size(); ++j)
                if (Lower(names[i]) == Lower(names[j]))
                    return {Kind::Duplicate, {},
                            "'" + names[i] + "' is named twice — one character cannot fill two slots"};

        Result out{Kind::Ok, {}, {}};
        out.members.reserve(names.size());
        for (std::size_t i = 0; i < names.size(); ++i)
            out.members.push_back({names[i], RoleForIndex(i, size)});
        return out;
    }
}
