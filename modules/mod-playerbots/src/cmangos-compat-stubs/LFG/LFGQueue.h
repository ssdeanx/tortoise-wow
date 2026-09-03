#pragma once

// Intentionally empty. cmangos has a separate LFGQueue class; Penqle's LFGMgr
// owns the queue state directly. The bot module's LFGQueue references resolve
// through this empty header to no-op stubs in the compat shim.
