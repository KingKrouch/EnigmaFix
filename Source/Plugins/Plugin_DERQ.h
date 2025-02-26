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
