# Included from modules/CMakeLists.txt AFTER the module targets exist.
if(NOT BUILD_PLAYERBOTS)
  return()
endif()

set(PB_ROOT "${CMAKE_SOURCE_DIR}/modules/mod-playerbots")

# Boost. The vendored sources reach for it directly (TravelNode's mmap scan uses
# boost::filesystem::directory_iterator) and botpch.h used to carry the headers;
# the old vendor CMakeLists did the find_package and the linking. Both have to
# come along, or the build fails deep inside a source file with
# "'boost::filesystem' has not been declared".
find_package(Boost 1.70 REQUIRED COMPONENTS thread filesystem system)

# Both linkage modes: the static path folds this module into `modules`, the
# dynamic path gives it a target of its own. That target is `mod_mod_playerbots`
# with UNDERSCORES - the module system replaces the hyphens in `mod-playerbots`.
# I had written `mod_mod-playerbots` here, which matches nothing, so the dynamic
# build silently received no defines, no shim and no Boost and died in
# WorldPosition.h on `'discrete_distribution' is not a member of 'std'`. Both
# spellings are listed because a target that does not exist is skipped anyway.
foreach(PB_TARGET modules mod_mod_playerbots mod_mod-playerbots)
  if(NOT TARGET ${PB_TARGET})
    continue()
  endif()

  #   CMANGOS         - selects the cmangos codepath in vendored headers
  #                     (vs. TrinityCore / MaNGOS-Zero alternates).
  #   MANGOSBOT_ZERO  - Classic (1.12). Switches level caps, talent trees,
  #                     spell ranges. MANGOSBOT_ONE for TBC, _TWO for WotLK.
  #   ENABLE_PLAYERBOTS - the vendor tree's own on/off wall.
  target_compile_definitions(${PB_TARGET} PRIVATE CMANGOS MANGOSBOT_ZERO ENABLE_PLAYERBOTS)

  # The vendored sources were built with botpch.h. Besides speeding up their
  # build, it is their common compatibility boundary: cmangos-compat-shim.h
  # maps CMaNGOS names used throughout the bot sources to this core's API.
  # The aggregate module target used after the move had no PCH, so every one
  # of those declarations silently disappeared from the translation units.
  # botpch.h is not an optimisation here, it is the compatibility boundary
  # (cmangos-compat-shim.h: GuidSet, AreaTableEntry, GenericTransport, ...).
  # It has to reach EVERY translation unit whether or not precompiled headers
  # are in use. target_precompile_headers silently does nothing when PCH is
  # off (USE_PCH=OFF, or CMAKE_DISABLE_PRECOMPILE_HEADERS - the usual escape
  # from MSVC's PCH size limits), and until 2026-09-04 that was the only
  # path on any CMake >= 3.16: a Windows dynamic-modules build without PCH
  # died in WorldPosition.h / PlayerbotAI.h on exactly those shim names
  # ("GenericTransport: undeclared identifier", "m_banned: unknown override
  # specifier") while the same tree built on Linux with PCH. Force-include
  # the header instead whenever the PCH route is not taken.
  if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.16" AND USE_PCH AND NOT CMAKE_DISABLE_PRECOMPILE_HEADERS)
    target_precompile_headers(${PB_TARGET} PRIVATE "${PB_ROOT}/botpch.h")
  elseif(MSVC)
    target_compile_options(${PB_TARGET} PRIVATE "/FI${PB_ROOT}/botpch.h")
  else()
    target_compile_options(${PB_TARGET} PRIVATE "-include${PB_ROOT}/botpch.h")
  endif()

  target_link_libraries(${PB_TARGET}
    PRIVATE Boost::thread
    PRIVATE Boost::filesystem
    PRIVATE Boost::system)

  # PUBLIC, not PRIVATE: mod-dungeon-clear includes "playerbot/playerbot.h" and
  # friends. It used to get these transitively through
  # target_link_libraries(modules PUBLIC playerbots); with both modules in one
  # target that link is gone, so the paths have to be on the target itself.
  #
  # Three roots, because the vendored sources use all three spellings:
  #   "playerbot/playerbot.h"       -> module root
  #   "PlayerbotMgr.h"              -> playerbot/
  #   "strategy/values/Foo.h"       -> playerbot/
  #
  # The core dirs below are NOT redundant with MODULES_COMMON_INCLUDES. They
  # used to reach the other module transitively: playerbots was its own library
  # and exported them PUBLIC, and mod-dungeon-clear picked them up through
  # target_link_libraries(modules PUBLIC playerbots). Merging both modules into
  # one target removed that link and with it the inheritance - the first build
  # after the move failed on mod-dungeon-clear (not on a bot source) with
  # "Config.h: No such file or directory". Recovered verbatim from the removed
  # root-CMakeLists block; duplicates with the common list are harmless.
  target_include_directories(${PB_TARGET} PUBLIC
    ${PB_ROOT}
    ${PB_ROOT}/src
    ${PB_ROOT}/src/playerbot
    ${PB_ROOT}/src/ahbot
    ${PB_ROOT}/src/cmangos-compat-stubs
    ${CMAKE_SOURCE_DIR}/dep/include/g
    ${CMAKE_SOURCE_DIR}/src/framework
    ${CMAKE_SOURCE_DIR}/src/framework/Network
    ${CMAKE_SOURCE_DIR}/src/game
    ${CMAKE_SOURCE_DIR}/src/game/AI
    ${CMAKE_SOURCE_DIR}/src/game/AuctionHouse
    ${CMAKE_SOURCE_DIR}/src/game/Battlegrounds
    ${CMAKE_SOURCE_DIR}/src/game/Chat
    ${CMAKE_SOURCE_DIR}/src/game/Commands
    ${CMAKE_SOURCE_DIR}/src/game/Database
    ${CMAKE_SOURCE_DIR}/src/game/Group
    ${CMAKE_SOURCE_DIR}/src/game/Guild
    ${CMAKE_SOURCE_DIR}/src/game/Handlers
    ${CMAKE_SOURCE_DIR}/src/game/LFG
    ${CMAKE_SOURCE_DIR}/src/game/Mail
    ${CMAKE_SOURCE_DIR}/src/game/MapNodes
    ${CMAKE_SOURCE_DIR}/src/game/Maps
    ${CMAKE_SOURCE_DIR}/src/game/Maps/Pool
    ${CMAKE_SOURCE_DIR}/src/game/Movement
    ${CMAKE_SOURCE_DIR}/src/game/Movement/spline
    ${CMAKE_SOURCE_DIR}/src/game/Objects
    ${CMAKE_SOURCE_DIR}/src/game/OutdoorPvP
    ${CMAKE_SOURCE_DIR}/src/game/PacketBroadcast
    ${CMAKE_SOURCE_DIR}/src/game/Protocol
    ${CMAKE_SOURCE_DIR}/src/game/Spells
    ${CMAKE_SOURCE_DIR}/src/game/Threat
    ${CMAKE_SOURCE_DIR}/src/game/Transports
    ${CMAKE_SOURCE_DIR}/src/game/vmap
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/shared/Config
    ${CMAKE_SOURCE_DIR}/src/shared/Database
    ${CMAKE_SOURCE_DIR}/src/shared/Log
    ${CMAKE_SOURCE_DIR}/src/shared/Util)

  if(WIN32)
    target_include_directories(${PB_TARGET} PUBLIC
      ${CMAKE_SOURCE_DIR}/dep/windows/include
      ${CMAKE_SOURCE_DIR}/dep/windows/include/mysql)
  endif()
