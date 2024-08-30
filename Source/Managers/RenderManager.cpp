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
#include "RenderManager.h"
#include "../Settings/PlayerSettings.h"
#include "../Localization/Localization.h"
#include "UIManager.h"
// System Libraries
//#include <comip.h>
#include <memory>
// Third Party Libraries
#include <spdlog.h>
#include "../ThirdParty/Kiero/kiero.h"
#include "../ThirdParty/ImGui/imgui.h"
#include "../ThirdParty/ImGui/backends/imgui_impl_win32.h"
#include "../ThirdParty/ImGui/backends/imgui_impl_dx11.h"

// Variables
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//// D3D11 related variables
HWND window = nullptr;
WNDPROC oWndProc;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView;
//// Various PlayerSettings and Localization String Accessors
auto& PlayerSettingsRm      = EnigmaFix::PlayerSettings::Get();
auto& LocalizationRm        = EnigmaFix::Localization::Get();
int HorizontalRes           = PlayerSettingsRm.RES.HorizontalRes;
int VerticalRes             = PlayerSettingsRm.RES.VerticalRes;
int InternalHorizontalRes   = PlayerSettingsRm.INS.InternalHorizontalRes;
int InternalVerticalRes     = PlayerSettingsRm.INS.InternalVerticalRes;
int ScreenSpaceEffectsScale = PlayerSettingsRm.RS.ScreenSpaceEffectsScale;
//// Hook Init Check
bool InitHook               = false;
// Namespaces
using namespace ImGui;
using namespace kiero;
using namespace std; // This is for unique_ptrs
// Singleton References
auto& UIManagerRenMan = EnigmaFix::UIManager::Get();
// Singleton Instance
EnigmaFix::RenderManager EnigmaFix::RenderManager::rm_Instance; // Seemingly need this declared in RenderManager.cpp so a bunch of linker errors don't happen.

// A COM Pointer Exception handler when something goes awry with any of the D3D11 Hooks.
namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception {
    public:
        com_exception(HRESULT hr) : result(hr) {
        }
        const char* what() const noexcept override {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X",
                      static_cast<unsigned int>(result));
            return s_str;
        }
    private:
        HRESULT result;
    };
    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr) {
        if (FAILED(hr)) {
            throw com_exception(hr);
        }
    }

    template<typename T>
    struct Release
    {
        void operator()(T* obj) const
        {
            if (obj)
            {
                obj->Release();
            }
        }
    };
}

void InitImGui() // Initializes ImGui, alongside the needed fonts.
{
    CreateContext();
    ImGuiIO& io = GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;
    LocalizationRm.InitFont();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);
}

// Another function relating to imgui functionality.
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
        return true;
    }
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

// An easily callable function to make the CreateTexture2D hook a little cleaner.
void resizeRt(D3D11_TEXTURE2D_DESC *pDesc, int Width, int Height, int DesiredWidth, int DesiredHeight)
{
    if (pDesc->Width == Width || pDesc->Height == Height) {
        pDesc->Width  = static_cast<UINT>(DesiredWidth);
        pDesc->Height = static_cast<UINT>(DesiredHeight);
    }
}

