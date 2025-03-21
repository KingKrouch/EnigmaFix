/**
Display Utilities code that is from Lyall's C++ mods (https://github.com/Lyall/). Simply borrowing them here.
At least on the MGSHDFix repo, it's licensed under MIT. So I figured I'd mention that here.

There's some things that I modified or added, such as getting a list of available resolutions.
**/

#include "DisplayHelper.h"
#include <windows.h>
#include <algorithm>
#include <string>

namespace Util
{
    std::vector<DesktopResolution> GetAvailableResolutions() {
        std::vector<DesktopResolution> resolutions;

        DISPLAY_DEVICE displayDevice;
        displayDevice.cb = sizeof(DISPLAY_DEVICE);
        if (!EnumDisplayDevices(nullptr, 0, &displayDevice, 0)) {
            return resolutions;
        }

        std::string primaryMonitorName = displayDevice.DeviceName;

        DEVMODE devMode{};
        devMode.dmSize = sizeof(DEVMODE);
        int i = 0;

        while (EnumDisplaySettings(primaryMonitorName.c_str(), i++, &devMode)) {
            DesktopResolution res = {
                static_cast<int>(devMode.dmPelsWidth),
                static_cast<int>(devMode.dmPelsHeight)
            };

            if (std::find(resolutions.begin(), resolutions.end(), res) == resolutions.end()) {
                resolutions.push_back(res);
            }
        }

        return resolutions;
    }

    std::pair<int, int> GetPhysicalDesktopDimensions() {
        if (DEVMODE devMode{ .dmSize = sizeof(DEVMODE) }; EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode))
            return { devMode.dmPelsWidth, devMode.dmPelsHeight };

        return {};
    }
}