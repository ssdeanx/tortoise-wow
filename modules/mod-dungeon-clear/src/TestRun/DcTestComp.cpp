/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "TestRun/DcTestComp.h"

#include <set>

namespace DcTestComp
{
    namespace
    {
        // splitmix32: a tiny, self-contained, deterministic PRNG. We can't lean
        // on the project's global urand()/rand32() here because BuildComp must
        // be PURE (same seed -> same comp, for replay and unit tests); those
        // draw from shared world state. This is comp selection, not gameplay
        // randomness, so distribution quality is irrelevant.
        std::uint32_t NextRand(std::uint32_t& state)
        {
            state += 0x9e3779b9u;
            std::uint32_t z = state;
            z = (z ^ (z >> 16)) * 0x21f0aaadu;
            z = (z ^ (z >> 15)) * 0x735a2d97u;
            return z ^ (z >> 15);
        }

        // Draw one entry from `pool` whose class is not already used, advancing
        // the PRNG. Distinct classes are a PREFERENCE, not a constraint: at the
        // five-slot default the preference always wins (nine pooled classes, five
        // seats), so this is the historical draw and the five-slot comp is
        // bit-identical for a given seed. A raid-sized comp runs out of unused
        // classes partway through, and then it draws from the whole pool rather
        // than pinning every remaining seat to pool[0] — which for the DPS pool
        // would make the entire back line fury warriors.
        Slot Draw(Slot const* pool, std::size_t count, std::set<std::uint8_t> const& used,
                  std::uint32_t& state)
        {
            std::vector<Slot> candidates;
            candidates.reserve(count);
            for (std::size_t i = 0; i < count; ++i)
                if (used.find(pool[i].classId) == used.end())
                    candidates.push_back(pool[i]);

            // Defensive: should never happen given the pool sizes, but returning
            // a valid slot beats indexing an empty vector.
            if (candidates.empty())
                return count ? pool[NextRand(state) % count] : pool[0];

            return candidates[NextRand(state) % candidates.size()];
        }
    }

    std::vector<Slot> BuildComp(std::uint32_t seed, std::size_t size)
    {
        std::vector<Slot> comp;
        if (size < 2)
            return comp;  // a run needs at least the tank + healer pair

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

        // Defensive cap so tanks + healers never exceed total slots
        if (numTanks + numHealers >= size)
        {
            numTanks = 1;
            numHealers = 1;
        }

        std::uint32_t state = seed;
        std::set<std::uint8_t> used;
        comp.resize(size);

        std::size_t slotIdx = 0;
        for (std::size_t i = 0; i < numTanks; ++i)
        {
            comp[slotIdx] = Draw(kTankPool, std::size(kTankPool), used, state);
            used.insert(comp[slotIdx].classId);
            ++slotIdx;
        }

        for (std::size_t i = 0; i < numHealers; ++i)
        {
            comp[slotIdx] = Draw(kHealPool, std::size(kHealPool), used, state);
            used.insert(comp[slotIdx].classId);
            ++slotIdx;
        }

        while (slotIdx < size)
        {
            comp[slotIdx] = Draw(kDpsPool, std::size(kDpsPool), used, state);
            used.insert(comp[slotIdx].classId);
            ++slotIdx;
        }

        return comp;
    }

    std::vector<Slot> RolePool(std::string_view role)
    {
        if (role == "tank")
            return { std::begin(kTankPool), std::end(kTankPool) };
        if (role == "heal")
            return { std::begin(kHealPool), std::end(kHealPool) };
        if (role == "dps")
            return { std::begin(kDpsPool), std::end(kDpsPool) };
        return {};
    }
}
