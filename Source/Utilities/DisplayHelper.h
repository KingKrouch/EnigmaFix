/**
Display Utilities code that is from Lyall's C++ mods (https://github.com/Lyall/). Simply borrowing them here.
At least on the MGSHDFix repo, it's licensed under MIT. So I figured I'd mention that here.

There's some things that I modified or added, such as getting a list of available resolutions.
**/

#ifndef DISPLAY_HELPER_HPP
#define DISPLAY_HELPER_HPP

#include <vector>
#include <utility>

namespace Util
{
    struct DesktopResolution {
        int x;
        int y;

        bool operator==(const DesktopResolution& other) const {
            return x == other.x && y == other.y;
        }
    };

    // Function declarations (only signatures)
    std::vector<DesktopResolution> GetAvailableDisplayResolutions(int displayIndex = 1);
    DesktopResolution GetCurrentDisplayResolution(int displayIndex = 1);
}

#endif // DISPLAY_HELPER_HPP