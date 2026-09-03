#pragma once

// Intentionally empty. cmangos uses ScriptDevAIMgr as a dispatcher for the
// ScriptDev2 script suite; Penqle's equivalent is ScriptMgr in src/game/.
// Bot module sites #including this header resolve their call sites against
// Penqle's ScriptMgr via the compat shim.
