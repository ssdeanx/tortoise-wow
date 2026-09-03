#pragma once

// Intentionally empty. cmangos's vendored bot module #includes "LFGDefines.h"
// for queue-state constants; Penqle's LFGMgr.h owns that data directly. This
// stub lets the include resolve so the surrounding TUs compile.
