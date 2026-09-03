#pragma once

// Intentionally empty. cmangos uses WorldState as a per-tick state registry;
// Penqle keeps the equivalent in World.cpp + GameEventMgr. PlayerbotAIConfig.cpp
// and RandomPlayerbotMgr.cpp #include this unconditionally; the call sites
// resolve against substitutes in the compat shim.
