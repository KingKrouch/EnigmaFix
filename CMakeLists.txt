cmake_minimum_required(VERSION 3.5)
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

# Set up our ThirdParty libraries
include(FetchContent)

## SafetyHook (With Zydis)
option(SAFETYHOOK_FETCH_ZYDIS "Option to fetch Zydis" ON)
### Pass the option to the compiler
if(SAFETYHOOK_FETCH_ZYDIS)
    add_definitions(-DSAFETYHOOK_FETCH_ZYDIS)
endif()
FetchContent_Declare(
        SafetyHook
        GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
        GIT_TAG "origin/main"
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/SafetyHook"
)
FetchContent_MakeAvailable(SafetyHook)

## SpdLog
FetchContent_Declare(
        SpdLog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG "origin/v1.x"
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/SpdLog"
)
FetchContent_MakeAvailable(SpdLog)

## ImGui
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
    GIT_TAG "origin/master"
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/ImGui"
)
FetchContent_MakeAvailable(ImGui)
add_library(ImGui STATIC
        Source/ThirdParty/ImGui/imgui.cpp
        Source/ThirdParty/ImGui/imgui.h
        Source/ThirdParty/ImGui/imgui_demo.cpp
        Source/ThirdParty/ImGui/imgui_draw.cpp
        Source/ThirdParty/ImGui/imgui_tables.cpp
        Source/ThirdParty/ImGui/imgui_widgets.cpp)
add_library(ImGuiBackends STATIC
        Source/ThirdParty/ImGui/backends/imgui_impl_dx11.cpp
        Source/ThirdParty/ImGui/backends/imgui_impl_dx11.h
        Source/ThirdParty/ImGui/backends/imgui_impl_win32.cpp
        Source/ThirdParty/ImGui/backends/imgui_impl_win32.h)
### Include the ImGui header files with the ImGuiBackends, as we don't want to manually modify the includes to add a "../".
target_include_directories(ImGuiBackends PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/ThirdParty/ImGui)
target_link_libraries(ImGuiBackends d3dcompiler dwmapi)
### We then finally link together ImGui and ImGuiBackends, so they're dependent on each other.
target_link_libraries(ImGui ImGuiBackends)

## Kiero
FetchContent_Declare(
    Kiero
    GIT_REPOSITORY "https://github.com/KingKrouch/Kiero-EnigmaFix.git"
    GIT_TAG "origin/master"
    SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/Kiero"
)
FetchContent_MakeAvailable(Kiero)
add_library(Kiero STATIC
    ${Kiero_SOURCE_DIR}/kiero.cpp
    ${Kiero_SOURCE_DIR}/kiero.h
)

## MinHook (We are simply gonna fork the latest version of MinHook with it's own CMakeLists.txt rather than using the one with Kiero).
FetchContent_Declare(
        MinHook
        GIT_REPOSITORY "https://github.com/TsudaKageyu/minhook.git"
        GIT_TAG "origin/master"
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/MinHook"
)
FetchContent_MakeAvailable(MinHook)
target_link_libraries(Kiero MinHook) # Include MinHook with Kiero, so we can just have it use that version instead of it's own.

## IniPP
FetchContent_Declare(
        IniPP
        GIT_REPOSITORY "https://github.com/mcmtroffaes/inipp.git"
        GIT_TAG "origin/develop"
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source/ThirdParty/IniPP"
)
FetchContent_MakeAvailable(IniPP)

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
add_dependencies(EnigmaFix ImGui Kiero inipp SafetyHook SpdLog)
target_link_libraries(EnigmaFix d3dcompiler ImGui Kiero inipp)
## Set up our linker flags for compiling.
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -municode")
set_target_properties(EnigmaFix PROPERTIES PREFIX "" OUTPUT_NAME EnigmaFix SUFFIX ".asi")
## Now we delete any miscellaneous .a static library files for release builds, as we don't need them.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE_DIR:${PROJECT_NAME}>/*.a)
endif()