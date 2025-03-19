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
#include "../Utilities/MemoryHelper.hpp"

// Third Party Libraries
#include "spdlog/spdlog.h"

EnigmaFix::Plugin_DERQ EnigmaFix::Plugin_DERQ::pq_Instance; // Seemingly need this declared in Plugin_DERQ.cpp so a bunch of linker errors don't happen.

namespace EnigmaFix
{
    void Plugin_DERQ::ResolutionPatches(HMODULE baseModule)
    {

        //int* hResPtr   = (int*)((intptr_t)PatchManagerRef.BaseModule + 0x4858CC);
        //int* vResPtr   = (int*)((intptr_t)PatchManagerRef.BaseModule + 0x4858D3);
        //int* hRes4KPtr = (int*)((intptr_t)PatchManagerRef.BaseModule + 0x4858A7);
        //int* vRes4KPtr = (int*)((intptr_t)PatchManagerRef.BaseModule + 0x4858AE);

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
        //"89 41 ?? 0F 10 ?? ?? 0F 11 ?? ?? 0F 10 ?? ?? 0F 11 ?? ?? 0F 10 ?? ?? 0F 11 ?? ?? 8B 82"
        //"F3 0F ?? ?? ?? E8 ?? ?? ?? ?? 85 C0 75"
        //"F3 0F ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 66 0F"
        //"8B 42 ?? 89 41 ?? 0F 10 ?? ?? 0F 11 ?? ?? 0F 10 ?? ?? 0F 11 ?? ?? 0F 10 ?? ?? 0F 11 ?? ?? 8B 82"

        //float* aspectRatioPtr1 = (float*)((intptr_t)baseModule + 0x25EFC6);
        //float* aspectRatioPtr2 = (float*)((intptr_t)baseModule + 0x789B2C);
        //float* aspectRatioPtr2 = (float*)((intptr_t)baseModule + 0xE32A30);

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
        if (auto framerateCapFunc = Memory::PatternScan(baseModule, "8B 80 ?? ?? ?? ?? 89 44 ?? ?? 83 7C 24 44 ?? 74 ?? 83 7C 24 44")) {
            spdlog::info("Found Framerate Limiter Signature");
            // TODO: Patch out the framerate limiter call, and replace it with our own framelimiter.
        }
        //safetyhook::create_inline()

        //asm volatile (
            //"nop"
        //);
    }

    void Plugin_DERQ::GraphicsSettingsPatches(HMODULE baseModule)
    {

    }

    void Plugin_DERQ::CameraPatches(HMODULE baseModule)
    {

    }

    void Plugin_DERQ::PhotoModePatches(HMODULE baseModule)
    {

    }

    void Plugin_DERQ::LoggingPatches(HMODULE baseModule)
    {
        if (auto loggingFunc = Memory::PatternScan(baseModule, "4c 89 44 24 ? 4c 89 4c 24 ? c3 cc cc cc cc cc 48 8b 01")) {
            spdlog::info("Found Logging Function Signature");
            // TODO: Print the logs to the console.
        }
    }

    // ALT+F4 Window Signatures:
    // FF 15 ? ? ? ? 83 F8 ? 0F 85
    // 83 F8 ? 0F 85 ? ? ? ? 45 89 ? 33 C0
} // EnigmaFix