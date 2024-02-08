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

#ifndef ENIGMAFIX_LOCALIZATION_H
#define ENIGMAFIX_LOCALIZATION_H

namespace EnigmaFix {
    class Localization {
    public:
        static Localization& Get() {
            return loc_Instance;
        }
        // Localization Functionality
        void InitLoc();
        void InitFont();
        // Variables
        float fontSize = 18.0;
        int language;
        // Strings
        struct S_Strings {
            // Common Strings
            const char* enigmaFixName;
            const char* gameName;
            // About Window Strings
            const char* aboutEnigmaFix;
            const char* versionNumber;
            const char* enigmaFixLicense;
            const char* donations;
            const char* specialThanksTo;
            const char* specialThanksLine;
            const char* specialThanksLine2;
            const char* specialThanksLine3;
            const char* specialThanksLine4;
            const char* thirdPartySoftware;
            const char* imguiLicense;
            const char* kieroHookLicense;
            const char* ghFearLicense;
            const char* thirteenAGIniLicense;
            const char* thirteenAGAsiLicense;
            const char* minHookLicense;
            const char* modUtilsLicense;
            const char* fontsLicense;
            const char* fontAwesomeLicense;
            const char* iconFontCppHeadersLicense;
            const char* button_Close;
            // User Interface Strings
            const char* collapsingHeader_Resolution;
            const char* checkbox_UseCustomRes;
            const char* combobox_CustomResolution;
            const char* helpmarker_CustomResolution;
            const char* checkbox_UseCustomResScale;
            const char* dragInt_CustomResScale;
            const char* helpmarker_CustomResScale;
            const char* collapsingHeader_Fov;
            const char* checkbox_customFOV;
            const char* sliderInt_customFOV;
            const char* helpmarker_customFOV;
            const char* checkbox_VSync;
            const char* helpmarker_VSync;
            const char* sliderInt_syncInterval;
            const char* helpmarker_syncInterval;
            const char* collapsingHeader_Framerate;
            const char* sliderInt_FramerateCap;
            const char* helpmarker_FramerateCap;
            const char* text_Frametime;
            const char* text_Framerate;
            const char* collapsingHeader_Rendering;
            const char* checkbox_ShadowRendering;
            const char* combobox_ShadowQuality;
            const char* helpmarker_ShadowQuality;
            const char* checkbox_SSAO;
            const char* helpmarker_SSAO;
            const char* combobox_ssaoQuality;
            const char* helpmarker_ssaoQuality;
            const char* checkbox_SSR;
            const char* helpmarker_SSR;
            const char* helpmarker_ssrQuality;
            const char* combobox_ssrQuality;
            const char* checkbox_CharacterOutlines;
            const char* helpmarker_CharacterOutlines;
            const char* checkbox_GI;
            const char* helpmarker_GI;
            const char* checkbox_DoF;
            const char* helpmarker_DoF;
            const char* checkbox_TAA;
            const char* helpmarker_TAA;
            const char* checkbox_MotionBlur;
            const char* helpmarker_MotionBlur;
            const char* checkbox_Foliage;
            const char* helpmarker_Foliage;
            const char* collapsingHeader_Input;
            const char* combobox_ControlType;
            const char* helpmarker_ControlType;
            const char* collapsingHeader_Misc;
            const char* checkbox_SkipOP;
            const char* checkbox_CameraTweaks;
            const char* helpmarker_CameraTweaks;
            const char* collapsingHeader_Launcher;
            const char* checkbox_IgnoreUpdates;
            const char* button_Save;
            const char* button_About;
            // Developer Console
            const char* consoleName;
            // Startup Overlay Strings
            const char* startupWindowWelcome;
            const char* startupWindowInputPrompt;
            const char* startupWindowCrackWarning;
            // Game Exit Strings
            const char* gameExit;
            const char* exitTextPrompt;
            const char* exitOK;
            const char* exitCancel;
        };
        S_Strings Strings;
    private:
        static Localization loc_Instance;
    };
}



#endif //ENIGMAFIX_LOCALIZATION_H
