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
#include "ConfigManager.h"
#include "../Settings/PlayerSettings.h"
// System Libraries
#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
// Third Party Libraries
#include <inipp.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>    // support for basic file logging
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console logging
// Variables

auto& PlayerSettingsConf = EnigmaFix::PlayerSettings::Get();
// Namespaces
using namespace std;
// Singleton Instance
EnigmaFix::ConfigManager EnigmaFix::ConfigManager::cm_Instance; // Seemingly need this declared in PlayerSettings.cpp so a bunch of linker errors don't happen.

namespace EnigmaFix {
    inipp::Ini<char> config;
    std::ifstream is("Config.ini");

    void ConfigManager::Init() {
        ifstream configName("Config.ini");
        config.parse(configName);
        config.generate(cout);
        config.default_section(config.sections["Settings"]);
        config.interpolate();
    }


    void ConfigManager::SaveConfig() { // TODO: Find a way of writing configs with IniPP.
        if (AlreadyReadConfig) {
            spdlog::info("Saving Config...");
            // Resolution Settings
            //ini.WriteBoolean("Resolution",  "UseCustomResolution",       PlayerSettingsConf.RES.UseCustomRes,      0);
            //ini.WriteInteger("Resolution",  "HorizontalResolution",      PlayerSettingsConf.RES.HorizontalRes,     0);
            //ini.WriteInteger("Resolution",  "VerticalResolution",        PlayerSettingsConf.RES.VerticalRes,       0);
            //ini.WriteBoolean("Resolution",  "UseResolutionScale",        PlayerSettingsConf.RES.UseCustomResScale, 0);
            //ini.WriteInteger("Resolution",  "ResolutionScalePercentage", PlayerSettingsConf.RES.CustomResScale,    0);
            // FOV Settings
            //ini.WriteBoolean("FieldOfView", "UseCustomFOV",              PlayerSettingsConf.FOV.UseCustomFOV,      0);
            //ini.WriteBoolean("FieldOfView", "UseVertPlusScaling",        PlayerSettingsConf.FOV.VertPlusScaling,   0);
            //ini.WriteInteger("FieldOfView", "FieldOfView",               PlayerSettingsConf.FOV.FieldOfView,       0);
            // Sync and Framerate Settings
            //ini.WriteBoolean("Framerate",   "VSync",                     PlayerSettingsConf.SYNC.VSync,            0);
            //ini.WriteInteger("Framerate",   "SyncInterval",              PlayerSettingsConf.SYNC.SyncInterval,     0);
            //ini.WriteInteger("Framerate",   "MaxFPS",                    PlayerSettingsConf.SYNC.MaxFPS,           0);
            // Rendering Settings
            //ini.WriteBoolean("Rendering",   "CameraDistortion",          PlayerSettingsConf.RS.CameraDistortion,   0);
            //ini.WriteBoolean("Rendering",   "EdgeRendering",             PlayerSettingsConf.RS.EdgeRendering,      0);
            //ini.WriteBoolean("Rendering",   "ColorCorrection",           PlayerSettingsConf.RS.ColorCorrection,    0);
            //ini.WriteBoolean("Rendering",   "DepthOfField",              PlayerSettingsConf.RS.DepthOfField,       0);
            //ini.WriteBoolean("Rendering",   "Fog",                       PlayerSettingsConf.RS.Fog,                0);
            //ini.WriteBoolean("Rendering",   "Foliage",                   PlayerSettingsConf.RS.FoliageRendering,   0);
            //ini.WriteBoolean("Rendering",   "Bloom",                     PlayerSettingsConf.RS.Bloom,              0);
            //ini.WriteBoolean("Rendering",   "IBL",                       PlayerSettingsConf.RS.IBL,                0);
            //ini.WriteBoolean("Rendering",   "LensFlare",                 PlayerSettingsConf.RS.LensFlare,          0);
            //ini.WriteBoolean("Rendering",   "MotionBlur",                PlayerSettingsConf.RS.MotionBlur,         0);
            //ini.WriteInteger("Rendering",   "NotionBlurPreset",          PlayerSettingsConf.RS.MotionBlurPreset,   0);
            //ini.WriteBoolean("Rendering",   "RLRLighting",               PlayerSettingsConf.RS.RLRLighting,        0);
            //ini.WriteBoolean("Rendering",   "Shadows",                   PlayerSettingsConf.RS.Shadows,            0);
            //// Add Shadow Quality here
            //ini.WriteBoolean("Rendering",   "SSAO",                      PlayerSettingsConf.RS.SSAO,               0);
            //// Add SSAO Quality here
            //ini.WriteBoolean("Rendering",   "SSR",                       PlayerSettingsConf.RS.SSR,                0);
            //// Add SSR Quality here
            //ini.WriteBoolean("Rendering",   "TAA",                       PlayerSettingsConf.RS.TAA,                0);
            //ini.WriteBoolean("Rendering",   "Tonemapping",               PlayerSettingsConf.RS.Tonemapping,        0);
            //ini.WriteBoolean("Rendering",   "Vignette",                  PlayerSettingsConf.RS.Vignette,           0);
            // Input Settings
            //ini.WriteBoolean("Input",       "KBMPrompts",                PlayerSettingsConf.IS.KBMPrompts,         0);
            //ini.WriteBoolean("Input",       "DisableSteamInput",         PlayerSettingsConf.IS.DisableSteamInput,  0);
            //ini.WriteInteger("Input",       "InputType",                   PlayerSettingsConf.IS.InputDeviceType,    0);
            // Misc Settings
            //ini.WriteBoolean("Misc",        "SkipOpeningVideos",         PlayerSettingsConf.MS.SkipOpeningVideos,  0);
            //ini.WriteBoolean("Misc",        "CameraTweaks",              PlayerSettingsConf.MS.CameraTweaks,       0);
            //ini.WriteBoolean("Misc",        "EnableConsoleLog",          PlayerSettingsConf.MS.EnableConsoleLog,   0);
            // Launcher Settings
            //ini.WriteBoolean("Launcher",    "IgnoreUpdates",             PlayerSettingsConf.LS.IgnoreUpdates,      0);
        }
    }

