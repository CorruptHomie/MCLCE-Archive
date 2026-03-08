#pragma once

#ifdef _WINDOWS64
// Enable debug window in both Debug and Release builds
//#ifdef _DEBUG

#include <windows.h>
#include <commctrl.h>

class Options;

class DebugSettingsWindow
{
private:
    static HWND s_hWnd;
    static HWND s_hMusicSlider;
    static HWND s_hSoundSlider;
    static HWND s_hSensitivitySlider;
    static HWND s_hFOVSlider;
    static HWND s_hFOVEdit;
    static HWND s_hGammaSlider;
    static HWND s_hViewDistanceCombo;
    static HWND s_hFramerateLimitCombo;
    static HWND s_hDifficultyCombo;
    static HWND s_hGraphicsCombo;
    static HWND s_hParticlesCombo;
    static HWND s_hGuiScaleCombo;
    static HWND s_hInvertMouseCheck;
    static HWND s_hBobViewCheck;
    static HWND s_hAnaglyphCheck;
    static HWND s_hAdvancedOpenGLCheck;
    static HWND s_hAmbientOcclusionCheck;
    static HWND s_hRenderCloudsCheck;
    static HWND s_hFancyGraphicsCheck;
    static HWND s_hShowFPSCheck;
    static HWND s_hResetButton;
    static HWND s_hFPSLabel;
    
    static Options* s_pOptions;
    static DWORD s_dwThreadId;
    static HANDLE s_hThread;
    static bool s_bRunning;
    static bool s_bNeedsUpdate;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static DWORD WINAPI ThreadProc(LPVOID lpParam);
    static void CreateControls(HWND hwnd);
    static void UpdateFromOptions();
    static void ApplyToOptions();
    static void ResetToDefaults();

public:
    static void Create(Options* pOptions);
    static void Destroy();
    static void Update();
    static void UpdateFPS(float fps);
    static bool IsRunning() { return s_bRunning; }
};

//#endif // _DEBUG
#endif // _WINDOWS64
