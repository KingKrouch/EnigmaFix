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
#include <d3d11.h>
#include <vector>
#include <string>
#include <iostream>

namespace EnigmaFix {
    class UIManager {
    public:
        static UIManager &Get() {
            return um_Instance;
        };
        static void Begin(ID3D11Device* pDevice);
    private:
        static UIManager um_Instance;
        static void BeginRender();
        static void ShowMainMenu(bool* p_open);
        static void ShowStartupOverlay(bool* p_open);
        static void ShowAboutWindow(bool* p_open);
        static void HelpMarker(const char* desc);
        static void InitLocalization();
        void ActivateTheme();
        static void MainMenuOptions();
        static void LoopChecks();
        static void ShowExitPrompt();
        static void WindowButtons();
        static void LoadLogoTexture(ID3D11Device* pDevice);
        static void ResolutionOptions();
        static void AdjustDPIScaling(float scale_factor);


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
    };
}

#endif //ENIGMAFIX_CLION_UIMANAGER_H
