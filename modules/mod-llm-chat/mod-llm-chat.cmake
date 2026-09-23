# Optional CMake hooks for mod-llm-chat.
#
# Most modules do not need custom CMake here. Source files under src/ are discovered automatically,
# conf/*.conf.dist files are copied automatically, and Addmod_llm_chatScripts() is invoked by the
# generated module loader.
#
# This file is included twice:
#   TORTOISE_MODULE_CMAKE_PHASE=DISCOVERY
#   TORTOISE_MODULE_CMAKE_PHASE=POST_TARGETS
#
# Use TW_* helpers for compatibility with this core's module system. Do not use AzerothCore AC_* names.

if(TORTOISE_MODULE_CMAKE_PHASE STREQUAL "DISCOVERY")
  # Extra script sources or headers can be configured here
elseif(TORTOISE_MODULE_CMAKE_PHASE STREQUAL "POST_TARGETS")
  find_package(CURL REQUIRED)
  foreach(LLM_TARGET modules mod_mod-llm-chat)
    if(TARGET ${LLM_TARGET})
      target_link_libraries(${LLM_TARGET} PRIVATE CURL::libcurl)
    endif()
  endforeach()
endif()

