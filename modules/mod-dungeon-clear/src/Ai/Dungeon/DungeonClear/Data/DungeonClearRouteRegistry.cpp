/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "DungeonClearRouteRegistry.h"
#include <mutex>
#include "Config.h"
#include <cstdio>
#include <fstream>
#include <string>
#include <filesystem>
#include <system_error>

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"

// Generated collector (routes/RecordedRoutes.cpp). Declared at file scope:
// inside the anonymous namespace it would get internal linkage and never
// find its definition.
void RegisterAllRecordedRoutes();

// Reads modules/mod-dungeon-clear/src/Routes/*.route (written by
// DcRouteRecorder alongside its .cpp twin) and registers each one. This is
// what makes a route usable after a plain restart instead of a rebuild.
static void LoadRecordedRoutesFromDisk()
{
    std::string dir = sConfig.GetStringDefault("DungeonClear.RouteRecorderDir", "");
    if (dir.empty())
        return;

    uint32 loaded = 0;
    // std::filesystem statt dirent.h: MSVC kennt dirent nicht, und der
    // Iterator spart den manuellen Endungsvergleich samt closedir.
    std::error_code ec;
    std::filesystem::directory_iterator it(dir, ec), ende;
    if (!ec)
    {
        for (; it != ende; it.increment(ec))
        {
            if (ec)
                break;
            if (!it->is_regular_file(ec) || ec)
                continue;
            // .route  = the LEARNED slot; the recorder owns and rewrites it.
            // .fallback = the hand-authored net; the recorder never touches it,
            //             so the two can coexist for the same boss.
            bool const isFallback = it->path().extension() == ".fallback";
            if (it->path().extension() != ".route" && !isFallback)
                continue;
            std::ifstream in(it->path());
            if (!in.is_open())
                continue;
            std::string header;
            std::getline(in, header);
            uint32 mapId = 0, bossEntry = 0;
            if (std::sscanf(header.c_str(), "# map %u boss %u", &mapId, &bossEntry) != 2)
                continue;
            std::vector<WaypointHint> hints;
            // "x y z" or "x y z flags". The fourth field is an AnchorFlag
            // bitmask (JUMP_DOWN 1, JUMP_GAP 2, DOOR_AHEAD 4, PIVOT_TIGHT 8);
            // absent means none, so every existing file reads unchanged.
            //
            // Without it a route could not express a drop, and a leg you can
            // only reach by falling was unreachable by construction - the
            // pathfinder correctly reports "no navigable route" for it and
            // will not invent a jump. Dragonmaw Retreat's Cavernweb Broodmother
            // is exactly that leg.
            std::string line;
            while (std::getline(in, line))
            {
                if (line.empty() || line[0] == '#')
                    continue;
                float x = 0.0f, y = 0.0f, z = 0.0f;
                unsigned int flags = 0;
                int const got = std::sscanf(line.c_str(), "%f %f %f %u", &x, &y, &z, &flags);
                if (got < 3)
                    continue;
                hints.push_back(WaypointHint{x, y, z, 0,
                                             static_cast<uint16>(got >= 4 ? flags : 0u), 6.0f});
            }
            // Say so out loud. A file under the minimum used to be read,
            // counted in nothing, and silently never registered - a two-point
            // Garlok route looked loaded (the total went up by one, because the
            // total counts files that made it PAST here) and was simply never
            // used. That cost a build and a measurement window to find by
            // comparing log labels against a route that did work.
            if (hints.size() < 3)
            {
                LOG_INFO("playerbots.dungeonclear",
                         "[DC-ROUTE] IGNORING {}: map {} boss {} has only {} waypoint(s), "
                         "the minimum is 3",
                         it->path().filename().string(), mapId, bossEntry, int(hints.size()));
                continue;
            }
            {
                if (isFallback)
                {
                    DungeonClearRouteRegistry::RegisterFallback(
                        mapId, DUNGEON_DIFFICULTY_NORMAL, bossEntry, std::move(hints));
                    LOG_INFO("playerbots.dungeonclear",
                             "[DC-ROUTE] map {} boss {}: hand-authored FALLBACK loaded "
                             "(the recorder still owns the learned route)",
                             mapId, bossEntry);
                    ++loaded;
                    continue;
                }
                // "pinned" anywhere in the header marks a route the recorder may
                // neither replace nor discard. Pin BEFORE registering, so the
                // very first Register is the one that sticks.
                bool const pinned = header.find("pinned") != std::string::npos;
                if (pinned)
                    DungeonClearRouteRegistry::Pin(mapId, DUNGEON_DIFFICULTY_NORMAL, bossEntry);
                DungeonClearRouteRegistry::Register(mapId, DUNGEON_DIFFICULTY_NORMAL,
                                                    bossEntry, std::move(hints));
                if (pinned)
                    LOG_INFO("playerbots.dungeonclear",
                             "[DC-ROUTE] map {} boss {}: route is PINNED (recorder will not touch it)",
                             mapId, bossEntry);
                ++loaded;
            }
        }
    }
    if (loaded)
        LOG_INFO("playerbots.dungeonclear",
                 "[DC-ROUTE] loaded {} recorded route(s) from {}", loaded, dir);
}

