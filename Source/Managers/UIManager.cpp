/**
EnigmaFix Copyright (c) 2024 Bryce Q.

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

#include <d3d11.h>
#include "UIManager.h"
#include "PluginManager.h"
#include "PatchManager.h"
#include "../Settings/PlayerSettings.h"
#include "../Localization/Localization.h"
#include "../Utilities/UITextureLoader.h"
#include "../ThirdParty/ImGui/imgui.h"
#include "ConfigManager.h"

using namespace ImGui;
using namespace EnigmaFix;

bool initLoc = false;
bool startupNotice = false;
bool exitPrompt = false;
bool aboutPage = false;

static const char* resolutionOptions[]{ "2560x1080", "3440x1440", "3840x1080", "5120x1440" };
static int selectedResolutionOption = 0;

// Test Variables
bool PressedSave = false;
bool TestFullResPP = false;
static const char* ShadowOptions[]{ "Very Low (512)", "Low (1024)", "Medium (2048)", "High (4096)", "Ultra (8192)", "Extreme (16384)" };
static int ShadowOptionResolution[]{ 512, 1024, 2048, 4096, 8192, 16384 };
int ShadowResolution;

static const char* SSAOQualityOptions[]{ "Quarter Res", "Half Res", "Full" };
static int SSAOQualityDivider[]{ 4, 2, 1 };
int SelectedSSAOOption = 2;

static const char* SSRQualityOptions[]{ "Quarter Res", "Half Res", "Full" };
static int SSRQualityDivider[]{ 4, 2, 1 };
int SelectedSSROption = 2;

int SelectedShadowOption = 2;
static const char* InputOptions[]{ "Auto", "Xbox", "PlayStation", "Switch" };

int LogoWidth = 0;
int LogoHeight = 0;
ID3D11ShaderResourceView* Logo = nullptr;

// Singleton References
auto& LocUI = EnigmaFix::Localization::Get();
auto& ConManUI = EnigmaFix::ConfigManager::Get();
auto& SettingsUI = EnigmaFix::PlayerSettings::Get();
// Singleton Instance
EnigmaFix::UIManager EnigmaFix::UIManager::um_Instance;

namespace EnigmaFix {

    void UIManager::showStartupOverlay(bool p_open)
    {
        const float PAD = 10.0f;
        static int corner = 0;
        if (corner != -1)
        {
            const ImGuiViewport* viewport = GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
            SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        }
        if (::Begin("Startup Overlay", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove))
        {
            Text(LocUI.Strings.startupWindowWelcome);
            Text(LocUI.Strings.startupWindowInputPrompt);
            Separator();
            Text(LocUI.Strings.startupWindowCrackWarning);
        }
        End();
    }

    void UIManager::showAboutWindow(bool p_open)
    {
        if (::Begin(LocUI.Strings.aboutEnigmaFix, &p_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
        {
            ImGui::Image((void*)Logo, ImVec2(LogoWidth, LogoHeight));
            Text(LocUI.Strings.enigmaFixName, LocUI.Strings.versionNumber);
            Separator();
            Text("(C) 2021 Bryce Q.");
            Text(LocUI.Strings.enigmaFixLicense);
            Text(LocUI.Strings.donations);
            Separator();
            Text(LocUI.Strings.specialThanksTo);
            Text(LocUI.Strings.specialThanksLine);
            Text(LocUI.Strings.specialThanksLine2);
            Separator();
            Text(LocUI.Strings.thirdPartySoftware);
            Text(LocUI.Strings.imguiLicense);
            Text(LocUI.Strings.kieroHookLicense);
            Text(LocUI.Strings.ghFearLicense);
            Text(LocUI.Strings.thirteenAGIniLicense);
            Text(LocUI.Strings.thirteenAGAsiLicense);
            Text(LocUI.Strings.minHookLicense);
            Text(LocUI.Strings.modUtilsLicense);
            Text(LocUI.Strings.fontsLicense);
            Text(LocUI.Strings.fontAwesomeLicense);
            Separator();
            if (Button(LocUI.Strings.button_Close))
            {
                aboutPage = false;
            }
        }
        End();
    }

    void UIManager::HelpMarker(const char* desc)
    {
        TextDisabled("(?)");
        if (IsItemHovered())
        {
            BeginTooltip();
            PushTextWrapPos(GetFontSize() * 35.0f);
            TextUnformatted(desc);
            PopTextWrapPos();
            EndTooltip();
        }
    }

    void UIManager::initLocalization()
    {
        while (!initLoc) {
            // Initialize localization strings. I still need to find a way to get Japanese and Traditional Chinese strings to appear without "????"s.
            LocUI.InitLoc();
            break;
        }
        initLoc = true; // Confirms that these values have been initialized.
    }

    ImVec4 VectorToVec4(std::vector<float> Vec){
        Vec.resize(4);
        return ImVec4(Vec[0], Vec[1], Vec[2], Vec[3]);
    }

    void UIManager::activateTheme()
    {
        // Window and Border theming
        ImGuiStyle* style        = &GetStyle();
        ImVec4* colors           = style->Colors;
        style->FrameRounding     = 2.0f;
        style->FrameBorderSize   = 1.0f;
        style->PopupBorderSize   = 2.0f;
        style->PopupBorderSize   = 1.0f;
        style->WindowRounding    = 6.0f;
        style->PopupRounding     = 6.0f;
        style->ScrollbarRounding = 6.0f;
        UIProperties Color;

        // Color Scheme
        switch (SettingsUI.GameMode) // I should find out how to put this in a struct instead and just have it access a pointer that switches the reference based on the game mode.
        {
            case PlayerSettings::DERQ: // Death end re;Quest
            {
                Color = ColorSchemeDERQ;
                break;
            }
            case PlayerSettings::DERQ2: // Death end re;Quest 2
            {
                Color = ColorSchemeDERQ2;
                break;
            }
            default: // Misc
            {
                Color = ColorSchemeMISC;
                break;
            }
        }
        colors[ImGuiCol_Text]                  = VectorToVec4(Color.ImGuiCol_Text);
        colors[ImGuiCol_TextDisabled]          = VectorToVec4(Color.ImGuiCol_Text);
        colors[ImGuiCol_WindowBg]              = VectorToVec4(Color.ImGuiCol_WindowBg);
        colors[ImGuiCol_ChildBg]               = VectorToVec4(Color.ImGuiCol_ChildBg);
        colors[ImGuiCol_PopupBg]               = VectorToVec4(Color.ImGuiCol_PopupBg);
        colors[ImGuiCol_Border]                = VectorToVec4(Color.ImGuiCol_Border);
        colors[ImGuiCol_BorderShadow]          = VectorToVec4(Color.ImGuiCol_BorderShadow);
        colors[ImGuiCol_FrameBg]               = VectorToVec4(Color.ImGuiCol_FrameBg);
        colors[ImGuiCol_FrameBgHovered]        = VectorToVec4(Color.ImGuiCol_FrameBgHovered);
        colors[ImGuiCol_FrameBgActive]         = VectorToVec4(Color.ImGuiCol_FrameBgActive);
        colors[ImGuiCol_TitleBg]               = VectorToVec4(Color.ImGuiCol_TitleBg);
        colors[ImGuiCol_TitleBgActive]         = VectorToVec4(Color.ImGuiCol_TitleBgActive);
        colors[ImGuiCol_TitleBgCollapsed]      = VectorToVec4(Color.ImGuiCol_TitleBgCollapsed);
        colors[ImGuiCol_MenuBarBg]             = VectorToVec4(Color.ImGuiCol_MenuBarBg);
        colors[ImGuiCol_ScrollbarBg]           = VectorToVec4(Color.ImGuiCol_ScrollbarBg);
        colors[ImGuiCol_ScrollbarGrab]         = VectorToVec4(Color.ImGuiCol_ScrollbarGrab);
        colors[ImGuiCol_ScrollbarGrabHovered]  = VectorToVec4(Color.ImGuiCol_ScrollbarGrabHovered);
        colors[ImGuiCol_ScrollbarGrabActive]   = VectorToVec4(Color.ImGuiCol_ScrollbarGrabActive);
        colors[ImGuiCol_CheckMark]             = VectorToVec4(Color.ImGuiCol_CheckMark);
        colors[ImGuiCol_SliderGrab]            = VectorToVec4(Color.ImGuiCol_SliderGrab);
        colors[ImGuiCol_SliderGrabActive]      = VectorToVec4(Color.ImGuiCol_SliderGrabActive);
        colors[ImGuiCol_Button]                = VectorToVec4(Color.ImGuiCol_Button);
        colors[ImGuiCol_ButtonHovered]         = VectorToVec4(Color.ImGuiCol_ButtonHovered);
        colors[ImGuiCol_ButtonActive]          = VectorToVec4(Color.ImGuiCol_ButtonActive);
        colors[ImGuiCol_Header]                = VectorToVec4(Color.ImGuiCol_Header);
        colors[ImGuiCol_HeaderHovered]         = VectorToVec4(Color.ImGuiCol_HeaderHovered);
        colors[ImGuiCol_HeaderActive]          = VectorToVec4(Color.ImGuiCol_HeaderActive);
        colors[ImGuiCol_Separator]             = VectorToVec4(Color.ImGuiCol_Separator);
        colors[ImGuiCol_SeparatorHovered]      = VectorToVec4(Color.ImGuiCol_SeparatorHovered);
        colors[ImGuiCol_SeparatorActive]       = VectorToVec4(Color.ImGuiCol_SeparatorActive);
        colors[ImGuiCol_ResizeGrip]            = VectorToVec4(Color.ImGuiCol_ResizeGrip);
        colors[ImGuiCol_ResizeGripHovered]     = VectorToVec4(Color.ImGuiCol_ResizeGripHovered);
        colors[ImGuiCol_ResizeGripActive]      = VectorToVec4(Color.ImGuiCol_ResizeGripActive);
        colors[ImGuiCol_Tab]                   = VectorToVec4(Color.ImGuiCol_Tab);
        colors[ImGuiCol_TabHovered]            = VectorToVec4(Color.ImGuiCol_TabHovered);
        colors[ImGuiCol_TabActive]             = VectorToVec4(Color.ImGuiCol_TabActive);
        colors[ImGuiCol_TabUnfocused]          = VectorToVec4(Color.ImGuiCol_TabUnfocused);
        colors[ImGuiCol_TabUnfocusedActive]    = VectorToVec4(Color.ImGuiCol_TabUnfocusedActive);
        colors[ImGuiCol_PlotLines]             = VectorToVec4(Color.ImGuiCol_PlotLines);
        colors[ImGuiCol_PlotLinesHovered]      = VectorToVec4(Color.ImGuiCol_PlotLinesHovered);
        colors[ImGuiCol_PlotHistogram]         = VectorToVec4(Color.ImGuiCol_PlotHistogram);
        colors[ImGuiCol_PlotHistogramHovered]  = VectorToVec4(Color.ImGuiCol_PlotHistogramHovered);
        colors[ImGuiCol_TableHeaderBg]         = VectorToVec4(Color.ImGuiCol_TableHeaderBg);
        colors[ImGuiCol_TableBorderStrong]     = VectorToVec4(Color.ImGuiCol_TableBorderStrong);
        colors[ImGuiCol_TableBorderLight]      = VectorToVec4(Color.ImGuiCol_TableBorderLight);
        colors[ImGuiCol_TableRowBg]            = VectorToVec4(Color.ImGuiCol_TableRowBg);
        colors[ImGuiCol_TableRowBgAlt]         = VectorToVec4(Color.ImGuiCol_TableRowBgAlt);
        colors[ImGuiCol_TextSelectedBg]        = VectorToVec4(Color.ImGuiCol_TextSelectedBg);
        colors[ImGuiCol_DragDropTarget]        = VectorToVec4(Color.ImGuiCol_DragDropTarget);
        colors[ImGuiCol_NavHighlight]          = VectorToVec4(Color.ImGuiCol_NavHighlight);
        colors[ImGuiCol_NavWindowingHighlight] = VectorToVec4(Color.ImGuiCol_NavWindowingHighlight);
        colors[ImGuiCol_NavWindowingDimBg]     = VectorToVec4(Color.ImGuiCol_NavWindowingDimBg);
        colors[ImGuiCol_ModalWindowDimBg]      = VectorToVec4(Color.ImGuiCol_ModalWindowDimBg);
    }

    void UIManager::mainMenuOptions()
    {
        if (CollapsingHeader(LocUI.Strings.collapsingHeader_Resolution), ImGuiTreeNodeFlags_Leaf)
        {
            ImGui::Checkbox(LocUI.Strings.checkbox_UseCustomRes, &SettingsUI.RES.UseCustomRes);
            if (SettingsUI.RES.UseCustomRes)
            {
                Combo(LocUI.Strings.combobox_CustomResolution, &selectedResolutionOption, resolutionOptions, IM_ARRAYSIZE(resolutionOptions));
                SameLine(); HelpMarker(LocUI.Strings.helpmarker_CustomResolution);
            }
            ImGui::Checkbox(LocUI.Strings.checkbox_UseCustomResScale, &SettingsUI.RES.UseCustomResScale);
            if (SettingsUI.RES.UseCustomResScale)
            {
                ImGui::DragInt(LocUI.Strings.dragInt_CustomResScale, &SettingsUI.RES.CustomResScale, 25, 100);
                SameLine(); HelpMarker(LocUI.Strings.helpmarker_CustomResScale);
            }
            ImGui::Text("Internal Resolution: %d x %d", static_cast<int>(SettingsUI.INS.InternalHorizontalRes), static_cast<int>(SettingsUI.INS.InternalVerticalRes));
        }
        if (CollapsingHeader(LocUI.Strings.collapsingHeader_Fov), ImGuiTreeNodeFlags_Leaf)
        {
            ImGui::Checkbox(LocUI.Strings.checkbox_customFOV, &SettingsUI.FOV.UseCustomFOV);
            if (SettingsUI.FOV.UseCustomFOV)
            {
                ImGui::SliderInt(LocUI.Strings.sliderInt_customFOV, &SettingsUI.FOV.FieldOfView, 44, 90);
                SameLine(); HelpMarker(LocUI.Strings.helpmarker_customFOV);
                //fovPatch(SettingsUI.FOV.FieldOfView);
            }
            else
            {

            }
        }
        if (CollapsingHeader(LocUI.Strings.collapsingHeader_Framerate), ImGuiTreeNodeFlags_Leaf)
        {
            ImVec4 framerateBad = ImVec4(1.0f, 0.7f, 0.7f, 1.0f);
            ImVec4 framerateGood = ImVec4(0.7f, 1.0f, 0.7f, 1.0f);
            ImGui::Checkbox(LocUI.Strings.checkbox_VSync, &SettingsUI.SYNC.VSync);
            SameLine(); HelpMarker(LocUI.Strings.helpmarker_VSync);
            if (SettingsUI.SYNC.VSync)
            {
                ImGui::SliderInt(LocUI.Strings.sliderInt_syncInterval, &SettingsUI.SYNC.SyncInterval, 1, 4);
                SameLine(0.0, -1.0); HelpMarker(LocUI.Strings.helpmarker_syncInterval);
            }
            else
            {
                SettingsUI.SYNC.SyncInterval = 0;
            }
            ImGui::InputInt(LocUI.Strings.sliderInt_FramerateCap, &SettingsUI.SYNC.MaxFPS, 1, 100, 0);
            SameLine(0.0, -1.0); HelpMarker(LocUI.Strings.helpmarker_FramerateCap);
            // Adds the Framerate and Frametime counter and changes the color of the text based on if the framerate is < 60FPS (red), or >= 60FPS (green).
            // Since I can't put this in a Switch/Case statement apparently, the YandereDev-tier "IF/ELSE" statement shenanigans will have to do for now.
            if (GetIO().Framerate >= 60.0f)
            {
                Text(LocUI.Strings.text_Frametime); SameLine(); TextColored(framerateGood, "%.3f ms", 1000.0f / GetIO().Framerate);
                Text(LocUI.Strings.text_Framerate); SameLine(); TextColored(framerateGood, "%.1f FPS", GetIO().Framerate);
            }
            else
            {
                Text(LocUI.Strings.text_Frametime); SameLine(); TextColored(framerateBad, "%.3f ms", 1000.0f / GetIO().Framerate);
                Text(LocUI.Strings.text_Framerate); SameLine(); TextColored(framerateBad, "%.1f FPS", GetIO().Framerate);
            }
        }
        if (CollapsingHeader(LocUI.Strings.collapsingHeader_Rendering), ImGuiTreeNodeFlags_Leaf)
        {
            ImGui::Checkbox(LocUI.Strings.checkbox_ShadowRendering, &SettingsUI.RS.Shadows);
            if (SettingsUI.RS.Shadows)
            {
                Combo(LocUI.Strings.combobox_ShadowQuality, &SelectedShadowOption, ShadowOptions, IM_ARRAYSIZE(ShadowOptions));
                SameLine();
                HelpMarker(LocUI.Strings.helpmarker_ShadowQuality);
            }
            ImGui::Checkbox(LocUI.Strings.checkbox_SSAO, &SettingsUI.RS.SSAO);
            SameLine();
            HelpMarker(LocUI.Strings.helpmarker_SSAO);
            ImGui::Checkbox(LocUI.Strings.checkbox_CharacterOutlines, &SettingsUI.RS.EdgeRendering);
            SameLine();
            HelpMarker(LocUI.Strings.helpmarker_CharacterOutlines);
            ImGui::Checkbox(LocUI.Strings.checkbox_GI, &SettingsUI.RS.IBL);
            SameLine();
            HelpMarker(LocUI.Strings.helpmarker_GI);
            ImGui::Checkbox(LocUI.Strings.checkbox_DoF, &SettingsUI.RS.DepthOfField);
            SameLine();
            HelpMarker(LocUI.Strings.helpmarker_DoF);
            ImGui::Checkbox(LocUI.Strings.checkbox_TAA, &SettingsUI.RS.TAA);
            SameLine();
            HelpMarker(LocUI.Strings.helpmarker_TAA);
            ImGui::Checkbox(LocUI.Strings.checkbox_Foliage, &SettingsUI.RS.FoliageRendering);
            SameLine();
            HelpMarker(LocUI.Strings.helpmarker_Foliage);
        }
        if (CollapsingHeader(LocUI.Strings.collapsingHeader_Input), ImGuiTreeNodeFlags_Leaf)
        {
            ImGui::Combo(LocUI.Strings.combobox_ControlType, &SettingsUI.IS.InputDeviceType, InputOptions, IM_ARRAYSIZE(InputOptions));
            SameLine();
            HelpMarker(LocUI.Strings.helpmarker_ControlType);
            // Add something here for KB/M prompts if that ever becomes a thing, due to the complexities of remapping, and other circumstances.
        }
        if (CollapsingHeader(LocUI.Strings.collapsingHeader_Misc), ImGuiTreeNodeFlags_Leaf)
        {
            ImGui::Checkbox(LocUI.Strings.checkbox_SkipOP, &SettingsUI.MS.SkipOpeningVideos);
            ImGui::Checkbox(LocUI.Strings.checkbox_CameraTweaks, &SettingsUI.MS.CameraTweaks);
            SameLine();
            HelpMarker(LocUI.Strings.helpmarker_CameraTweaks);
        }
        if (CollapsingHeader(LocUI.Strings.collapsingHeader_Launcher), ImGuiTreeNodeFlags_Leaf)
        {
            ImGui::Checkbox(LocUI.Strings.checkbox_IgnoreUpdates, &SettingsUI.LS.IgnoreUpdates);
        }
    }

    void UIManager::windowButtons()
    {
        if (Button(LocUI.Strings.button_Save))
        {
            ConManUI.SaveConfig();
            startupNotice = true; // For now, this is tied to the save function for quick testing.
        }
        SameLine();
        if (Button(LocUI.Strings.button_About))
        {
            aboutPage = true;
        }
        SameLine();
        if (Button(LocUI.Strings.button_Close))
        {
            //showUI = false;

            // Always center this window when appearing
            ImVec2 center = GetMainViewport()->GetCenter();
            SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            OpenPopup(LocUI.Strings.gameExit);
        }
    }

    void UIManager::showExitPrompt()
    {
        Text(LocUI.Strings.exitTextPrompt);
        Separator();

        PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        PopStyleVar();

        if (Button(LocUI.Strings.exitOK, ImVec2(120, 0)))
        {
            exit(0);
        }
        SetItemDefaultFocus();
        SameLine();
        if (Button(LocUI.Strings.exitCancel, ImVec2(120, 0)))
        {
            CloseCurrentPopup();
        }
        EndPopup();
    }

    void UIManager::loopChecks()
    {
        if (startupNotice)
        {
            showStartupOverlay(&startupNotice);
        }
        if (aboutPage)
        {
            showAboutWindow(&aboutPage);
        }
        if (BeginPopupModal(LocUI.Strings.gameExit, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
        {
            showExitPrompt();
        }

        switch (SelectedShadowOption)
        {
            case 0:
            {
                ShadowResolution = ShadowOptionResolution[0];
                break;
            }
            case 1:
            {
                ShadowResolution = ShadowOptionResolution[1];
                break;
            }
            case 2:
            {
                ShadowResolution = ShadowOptionResolution[2];
                break;
            }
            case 3:
            {
                ShadowResolution = ShadowOptionResolution[3];
                break;
            }
            case 4:
            {
                ShadowResolution = ShadowOptionResolution[4];
                break;
            }
            case 5:
            {
                ShadowResolution = ShadowOptionResolution[5];
                break;
            }
        }
    }

    void UIManager::showMainMenu(bool p_open)
    {
        if (ImGui::Begin(LocUI.Strings.gameName, &p_open, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            // Creates the main menu UI and enables our custom theming.
            um_Instance.activateTheme();
            // Spawns the main menu logic.
            um_Instance.mainMenuOptions();
            Separator();
            um_Instance.windowButtons();
        }
    }

    void UIManager::LoadLogoTexture() // TODO: Fix this not loading the texture properly.
    {
        bool ret = LoadTextureFromFile("Resources/EnigmaFix_Logo.png", &Logo, &LogoWidth, &LogoHeight);
        IM_ASSERT(ret);
    }

    void UIManager::BeginRender()
    {
        um_Instance.showMainMenu(SettingsUI.ShowUI); // showMainMenu is fine on it's own, we just need a boolean that actually works
        // Runs some menu checks per-frame, since I seemingly can't get it so these checks don't happen per-frame.
        um_Instance.loopChecks();
    }

    void UIManager::Begin()
    {
        // Checks if the localization strings have been initialized.
        um_Instance.initLocalization();
        // Loads the About screen's logo texture.
        um_Instance.LoadLogoTexture();
        // Begins drawing the UI.
        um_Instance.BeginRender();
    }
}