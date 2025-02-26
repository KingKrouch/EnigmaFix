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

#ifndef ENIGMAFIX_CLION_UIMANAGER_H
#define ENIGMAFIX_CLION_UIMANAGER_H

// System Libraries
#include <vector>
#include <string>
#include <iostream>

namespace EnigmaFix {
    class UIManager {
    public:
        static UIManager &Get() {
            return um_Instance;
        };
        static void Begin();
    private:
        static UIManager um_Instance;
        static void BeginRender();
        static void showMainMenu(bool p_open);
        static void showStartupOverlay(bool p_open);
        static void showAboutWindow(bool p_open);
        static void HelpMarker(const char* desc);
        static void initLocalization();
        void activateTheme();
        static void mainMenuOptions();
        static void loopChecks();
        static void showExitPrompt();
        static void windowButtons();
        static void LoadLogoTexture();


        struct UIProperties {
            std::vector<float> ImGuiCol_Text;
            std::vector<float> ImGuiCol_TextDisabled;
            std::vector<float> ImGuiCol_WindowBg;
            std::vector<float> ImGuiCol_ChildBg;
            std::vector<float> ImGuiCol_PopupBg;
            std::vector<float> ImGuiCol_Border;
            std::vector<float> ImGuiCol_BorderShadow;
            std::vector<float> ImGuiCol_FrameBg;
            std::vector<float> ImGuiCol_FrameBgHovered;
            std::vector<float> ImGuiCol_FrameBgActive;
            std::vector<float> ImGuiCol_TitleBg;
            std::vector<float> ImGuiCol_TitleBgActive;
            std::vector<float> ImGuiCol_TitleBgCollapsed;
            std::vector<float> ImGuiCol_MenuBarBg;
            std::vector<float> ImGuiCol_ScrollbarBg;
            std::vector<float> ImGuiCol_ScrollbarGrab;
            std::vector<float> ImGuiCol_ScrollbarGrabHovered;
            std::vector<float> ImGuiCol_ScrollbarGrabActive;
            std::vector<float> ImGuiCol_CheckMark;
            std::vector<float> ImGuiCol_SliderGrab;
            std::vector<float> ImGuiCol_SliderGrabActive;
            std::vector<float> ImGuiCol_Button;
            std::vector<float> ImGuiCol_ButtonHovered;
            std::vector<float> ImGuiCol_ButtonActive;
            std::vector<float> ImGuiCol_Header;
            std::vector<float> ImGuiCol_HeaderHovered;
            std::vector<float> ImGuiCol_HeaderActive;
            std::vector<float> ImGuiCol_Separator;
            std::vector<float> ImGuiCol_SeparatorHovered;
            std::vector<float> ImGuiCol_SeparatorActive;
            std::vector<float> ImGuiCol_ResizeGrip;
            std::vector<float> ImGuiCol_ResizeGripHovered;
            std::vector<float> ImGuiCol_ResizeGripActive;
            std::vector<float> ImGuiCol_Tab;
            std::vector<float> ImGuiCol_TabHovered;
            std::vector<float> ImGuiCol_TabActive;
            std::vector<float> ImGuiCol_TabUnfocused;
            std::vector<float> ImGuiCol_TabUnfocusedActive;
            std::vector<float> ImGuiCol_PlotLines;
            std::vector<float> ImGuiCol_PlotLinesHovered;
            std::vector<float> ImGuiCol_PlotHistogram;
            std::vector<float> ImGuiCol_PlotHistogramHovered;
            std::vector<float> ImGuiCol_TableHeaderBg;
            std::vector<float> ImGuiCol_TableBorderStrong;
            std::vector<float> ImGuiCol_TableBorderLight;
            std::vector<float> ImGuiCol_TableRowBg;
            std::vector<float> ImGuiCol_TableRowBgAlt;
            std::vector<float> ImGuiCol_TextSelectedBg;
            std::vector<float> ImGuiCol_DragDropTarget;
            std::vector<float> ImGuiCol_NavHighlight;
            std::vector<float> ImGuiCol_NavWindowingHighlight;
            std::vector<float> ImGuiCol_NavWindowingDimBg;
            std::vector<float> ImGuiCol_ModalWindowDimBg;
        };