namespace
{
    // One-time seed of the hand-authored routes.
    //
    // The per-dungeon appenders are called EXPLICITLY, for the same reason the
    // event and roster tables do it (see DungeonEventTables.h): the module is a
    // static lib, and a translation unit whose only output is constructor side
    // effects — which is what the "static Register instance" pattern this header
    // used to describe would be — is dropped by the linker along with its rows.
    //
    // Seeded lazily from Get() rather than from a namespace-scope initialiser so
    // it cannot race the Store() static's own construction. Register() is still
    // callable directly; the unit tests use it with synthetic map ids.
    void SeedAuthoredRoutes()
    {
        static bool const seeded = []
        {
            RegisterAzjolNerubRoute();
            // Everything the route recorder captured from live clears (see
            // modules/mod-dungeon-clear/routes/). Generated collector; a
            // recorded route only becomes live once it is called from here.
            RegisterAllRecordedRoutes();
            // ...and then whatever the recorder has captured SINCE that build.
            // Loaded last so a freshly recorded (and, by the recorder's own
            // shortest-wins rule, better) route wins over the compiled one.
            LoadRecordedRoutesFromDisk();
            return true;
        }();
        (void)seeded;
    }
}

std::mutex& DungeonClearRouteRegistry::RegistryLock()
{
    static std::mutex instance;
    return instance;
}

std::unordered_map<DungeonClearRouteRegistry::Key, std::vector<WaypointHint>, DungeonClearRouteRegistry::KeyHash>&
DungeonClearRouteRegistry::Store()
{
    static std::unordered_map<Key, std::vector<WaypointHint>, KeyHash> instance;
    return instance;
}

std::unordered_map<DungeonClearRouteRegistry::Key, std::vector<WaypointHint>,
                   DungeonClearRouteRegistry::KeyHash>&
DungeonClearRouteRegistry::FallbackStore()
{
    static std::unordered_map<Key, std::vector<WaypointHint>, KeyHash> instance;
    return instance;
}

void DungeonClearRouteRegistry::RegisterFallback(uint32 mapId, Difficulty difficulty,
                                                 uint32 bossEntry,
                                                 std::vector<WaypointHint> hints)
{
    std::lock_guard<std::mutex> lock(RegistryLock());
    // Always replaces: the .fallback FILE is the source of truth for this slot,
    // so a reload picks up an edited net without a special case.
    FallbackStore()[Key{mapId, difficulty, bossEntry}] = std::move(hints);
}

std::unordered_set<DungeonClearRouteRegistry::Key, DungeonClearRouteRegistry::KeyHash>&
DungeonClearRouteRegistry::PinnedSet()
{
    static std::unordered_set<Key, KeyHash> instance;
    return instance;
}

void DungeonClearRouteRegistry::Pin(uint32 mapId, Difficulty difficulty, uint32 bossEntry)
{
    std::lock_guard<std::mutex> lock(RegistryLock());
    PinnedSet().insert(Key{mapId, difficulty, bossEntry});
}

bool DungeonClearRouteRegistry::IsPinned(uint32 mapId, Difficulty difficulty, uint32 bossEntry)
{
    std::lock_guard<std::mutex> lock(RegistryLock());
    return PinnedSet().count(Key{mapId, difficulty, bossEntry}) != 0;
}

