#include "Plugin_DERQ.h"

#include "spdlog/spdlog.h"

EnigmaFix::Plugin_DERQ EnigmaFix::Plugin_DERQ::pq_Instance; // Seemingly need this declared in Plugin_DERQ.cpp so a bunch of linker errors don't happen.

namespace EnigmaFix
{
    void Plugin_DERQ::ResolutionPatches()
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

    void Plugin_DERQ::AspectRatioPatches()
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

    void Plugin_DERQ::FOVPatches()
    {

    }

    void Plugin_DERQ::UIPatches()
    {

    }

    //void __attribute__((naked)) FramerateUnlockHook() {
        //asm volatile (
            //"movl $1, %eax\n\t"
            //"ret\n\t"
        //);
    //}

    void Plugin_DERQ::FrameratePatches()
    {
        //auto framerateCapFunc = Memory::PatternScan(pchMgr.BaseModule, "8B 80 ?? ?? ?? ?? 89 44 ?? ?? 83 7C 24 44 ?? 74 ?? 83 7C 24 44");
        //safetyhook::create_inline()

        // NOTE: 8B 80 ? ? ? ? 89 44 ? ? 83 7C 24 44 ? 74 ? 83 7C 24 44 is the pattern we want to create a codecave with
    }

    void Plugin_DERQ::GraphicsSettingsPatches()
    {

    }

    void Plugin_DERQ::CameraPatches()
    {

    }

    void Plugin_DERQ::PhotoModePatches()
    {

    }
} // EnigmaFix