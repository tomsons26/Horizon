# Build and link the DLL.
set(GAMEENGINE_INCLUDES
    game
    game/common
    game/debug
)

set(HOOKER_SRC
    hooker/dllmain.cpp
    hooker/hooker.cpp
)

set(GAMEENGINE_SRC
    game/common/critsection.cpp
    game/debug/gameassert.cpp
    game/debug/gamelogging.cpp
)

# Glob all the header files together to add to the project for msvc/xcode.
if(CMAKE_CONFIGURATION_TYPES)
    file(GLOB_RECURSE GAMEENGINE_HEADERS "*.h")
endif()

if(STANDALONE)
    add_executable(horizon ${GAMEENGINE_SRC} ${GAMEENGINE_HEADERS})
    target_include_directories(horizon PUBLIC ${GAMEENGINE_INCLUDES})
else()
    add_library(horizon SHARED ${HOOKER_SRC} ${GAMEENGINE_SRC} ${GAMEENGINE_HEADERS})
    target_include_directories(horizon PUBLIC ${GAMEENGINE_INCLUDES} hooker)
endif()

# For MSVC/XCode this will add the headers to the project.
source_group("Header Files" FILES ${GAMEENGINE_HEADERS})

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

if(STANDALONE)
    target_compile_definitions(horizon PUBLIC HORIZON_STANDALONE=1)
endif()

# Enable using bools in place of bitfields for boolean class members.
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