        std::vector<float> HexToRGBA(const std::string& hex) // TODO: Figure out why colors look so saturated.
        {
            std::string hexValue = hex;
            if (hexValue[0] == '#') {
                hexValue = hexValue.substr(1);
            }

            // Validate hex value
            if (hexValue.length() != 8) {
                std::cout << "Error: Invalid hex value. Hex value must be in the format RRGGBBAA." << std::endl;
                return {};
            }

            float r, g, b, a;

            try {
                r = static_cast<float>(std::stoi(hexValue.substr(0, 2), nullptr, 16)) / 255.0f;
                g = static_cast<float>(std::stoi(hexValue.substr(2, 2), nullptr, 16)) / 255.0f;
                b = static_cast<float>(std::stoi(hexValue.substr(4, 2), nullptr, 16)) / 255.0f;
                a = static_cast<float>(std::stoi(hexValue.substr(6, 2), nullptr, 16)) / 255.0f;
            }
            catch (const std::exception& e) {
                std::cout << "Error: Invalid hex value. " << e.what() << std::endl;
                return {};
            }

            return { r, g, b, a };
        }

        UIProperties ColorSchemeDERQ {
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#FF8C8CFF"),
                HexToRGBA("#A61E1EFF"),
                HexToRGBA("#66111EFF"),
                HexToRGBA("#80111EFF"),
                HexToRGBA("#FF666A7F"),
                HexToRGBA("#30000000"),
                HexToRGBA("#73271A8A"),
                HexToRGBA("#FF666666"),
                HexToRGBA("#FF5555AA"),
                HexToRGBA("#701C1CFF"),
                HexToRGBA("#A43737FF"),
                HexToRGBA("#82010180"),
                HexToRGBA("#EB6F6FFF"),
                HexToRGBA("#7A1A1A8A"),
                HexToRGBA("#F0818199"),
                HexToRGBA("#FB575799"),
                HexToRGBA("#CC2E2E99"),
                HexToRGBA("#FC8282FF"),
                HexToRGBA("#F27B7BFF"),
                HexToRGBA("#CF2929FF"),
                HexToRGBA("#FB424266"),
                HexToRGBA("#FB4242FF"),
                HexToRGBA("#FB0A0AFF"),
                HexToRGBA("#4F0F0F4F"),
                HexToRGBA("#FB4242CC"),
                HexToRGBA("#F66A6AFF"),
                HexToRGBA("#FF8C8C7F"),
                HexToRGBA("#D855557C"),
                HexToRGBA("#DB3C3CFF"),
                HexToRGBA("#FB42424C"),
                HexToRGBA("#F8353521"),
                HexToRGBA("#FF5C5CF2"),
                HexToRGBA("#E94343DB"),
                HexToRGBA("#C43434CC"),
                HexToRGBA("#DE4747ff"),
                HexToRGBA("#261111F7"),
                HexToRGBA("#6B2424FF"),
                HexToRGBA("#9C9C9CFF"),
                HexToRGBA("#FF6F59FF"),
                HexToRGBA("#E6B300FF"),
                HexToRGBA("#FF9900FF"),
                HexToRGBA("#313131FF"),
                HexToRGBA("#4F4F55FF"),
                HexToRGBA("#3B3B40FF"),
                HexToRGBA("#00000000"),
                HexToRGBA("#FFFFFF0F"),
                HexToRGBA("#FA424259"),
                HexToRGBA("#FFFF00E6"),
                HexToRGBA("#FA4242FF"),
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#CCCCCC33"),
                HexToRGBA("#CCCCCC59")
        };

