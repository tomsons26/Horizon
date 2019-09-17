set(GAMEENGINE_INCLUDES
    game
    game/common
    game/debug
    game/gfx
    game/memory
)

set(HOOKER_SRC hooker/dllmain.cpp hooker/hooker.cpp hooker/setupglobals.cpp hooker/setuphooks.cpp)

set(GAMEENGINE_SRC
    game/common/buffer.cpp
    game/common/crc.cpp
    game/common/critsection.cpp
    game/common/dipthong.cpp
    game/common/lcw.cpp
    game/common/ostimer.cpp
    game/common/timer.cpp
    game/debug/gameassert.cpp
    game/debug/gamelogging.cpp
    game/gfx/pal.cpp
    game/gfx/surfacemonitor.cpp
    game/memory/alloc.cpp
    game/memory/newdel.cpp
)


if(CMAKE_CONFIGURATION_TYPES)
# Glob all the header files together to add to the project for msvc/xcode.
    # Not ideal as CMake won't notice if you add any until something else prompts a CMake run
    # but this is largely a convenience feature for MSVC/XCode users.
    file(GLOB_RECURSE GAMEENGINE_HEADERS "*.h")
    set(_GAMEENGINE_SOURCES ${GAMEENGINE_SRC})

    if(NOT STANDALONE)
        list(APPEND _GAMEENGINE_SOURCES ${HOOKER_SRC})
endif()

    # Filter out headers from included 3rd party libraries.
    list(FILTER GAMEENGINE_HEADERS EXCLUDE REGEX "libs/.*")
    list(FILTER GAMEENGINE_HEADERS EXCLUDE REGEX "base/.*")

    foreach(_GAMEENGINE_SOURCE IN ITEMS ${_GAMEENGINE_SOURCES})
        get_filename_component(_GAMEENGINE_SOURCE_PATH "${_GAMEENGINE_SOURCE}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" _GAMEENGINE_GROUP_PATH "${_GAMEENGINE_SOURCE_PATH}")
        string(REPLACE "/" "\\" _GAMEENGINE_GROUP_PATH "${_GAMEENGINE_GROUP_PATH}")
        source_group("Source Files\\${_GAMEENGINE_GROUP_PATH}" FILES "${_GAMEENGINE_SOURCE}")
    endforeach()

    foreach(_GAMEENGINE_HEADER IN ITEMS ${GAMEENGINE_HEADERS})
        get_filename_component(_GAMEENGINE_HEADER_PATH "${_GAMEENGINE_HEADER}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" _GAMEENGINE_HGROUP_PATH "${_GAMEENGINE_HEADER_PATH}")
        string(REPLACE "/" "\\" _GAMEENGINE_HGROUP_PATH "${_GAMEENGINE_HGROUP_PATH}")
        source_group("Header Files\\${_GAMEENGINE_HGROUP_PATH}" FILES "${_GAMEENGINE_HEADER}")
    endforeach()
endif()

# Build and link the EXE or DLL.
if(STANDALONE)
    add_executable(horizon ${GAMEENGINE_SRC} ${GAMEENGINE_HEADERS})
    target_include_directories(horizon PUBLIC ${GAMEENGINE_INCLUDES})
else()
    add_library(horizon SHARED ${HOOKER_SRC} ${GAMEENGINE_SRC} ${GAMEENGINE_HEADERS})
    target_include_directories(horizon BEFORE PUBLIC hooker ${GAMEENGINE_INCLUDES})
    target_compile_definitions(horizon PUBLIC GAME_DLL=1)
endif()

if(WIN32 OR "${CMAKE_SYSTEM}" MATCHES "Windows")
    target_link_libraries(horizon winmm)

    if(STANDALONE)
        target_link_libraries(horizon ws2_32)
    else()
        target_link_libraries(horizon wsock32) # Original links wsock32, not ws2_32
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    target_link_libraries(horizon rt)
endif()

target_compile_definitions(horizon PUBLIC $<$<CONFIG:DEBUG>:HORIZON_DEBUG=1>)

# Enable using bools in place of bitfields for boolean class members.
# This is for matching the ABI of the original game.
if(NO_BITFIELDS)
    target_compile_definitions(horizon PUBLIC HORIZON_NO_BITFIELDS=1)
endif()

# Enable Horizon debug logging.
if(LOGGING)
    target_compile_definitions(horizon PUBLIC HORIZON_LOGGING=1)
endif()

# Enable Horizon debug assertions.
if(ASSERTIONS)
    target_compile_definitions(horizon PUBLIC HORIZON_ASSERTS=1)
endif()

target_link_libraries(horizon base)

if(DDRAW_FOUND)
    target_compile_definitions(horizon PRIVATE BUILD_WITH_DDRAW=1)
    target_link_libraries(horizon DirectX::ddraw)
endif()

if(DSOUND_FOUND)
    target_compile_definitions(horizon PRIVATE BUILD_WITH_DSOUND=1)
    target_link_libraries(horizon DirectX::dsound)
endif()

if(SDL2_FOUND)
    target_compile_definitions(horizon PRIVATE BUILD_WITH_SDL2=1)
    target_link_libraries(horizon SDL2::SDL2)
endif()

if(OPENAL_FOUND)
    target_compile_definitions(horizon PRIVATE BUILD_WITH_OPENAL=1)
    target_link_libraries(horizon ${OPENAL_LIBRARIES})
    target_include_directories(horizon PUBLIC ${OPENAL_INCLUDE_DIRS})
endif()

