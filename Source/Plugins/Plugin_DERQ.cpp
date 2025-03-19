/**
EnigmaFix Copyright (c) 2025 Bryce Q.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

// Internal Functionality
#include "Plugin_DERQ.h"
#include "../Settings/PlayerSettings.h"
#include "../Utilities/MemoryHelper.hpp"
#include "../Managers/PatchManager.h"

// Third Party Libraries
#include <safetyhook.hpp>

#include "../Managers/FramerateManager.h"
#include "../Managers/LogManager.h"
#include "spdlog/spdlog.h"

auto& PlayerSettingsPDQ = EnigmaFix::PlayerSettings::Get();
auto& PatchManagerPDQ = EnigmaFix::PatchManager::Get();
auto& LogManagerPDQ = EnigmaFix::LogManager::Get();
auto& FramerateManagerPDQ = EnigmaFix::FramerateManager::Get();

EnigmaFix::Plugin_DERQ EnigmaFix::Plugin_DERQ::pq_Instance; // Seemingly need this declared in Plugin_DERQ.cpp so a bunch of linker errors don't happen.

namespace EnigmaFix
{

    struct ResolutionPtr {
        int* X;
        int* Y;

        // Constructor that takes offsets and calculates addresses dynamically
        ResolutionPtr(intptr_t xOffset, intptr_t yOffset) {
            intptr_t baseModule = reinterpret_cast<intptr_t>(PatchManagerPDQ.BaseModule);
            X = reinterpret_cast<int*>(baseModule + xOffset);
            Y = reinterpret_cast<int*>(baseModule + yOffset);
        }
    };

    ResolutionPtr resolutionList[] = {
        { 0x140F58720, 0x140F58724 },  // 640x360 (0)
        { 0x140F58728, 0x140F5872C },  // 720x405 (1)
        { 0x140F58730, 0x140F58734 },  // 800x450 (2)
        { 0x140F58738, 0x140F5873C },  // 1024x576 (3)
        { 0x140F58740, 0x140F58744 },  // 1152x648 (4)
        { 0x140F58748, 0x140F5874C },  // 1280x720 (5)
        { 0x140F58750, 0x140F58754 },  // 1360x765 (6)
        { 0x140F58758, 0x140F5875C },  // 1366x768 (7)
        { 0x140F58760, 0x140F58764 },  // 1600x900 (8)
        { 0x140F58764, 0x140F5876C },  // 1920x1080 (9)
        { 0x140F58770, 0x140F58774 },  // 2560x1440 (10)
        { 0x140F58778, 0x140F5877C },  // 3840x2160 (11)
        //{ 0xF58780, 0xF58784 },        // 4K Native (12)
    };

    void Plugin_DERQ::ResolutionPatches(HMODULE baseModule)
    {
        // Signature Scan Horizontal Res: "80 07 ?? 00 C7 45 ?? 38 04 ?? 00 E8"
        // Signature Scan Vertical Res:   "38 04 ?? 00 E8 64 BF"
        int* hResPtr   = (int*)((intptr_t)baseModule + 0x4858CC);
        int* vResPtr   = (int*)((intptr_t)baseModule + 0x4858D3);

        // Signature Scan Horizontal Res 4K Native: "00 0F 00 00 C7 45 C3"
        // Signature Scan Vertical Res 4K Native:   "70 ?? 00 00 E8 ?? ?? ?? ?? EB ?? E8"
        // TODO: Figure out why writing to these doesn't work.
        int* hRes4KPtr = (int*)((intptr_t)baseModule + 0x4858A7);
        int* vRes4KPtr = (int*)((intptr_t)baseModule + 0x4858AE);

        // Signature Scan Horizontal Window Size 4K Native: "00 0F 00 00 70 ? 00 00 C0 5D 00"
        // Signature Scan Vertical Window Size 4K Native: "70 ? 00 00 C0 5D 00"
        int* hWinSize4KPtr = (int*)((intptr_t)baseModule + 0xF58780);
        int* vWinSize4KPtr = (int*)((intptr_t)baseModule + 0xF58784);

        // For now, let's just set this, so we can see how the game reacts to it.
        hRes4KPtr     = &PlayerSettingsPDQ.RES.HorizontalRes;
        vRes4KPtr       = &PlayerSettingsPDQ.RES.VerticalRes;
        hWinSize4KPtr = &PlayerSettingsPDQ.RES.HorizontalRes;
        vWinSize4KPtr = &PlayerSettingsPDQ.RES.VerticalRes;

        // TODO: Find a good place to put this check inside of the game code, just before the resolution change occurs.
        // Example for setting up our actual internal resolution to one of the available hardcoded ones.
        //int currentResolution;
        //switch (currentResolution)
        //{
            //case 0: // 640x360
                //hResPtr = resolutionList[0].X;
                //vResPtr = resolutionList[0].Y;
                //break;
            //case 1: // 720x405
                //hResPtr = resolutionList[1].X;
                //vResPtr = resolutionList[1].Y;
                //break;
            //case 2: // 800x450
                //hResPtr = resolutionList[2].X;
                //vResPtr = resolutionList[2].Y;
                //break;
            //case 3: // 1024x576
                //hResPtr = resolutionList[3].X;
                //vResPtr = resolutionList[3].Y;
            //case 4: // 1152x648
                //hResPtr = resolutionList[4].X;
                //vResPtr = resolutionList[4].Y;
            //case 5: // 1280x720
                //hResPtr = resolutionList[5].X;
                //vResPtr = resolutionList[5].Y;
            //case 6: // 1360x765
                //hResPtr = resolutionList[6].X;
                //vResPtr = resolutionList[6].Y;
            //case 7: // 1366x768
                //hResPtr = resolutionList[7].X;
                //vResPtr = resolutionList[7].Y;
            //case 8: // 1600x900
                //hResPtr = resolutionList[8].X;
                //vResPtr = resolutionList[8].Y;
            //case 9: // 1920x1080
                //hResPtr = resolutionList[9].X;
                //vResPtr = resolutionList[9].Y;
            //case 10: // 2560x1440
                //hResPtr = resolutionList[10].X;
                //vResPtr = resolutionList[10].Y;
            //case 11: // 3840x2160
                //hResPtr = resolutionList[11].X;
                //vResPtr = resolutionList[11].Y;
            //case 12: // 4K Native, in this case, we need to adjust both the window size and internal resolution
                //hRes4KPtr = &PlayerSettingsPDQ.RES.HorizontalRes;
                //vResPtr   = &PlayerSettingsPDQ.RES.VerticalRes;
                //hWinSize4KPtr = &PlayerSettingsPDQ.RES.HorizontalRes;
                //vWinSize4KPtr = &PlayerSettingsPDQ.RES.VerticalRes;
        //}

        // Signature for 4K Native text: "34 ?? 20 4E 61 74 69 76 65"
        // TODO: Take into account the other parts of memory that will inevitably duplicate this during runtime. If at any point the in-game overlay changes the custom resolution, we need to run a loop that changes the memory values to reflect this in-game.
        if (auto native4kText = Memory::PatternScan(baseModule, "34 ?? 20 4E 61 74 69 76 65")) {
            spdlog::info("Aspect Ratio: Found First Block Function Signature at: {}", reinterpret_cast<void*>(native4kText));

            // Generate resolution string (max 9 bytes including null terminator)
            std::string replacement = std::to_string(PlayerSettingsPDQ.RES.HorizontalRes) + "x" + std::to_string(PlayerSettingsPDQ.RES.VerticalRes);

            // Ensure we do not exceed 9 bytes
            if (replacement.size() > 9) {
                spdlog::error("Resolution string '{}' is too long! Defaulting to 'Custom'.", replacement);
                std::string replacement = "Custom";
                return;
            }

            DWORD oldProtect;
            if (VirtualProtect(native4kText, 9, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                std::memcpy(native4kText, replacement.c_str(), replacement.size()); // Copy actual data
                std::memset(native4kText + replacement.size(), 0, 9 - replacement.size()); // Null-pad remaining bytes

                VirtualProtect(native4kText, 9, oldProtect, &oldProtect);
                spdlog::info("Successfully patched '4K Native' -> '{}'.", replacement);
            } else {
                spdlog::error("Failed to modify memory protection.");
            }
        }

        // Signatures for resolution checks to NOP out:
        // "41 89 ?? ?? 48 83 C4 ?? 41 ?? 5F 5E C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC" ("Application.exe"+485576 - 41 89 7E 0C)
        // "83 3D 73 F2 78 00"       ("Application.exe"+7C9292 - 83 3D 73 F2 78 00 0A)
        // "83 3D DC F1 78 00"       ("Application.exe"+7C9329 - 83 3D DC F1 78 00 0B)
        // "83 3D 21 F1 78 00"       ("Application.exe"+7C93E4 - 83 3D 21 F1 78 00 0B)
        // "83 3D 51 19 79 00"       ("Application.exe"+7C6BB4 - 83 3D 51 19 79 00 0B)
        // "39 7B ?? 0F 4C"          ("Application.exe"+4852BA - 39 7B 0C)
        // "8B 53 ?? 8B 43 ?? 89 41" ("Application.exe"+48530A - 8B 53 0C)

        // Signature for currently selected screen mode index:
        // "00 00 00 00 00 00 00 00 FF 01 00 00 4C 00 ?? 00 01 00 02 00 04 ?? 05 ?? ?? ?? ?? 00 08 00 0E 00 0D ?? ?? ?? ?? 00 12 00 13 00 15 ?? ?? ?? ?? 00 19 00 1B 00 1C ?? 1F 00 21 00 23 00 25 ?? ?? ?? ?? 00 29 00 2A 00 2C ?? 2F 00 31 00 32 00 34 ?? 35 ?? ?? ?? ?? 00 3B 00 3C ?? 3D ?? ?? ?? ?? 00 42 ?? 43 00 ?? ?? 48 00 ?? ?? ?? 00 4A ?? 4B 00 ?? ?? 4E 00 ?? ?? 50 00 51 ?? 52 00 53 ?? 54 00 55 ?? 56 00 57 ?? 58 00 59 ?? 5A 00 5B ?? 5C 00 5D ?? 5E 00 5F ?? 60 00 14 ?? 2B 00 61 00 62 ?? 63 00 64 00 ?? 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 03 02 03 02 00 00 00 00 02 02 02 02 02 02 02 02 00 00 01 01 01 01 01 01 01 01 02 02 02 02 02 02 01 01 01 01 01 01 01 01 01 01 02 02 00 00 02 02 01 01 01 01 02 02 02 02 02 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 01 01 01 00 00 00 00 00 00 00 00 00 00 01 01 03 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 08 00 00 00 3D ?? ?? ?? ?? 4E 01 ?? 34 ?? 01 00 2C ?? 01 00 2B 4E ?? 00 27 4E 01 ?? 24 ?? 01 00 23 4E ?? 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
        // Signature for currently selected resolution index:
        // "09 00 00 00 00 00 00 00 00 00 00 00 FF 01 00 00 4C 00 ?? 00 01 00 02 00 04 ?? 05 ?? ?? ?? ?? 00 08 00 0E 00 0D ?? ?? ?? ?? 00 12 00 13 00 15 ?? ?? ?? ?? 00 19 00 1B 00 1C ?? 1F 00 21 00 23 00 25 ?? ?? ?? ?? 00 29 00 2A 00 2C ?? 2F 00 31 00 32 00 34 ?? 35 ?? ?? ?? ?? 00 3B 00 3C ?? 3D ?? ?? ?? ?? 00 42 ?? 43 00 ?? ?? 48 00 ?? ?? ?? 00 4A ?? 4B 00 ?? ?? 4E 00 ?? ?? 50 00 51 ?? 52 00 53 ?? 54 00 55 ?? 56 00 57 ?? 58 00 59 ?? 5A 00 5B ?? 5C 00 5D ?? 5E 00 5F ?? 60 00 14 ?? 2B 00 61 00 62 ?? 63 00 64 00 ?? 00 10 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 03 02 03 02 00 00 00 00 02 02 02 02 02 02 02 02 00 00 01 01 01 01 01 01 01 01 02 02 02 02 02 02 01 01 01 01 01 01 01 01 01 01 02 02 00 00 02 02 01 01 01 01 02 02 02 02 02 02 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 01 01 01 00 00 00 00 00 00 00 00 00 00 01 01 03 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 08 00 00 00 3D ?? ?? ?? ?? 4E 01 ?? 34 ?? 01 00 2C ?? 01 00 2B 4E ?? 00 27 4E 01 ?? 24 ?? 01 00 23 4E ?? 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
        


        //spdlog::info("Internal Resolution : {}x{}", *hResPtr, *vResPtr);
        //spdlog::info("4K Option Internal Resolution : {}x{}", *hRes4KPtr, *vRes4KPtr);

        //if (PlayerSettingsRef.RES.UseCustomRes) {
            //*hRes4KPtr = PlayerSettingsRef.RES.HorizontalRes;
            //*vRes4KPtr = PlayerSettingsRef.RES.VerticalRes;
            //spdlog::info("Patched 4K Option Internal Resolution to : {}x{}", *hRes4KPtr, *vRes4KPtr);
        //}
    }

    void Plugin_DERQ::AspectRatioPatches(HMODULE baseModule)
    {
        // Disable aspect ratio values from being overwritten
        if (auto arBlockFunction1 = Memory::PatternScan(baseModule, "89 41 ?? 0F 10 ?? ?? 0F 11 ?? ?? 0F 10 ?? ?? 0F 11 ?? ?? 0F 10 ?? ?? 0F 11 ?? ?? 8B 82")) { // (89 41 50)
            spdlog::info("Aspect Ratio: Found First Block Function Signature at: {}", reinterpret_cast<void*>(arBlockFunction1));
        }
        if (auto arBlockFunction2 = Memory::PatternScan(baseModule, "F3 0F ?? ?? ?? E8 ?? ?? ?? ?? 85 C0 75")) { // (F3 0F 11 4F 50)
            spdlog::info("Aspect Ratio: Found Second Block Function Signature at: {}", reinterpret_cast<void*>(arBlockFunction2));
        }
        if (auto arBlockFunction3 = Memory::PatternScan(baseModule, "F3 0F ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 66 0F")) { // (F3 0F 11 4F 50)
            spdlog::info("Aspect Ratio: Found Third Block Function Signature at: {}", reinterpret_cast<void*>(arBlockFunction3));
        }
        if (auto arBlockFunction4 = Memory::PatternScan(baseModule, "8B 42 ?? 89 41 ?? 0F 10 ?? ?? 0F 11 ?? ?? 0F 10 ?? ?? 0F 11 ?? ?? 0F 10 ?? ?? 0F 11 ?? ?? 8B 82")) { // (8B 42 50)
            spdlog::info("Aspect Ratio: Found Fourth Block Function Signature at: {}", reinterpret_cast<void*>(arBlockFunction4));
        }

        // TODO: Figure out what opcodes access these memory pointers, and update them to use our own internal aspect ratio variable.
        // Set up the pointer addresses for our aspect ratio variables
        //float* aspectRatioPtr1 = (float*)((intptr_t)baseModule + 0x25EFC6);
        //float* aspectRatioPtr2 = (float*)((intptr_t)baseModule + 0x789B2C);
        //float* aspectRatioPtr3 = (float*)((intptr_t)baseModule + 0xE32A30);
        // Overwrite the aspect ratio values with our own.
        //aspectRatioPtr1 = &PlayerSettingsPDQ.RES.InternalAspectRatio;
        //aspectRatioPtr2 = &PlayerSettingsPDQ.RES.InternalAspectRatio;
        //aspectRatioPtr3 = &PlayerSettingsPDQ.RES.InternalAspectRatio;

        // Write to the object aspect ratio pointer
        //float* objectAspectRatioPtr = (float*)((intptr_t)baseModule + 0x01043B30 + 0xC90);
        //if (objectAspectRatioPtr != nullptr) {
            //spdlog::info("Object Aspect Ratio: {}", *objectAspectRatioPtr);
            // Patch aspect ratio value here.
            // TODO: Find a way to change run resolution and aspect ratio checks every time the internal rendering resolution changes.
            //float newAspectRatio = PlayerSettingsRef.INS.InternalHorizontalRes / PlayerSettingsRef.INS.InternalVerticalRes;
            //*objectAspectRatioPtr = newAspectRatio;
            //spdlog::info("Patched Aspect Ratio to: {}", *objectAspectRatioPtr);
        //}
        //else {
            //spdlog::error("Aspect ratio pointer is NULL");
            //return;
        //}

    }

    void Plugin_DERQ::FOVPatches(HMODULE baseModule)
    {

    }

    void Plugin_DERQ::UIPatches(HMODULE baseModule)
    {

    }

    //void __attribute__((naked)) FramerateUnlockHook() {
        //asm volatile (
            //"movl $1, %eax\n\t"
            //"ret\n\t"
        //);
    //}

    void Plugin_DERQ::FrameratePatches(HMODULE baseModule)
    {
        safetyhook::InlineHook framerateHook;
        using FramerateLimiterFunc = int(__stdcall*)(void* gameInstance);
        FramerateLimiterFunc originalFramerateLimiter = nullptr;

        if (auto framerateCapFunc = Memory::PatternScan(baseModule, "8B 80 ?? ?? ?? ?? 89 44 ?? ?? 83 7C 24 44 ?? 74 ?? 83 7C 24 44")) {
            spdlog::info("Found Framerate Limiter Signature at: {}", reinterpret_cast<void*>(framerateCapFunc));

            // Hook the function
            //framerateHook = safetyhook::create_inline(framerateCapFunc, FramerateManagerPDQ.Limit());
            //originalFramerateLimiter = framerateHook.original<FramerateLimiterFunc>();

            // TODO: Patch out the framerate limiter call, and replace it with our own framelimiter.
            // This seems to be the call: "8B 80 A0 00 00 00" ("Application.exe"+E88FA - mov eax,[rax+000000A0]) (8B 80 A0 00 00 00 are the bytes)

            // Address of signature = Application.exe + 0x000E8962
            // The modification after the framelimiter is called should jump to "F3 0F ?? ?? ?? ?? F3 0F ?? ?? ?? ?? F3 0F ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? ?? 83 B8 A4 00 00 00" (Application.exe+E8962 - movss xmm0,[rsp+40])
            // The actual bytes of the jmp location should be "Application.exe+E8962 - F3 0F 10 44 24 40"
        }
        //safetyhook::create_inline()

        //asm volatile (
            //"nop"
        //);
    }

    void Plugin_DERQ::GraphicsSettingsPatches(HMODULE baseModule)
    {
        // NOTE: Because you can technically adjust these settings while in-game, maybe I should do the signature scans first, and then find a way to have those reference the PlayerSettings variables.

        // Toggle off post-processing (From what I see, this is only a thing in the CE Table, a debugging thing if you will)
        if (auto postProcessToggleFunc = Memory::PatternScan(baseModule, "83 F8 ?? 75 ?? 41 8B ?? 49 8D")) { // Application.exe+6D8639: cmp eax,01 (83 F8 01)
            spdlog::info("Post Processing: Found Post Processing Toggle Signature at: {}", reinterpret_cast<void*>(postProcessToggleFunc));
        }

        if (PlayerSettingsPDQ.RS.Tonemapping) {
            if (auto tonemappingToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 0F ?? ?? ?? ?? ?? ?? 49 69 CF ?? ?? ?? ?? 42 88 ?? ?? ?? 41 0F")) { // "Application.exe"+29C420: mov [rcx+r13+70],al (42 88 44 29 70)
                spdlog::info("Post Processing: Found Color Correction Signature at: {}", reinterpret_cast<void*>(tonemappingToggleFunc));
                static SafetyHookMid tonemappingToggleHook;
            }
        }

        if (PlayerSettingsPDQ.RS.ColorCorrection) {
            if (auto colorCorrectionToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 66 41")) { // "Application.exe"+29C858: mov [rcx+r13+04],al (42 88 44 29 04)
                spdlog::info("Post Processing: Found Color Correction Signature at: {}", reinterpret_cast<void*>(colorCorrectionToggleFunc));
            }
        }

        if (PlayerSettingsPDQ.RS.LensFlare) {
            if (auto lensFlareToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 69 C7 ?? ?? ?? ?? 0F 28 ?? 0F 28 ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 69 C7 ?? ?? ?? ?? 0F 28 ?? 0F 28 ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 0F")) { // "Application.exe"+29CE43: mov [rcx+r13+000000EC],al (42 88 84 29 EC 00 00 00)
                spdlog::info("Post Processing: Found Lens Flare Signature at: {}", reinterpret_cast<void*>(lensFlareToggleFunc));
            }
        }

        if (PlayerSettingsPDQ.RS.Fog) {
            if (auto fogToggleFunc = Memory::PatternScan(baseModule, "88 44 ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 6B C7 ?? 0F 28 ?? 0F 28 ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 8D ?? ?? ?? ?? ?? 0F 28 ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 44 8B")) { // "Application.exe"+29D495: mov [rcx+rsi+04],al; lea rdx,[rsp+50] (88 44 31 04 48 8D 54 24 50; F3 0F 58 C8 F3 42 0F 11 8C 28 64 01 00 00)
                spdlog::info("Post Processing: Found Fog Signature at: {}", reinterpret_cast<void*>(fogToggleFunc));
            }
        }

        if (PlayerSettingsPDQ.RS.RLRLighting) {
            // Disable IBL Lighting:
            // E8 ?? ?? ?? ?? 49 8B ?? E8 ?? ?? ?? ?? 48 8D ?? ?? 4C 89 (Application.exe+2981AD - E8 8E AE FF FF - call Application.exe+293040) This needs to have a switch statement that checks things before running it.

            // In the post processing settings
            if (auto rlrLightingToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 69 C7 ?? ?? ?? ?? 0F 28 ?? 0F 28 ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 69 C7 ?? ?? ?? ?? 0F 28 ?? 0F 28 ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D")) { // "Application.exe"+29D3DF: mov [rcx+r13+000001C4],al (42 88 84 29 C4 01 00 00)
                spdlog::info("Post Processing: Found RLR Lighting Signature at: {}", reinterpret_cast<void*>(rlrLightingToggleFunc));
            }
        }

        if (PlayerSettingsPDQ.RS.CameraDistortion) {
            if (auto cameraDistortionToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 69 C7 ?? ?? ?? ?? 0F 28 ?? 0F 28 ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 0F")) { // "Application.exe"+29CEF5: mov [rcx+r13+00000160],al (42 88 84 29 60 01 00 00)
                spdlog::info("Post Processing: Found Camera Distortion Signature at: {}", reinterpret_cast<void*>(cameraDistortionToggleFunc));
            }
        }

        if (PlayerSettingsPDQ.RS.Bloom) {
            if (auto bloomToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 66 41 ?? ?? ?? ?? ?? ?? ?? 49 69 CF ?? ?? ?? ?? 0F 5B ?? F3 0F ?? ?? 66 42 ?? ?? ?? ?? ?? ?? ?? ?? 0F 5B ?? F3 0F ?? ?? F3 0F ?? ?? F3 0F ?? ?? 42 89 ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 8B")) { // "Application.exe"+29CB44: mov [rcx+r13+000000BC],al (42 88 84 29 BC 00 00 00)
                spdlog::info("Post Processing: Found Bloom Signature at: {}", reinterpret_cast<void*>(bloomToggleFunc));
            }
        }

        if (PlayerSettingsPDQ.RS.MotionBlur == true) {
            if (auto motionBlurToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 8B")) { // "Application.exe"+29CFF3: mov [rcx+r13+58],al (42 88 44 29 58)
                spdlog::info("Post Processing: Found Motion Blur Signature at: {}", reinterpret_cast<void*>(motionBlurToggleFunc));
            }
            //switch (PlayerSettingsPDQ.RS.MotionBlurPreset) {
                //case 0:  // Disabled, will probably keep this around for motion vectors
                //case 1:  // Short
                //case 2:  // Medium
                //case 3:  // Long
                //default: // Shutter Ratio (Application.exe+7735D8, default: 0.67542696) and Max Blur Length (Application.exe+25EE50, default: 0.1000000015)
            //}
        }

        if (PlayerSettingsPDQ.RS.DepthOfField) {
            if (auto dofToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 69 C7 ?? ?? ?? ?? 0F 28 ?? 0F 28 ?? F3 41 ?? ?? ?? ?? ?? ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? F3 0F ?? ?? F3 42 ?? ?? ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 8B")) { // "Application.exe"+29C600: mov [rcx+r13+00000118],al (42 88 84 29 18 01 00 00)
                spdlog::info("Post Processing: Found Depth of Field Signature at: {}", reinterpret_cast<void*>(dofToggleFunc));
            }
        }

        if (PlayerSettingsPDQ.RS.SSAO) {
            if (auto ssaoToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 66 41 ?? ?? ?? ?? ?? ?? ?? 49 69 CF ?? ?? ?? ?? 0F 5B ?? F3 0F ?? ?? 66 42 ?? ?? ?? ?? ?? ?? ?? ?? 0F 5B ?? F3 0F ?? ?? F3 0F ?? ?? F3 0F ?? ?? 42 89 ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 69 C7")) { // "Application.exe"+29D0BE: mov [rcx+r13+00000168],al (42 88 84 29 68 01 00 00)
                spdlog::info("Post Processing: Found SSAO Signature at: {}", reinterpret_cast<void*>(ssaoToggleFunc));
            }
        }

        if (PlayerSettingsPDQ.RS.TAA) {
            if (auto taaToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 49 69 C7")) { // "Application.exe"+29CF8D: mov [rcx+r13+69],al (42 88 44 29 69)
                spdlog::info("Post Processing: Found TAA Signature at: {}", reinterpret_cast<void*>(taaToggleFunc));
            }
            if (auto aaToggleFunc = Memory::PatternScan(baseModule, "42 88 ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 0F ?? ?? ?? ?? ?? ?? 49 69 CF ?? ?? ?? ?? 42 88 ?? ?? ?? 48 8D")) { // "Application.exe"+29CF64: mov [rcx+r13+68],al (42 88 44 29 68)
                spdlog::info("Post Processing: Found Post AA Signature at: {}", reinterpret_cast<void*>(aaToggleFunc));
            }
        }

        if (!PlayerSettingsPDQ.RS.Vignette) { // This one works a little bit differently, as you need to move a new float variable to the xmm1 register BEFORE the opcode is done, that way we can override the float variable used for the vignette intensity.
            if (auto vignetteFunc = Memory::PatternScan(baseModule, "F3 42 ?? ?? ?? ?? ?? ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 0F ?? ?? ?? ?? ?? ?? 49 69 CF ?? ?? ?? ?? 42 88 ?? ?? ?? 48 8D ?? ?? ?? 48 8D ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 41 0F")) { // "Application.exe"+29CF36: movss [rax+r13+00000164],xmm1 (F3 42 0F 11 8C 28 64 01 00 00)
                spdlog::info("Post Processing: Found Vignette Signature at: {}", reinterpret_cast<void*>(vignetteFunc));
                static SafetyHookMid vignetteFuncMidHook{};
                vignetteFuncMidHook = safetyhook::create_mid(vignetteFunc,
                    [](SafetyHookContext& ctx)
                    {
                        ctx.xmm1.f32[0] = 1.0f; // The default is "0.200000003"
                    });
            }
        }
    }

    void Plugin_DERQ::CameraPatches(HMODULE baseModule)
    {

    }

    void Plugin_DERQ::PhotoModePatches(HMODULE baseModule)
    {

    }

    void Plugin_DERQ::LoggingPatches(HMODULE baseModule)
    {
        if (auto loggingFunc = Memory::PatternScan(baseModule, "4c 89 44 24 ?? 4c 89 4c 24 ?? c3 cc cc cc cc cc 48 8b 01")) {
            spdlog::info("Found Logging Function Signature");
            // TODO: Print the logs to the console.
        }

        //spdlog::info();
        //spdlog::warn();
        //spdlog::error();
    }

    // ALT+F4 Window Signatures:
    // FF 15 ?? ?? ?? ?? 83 F8 ?? 0F 85
    // 83 F8 ?? 0F 85 ?? ?? ?? ?? 45 89 ?? 33 C0
} // EnigmaFix