void DungeonClearRouteRegistry::Register(uint32 mapId, Difficulty difficulty, uint32 bossEntry,
                                         std::vector<WaypointHint> hints)
{
    std::lock_guard<std::mutex> lock(RegistryLock());
    Key const key{mapId, difficulty, bossEntry};
    // A pinned route is never replaced. The recorder's shortest-wins rule is
    // right for ordinary ground and wrong for a ledge: "shorter" there usually
    // means the line was cut across the water the ledge exists to avoid.
    if (PinnedSet().count(key) && !Store()[key].empty())
        return;
    Store()[key] = std::move(hints);
}

bool DungeonClearRouteRegistry::Forget(uint32 mapId, Difficulty difficulty, uint32 bossEntry)
{
    SeedAuthoredRoutes();
    std::lock_guard<std::mutex> lock(RegistryLock());
    Key const key{mapId, difficulty, bossEntry};
    // A pinned route survives the stuck ladder. Returning false here also stops
    // the caller from renaming the files to .bad (DcRouteRecorder::DiscardRoute
    // runs only when Forget reported a removal), so the decision holds across
    // restarts instead of only until the next wedge.
    if (PinnedSet().count(key))
    {
        LOG_INFO("playerbots.dungeonclear",
                 "[DC-ROUTE] keeping PINNED route for map {} boss {} despite a stuck ladder",
                 mapId, bossEntry);
        return false;
    }
    if (Store().erase(key) > 0)
    {
        if (FallbackStore().count(key))
            LOG_INFO("playerbots.dungeonclear",
                     "[DC-ROUTE] dropped the learned route for map {} boss {} — "
                     "falling back to the hand-authored net",
                     mapId, bossEntry);
        return true;
    }
    // The net is NEVER dropped. It was droppable for exactly one measurement
    // window and that was a design error: Dragonmaw's descent wedges on nearly
    // every attempt - the pin it replaced refused 442 discards in forty minutes
    // - so "drop it if it wedges too" threw the route away within minutes of
    // the first run and left 590 path builds with no route at all. Two windows,
    // ~95 minutes, zero kills on a boss that had fallen five times the hour
    // before. A net that can be discarded is not a net.
    //
    // Returning FALSE here also keeps DcRouteRecorder::DiscardRoute from
    // renaming the .fallback file to .bad, so the hand-authored line survives
    // restarts as well - the same protection the pin gave.
    if (FallbackStore().count(key))
    {
        LOG_INFO("playerbots.dungeonclear",
                 "[DC-ROUTE] keeping the hand-authored FALLBACK for map {} boss {} "
                 "despite a stuck ladder",
                 mapId, bossEntry);
        return false;
    }
    return false;
}

bool DungeonClearRouteRegistry::Has(uint32 mapId, Difficulty difficulty, uint32 bossEntry)
{
    // Seed BEFORE taking the lock: seeding registers, and Register() takes
    // this same lock.
    SeedAuthoredRoutes();
    std::lock_guard<std::mutex> lock(RegistryLock());
    Key const key{mapId, difficulty, bossEntry};
    auto const it = Store().find(key);
    if (it != Store().end() && !it->second.empty())
        return true;
    auto const fb = FallbackStore().find(key);
    return fb != FallbackStore().end() && !fb->second.empty();
}

bool DungeonClearRouteRegistry::TryGet(uint32 mapId, Difficulty difficulty, uint32 bossEntry,
                                       std::vector<WaypointHint>& out)
{
    // Same order as Has(): seed first, lock second.
    SeedAuthoredRoutes();
    std::lock_guard<std::mutex> lock(RegistryLock());
    Key const key{mapId, difficulty, bossEntry};
    auto const it = Store().find(key);
    if (it != Store().end() && !it->second.empty())
    {
        out = it->second;
        return true;
    }
    // Learned route gone (never recorded, or dropped by the stuck ladder):
    // hand the party the hand-authored net instead of nothing.
    auto const fb = FallbackStore().find(key);
    if (fb == FallbackStore().end() || fb->second.empty())
        return false;
    out = fb->second;
    return true;
}

