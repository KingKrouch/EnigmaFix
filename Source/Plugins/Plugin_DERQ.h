//
// Created by Bryce on 11/9/23.
//

#ifndef PLUGIN_DERQ_H
#define PLUGIN_DERQ_H

namespace EnigmaFix {
class Plugin_DERQ {
    public:
        static Plugin_DERQ& Get() {
            return pq_Instance;
        }
        static void ResolutionPatches();
        static void AspectRatioPatches();
        static void FOVPatches();
        static void UIPatches();
        static void FrameratePatches();
        static void GraphicsSettingsPatches();
        static void CameraPatches();
        static void PhotoModePatches();
    private:
        Plugin_DERQ() {}
        static Plugin_DERQ pq_Instance;
};
}; // EnigmaFix

#endif //PLUGIN_DERQ_H