bool vpResize(ID3D11DeviceContext* pContext)
{
    UINT numViewports = 0;
    pContext->RSGetViewports(&numViewports, nullptr);
    if (numViewports == 1) {
        D3D11_VIEWPORT vp = {};
        pContext->RSGetViewports(&numViewports, &vp);
        // This is where we can call each viewport that needs to be resized.
        // Since the post-process pipeline is hardcoded to 1080p, I think we can get away with murder when it comes to viewport resizing.
        if ((vp.Width == 1920.0f && vp.Height == 1080.0f) ||
            (vp.Width == 960.0f && vp.Height == 540.0f) ||
            //(vp.Width == 640.0f  && vp.Height == 360.0f)  ||
            (vp.Width == 480.0f && vp.Height == 270.0f) ||
            (vp.Width == 384.0f && vp.Height == 216.0f) ||
            (vp.Width == 320.0f && vp.Height == 180.0f) ||
            (vp.Width == 240.0f && vp.Height == 135.0f) ||
            (vp.Width == 192.0f && vp.Height == 108.0f) ||
            //(vp.Width == 160.0f  && vp.Height == 90.0f)   ||
            (vp.Width == 96.0f && vp.Height == 54.0f) ||
            //(vp.Width == 80.0f   && vp.Height == 46.0f)   ||
            (vp.Width == 48.0f && vp.Height == 28.0f) ||
            (vp.Width == 48.0f && vp.Height == 27.0f) ||
            //(vp.Width == 40.0f   && vp.Height == 24.0f)   ||
            (vp.Width == 24.0f && vp.Height == 14.0f) ||
            //(vp.Width == 20.0f   && vp.Height == 12.0f)   ||
            (vp.Width == 12.0f && vp.Height == 8.0f) ||
            (vp.Width == 12.0f && vp.Height == 7.0f) ||
            //(vp.Width == 10.0f   && vp.Height == 6.0f)    ||
            (vp.Width == 6.0f && vp.Height == 4.0f)) {
            std::shared_ptr<ID3D11RenderTargetView> rtView = std::dynamic_pointer_cast<ID3D11RenderTargetView>(rtView);
            ID3D11RenderTargetView* rawRTView = rtView.get();
            pContext->OMGetRenderTargets(1, &rawRTView, nullptr);
            if (rtView) {
                D3D11_RENDER_TARGET_VIEW_DESC desc;
                rtView->GetDesc(&desc);
                if (desc.Format == DXGI_FORMAT_R16G16B16A16_TYPELESS ||
                    desc.Format == DXGI_FORMAT_R11G11B10_FLOAT ||
                    desc.Format == DXGI_FORMAT_R24G8_TYPELESS ||
                    desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM) {
                    std::shared_ptr<ID3D11Resource> rt = std::dynamic_pointer_cast<ID3D11Resource>(rt);
                    ID3D11Resource* rawRT = rt.get();
                    rtView->GetResource(&rawRT);
                    if (rt != nullptr) {
                        auto rttex = std::dynamic_pointer_cast<ID3D11Texture2D>(rt);
                        if (rttex) {
                            D3D11_TEXTURE2D_DESC texdesc = {};
                            rttex->GetDesc(&texdesc);
                            if (texdesc.Width != vp.Width) {
                                // Here we go!
                                // Viewport is the easy part
                                vp.Width = static_cast<FLOAT>(texdesc.Width);
                                vp.Height = static_cast<FLOAT>(texdesc.Height);
                                pContext->RSSetViewports(1, &vp);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool srResize(ID3D11DeviceContext* pContext)
{
    // This is where scissor rect resizing will occur.
    UINT numRects = 0;
    pContext->RSGetScissorRects(&numRects, nullptr);
    if (numRects == 1) {
        D3D11_RECT rect = {};
        pContext->RSGetScissorRects(&numRects, &rect);
        if ((rect.right == 1920 && rect.bottom == 1080) ||
            (rect.right ==  960 && rect.bottom ==  540) ||
            //(rect.right == 640  && rect.bottom == 360)  ||
            (rect.right ==  480 && rect.bottom ==  270) ||
            (rect.right ==  384 && rect.bottom ==  216) ||
            (rect.right ==  320 && rect.bottom ==  180) ||
            (rect.right ==  240 && rect.bottom ==  135) ||
            (rect.right ==  192 && rect.bottom ==  108) ||
            //(rect.right == 160.0f  && rect.bottom == 90.0f)   ||
            (rect.right ==   96 && rect.bottom ==   54) ||
            //(rect.right == 80.0f   && rect.bottom == 46.0f)   ||
            (rect.right ==   48 && rect.bottom ==   28) ||
            (rect.right ==   48 && rect.bottom ==   27) ||
            //(rect.right == 40.0f   && rect.bottom == 24.0f)   ||
            (rect.right ==   24 && rect.bottom ==   14) ||
            //(rect.right == 20.0f   && rect.bottom == 12.0f)   ||
            (rect.right ==   12 && rect.bottom ==    8) ||
            (rect.right ==   12 && rect.bottom ==    7) ||
            //(rect.right == 10.0f   && rect.bottom == 6.0f)    ||
            (rect.right ==    6 && rect.bottom ==    4)) {
            std::shared_ptr<ID3D11RenderTargetView> rtView = std::dynamic_pointer_cast<ID3D11RenderTargetView>(rtView);
            ID3D11RenderTargetView* rawRTView = rtView.get();
            pContext->OMGetRenderTargets(1, &rawRTView, nullptr);
            if (rtView) {
                D3D11_RENDER_TARGET_VIEW_DESC desc;
                rtView->GetDesc(&desc);
                if (desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM ||
                    desc.Format == DXGI_FORMAT_R16G16B16A16_TYPELESS ||
                    desc.Format == DXGI_FORMAT_R11G11B10_FLOAT ||
                    desc.Format == DXGI_FORMAT_R24G8_TYPELESS) {
                    //_com_ptr_t <_com_IIID<ID3D11Resource, &__uuidof(ID3D11Resource)>> rt;
                    std::shared_ptr<ID3D11Resource> rt = std::dynamic_pointer_cast<ID3D11Resource>(rt);
                    ID3D11Resource* rawRT = rt.get();
                    rtView->GetResource(&rawRT);
                    if (rt != nullptr) {
                        auto rttex = std::dynamic_pointer_cast<ID3D11Texture2D>(rt);
                        if (rttex != nullptr) {
                            D3D11_TEXTURE2D_DESC texdesc = {};
                            rttex->GetDesc(&texdesc);
                            if (texdesc.Width != rect.right) {
                                // Here we go!
                                // Viewport is the easy part
                                rect.right = static_cast<LONG>(texdesc.Width);
                                rect.bottom = static_cast<LONG>(texdesc.Height);
                                pContext->RSSetScissorRects(1, &rect);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

namespace EnigmaFix {
    // A hook that contains the needed logic to resize the framebuffer whenever the game resolution changes.
    HRESULT __stdcall RenderManager::hkResizeBuffers(IDXGISwapChain *pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
    {
        if (mainRenderTargetView) {
            pContext->OMSetRenderTargets(0, 0, 0);
            mainRenderTargetView->Release(); // WE DON'T NEED TO SET THE RTVIEW TO NULL AFTER RELEASING IT, GPT!
        }

        // TODO: Potentially investigate why oResizeBuffers isn't working.
        // I have a theory that it might be the rm_Instance.original functions that are causing problems.
        HRESULT hr = rm_Instance.oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
        
        ID3D11Texture2D* pBuffer = nullptr;
        hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
        if (SUCCEEDED(hr)) {
            if (mainRenderTargetView) {
                mainRenderTargetView->Release(); // WE DON'T NEED TO SET THE MRTVIEW TO NULL AFTER RELEASING IT, GPT!
            }
            DX::ThrowIfFailed(pDevice->CreateRenderTargetView(pBuffer, nullptr, &mainRenderTargetView));
            pBuffer->Release(); // Release the reference acquired by GetBuffer
        }
        else { throw std::runtime_error("Failed to create render target view."); } // Throw a standard C++ exception with an error message.

        pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);

        // Set up the viewport.
        D3D11_VIEWPORT vp;
        vp.Width    = static_cast<float>(Width);
        vp.Height   = static_cast<float>(Height);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        pContext->RSSetViewports(1, &vp); //how to check if any boolean is equal to true
        return hr;
    }

    // A hook that contains the needed logic for adding the imgui interface, and forcing flip model presentation.
    HRESULT __stdcall RenderManager::hkPresent(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags) // Here's what happens when the swapchain is ready to be presented.
    {
        if (!InitHook) { // Checks if the hook hasn't been initalized, and if not, does the needed deeds to hook ImGui.
            HRESULT hr = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
            if (SUCCEEDED(hr))
            {
                pDevice->GetImmediateContext(&pContext);
                DXGI_SWAP_CHAIN_DESC sd;
                pSwapChain->GetDesc(&sd);
                window = sd.OutputWindow;
                ID3D11Texture2D* pBackBuffer;
                hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
                if (SUCCEEDED(hr)) {
                    pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
                    pBackBuffer->Release();
                }
                else { throw std::runtime_error("Failed to get swapchain back buffer."); } // Throw a standard C++ exception with an error message.
                oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
                InitImGui();
                InitHook = true;
            }
            else { throw std::runtime_error("Failed to get swapchain device."); }
        }

        if (GetAsyncKeyState(VK_DELETE) & 1) { // Checks if "DEL" key was pressed, and toggles the boolean that shows the UI.
            PlayerSettingsRm.ShowEFUI = !PlayerSettingsRm.ShowEFUI;
        }

        if (GetAsyncKeyState(VK_OEM_3) & 1) { // Checks if "~/`/Tilde" key was pressed, and toggles the developer console UI.
            PlayerSettingsRm.ShowDevConsole = !PlayerSettingsRm.ShowDevConsole;
        }

        PlayerSettingsRm.ShowUI = PlayerSettingsRm.ShowEFUI || PlayerSettingsRm.ShowDevConsole; // Checks if either EFUI or the dev console are enabled, and if so, enable the showUI flag.

        if (PlayerSettingsRm.ShowUI) { // Checks if the showUI flag is enabled, and if so, draws the ImGui interface.
            // Creates a new ImGui frame.
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            NewFrame();
            // Start up the ImGui UI for EnigmaFix.
            UIManagerRenMan.Begin();
            // Ends drawing the ImGui frame.
            End();
            // Render the ImGui frame. We don't need to call EndFrame because that's already being done with Render.
            Render();
            // Finally draws the ImGui overlay on screen.
            pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
            ImGui_ImplDX11_RenderDrawData(GetDrawData());
        }

        UINT syncInterval = PlayerSettingsRm.SYNC.VSync ? PlayerSettingsRm.SYNC.SyncInterval : 0; // Converts the vSync checks to a quick variable to clean up space.
        return rm_Instance.oPresent(pSwapChain, syncInterval, DXGI_SWAP_EFFECT_FLIP_DISCARD | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
    }

    // A hook that contains the needed logic for changing the Shadow, SSR, SSAO, and Post Processing resolution.
    HRESULT __stdcall RenderManager::hkCreateTexture2D(ID3D11Device *pDevice, D3D11_TEXTURE2D_DESC *pDesc, D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D **ppTexture2D)
    {
        // Update our rendering settings related settings before we modify anything.
        int ShadowRes = PlayerSettingsRm.RS.ShadowRes;
        int ScreenSpaceEffectsScale = PlayerSettingsRm.RS.ScreenSpaceEffectsScale;
        int SSRScale = PlayerSettingsRm.RS.SSRScale;

        
        // We are simply using this to update our current internal resolution for the rest of the logic.
        // Checks for the specific texture format for CopyDeferredColor_Hist, and checks to see if it has twelve mipmaps, if so, we got our suspect render target. 11 only works with resolutions below 1440p, while 12 only works with anything higher than 1080p.
        // TODO: Figure out why our hook isn't fucking working. This should be working, yet it isn't doing anything.
        if (pDesc->Format == DXGI_FORMAT_R16G16B16A16_FLOAT && (pDesc->MipLevels == 11 || pDesc->MipLevels == 12)) { // Checks to see if it has twelve mipmaps, if so, we got our suspect render target. 11 only works with resolutions below 1440p, while 12 only works with anything higher than 1080p.
            InternalVerticalRes	  = pDesc->Height;
            InternalHorizontalRes =  pDesc->Width;
        }

        // Do our shadow, ambient occlusion, and SSR patches first.
        // TODO: Figure out a way of checking this based on the game, as DERQ2 and the other Mizuchi games probably are using a modified version of the engine, so it probably does this a little differently.
        switch (pDesc->Format) { // For some weird reason, this switch statement won't detect SSAO or Screen Space Reflections unless I prioritize them.
            case DXGI_FORMAT_R32_TYPELESS: { // This checks for the R32_TYPELESS format which is used for shadows
                resizeRt(pDesc, 2048, 2048, ShadowRes, ShadowRes);
                break;
            }
            case DXGI_FORMAT_R16G16_FLOAT: { // ImageSpaceAO and ImageSpaceCrossBilateralH (SSAO and Horizontal SSAO Blurring) Render Targets
            // Checks for render targets half the size of the current in-game resolution.
                resizeRt(pDesc, (InternalHorizontalRes / 2), (InternalVerticalRes / 2), (InternalHorizontalRes / ScreenSpaceEffectsScale), (InternalVerticalRes / ScreenSpaceEffectsScale));
                break;
            }
            case DXGI_FORMAT_R8_UNORM: { // ImageSpaceCrossBilateralV and ImageSpaceReflectionOutput2
                resizeRt(pDesc, (InternalHorizontalRes / 2), (InternalVerticalRes / 2), (InternalHorizontalRes / ScreenSpaceEffectsScale), (InternalVerticalRes / ScreenSpaceEffectsScale));
                break;
            }
            case DXGI_FORMAT_R16G16B16A16_UNORM: { // DownsampleGBuffer0, which is used as an input for ImageSpaceReflection
                resizeRt(pDesc, (InternalHorizontalRes / 2), (InternalVerticalRes / 2), (InternalHorizontalRes / ScreenSpaceEffectsScale), (InternalVerticalRes / ScreenSpaceEffectsScale));
                break;
            }
            case DXGI_FORMAT_R32_FLOAT: { // CompositeDepthForRLR and ImageSpaceHiZ, which is used as an input for ImageSpaceReflection
                resizeRt(pDesc, (InternalHorizontalRes / 2), (InternalVerticalRes / 2), (InternalHorizontalRes / ScreenSpaceEffectsScale), (InternalVerticalRes / ScreenSpaceEffectsScale));
                break;
            }
            case DXGI_FORMAT_R11G11B10_FLOAT: { // ImageSpaceReflectionOutput1
                resizeRt(pDesc, (InternalHorizontalRes / 2), (InternalVerticalRes / 2), (InternalHorizontalRes / ScreenSpaceEffectsScale), (InternalVerticalRes / ScreenSpaceEffectsScale));
                break;
            }
        }

        // TODO: Need to decouple this from the resolution setting. Ideally, the post-processing should be changed based on the internal rendering resolution rather than the user decided resolution.
        // As we do plan on patching the in-game resolution option separately from the reported internal resolution, and the reported internal resolution should change as a result.
        if (InternalHorizontalRes > 1920 || InternalHorizontalRes > 1080) // Unsure if the InternalHorizontalRes > 1080 will cause a problem.
        {
            switch (pDesc->Format) {
                case DXGI_FORMAT_R16G16B16A16_TYPELESS: {
                    resizeRt(pDesc, 1920, 1080, InternalHorizontalRes, InternalVerticalRes);
                    resizeRt(pDesc,  960,  540, (InternalHorizontalRes /  2), (InternalVerticalRes /  2));
                    resizeRt(pDesc,  480,  270, (InternalHorizontalRes /  4), (InternalVerticalRes /  4));
                    resizeRt(pDesc,  240,  135, (InternalHorizontalRes /  8), (InternalVerticalRes /  8));
                    resizeRt(pDesc,  192,  108, (InternalHorizontalRes / 10), (InternalVerticalRes / 10));
                }
                case DXGI_FORMAT_R11G11B10_FLOAT: {
                    resizeRt(pDesc, 384, 216, (InternalHorizontalRes /   5), (InternalVerticalRes /   5));
                    resizeRt(pDesc, 320, 180, (InternalHorizontalRes /   6), (InternalVerticalRes /   6));
                    resizeRt(pDesc, 192, 108, (InternalHorizontalRes /  10), (InternalVerticalRes /  10));
                    resizeRt(pDesc,  96,  54, (InternalHorizontalRes /  20), (InternalVerticalRes /  20));
                    resizeRt(pDesc,  48,  28, (InternalHorizontalRes /  40), (InternalVerticalRes /  40));
                    resizeRt(pDesc,  48,  27, (InternalHorizontalRes /  40), (InternalVerticalRes /  40));
                    resizeRt(pDesc,  24,  14, (InternalHorizontalRes /  80), (InternalVerticalRes /  80));
                    resizeRt(pDesc,  12,   7, (InternalHorizontalRes / 160), (InternalVerticalRes / 160));
                    resizeRt(pDesc,  12,   8, (InternalHorizontalRes / 160), (InternalVerticalRes / 160));
                    resizeRt(pDesc,   6,   4, (InternalHorizontalRes / 320), (InternalVerticalRes / 320));
                }
                case DXGI_FORMAT_R24G8_TYPELESS: {
                    resizeRt(pDesc, 1920, 1080, InternalHorizontalRes, InternalVerticalRes);
                }
                case DXGI_FORMAT_B8G8R8A8_UNORM: {
                    resizeRt(pDesc, 1920, 1080, InternalHorizontalRes, InternalVerticalRes);
                }
            }
        }
        return rm_Instance.oCreateTexture2D(pDevice, pDesc, pInitialData, ppTexture2D);
    }

    // A hook that contains the needed logic for running checks on Viewports and Scissor Rects.
    HRESULT __stdcall RenderManager::hkDrawIndexed(ID3D11DeviceContext *pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
    {
        bool preDraw = false;

        if (IndexCount == 4 && StartIndexLocation == 0 && BaseVertexLocation == 0) {
            preDraw = vpResize(pContext);
        }
        // Checks if both the viewports and scissor rects haven't been resized, and then return oDrawIndexed. I may need to find a better way of doing this to account for one not passing.
        if (!preDraw) {
            return rm_Instance.oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
        }
        return E_FAIL; // Default Return Statement, so Intellisense stops complaining.
    }

    // Another hook that contains the needed logic for running checks on Viewports and Scissor Rects.
    HRESULT __stdcall RenderManager::hkDraw(ID3D11DeviceContext *pContext, UINT VertexCount, UINT StartVertexLocation)
    {
        bool preDraw = false;

        if (VertexCount == 4 && StartVertexLocation == 0) {
            preDraw = vpResize(pContext) && srResize(pContext);
        }
        if (!preDraw) {
            return rm_Instance.oDraw(pContext, VertexCount, StartVertexLocation);
        }
        return E_FAIL; // Default Return Statement, so Intellisense stops complaining.
    }

    DWORD __stdcall RenderManager::InitD3D11Hook(LPVOID lpReserved) {
        // Starts Hooking
        bool InitHook = false;
        do {
            if (init(RenderType::D3D11) == Status::Success) {
                // Binds the function we will be using to get imgui to draw on-screen.
                kiero::bind( 8, reinterpret_cast<void**>(&oPresent),reinterpret_cast<void*>(this->hkPresent));
                // Binds the function we will be using to resize the framebuffer when needed.
                kiero::bind(13, reinterpret_cast<void**>(&oResizeBuffers),reinterpret_cast<void*>(this->hkResizeBuffers));
                switch (PlayerSettingsRm.GameMode) { // Only Initialize rendering hooks for DERQ for now, since we don't want to cause issues with the other games until support rolls out for those.
                    case PlayerSettings::DERQ: {
                        // Binds the function we will be using to modify the render target size for shadows and other things.
                        kiero::bind(23, reinterpret_cast<void**>(&oCreateTexture2D), reinterpret_cast<void*>(this->hkCreateTexture2D));
                        // Binds the function we will be using for resizing viewports.
                        kiero::bind(74, reinterpret_cast<void**>(&oDraw), reinterpret_cast<void*>(this->hkDraw));
                        // Binds another function we will be using for resizing viewports.
                        kiero::bind(73, reinterpret_cast<void**>(&oDrawIndexed),reinterpret_cast<void*>(this->hkDrawIndexed));
                        break;
                    }
                    case PlayerSettings::DERQ2: {
                        // Binds the function we will be using to modify the render target size for shadows and other things.
                        kiero::bind(23, reinterpret_cast<void**>(&oCreateTexture2D), reinterpret_cast<void*>(this->hkCreateTexture2D));
                        // Binds the function we will be using for resizing viewports.
                        kiero::bind(74, reinterpret_cast<void**>(&oDraw), reinterpret_cast<void*>(this->hkDraw));
                        // Binds another function we will be using for resizing viewports.
                        kiero::bind(73, reinterpret_cast<void**>(&oDrawIndexed),reinterpret_cast<void*>(this->hkDrawIndexed));
                        break;
                    }
                    case PlayerSettings::Varnir: {
                        break;
                    }
                    case PlayerSettings::VIIR: {
                        break;
                    }
                    case PlayerSettings::NVS: {
                        break;
                    }
                    case PlayerSettings::MS2: {
                        break;
                    }
                    case PlayerSettings::MSF: {
                        break;
                    }
                    default: {
                        break;
                    }
                }
                InitHook = true;
            }
        }
        while (!InitHook);
        return TRUE;
    }
}