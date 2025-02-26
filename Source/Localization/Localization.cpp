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
#include "Localization.h"
#include "../Settings/PlayerSettings.h"
#include "tchar.h"
#include "imgui.h"
// System Libraries
#include <windows.h>
// Variables
auto& PlayerSettingsLoc = EnigmaFix::PlayerSettings::Get();
// Singleton Instance
EnigmaFix::Localization EnigmaFix::Localization::loc_Instance;

namespace EnigmaFix {
    void Localization::InitFont() {
        language = 0; // Dummy Variable for testing language support.
        ImGuiIO& io = ImGui::GetIO();
        // Makes an ImFontConfig for our main fonts.
        ImFontConfig font_Config;
        // Loads some Windows English, Japanese, and Traditional Chinese fonts. Note that these locales need to be installed in order to use these.
        io.Fonts->AddFontFromFileTTF("resources\\fonts\\NotoSans-Regular.ttf", fontSize, &font_Config, io.Fonts->GetGlyphRangesDefault()); // Loads the font for English strings first.
        // Uses a switch statement instead of loading all of these at once, to prevent even worse massive hitching from happening. It's already bad enough with just JP and CN fonts.
        switch (language) {
            case 1: { // Japanese
                io.Fonts->AddFontFromFileTTF("resources\\fonts\\NotoSansJP-Regular.otf", fontSize, &font_Config, io.Fonts->GetGlyphRangesJapanese());
                break;
            }
            case 2: { // Traditional Chinese
                io.Fonts->AddFontFromFileTTF("resources\\fonts\\NotoSansTC-Regular.otf", fontSize, &font_Config, io.Fonts->GetGlyphRangesChineseFull());
                break;
            }
        }
        // Adds FontAwesome font support to ImGui, so icons can be used in the User Interface.
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = fontSize;
        static const ImWchar icon_ranges[] = { 0xe005, 0xf8ff, 0 };
        // Finally adds the FontAwesome fonts to the available fonts for ImGui to pick from.
        io.Fonts->AddFontFromFileTTF("resources\\fonts\\fa-solid-900.ttf", fontSize, &icons_config, icon_ranges);
        io.Fonts->Build(); // Build the font atlas, which seemingly fixes our hitching issue from earlier when using Japanese or Chinese fonts.
    }
    void Localization::InitLoc() {
        switch (language) { //checks what Steam's API says, if the game is running in english, japanese, or tchinese (traditional chinese), and then switches strings based on that.
            case 0: { // English
                // General Localization.
                switch (PlayerSettingsLoc.GameMode) {
                    case PlayerSettings::DERQ: { // Death end re;Quest
                        Strings.gameName = "EnigmaFix (Death end re;Quest)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Death end re;Quest."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::DERQ2: { // Death end re;Quest 2
                        Strings.gameName = "EnigmaFix (Death end re;Quest 2)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Death end re;Quest 2."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::Varnir: { // Dragon Star Varnir
                        Strings.gameName = "EnigmaFix (Dragon Star Varnir)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Dragon Star Varnir."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::VIIR: { // Neptunia VIIR
                        Strings.gameName = "EnigmaFix (MegaDimension Neptunia VIIR)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for MegaDimension Neptunia VIIR."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::NVS: { // Neptunia Virtual Stars
                        Strings.gameName = "EnigmaFix (Neptunia Virtual Stars)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Neptunia Virtual Stars."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::MS2: { // Mary Skelter 2
                        Strings.gameName = "EnigmaFix (Mary Skelter 2)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Mary Skelter 2."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::MSF: { // Mary Skelter Finale
                        Strings.gameName = "EnigmaFix (Mary Skelter Finale)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Mary Skelter Finale."; // Startup Overlay String
                        break;
                    }
                }
                Strings.enigmaFixName = "EnigmaFix";
                //Localization for the About EnigmaFix Window.
                Strings.aboutEnigmaFix = "About EnigmaFix";
                Strings.versionNumber = "Version 1.0";
                Strings.enigmaFixLicense = "EnigmaFix is licensed under the MIT License, see LICENSE for more information.";
                Strings.donations = "Donations are accepted at: ko-fi.com/kingkrouch";
                Strings.specialThanksTo = "Special thanks to:";
                Strings.specialThanksLine = "Durante (PeterTh) for his wonderful guide on graphics API manipulation, and for answering some questions of mine.";
                Strings.specialThanksLine2 = "Kaldaien for consulting and advice regarding several things. Much obliged!";
                Strings.specialThanksLine3 = "Silent/CookiePLMonster for patiently helping me out with questions regarding his memory utilities and memory manipulation stuff in general. I owe you one!";
                Strings.specialThanksLine4 = "tlaik (author of the Neptastic mod for HyperDimension Neptunia Rebirth 1-3), AterialDawn (author of the MegaResolution Neptunia mod), Skyth/blueskythlikesclouds, and SeleDreams (author of the CoroHeart framework) for programming consultation.";
                Strings.thirdPartySoftware = "Third-Party Software:";
                Strings.imguiLicense = "Uses ImGui, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.kieroHookLicense = "Uses rdbo's ImGui-DirectX-11-Kiero-Hook.";
                Strings.ghFearLicense = "Uses portions of GHFear's Universal-ImGui-D3D11-Hook-WithResize.";
                Strings.thirteenAGIniLicense = "Uses ThirteenAG's IniReader, which is licensed under the stipulation that the software license is mentioned.";
                Strings.thirteenAGAsiLicense = "Uses ThirteenAG's Ultimate-ASI-Loader, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.minHookLicense = "Uses MinHook, which is licensed under the stipulation that the software license is included, see LICENSE for more information.";
                Strings.modUtilsLicense = "Uses CookiePLMonster's ModUtils, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.fontsLicense = "Uses NotoSans, NotoSans-JP, and NotoSans-TC, which are licensed under the APACHE and SIL Open Font Licenses. See LICENSE and OFL for more information.";
                Strings.fontAwesomeLicense = "Uses FontAwesome by Dave Gandy - fontawesome.io";
                Strings.button_Close = "Close";
                //Localization for the EnigmaFix User Interface.
                Strings.collapsingHeader_Resolution = "\xef\x89\xac" "  Resolution";
                Strings.checkbox_UseCustomRes = "Use Custom Resolution";
                Strings.combobox_CustomResolution = "Resolution (Replaces the 4K Native option)";
                Strings.helpmarker_CustomResolution = "This option will allow selecting a resolution different than the ones available in-game.";
                Strings.checkbox_UseCustomResScale = "Custom Resolution Scale";
                Strings.dragInt_CustomResScale = "Custom Resolution Scale";
                Strings.helpmarker_CustomResScale = "This option will allow upsampling from a lower screen resolution, usually done for performance reasons.";
                Strings.collapsingHeader_Fov = "\xef\x81\xae" "  Field of View";
                Strings.checkbox_customFOV = "Enable Custom FOV";
                //Strings.sliderInt_customFOV = u8"FOV (Default: 44°)";
                Strings.sliderInt_customFOV = "FOV (Default: 44°)";
                Strings.helpmarker_customFOV = "This option will allow increasing the FOV during gameplay.";
                Strings.collapsingHeader_Framerate = "\xef\x8f\xbd" "  Framerate";
                Strings.checkbox_VSync = "VSync";
                Strings.helpmarker_VSync = "This option will enable or disable the enforced vertical sync that the game enforces.\n" "\n" "Keep in mind that while disabling VSync may reduce input latency, it can cause screen tearing.";
                Strings.sliderInt_syncInterval = "Sync Interval";
                Strings.helpmarker_syncInterval = "This option will allow adjusting the Sync Interval when VSync is enabled.\n" "\n" "1 = Default VSync behavior\n" "\n" "2 = Half refresh rate\n" "\n" "3 = 1/3rd refresh rate\n" "\n" "4 = Quarter refresh rate.";
                Strings.sliderInt_FramerateCap = "Framerate Cap";
                Strings.helpmarker_FramerateCap = "You can adjust the maximum framerate that the game can run at.\n" "\n" "Speed related issues with Character movement (in battles) or 2D animations may occur.";
                Strings.text_Frametime = "Frametime: ";
                Strings.text_Framerate = "Framerate: ";
                Strings.collapsingHeader_Rendering = "\xef\x87\xbc" "  Rendering";
                Strings.checkbox_ShadowRendering = "Shadow Rendering";
                Strings.combobox_ShadowQuality = "Shadow Quality";
                Strings.helpmarker_ShadowQuality = "This option allows for the adjustment of shadow resolution.\n" "\n" "As a result, this option may come with a performance impact when using higher quality presets.\n" "\n" "Available options are Very Low (512), Low (1024), Medium (2048), High (4096), Ultra (8192), and Extreme (16384).";
                Strings.checkbox_SSAO = "Screen Space Ambient Occlusion (SSAO)";
                Strings.helpmarker_SSAO = "This option controls whether or not objects that are occluded from light should behave as such.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.combobox_ssaoQuality = "SSAO Quality";
                Strings.helpmarker_ssaoQuality = "";
                Strings.checkbox_SSR = "Screen Space Reflections (SSR)";
                Strings.helpmarker_SSR = "This option controls whether or not objects in screen space are reflected.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.combobox_ssrQuality = "SSR Quality";
                Strings.helpmarker_ssrQuality = "";
                Strings.checkbox_CharacterOutlines = "Character Outlines";
                Strings.helpmarker_CharacterOutlines = "This option disables character outlines.\n" "\n" "Disabling this option may or may not result in improved performance.\n";
                Strings.checkbox_GI = "Global Illumination/Bounce Lighting";
                Strings.helpmarker_GI = "This option controls whether or not the screen should have darkened edges.\n" "\n" "Disabling this option will likely not improve/worsen performance, so it's mostly up to preference.";
                Strings.checkbox_DoF = "Depth of Field";
                Strings.helpmarker_DoF = "This option controls whether or not elements on screen should be blurred when not in focus.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_TAA = "Temporal Anti-Aliasing (TAA)";
                Strings.helpmarker_TAA = "This option controls whether or not TAA is used.\n" "\n" "Disabling may result in more details being clear, but at the cost of edge and specular aliasing.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_Foliage = "Foliage";
                Strings.helpmarker_Foliage = "This option controls whether or not clusters of foliage are rendered.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_MotionBlur = "Motion Blur";
                Strings.helpmarker_MotionBlur = "This option enables motion blur, which is in the engine, but not used in-game. It helps make running at 30FPS (or anything below 120FPS) more bearable.";
                Strings.collapsingHeader_Input = "\xef\x84\x9b" "  Input";
                Strings.combobox_ControlType = "Control Type";
                Strings.helpmarker_ControlType = "This option controls what type of input prompts on game UI elements should appear when using a controller.\n" "\n" "Auto will use SteamInput to automatically switch the input prompts based on what controller is being used.";
                Strings.collapsingHeader_Misc = "\xef\x81\x99" "  Misc";
                Strings.checkbox_SkipOP = "Skip Opening Videos";
                Strings.checkbox_CameraTweaks = "Camera Tweaks";
                Strings.helpmarker_CameraTweaks = "This option will improve the vertical clamping range of camera movement.";
                Strings.collapsingHeader_Launcher = "\xef\x8b\x90" "  Launcher";
                Strings.checkbox_IgnoreUpdates = "Ignore Updates";
                Strings.button_Save = "Save";
                Strings.button_About  = "About";
                // Developer Console
                Strings.consoleName = "Developer Console";
                // Startup Overlay Strings
                Strings.startupWindowInputPrompt = "Press the 'DELETE' key to open the configuration window.";
                Strings.startupWindowCrackWarning = "Cracked copies will not receive technical support.";
                // Exit Window
                Strings.gameExit = "Game Exit";
                Strings.exitTextPrompt = "Quit the game?" "\n" "Game progress will be lost if you have not saved your game." "\n";
                Strings.exitOK = "OK";
                Strings.exitCancel = "Cancel";
                break;
            }
            case 1: { // Japanese
                // General Localization.
                switch (PlayerSettingsLoc.GameMode) {
                    case PlayerSettings::DERQ: { // Death end re;Quest
                        Strings.gameName = "エニグマフィクス (デス エンド リクエスト)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Death end re;Quest."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::DERQ2: { // Death end re;Quest 2
                        Strings.gameName = "エニグマフィクス (	デス エンド リクエスト2)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Death end re;Quest 2."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::Varnir: { // Dragon Star Varnir
                        Strings.gameName = "エニグマフィクス (竜星のヴァルニール)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Dragon Star Varnir."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::VIIR: { // Neptunia VIIR
                        Strings.gameName = "エニグマフィクス (新次元ゲイム ネプテューヌVIIR)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for MegaDimension Neptunia VIIR."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::NVS: { // Neptunia Virtual Stars
                        Strings.gameName = "エニグマフィクス (ブイブイブイテューヌ)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Neptunia Virtual Stars."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::MS2: { // Mary Skelter 2
                        Strings.gameName = "エニグマフィクス (神獄塔 メアリスケルター2)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Mary Skelter 2."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::MSF: { // Mary Skelter Finale
                        Strings.gameName = "エニグマフィクス (神獄塔 メアリスケルターFinale)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Mary Skelter Finale."; // Startup Overlay String
                        break;
                    }
                }
                Strings.enigmaFixName = "エニグマフィクス";
                Strings.aboutEnigmaFix = "エニグマフィクスについて";
                Strings.enigmaFixLicense = "EnigmaFix is licensed under the MIT License, see LICENSE for more information.";
                Strings.donations = "Donations are accepted at: ko-fi.com/kingkrouch";
                Strings.specialThanksTo = "Special thanks to:";
                Strings.specialThanksLine = "Durante (PeterTh) for his wonderful guide on graphics API manipulation, and for answering some questions of mine.";
                Strings.specialThanksLine2 = "Kaldaien for consulting and advice regarding several things. Much obliged!";
                Strings.specialThanksLine3 = "Silent/CookiePLMonster for patiently helping me out with questions regarding his memory utilities and memory manipulation stuff in general. I owe you one!";
                Strings.specialThanksLine4 = "tlaik (author of the Neptastic mod for HyperDimension Neptunia Rebirth 1-3), AterialDawn (author of the MegaResolution Neptunia mod), Skyth/blueskythlikesclouds, and SeleDreams (author of the CoroHeart framework) for programming consultation.";
                Strings.thirdPartySoftware = "Third-Party Software:";
                Strings.imguiLicense = "Uses ImGui, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.kieroHookLicense = "Uses rdbo's ImGui-DirectX-11-Kiero-Hook.";
                Strings.ghFearLicense = "Uses portions of GHFear's Universal-ImGui-D3D11-Hook-WithResize.";
                Strings.thirteenAGIniLicense = "Uses ThirteenAG's IniReader, which is licensed under the stipulation that the software license is mentioned.";
                Strings.thirteenAGAsiLicense = "Uses ThirteenAG's Ultimate-ASI-Loader, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.minHookLicense = "Uses MinHook, which is licensed under the stipulation that the software license is included, see LICENSE for more information.";
                Strings.modUtilsLicense = "Uses CookiePLMonster's ModUtils, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.fontsLicense = "Uses NotoSans, NotoSans-JP, and NotoSans-TC, which are licensed under the APACHE and SIL Open Font Licenses. See LICENSE and OFL for more information.";
                Strings.fontAwesomeLicense = "Uses FontAwesome by Dave Gandy - fontawesome.io";
                Strings.button_Close = "Close";
                //Localization for the EnigmaFix User Interface.
                Strings.collapsingHeader_Resolution = "\xef\x89\xac" "  解像度";
                Strings.checkbox_UseCustomRes = "カスタム解像度";
                Strings.combobox_CustomResolution = "解像度 (「4K Native」オプションを置き換えます)";
                Strings.helpmarker_CustomResolution = "This option will allow selecting a resolution different than the ones available in-game.";
                Strings.checkbox_UseCustomResScale = "解像度規模";
                Strings.dragInt_CustomResScale = "解像度規模";
                Strings.helpmarker_CustomResScale = "This option will allow upsampling from a lower screen resolution, usually done for performance reasons.";
                Strings.collapsingHeader_Fov = "\xef\x81\xae" "  視野";
                Strings.checkbox_customFOV = "カスタム視野 ";
                Strings.sliderInt_customFOV = "視野 (デフォルト: 45°)";
                Strings.helpmarker_customFOV = "This option will allow increasing the FOV during gameplay.";
                Strings.collapsingHeader_Framerate = "\xef\x8f\xbd" "  フレームレート";
                Strings.checkbox_VSync = "VSync";
                Strings.helpmarker_VSync = "This option will enable or disable the enforced vertical sync that the game enforces.\n" "\n" "Keep in mind that while disabling VSync may reduce input latency, it can cause screen tearing.";
                Strings.sliderInt_syncInterval = "Sync Interval";
                Strings.helpmarker_syncInterval = "This option will allow adjusting the Sync Interval when VSync is enabled.\n" "\n" "1 = Default VSync behavior\n" "\n" "2 = Half refresh rate\n" "\n" "3 = 1/3rd refresh rate\n" "\n" "4 = Quarter refresh rate.";
                Strings.sliderInt_FramerateCap = "フレームレートキャップ";
                Strings.helpmarker_FramerateCap = "You can adjust the maximum framerate that the game can run at.\n" "\n" "Speed related issues with Character movement (in battles) or 2D animations may occur.";
                Strings.text_Frametime = "フレームタイム: ";
                Strings.text_Framerate = "フレームレート: ";
                Strings.collapsingHeader_Rendering = "\xef\x87\xbc" "  レンダリング";
                Strings.checkbox_ShadowRendering = "Shadow Rendering";
                Strings.combobox_ShadowQuality = "Shadow Quality";
                Strings.helpmarker_ShadowQuality = "This option allows for the adjustment of shadow resolution.\n" "\n" "As a result, this option may come with a performance impact when using higher quality presets.\n" "\n" "Available options are Very Low (512), Low (1024), Medium (2048), High (4096), Ultra (8192), and Extreme (16384).";
                Strings.checkbox_SSAO = "Screen Space Ambient Occlusion (SSAO)";
                Strings.helpmarker_SSAO = "This option controls whether or not objects that are occluded from light should behave as such.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.combobox_ssaoQuality = "SSAO Quality";
                Strings.helpmarker_ssaoQuality = "";
                Strings.checkbox_SSR = "Screen Space Reflections (SSR)";
                Strings.helpmarker_SSR = "This option controls whether or not objects in screen space are reflected.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.combobox_ssrQuality = "SSR Quality";
                Strings.helpmarker_ssrQuality = "";
                Strings.checkbox_CharacterOutlines = "Character Outlines";
                Strings.helpmarker_CharacterOutlines = "This option disables character outlines.\n" "\n" "Disabling this option may or may not result in improved performance.\n";
                Strings.checkbox_GI = "Global Illumination/Bounce Lighting";
                Strings.helpmarker_GI = "This option controls whether or not the screen should have darkened edges.\n" "\n" "Disabling this option will likely not improve/worsen performance, so it's mostly up to preference.";
                Strings.checkbox_DoF = "Depth of Field";
                Strings.helpmarker_DoF = "This option controls whether or not elements on screen should be blurred when not in focus.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_TAA = "Temporal Anti-Aliasing (TAA)";
                Strings.helpmarker_TAA = "This option controls whether or not TAA is used.\n" "\n" "Disabling may result in more details being clear, but at the cost of edge and specular aliasing.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_Foliage = "Foliage";
                Strings.helpmarker_Foliage = "This option controls whether or not clusters of foliage are rendered.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_MotionBlur = "Motion Blur";
                Strings.helpmarker_MotionBlur = "This option enables motion blur, which is in the engine, but not used in-game. It helps make running at 30FPS (or anything below 120FPS) more bearable.";
                Strings.collapsingHeader_Input = "\xef\x84\x9b" "  入力";
                Strings.combobox_ControlType = "Control Type";
                Strings.helpmarker_ControlType = "This option controls what type of input prompts on game UI elements should appear when using a controller.\n" "\n" "Auto will use SteamInput to automatically switch the input prompts based on what controller is being used.";
                Strings.collapsingHeader_Misc = "\xef\x81\x99" "  その他";
                Strings.checkbox_SkipOP = "Skip Opening Videos";
                Strings.checkbox_CameraTweaks = "Camera Tweaks";
                Strings.helpmarker_CameraTweaks = "This option will improve the vertical clamping range of camera movement.";
                Strings.collapsingHeader_Launcher = "\xef\x8b\x90" "  ランチャー";
                Strings.checkbox_IgnoreUpdates = "Ignore Updates";
                Strings.button_Save = "セーブ";
                Strings.button_About = "About";
                // Developer Console
                Strings.consoleName = "Developer Console";
                // Startup Overlay Strings
                Strings.startupWindowInputPrompt = "Press the 'DELETE' key to open the configuration window.";
                Strings.startupWindowCrackWarning = "Cracked copies will not receive technical support.";
                // Exit Window
                Strings.gameExit = "Game Exit";
                Strings.exitTextPrompt = "Quit the game?" "\n" "Game progress will be lost if you have not saved your game." "\n";
                Strings.exitOK = "OK";
                Strings.exitCancel = "Cancel";
                break;
            }
            case 2: { // Traditional Chinese
                // General Localization.
                switch (PlayerSettingsLoc.GameMode) {
                    case PlayerSettings::DERQ: { // Death end re;Quest
                        Strings.gameName = "謎修理 (死亡終局 輪廻試練)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Death end re;Quest."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::DERQ2: { // Death end re;Quest 2
                        Strings.gameName = "謎修理 (死亡終局 輪迴試煉２)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Death end re;Quest 2."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::Varnir: { // Dragon Star Varnir
                        Strings.gameName = "謎修理 (龍星的瓦爾尼爾)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Dragon Star Varnir."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::VIIR: { // Neptunia VIIR
                        Strings.gameName = "謎修理 (新次元遊戲 戰機少女ＶⅡＲ)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for MegaDimension Neptunia VIIR."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::NVS: { // Neptunia Virtual Stars
                        Strings.gameName = "謎修理 (VVV戦機少女)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Neptunia Virtual Stars."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::MS2: { // Mary Skelter 2
                        Strings.gameName = "謎修理 (神獄塔 斷罪瑪麗 2)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Mary Skelter 2."; // Startup Overlay String
                        break;
                    }
                    case PlayerSettings::MSF: { // Mary Skelter Finale
                        Strings.gameName = "謎修理 (神獄塔 斷罪瑪麗 Finale)";
                        Strings.startupWindowWelcome = "Welcome to EnigmaFix for Mary Skelter Finale."; // Startup Overlay String
                        break;
                    }
                }
                Strings.enigmaFixName = "謎修理";
                Strings.aboutEnigmaFix = "關於謎修理";
                Strings.enigmaFixLicense = "EnigmaFix is licensed under the MIT License, see LICENSE for more information.";
                Strings.donations = "Donations are accepted at: ko-fi.com/kingkrouch";
                Strings.specialThanksTo = "Special thanks to:";
                Strings.specialThanksLine = "Durante (PeterTh) for his wonderful guide on graphics API manipulation, and for answering some questions of mine.";
                Strings.specialThanksLine2 = "Kaldaien for consulting and advice regarding several things. Much obliged!";
                Strings.specialThanksLine3 = "Silent/CookiePLMonster for patiently helping me out with questions regarding his memory utilities and memory manipulation stuff in general. I owe you one!";
                Strings.specialThanksLine4 = "tlaik (author of the Neptastic mod for HyperDimension Neptunia Rebirth 1-3), AterialDawn (author of the MegaResolution Neptunia mod), Skyth/blueskythlikesclouds, and SeleDreams (author of the CoroHeart framework) for programming consultation.";
                Strings.thirdPartySoftware = "Third-Party Software:";
                Strings.imguiLicense = "Uses ImGui, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.kieroHookLicense = "Uses rdbo's ImGui-DirectX-11-Kiero-Hook.";
                Strings.ghFearLicense = "Uses portions of GHFear's Universal-ImGui-D3D11-Hook-WithResize.";
                Strings.thirteenAGIniLicense = "Uses ThirteenAG's IniReader, which is licensed under the stipulation that the software license is mentioned.";
                Strings.thirteenAGAsiLicense = "Uses ThirteenAG's Ultimate-ASI-Loader, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.minHookLicense = "Uses MinHook, which is licensed under the stipulation that the software license is included, see LICENSE for more information.";
                Strings.modUtilsLicense = "Uses CookiePLMonster's ModUtils, which is licensed under the MIT License, see LICENSE for more information.";
                Strings.fontsLicense = "Uses NotoSans, NotoSans-JP, and NotoSans-TC, which are licensed under the APACHE and SIL Open Font Licenses. See LICENSE and OFL for more information.";
                Strings.fontAwesomeLicense = "Uses FontAwesome by Dave Gandy - fontawesome.io";
                Strings.button_Close = "Close";
                //Localization for the EnigmaFix User Interface.
                Strings.collapsingHeader_Resolution = "\xef\x89\xac" "  屏幕分辨";
                Strings.checkbox_UseCustomRes = "Use Custom Resolution";
                Strings.combobox_CustomResolution = "Resolution (Replaces the 4K Native option)";
                Strings.helpmarker_CustomResolution = "This option will allow selecting a resolution different than the ones available in-game.";
                Strings.checkbox_UseCustomResScale = "Custom Resolution Scale";
                Strings.dragInt_CustomResScale = "Custom Resolution Scale";
                Strings.helpmarker_CustomResScale = "This option will allow upsampling from a lower screen resolution, usually done for performance reasons.";
                Strings.collapsingHeader_Fov = "\xef\x81\xae" "  視場";
                Strings.checkbox_customFOV = "Enable Custom FOV";
                Strings.sliderInt_customFOV = "FOV (Default: 44°)";
                Strings.helpmarker_customFOV = "This option will allow increasing the FOV during gameplay.";
                Strings.collapsingHeader_Framerate = "\xef\x8f\xbd" "  幀率";
                Strings.checkbox_VSync = "VSync";
                Strings.helpmarker_VSync = "This option will enable or disable the enforced vertical sync that the game enforces.\n" "\n" "Keep in mind that while disabling VSync may reduce input latency, it can cause screen tearing.";
                Strings.sliderInt_syncInterval = "Sync Interval";
                Strings.helpmarker_syncInterval = "This option will allow adjusting the Sync Interval when VSync is enabled.\n" "\n" "1 = Default VSync behavior\n" "\n" "2 = Half refresh rate\n" "\n" "3 = 1/3rd refresh rate\n" "\n" "4 = Quarter refresh rate.";
                Strings.sliderInt_FramerateCap = "幀率上限";
                Strings.helpmarker_FramerateCap = "You can adjust the maximum framerate that the game can run at.\n" "\n" "Speed related issues with Character movement (in battles) or 2D animations may occur.";
                Strings.text_Frametime = "幀時間: ";
                Strings.text_Framerate = "幀率: ";
                Strings.collapsingHeader_Rendering = "\xef\x87\xbc" "  渲染";
                Strings.checkbox_ShadowRendering = "Shadow Rendering";
                Strings.combobox_ShadowQuality = "Shadow Quality";
                Strings.helpmarker_ShadowQuality = "This option allows for the adjustment of shadow resolution.\n" "\n" "As a result, this option may come with a performance impact when using higher quality presets.\n" "\n" "Available options are Very Low (512), Low (1024), Medium (2048), High (4096), Ultra (8192), and Extreme (16384).";
                Strings.checkbox_SSAO = "Screen Space Ambient Occlusion (SSAO)";
                Strings.helpmarker_SSAO = "This option controls whether or not objects that are occluded from light should behave as such.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.combobox_ssaoQuality = "SSAO Quality";
                Strings.helpmarker_ssaoQuality = "";
                Strings.checkbox_SSR = "Screen Space Reflections (SSR)";
                Strings.helpmarker_SSR = "This option controls whether or not objects in screen space are reflected.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.combobox_ssrQuality = "SSR Quality";
                Strings.helpmarker_ssrQuality = "";
                Strings.checkbox_CharacterOutlines = "Character Outlines";
                Strings.helpmarker_CharacterOutlines = "This option disables character outlines.\n" "\n" "Disabling this option may or may not result in improved performance.\n";
                Strings.checkbox_GI = "Global Illumination/Bounce Lighting";
                Strings.helpmarker_GI = "This option controls whether or not the screen should have darkened edges.\n" "\n" "Disabling this option will likely not improve/worsen performance, so it's mostly up to preference.";
                Strings.checkbox_DoF = "Depth of Field";
                Strings.helpmarker_DoF = "This option controls whether or not elements on screen should be blurred when not in focus.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_TAA = "Temporal Anti-Aliasing (TAA)";
                Strings.helpmarker_TAA = "This option controls whether or not TAA is used.\n" "\n" "Disabling may result in more details being clear, but at the cost of edge and specular aliasing.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_Foliage = "Foliage";
                Strings.helpmarker_Foliage = "This option controls whether or not clusters of foliage are rendered.\n" "\n" "Disabling this option may or may not result in improved performance.";
                Strings.checkbox_MotionBlur = "Motion Blur";
                Strings.helpmarker_MotionBlur = "This option enables motion blur, which is in the engine, but not used in-game. It helps make running at 30FPS (or anything below 120FPS) more bearable.";
                Strings.collapsingHeader_Input = "\xef\x84\x9b" "  輸入";
                Strings.combobox_ControlType = "Control Type";
                Strings.helpmarker_ControlType = "This option controls what type of input prompts on game UI elements should appear when using a controller.\n" "\n" "Auto will use SteamInput to automatically switch the input prompts based on what controller is being used.";
                Strings.collapsingHeader_Misc = "\xef\x81\x99" "  各種各樣的";
                Strings.checkbox_SkipOP = "Skip Opening Videos";
                Strings.checkbox_CameraTweaks = "Camera Tweaks";
                Strings.helpmarker_CameraTweaks = "This option will improve the vertical clamping range of camera movement.";
                Strings.collapsingHeader_Launcher = "\xef\x8b\x90" "  發射器";
                Strings.checkbox_IgnoreUpdates = "Ignore Updates";
                Strings.button_Save = "Save";
                Strings.button_About = "About";
                // Developer Console
                Strings.consoleName = "Developer Console";
                // Startup Overlay Strings
                Strings.startupWindowInputPrompt = "Press the 'DELETE' key to open the configuration window.";
                Strings.startupWindowCrackWarning = "Cracked copies will not receive technical support.";
                // Exit Window
                Strings.gameExit = "Game Exit";
                Strings.exitTextPrompt = "Quit the game?" "\n" "Game progress will be lost if you have not saved your game." "\n";
                Strings.exitOK = "OK";
                Strings.exitCancel = "Cancel";
                break;
            }
        }
    }
}