    void ConfigManager::ReadConfig() {
        try {
            // TODO: Fix the log sink so we can actually output to a log file.
            // Check and remove existing log file to create a new one on each boot
            if (std::filesystem::exists("EnigmaFix.log")) {
                std::filesystem::remove("EnigmaFix.log");
            }

            // Create basic file logger that writes to a new log file each time
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("EnigmaFix.log", true);

            // Combine file sink and console sink (if enabled) in a multi-sink logger
            std::vector<spdlog::sink_ptr> sinks { file_sink };
        }
        catch (const spdlog::spdlog_ex& ex) {
            std::cout << "Log initialization failed: " << ex.what() << std::endl;
        }
        spdlog::info("Reading Config...");
        cout.flush();
        cout.clear();
        cin.clear();
        Init();

        // Resolution Settings
        inipp::extract(config.sections["Resolution"]["useCustomResolution"], PlayerSettingsConf.RES.UseCustomRes);
        inipp::extract(config.sections["Resolution"]["HorizontalResolution"], PlayerSettingsConf.RES.HorizontalRes);
        inipp::extract(config.sections["Resolution"]["VerticalResolution"], PlayerSettingsConf.RES.VerticalRes);
        inipp::extract(config.sections["Resolution"]["UseResolutionScale"], PlayerSettingsConf.RES.UseCustomResScale);
        inipp::extract(config.sections["Resolution"]["ResolutionScalePercentage"], PlayerSettingsConf.RES.CustomResScale);
        // FOV Settings
        inipp::extract(config.sections["FieldOfView"]["UseCustomFOV"], PlayerSettingsConf.FOV.UseCustomFOV);
        inipp::extract(config.sections["FieldOfView"]["FieldOfView"], PlayerSettingsConf.FOV.FieldOfView);
        inipp::extract(config.sections["FieldOfView"]["UseAdaptiveFOVScaling"], PlayerSettingsConf.FOV.AdaptiveFOVScaling);
        // Sync and Framerate Settings
        inipp::extract(config.sections["Framerate"]["MaxFPS"], PlayerSettingsConf.SYNC.MaxFPS);
        inipp::extract(config.sections["Framerate"]["VSync"], PlayerSettingsConf.SYNC.VSync);
        inipp::extract(config.sections["Framerate"]["SyncInterval"], PlayerSettingsConf.SYNC.SyncInterval);
        // Rendering Settings
        inipp::extract(config.sections["Rendering"]["CameraDistortion"], PlayerSettingsConf.RS.CameraDistortion);
        inipp::extract(config.sections["Rendering"]["EdgeRendering"], PlayerSettingsConf.RS.EdgeRendering);
        inipp::extract(config.sections["Rendering"]["ColorCorrection"], PlayerSettingsConf.RS.ColorCorrection);
        inipp::extract(config.sections["Rendering"]["DepthOfField"], PlayerSettingsConf.RS.DepthOfField);
        inipp::extract(config.sections["Rendering"]["Fog"], PlayerSettingsConf.RS.Fog);
        inipp::extract(config.sections["Rendering"]["Foliage"], PlayerSettingsConf.RS.FoliageRendering);
        inipp::extract(config.sections["Rendering"]["Bloom"], PlayerSettingsConf.RS.Bloom);
        inipp::extract(config.sections["Rendering"]["IBL"], PlayerSettingsConf.RS.IBL);
        inipp::extract(config.sections["Rendering"]["LensFlare"], PlayerSettingsConf.RS.LensFlare);
        inipp::extract(config.sections["Rendering"]["MotionBlur"], PlayerSettingsConf.RS.MotionBlur);
        inipp::extract(config.sections["Rendering"]["MotionBlurPreset"], PlayerSettingsConf.RS.MotionBlurPreset);
        inipp::extract(config.sections["Rendering"]["RLRLighting"], PlayerSettingsConf.RS.RLRLighting);
        inipp::extract(config.sections["Rendering"]["Shadows"], PlayerSettingsConf.RS.Shadows);
        inipp::extract(config.sections["Rendering"]["ShadowResolution"], PlayerSettingsConf.RS.ShadowRes);
        inipp::extract(config.sections["Rendering"]["SSAO"], PlayerSettingsConf.RS.SSAO);
        string ssaoQuality;
        string ssrQuality;
        inipp::extract(config.sections["Rendering"]["SSAOQuality"], ssaoQuality);
        inipp::extract(config.sections["Rendering"]["SSR"], PlayerSettingsConf.RS.SSR);
        inipp::extract(config.sections["Rendering"]["SSRQuality"], ssrQuality);
        inipp::extract(config.sections["Rendering"]["TAA"], PlayerSettingsConf.RS.TAA);
        inipp::extract(config.sections["Rendering"]["Tonemapping"], PlayerSettingsConf.RS.Tonemapping);
        inipp::extract(config.sections["Rendering"]["Vignette"], PlayerSettingsConf.RS.Vignette);
        // Input Settings
        inipp::extract(config.sections["Input"]["KBMPrompts"], PlayerSettingsConf.IS.KBMPrompts);
        inipp::extract(config.sections["Input"]["DisableSteamInput"], PlayerSettingsConf.IS.DisableSteamInput);
        inipp::extract(config.sections["Input"]["InputType"], PlayerSettingsConf.IS.InputDeviceType);
        // Misc Settings
        inipp::extract(config.sections["Misc"]["SkipOpeningVideos"], PlayerSettingsConf.MS.SkipOpeningVideos);
        inipp::extract(config.sections["Misc"]["CameraTweaks"], PlayerSettingsConf.MS.CameraTweaks);
        inipp::extract(config.sections["Misc"]["EnableConsoleLog"], PlayerSettingsConf.MS.EnableConsoleLog);
        // Launcher Settings
        inipp::extract(config.sections["Launcher"]["IgnoreUpdates"], PlayerSettingsConf.LS.IgnoreUpdates);

        // Show console window if EnableConsoleLog is enabled.
        if (PlayerSettingsConf.MS.EnableConsoleLog) {
            AllocConsole();
            freopen_s((FILE**)stdout, "CONOUT$", "w", stdout); // Allows us to add outputs to the ASI Loader Console Window.

            // Initialize spdlog with a colored console sink
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::debug);  // Set log level to debug
            spdlog::set_default_logger(std::make_shared<spdlog::logger>("console", console_sink));

            // Optional: Set a global log level and pattern
            spdlog::set_level(spdlog::level::debug);  // Set global log level

            // Optional: Customize the log pattern (timestamp, log level, message)
            spdlog::set_pattern("[%m/%d/%Y - %I:%M:%S%p] [%^%l%$] [thread %t] %v");

            // Log initial message
            spdlog::info("Console logging initialized.");

            // Log initial message indicating successful initialization
            spdlog::info("Logger initialized successfully.");
        }

        AlreadyReadConfig = true; // After the INI file has successfully been read for the first time, allow writing.
    }
}