        UIProperties ColorSchemeDERQ2 {
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#8FC8FFFF"),
                HexToRGBA("#4D5484F2"),
                HexToRGBA("#4D5484F2"),
                HexToRGBA("#2B2E7FD2"),
                HexToRGBA("#80D9FFFF"),
                HexToRGBA("#00003300"),
                HexToRGBA("#3142938A"),
                HexToRGBA("#6680FFFF"),
                HexToRGBA("#5D53FFFF"),
                HexToRGBA("#243486FF"),
                HexToRGBA("#384BB5FF"),
                HexToRGBA("#03065180"),
                HexToRGBA("#6E74EFFF"),
                HexToRGBA("#2B2E7FD5"),
                HexToRGBA("#758DFEFF"),
                HexToRGBA("#5758FAFF"),
                HexToRGBA("#2E3AF0FF"),
                HexToRGBA("#829CFFFF"),
                HexToRGBA("#6E8CFFFF"),
                HexToRGBA("#2A2BCEFF"),
                HexToRGBA("#4B42F666"),
                HexToRGBA("#5342F8FF"),
                HexToRGBA("#2818F8FF"),
                HexToRGBA("#141F4F4F"),
                HexToRGBA("#4B42F8CC"),
                HexToRGBA("#687EFFCC"),
                HexToRGBA("#8CA9FFFF"),
                HexToRGBA("#5758D9C8"),
                HexToRGBA("#3D4CF0FF"),
                HexToRGBA("#4542F833"),
                HexToRGBA("#3235F7AA"),
                HexToRGBA("#5C6BFFFF"),
                HexToRGBA("#639AE5DA"),
                HexToRGBA("#3642C4CC"),
                HexToRGBA("#4860DEFF"),
                HexToRGBA("#12121FEC"),
                HexToRGBA("#242678FF"),
                HexToRGBA("#9c9c9cff"),
                HexToRGBA("#5A7DFFFF"),
                HexToRGBA("#E6B300FF"),
                HexToRGBA("#FF9900FF"),
                HexToRGBA("#313233FF"),
                HexToRGBA("#4f4f57FF"),
                HexToRGBA("#3B3B41FF"),
                HexToRGBA("#00000000"),
                HexToRGBA("#FFFFFF0F"),
                HexToRGBA("#444DFF59"),
                HexToRGBA("#FFFF00E6"),
                HexToRGBA("#424DFFFF"),
                HexToRGBA("#FFFFFFB2"),
                HexToRGBA("#CCCCCC33"),
                HexToRGBA("#CCCCCC59")
        };

        UIProperties ColorSchemeMISC {
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#808080FF"),
                HexToRGBA("#2E2E2EFF"),
                HexToRGBA("#47474700"),
                HexToRGBA("#505050FF"),
                HexToRGBA("#444444FF"),
                HexToRGBA("#00000000"),
                HexToRGBA("#292929FF"),
                HexToRGBA("#333333FF"),
                HexToRGBA("#474747FF"),
                HexToRGBA("#262626FF"),
                HexToRGBA("#262626FF"),
                HexToRGBA("#262626FF"),
                HexToRGBA("#313131FF"),
                HexToRGBA("#292929FF"),
                HexToRGBA("#464646FF"),
                HexToRGBA("#4D4D4DFF"),
                HexToRGBA("#FF641AFF"),
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#646464FF"),
                HexToRGBA("#FF641AFF"),
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#505050FF"),
                HexToRGBA("#767676FF"),
                HexToRGBA("#767676FF"),
                HexToRGBA("#8CB1FFFF"),
                HexToRGBA("#646464FF"),
                HexToRGBA("#ff641AFF"),
                HexToRGBA("#FFFFFF40"),
                HexToRGBA("#FFFFFFAA"),
                HexToRGBA("#FF641AFF"),
                HexToRGBA("#191919FF"),
                HexToRGBA("#5A5A5AFF"),
                HexToRGBA("#313131FF"),
                HexToRGBA("#191919FF"),
                HexToRGBA("#313131FF"),
                ////ImGuiCol_DockingPreview      = HexToRGBA("#FF6400C7"),
                ////ImGuiCol_DockingEmptyBg      = HexToRGBA("#2E2E2EFF"),
                HexToRGBA("#777777FF"),
                HexToRGBA("#FF6400FF"),
                HexToRGBA("#959595FF"),
                HexToRGBA("#FF6400FF"),
                HexToRGBA("#FFFFFFFF"),
                HexToRGBA("#FF6400FF"),
                HexToRGBA("#FF6400FF"),
                HexToRGBA("#FF6400FF"),
                HexToRGBA("#00000098"),
                HexToRGBA("#00000098"),
                HexToRGBA("#ff6400ff"),
                HexToRGBA("#ff6400ff"),
                HexToRGBA("#ff6400ff"),
                HexToRGBA("#00000094"),
                HexToRGBA("#00000094")
        };
    };
}

#endif //ENIGMAFIX_CLION_UIMANAGER_H
