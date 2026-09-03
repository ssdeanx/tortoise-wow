# cmangos/playerbots as a module.
#
# Vendored bot AI, moved here from src/modules/PlayerBots so it lives beside the
# other modules instead of inside the core tree. The decoupling was already done
# long before the move: everything the bots reach the core through is a ScriptMgr
# hook, and src/game/PlayerbotStubs.cpp covers the handful of genuinely core-side
# symbols for BUILD_PLAYERBOTS=OFF builds. What moved is the BUILD, not the wiring.
#
# BUILD_PLAYERBOTS keeps its old meaning and stays the single switch: it still
# gates the stub file in src/game/CMakeLists.txt, so returning early here is what
# makes "module absent" and "stubs present" agree.
if(NOT BUILD_PLAYERBOTS)
  return()
endif()

# Static modules are aggregated into ONE `modules` library (modules/CMakeLists.txt),
# so these sources compile beside mod-dungeon-clear's. That is fine here and was
# checked before the move: the vendor's three feature macros (CMANGOS,
# MANGOSBOT_ZERO, ENABLE_PLAYERBOTS) appear in 32/126/1 files inside this tree and
# in ZERO files anywhere else, so putting them on the shared target changes
# nothing for the other module.
#
# Only playerbot/ and ahbot/ carry sources - 458 of the tree's 459 .cpp files. The
# one outlier is botpch.cpp, the precompiled-header source, deliberately left out:
# the aggregate target has no per-module PCH, and a PCH is a build-time
# optimisation, not semantics.
# Under src/, and that is not cosmetic: ConfigureModules.cmake recognises a
# directory as a module only when modules/<name>/src exists
# (GetPathToModuleSource appends "/src"). With the vendor layout kept as-is the
# module configured cleanly, reported no error, and contributed ZERO object
# files - the discovery glob simply never saw it.
#
# botpch.cpp stays OUTSIDE src/ so these globs cannot pick it up.
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/playerbot")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/ahbot")
# The Addmod_playerbotsScripts() entry point the generated loader calls.
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/loader")
