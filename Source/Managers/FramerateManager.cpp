/**
EnigmaFix Copyright (c) 2023 Bryce Q.

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
#include "FramerateManager.h"
// System Libraries
#include <windows.h>
// Variables

// Singleton Instance
EnigmaFix::FramerateManager EnigmaFix::FramerateManager::frm_Instance;

namespace EnigmaFix {
    bool loop = true;
    LARGE_INTEGER currentTime;
    LONG lastTime;
    LARGE_INTEGER frequency; // Frequency of the performance counter

    void FramerateManager::Update() {
        QueryPerformanceFrequency(&frequency); // Get the frequency of the performance counter
        while (loop) {
            QueryPerformanceCounter(&currentTime); // Grabs the current time.
            double deltaTimeSeconds = static_cast<double>(currentTime.QuadPart - lastTime) / frequency.QuadPart;
            msTime = float(deltaTimeSeconds) * 1000;

            // Avoid division by zero by checking if deltaTimeSeconds is not too small.
            if (deltaTimeSeconds > 0.000001) {
                fps = 1.0 / deltaTimeSeconds; // Corrected framerate calculation
                timeOffset = originalFrTarget / float(deltaTimeSeconds);
            }
            lastTime = currentTime.QuadPart;
        }
    }
}
