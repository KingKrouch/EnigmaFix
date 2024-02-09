cmake_minimum_required(VERSION 3.20)
# Set the Project Name.
set(PROJECT_NAME                EnigmaFix)
# Set the name of the target operating system.
set(CMAKE_SYSTEM_NAME           Windows)
if (UNIX) # If compiling on Linux, manually configure MingW-64.
    set(CMAKE_C_COMPILER        x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER      x86_64-w64-mingw32-g++)
    # Set the Toolchain to MinGW on Linux.
    set(CMAKE_FIND_ROOT_PATH    /usr/x86_64-w64-mingw32)
    link_directories(${CMAKE_FIND_ROOT_PATH}/sys-root/mingw/lib) # TODO: Fix to also work with MinGW on Windows.
    # adjust the default behavior of the FIND_XXX() commands:
    # Never search for programs in the host environment
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # Search for headers and libraries only in the target environment
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif (UNIX)
# Set our output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})
# Set our intermediate directories.
set(CMAKE_BINARY_DIR               ${CMAKE_CURRENT_SOURCE_DIR}/Intermediate/${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_FILES_DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/Intermediate/${CMAKE_BUILD_TYPE})
# Set our compile method to Ninja.
set(CMAKE_GENERATOR Ninja)
set(CMAKE_CXX_STANDARD 23)
## Setup Boost (Still need to sort out the error regarding "the target platform not supporting dynamic linking"
find_package(Boost REQUIRED COMPONENTS) # Add Boost Components Here.
# Include Boost directories
include_directories(${Boost_INCLUDE_DIRS})

# Set up our ThirdParty libraries
include(FetchContent)

## SafetyHook (With Zydis)
option(SAFETYHOOK_FETCH_ZYDIS "Option to fetch Zydis" ON)
### Pass the option to the compiler
if(SAFETYHOOK_FETCH_ZYDIS)
    add_definitions(-DSAFETYHOOK_FETCH_ZYDIS)
endif()
set(SAFETYHOOK_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/SafetyHook")
message(STATUS "Fetching SafetyHook...")
FetchContent_Declare(
        SafetyHook
        GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
        GIT_TAG "origin/main"
        SOURCE_DIR "${SAFETYHOOK_SOURCE_DIR}"
)
FetchContent_MakeAvailable(SafetyHook)

## SpdLog
set(SPDLOG_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/SpdLog")
message(STATUS "Fetching SpdLog...")
FetchContent_Declare(
        SpdLog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG "origin/v1.x"
        SOURCE_DIR "${SPDLOG_SOURCE_DIR}"
)
FetchContent_MakeAvailable(SpdLog)

## ImGui
set(IMGUI_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/ImGui")
message(STATUS "Fetching ImGui...")
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
    GIT_TAG "origin/master"
    SOURCE_DIR "${IMGUI_SOURCE_DIR}"
)
FetchContent_MakeAvailable(ImGui)
add_library(imgui STATIC
        ${IMGUI_SOURCE_DIR}/imgui.cpp
        ${IMGUI_SOURCE_DIR}/imgui.h
        ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
        ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
        ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
        ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp)
add_library(imguibackends STATIC
        ${IMGUI_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
        ${IMGUI_SOURCE_DIR}/backends/imgui_impl_dx11.h
        ${IMGUI_SOURCE_DIR}/backends/imgui_impl_win32.cpp
        ${IMGUI_SOURCE_DIR}/backends/imgui_impl_win32.h)
### Include the ImGui header files with the ImGuiBackends, as we don't want to manually modify the includes to add a "../".
target_include_directories(imguibackends PRIVATE ${IMGUI_SOURCE_DIR})
target_link_libraries(imguibackends d3dcompiler dwmapi)
### We then finally link together ImGui and ImGuiBackends, so they're dependent on each other.
target_link_libraries(imgui imguibackends)

## Kiero
set(KIERO_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/Kiero")
message(STATUS "Fetching Kiero...")
FetchContent_Declare(
        Kiero
        GIT_REPOSITORY "https://github.com/KingKrouch/Kiero-EnigmaFix.git"
        GIT_TAG "origin/master"
        SOURCE_DIR "${KIERO_SOURCE_DIR}"
)
FetchContent_MakeAvailable(Kiero)
add_library(kiero STATIC 
        ${KIERO_SOURCE_DIR}/kiero.cpp
        ${KIERO_SOURCE_DIR}/kiero.h
)

## MinHook (We are simply gonna fork the latest version of MinHook with it's own CMakeLists.txt rather than using the one with Kiero).
set(MINHOOK_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/MinHook")
message(STATUS "Fetching MinHook...")
FetchContent_Declare(
        MinHook
        GIT_REPOSITORY "https://github.com/TsudaKageyu/minhook.git"
        GIT_TAG "origin/master"
        SOURCE_DIR "${MINHOOK_SOURCE_DIR}"
)
FetchContent_MakeAvailable(MinHook)
target_link_libraries(kiero minHook) # Include MinHook with Kiero, so we can just have it use that version instead of it's own.
target_include_directories(kiero PRIVATE "${MINHOOK_SOURCE_DIR}/include")

## IniPP
set(INIPP_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/IniPP")
message(STATUS "Fetching IniPP...")
FetchContent_Declare(
        IniPP
        GIT_REPOSITORY "https://github.com/mcmtroffaes/inipp.git"
        GIT_TAG "origin/develop"
        SOURCE_DIR "${INIPP_SOURCE_DIR}"
)
FetchContent_MakeAvailable(IniPP)

## JSON
set(JSON_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/JSON")
message(STATUS "Fetching JSON...")
FetchContent_Declare(
        JSON
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG "origin/develop"
        SOURCE_DIR "${JSON_SOURCE_DIR}"
)
FetchContent_MakeAvailable(JSON)

## STB
set(STB_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/STB")
message(STATUS "Fetching STB...")
FetchContent_Declare(
        STB
        GIT_REPOSITORY "https://github.com/nothings/stb.git"
        GIT_TAG "origin/master"
        SOURCE_DIR "${STB_SOURCE_DIR}"
)
FetchContent_MakeAvailable(STB)

# Now finally export our library file.
## Setup the project and set a few variables
project(EnigmaFix)
add_library(EnigmaFix SHARED
        Source/dllmain.cpp
        Source/Localization/Localization.cpp
        Source/Localization/Localization.h
        Source/Managers/ConfigManager.cpp
        Source/Managers/ConfigManager.h
        Source/Managers/FramerateManager.cpp
        Source/Managers/FramerateManager.h
        Source/Managers/HookManager.cpp
        Source/Managers/HookManager.h
        Source/Managers/PatchManager.cpp
        Source/Managers/PatchManager.h
        Source/Managers/PluginManager.cpp
        Source/Managers/PluginManager.h
        Source/Managers/RenderManager.cpp
        Source/Managers/RenderManager.h
        Source/Managers/UIManager.cpp
        Source/Managers/UIManager.h
        Source/Settings/PlayerSettings.cpp
        Source/Settings/PlayerSettings.h
        Source/Plugins/Plugin_DERQ.cpp
        Source/Plugins/Plugin_DERQ.h
)
## Add the dependencies we set up previously.
# NOTE: Add "" to the "add_dependencies" section when you finally get that shit working.
add_dependencies(EnigmaFix imgui kiero safetyhook inipp nlohmann_json spdlog)
target_link_libraries(EnigmaFix d3dcompiler imgui kiero inipp ${Boost_LIBRARIES})
## Set up our linker flags for compiling.
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -municode")
set_target_properties(EnigmaFix PROPERTIES PREFIX "" OUTPUT_NAME EnigmaFix SUFFIX ".asi")
## Now we delete any miscellaneous .a static library files for release builds, as we don't need them.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE_DIR:${PROJECT_NAME}>/*.a)
endif()