endforeach()

# Config file. Ported from the vendor CMakeLists (kept beside this one as
# CMakeLists.txt.vendor-reference): the expansion is chosen by project name and
# vanilla is the default, because this project is called TurtleWoW and matched
# none of the named cases - which is why aiplayerbot.conf.dist was never
# generated at all before that was fixed.
if(${CMAKE_PROJECT_NAME} MATCHES "TBC")
  configure_file(${PB_ROOT}/src/playerbot/aiplayerbot.conf.dist.in.tbc
                 ${CMAKE_BINARY_DIR}/aiplayerbot.conf.dist)
elseif(${CMAKE_PROJECT_NAME} MATCHES "WoTLK")
  configure_file(${PB_ROOT}/src/playerbot/aiplayerbot.conf.dist.in.wotlk
                 ${CMAKE_BINARY_DIR}/aiplayerbot.conf.dist)
else()
  configure_file(${PB_ROOT}/src/playerbot/aiplayerbot.conf.dist.in
                 ${CMAKE_BINARY_DIR}/aiplayerbot.conf.dist)
endif()
if(NOT CONF_INSTALL_DIR)
  set(CONF_INSTALL_DIR ${CONF_DIR})
endif()
install(FILES ${CMAKE_BINARY_DIR}/aiplayerbot.conf.dist DESTINATION ${CONF_INSTALL_DIR})
