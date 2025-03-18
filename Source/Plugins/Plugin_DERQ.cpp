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

        //float* objectAspectRatioPtr = (float*)((intptr_t)PatchManagerRef.BaseModule + 0x01043B30 + 0xC90);
        //if (objectAspectRatioPtr == nullptr) {
            //spdlog::error("Aspect ratio pointer is NULL");
            //return;
        //}
        //spdlog::info("Object Aspect Ratio: {}", *objectAspectRatioPtr);

        // TODO: Find a way to change run resolution and aspect ratio checks every time the internal rendering resolution changes.
        //float newAspectRatio = PlayerSettingsRef.INS.InternalHorizontalRes / PlayerSettingsRef.INS.InternalVerticalRes;
        //*objectAspectRatioPtr = newAspectRatio;
        //spdlog::info("Patched Aspect Ratio to: {}", *objectAspectRatioPtr);
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
        auto framerateCapFunc = Memory::PatternScan(baseModule, "8B 80 ?? ?? ?? ?? 89 44 ?? ?? 83 7C 24 44 ?? 74 ?? 83 7C 24 44");
        if (framerateCapFunc) {
            spdlog::info("Found Framerate Limiter Signature");
        }
        //safetyhook::create_inline()

        // NOTE: 8B 80 ? ? ? ? 89 44 ? ? 83 7C 24 44 ? 74 ? 83 7C 24 44 is the pattern we want to create a codecave with
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
} // EnigmaFix