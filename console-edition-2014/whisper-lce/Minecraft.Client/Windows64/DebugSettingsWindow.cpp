#include "stdafx.h"

#ifdef _WINDOWS64

#include "DebugSettingsWindow.h"
#include "Options.h"
#include "Minecraft.h"
#include "Common/Audio/SoundEngine.h"
#include <string>
#include <sstream>

#pragma comment(lib, "comctl32.lib")

HWND DebugSettingsWindow::s_hWnd = NULL;
HWND DebugSettingsWindow::s_hMusicSlider = NULL;
HWND DebugSettingsWindow::s_hSoundSlider = NULL;
HWND DebugSettingsWindow::s_hSensitivitySlider = NULL;
HWND DebugSettingsWindow::s_hFOVSlider = NULL;
HWND DebugSettingsWindow::s_hFOVEdit = NULL;
HWND DebugSettingsWindow::s_hGammaSlider = NULL;
HWND DebugSettingsWindow::s_hViewDistanceCombo = NULL;
HWND DebugSettingsWindow::s_hFramerateLimitCombo = NULL;
HWND DebugSettingsWindow::s_hDifficultyCombo = NULL;
HWND DebugSettingsWindow::s_hGraphicsCombo = NULL;
HWND DebugSettingsWindow::s_hParticlesCombo = NULL;
HWND DebugSettingsWindow::s_hGuiScaleCombo = NULL;
HWND DebugSettingsWindow::s_hInvertMouseCheck = NULL;
HWND DebugSettingsWindow::s_hBobViewCheck = NULL;
HWND DebugSettingsWindow::s_hAnaglyphCheck = NULL;
HWND DebugSettingsWindow::s_hAdvancedOpenGLCheck = NULL;
HWND DebugSettingsWindow::s_hAmbientOcclusionCheck = NULL;
HWND DebugSettingsWindow::s_hRenderCloudsCheck = NULL;
HWND DebugSettingsWindow::s_hFancyGraphicsCheck = NULL;
HWND DebugSettingsWindow::s_hShowFPSCheck = NULL;
HWND DebugSettingsWindow::s_hResetButton = NULL;
HWND DebugSettingsWindow::s_hFPSLabel = NULL;
Options* DebugSettingsWindow::s_pOptions = NULL;
DWORD DebugSettingsWindow::s_dwThreadId = 0;
HANDLE DebugSettingsWindow::s_hThread = NULL;
bool DebugSettingsWindow::s_bRunning = false;
bool DebugSettingsWindow::s_bNeedsUpdate = false;

#define ID_MUSIC_SLIDER 1001
#define ID_SOUND_SLIDER 1002
#define ID_SENSITIVITY_SLIDER 1003
#define ID_FOV_SLIDER 1004
#define ID_FOV_EDIT 1005
#define ID_GAMMA_SLIDER 1006
#define ID_VIEWDIST_COMBO 1007
#define ID_FRAMERATE_COMBO 1008
#define ID_DIFFICULTY_COMBO 1009
#define ID_GRAPHICS_COMBO 1010
#define ID_PARTICLES_COMBO 1011
#define ID_GUISCALE_COMBO 1012
#define ID_INVERTMOUSE_CHECK 1013
#define ID_BOBVIEW_CHECK 1014
#define ID_ANAGLYPH_CHECK 1015
#define ID_ADVANCEDGL_CHECK 1016
#define ID_AMBIENTOCCLUSION_CHECK 1017
#define ID_RENDERCLOUDS_CHECK 1018
#define ID_FANCYGRAPHICS_CHECK 1019
#define ID_SHOWFPS_CHECK 1020
#define ID_RESET_BUTTON 1021
#define ID_FPS_LABEL 1022

void DebugSettingsWindow::CreateControls(HWND hwnd)
{
    int y = 10;
    int labelWidth = 120;
    int controlWidth = 150;
    int spacing = 30;

    // FPS Counter at top
    s_hFPSLabel = CreateWindowW(L"STATIC", L"FPS: 0", WS_CHILD | WS_VISIBLE | SS_RIGHT, 
        300, 5, 80, 20, hwnd, (HMENU)ID_FPS_LABEL, NULL, NULL);

    // Reset Button
    s_hResetButton = CreateWindowW(L"BUTTON", L"Reset to Defaults", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
        10, y, 150, 25, hwnd, (HMENU)ID_RESET_BUTTON, NULL, NULL);
    y += spacing + 5;

    // Show FPS
    s_hShowFPSCheck = CreateWindowW(L"BUTTON", L"Show FPS Counter", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
        10, y, 200, 25, hwnd, (HMENU)ID_SHOWFPS_CHECK, NULL, NULL);
    y += spacing;

    // Music Volume
    CreateWindowW(L"STATIC", L"Music Volume:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hMusicSlider = CreateWindowW(TRACKBAR_CLASSW, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 
        labelWidth + 10, y, controlWidth, 20, hwnd, (HMENU)ID_MUSIC_SLIDER, NULL, NULL);
    SendMessage(s_hMusicSlider, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
    SendMessage(s_hMusicSlider, TBM_SETTICFREQ, 10, 0);
    y += spacing;

    // Sound Volume
    CreateWindowW(L"STATIC", L"Sound Volume:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hSoundSlider = CreateWindowW(TRACKBAR_CLASSW, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 
        labelWidth + 10, y, controlWidth, 20, hwnd, (HMENU)ID_SOUND_SLIDER, NULL, NULL);
    SendMessage(s_hSoundSlider, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
    SendMessage(s_hSoundSlider, TBM_SETTICFREQ, 10, 0);
    y += spacing;

    // Sensitivity
    CreateWindowW(L"STATIC", L"Mouse Sensitivity:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hSensitivitySlider = CreateWindowW(TRACKBAR_CLASSW, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 
        labelWidth + 10, y, controlWidth, 20, hwnd, (HMENU)ID_SENSITIVITY_SLIDER, NULL, NULL);
    SendMessage(s_hSensitivitySlider, TBM_SETRANGE, TRUE, MAKELONG(0, 200));
    SendMessage(s_hSensitivitySlider, TBM_SETTICFREQ, 20, 0);
    y += spacing;

    // FOV with number input
    CreateWindowW(L"STATIC", L"Field of View:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hFOVSlider = CreateWindowW(TRACKBAR_CLASSW, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 
        labelWidth + 10, y, controlWidth - 50, 20, hwnd, (HMENU)ID_FOV_SLIDER, NULL, NULL);
    SendMessage(s_hFOVSlider, TBM_SETRANGE, TRUE, MAKELONG(30, 110));
    SendMessage(s_hFOVSlider, TBM_SETTICFREQ, 10, 0);
    s_hFOVEdit = CreateWindowW(L"EDIT", L"70", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 
        labelWidth + controlWidth - 30, y, 40, 20, hwnd, (HMENU)ID_FOV_EDIT, NULL, NULL);
    y += spacing;

    // Gamma
    CreateWindowW(L"STATIC", L"Brightness:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hGammaSlider = CreateWindowW(TRACKBAR_CLASSW, NULL, WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, 
        labelWidth + 10, y, controlWidth, 20, hwnd, (HMENU)ID_GAMMA_SLIDER, NULL, NULL);
    SendMessage(s_hGammaSlider, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
    SendMessage(s_hGammaSlider, TBM_SETTICFREQ, 10, 0);
    y += spacing;

    // View Distance
    CreateWindowW(L"STATIC", L"Render Distance:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hViewDistanceCombo = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
        labelWidth + 10, y, controlWidth, 200, hwnd, (HMENU)ID_VIEWDIST_COMBO, NULL, NULL);
    SendMessageW(s_hViewDistanceCombo, CB_ADDSTRING, 0, (LPARAM)L"Far");
    SendMessageW(s_hViewDistanceCombo, CB_ADDSTRING, 0, (LPARAM)L"Normal");
    SendMessageW(s_hViewDistanceCombo, CB_ADDSTRING, 0, (LPARAM)L"Short");
    SendMessageW(s_hViewDistanceCombo, CB_ADDSTRING, 0, (LPARAM)L"Tiny");
    y += spacing;

    // Framerate Limit
    CreateWindowW(L"STATIC", L"FPS Limit:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hFramerateLimitCombo = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
        labelWidth + 10, y, controlWidth, 200, hwnd, (HMENU)ID_FRAMERATE_COMBO, NULL, NULL);
    SendMessageW(s_hFramerateLimitCombo, CB_ADDSTRING, 0, (LPARAM)L"280 FPS (Unlimited)");
    SendMessageW(s_hFramerateLimitCombo, CB_ADDSTRING, 0, (LPARAM)L"120 FPS");
    SendMessageW(s_hFramerateLimitCombo, CB_ADDSTRING, 0, (LPARAM)L"60 FPS");
    y += spacing;

    // Difficulty
    CreateWindowW(L"STATIC", L"Difficulty:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hDifficultyCombo = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
        labelWidth + 10, y, controlWidth, 200, hwnd, (HMENU)ID_DIFFICULTY_COMBO, NULL, NULL);
    SendMessageW(s_hDifficultyCombo, CB_ADDSTRING, 0, (LPARAM)L"Peaceful");
    SendMessageW(s_hDifficultyCombo, CB_ADDSTRING, 0, (LPARAM)L"Easy");
    SendMessageW(s_hDifficultyCombo, CB_ADDSTRING, 0, (LPARAM)L"Normal");
    SendMessageW(s_hDifficultyCombo, CB_ADDSTRING, 0, (LPARAM)L"Hard");
    y += spacing;

    // Graphics
    CreateWindowW(L"STATIC", L"Graphics:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hGraphicsCombo = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
        labelWidth + 10, y, controlWidth, 200, hwnd, (HMENU)ID_GRAPHICS_COMBO, NULL, NULL);
    SendMessageW(s_hGraphicsCombo, CB_ADDSTRING, 0, (LPARAM)L"Fast");
    SendMessageW(s_hGraphicsCombo, CB_ADDSTRING, 0, (LPARAM)L"Fancy");
    y += spacing;

    // Particles
    CreateWindowW(L"STATIC", L"Particles:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hParticlesCombo = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
        labelWidth + 10, y, controlWidth, 200, hwnd, (HMENU)ID_PARTICLES_COMBO, NULL, NULL);
    SendMessageW(s_hParticlesCombo, CB_ADDSTRING, 0, (LPARAM)L"All");
    SendMessageW(s_hParticlesCombo, CB_ADDSTRING, 0, (LPARAM)L"Decreased");
    SendMessageW(s_hParticlesCombo, CB_ADDSTRING, 0, (LPARAM)L"Minimal");
    y += spacing;

    // GUI Scale
    CreateWindowW(L"STATIC", L"GUI Scale:", WS_CHILD | WS_VISIBLE, 10, y, labelWidth, 20, hwnd, NULL, NULL, NULL);
    s_hGuiScaleCombo = CreateWindowW(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
        labelWidth + 10, y, controlWidth, 200, hwnd, (HMENU)ID_GUISCALE_COMBO, NULL, NULL);
    SendMessageW(s_hGuiScaleCombo, CB_ADDSTRING, 0, (LPARAM)L"Auto");
    SendMessageW(s_hGuiScaleCombo, CB_ADDSTRING, 0, (LPARAM)L"Small");
    SendMessageW(s_hGuiScaleCombo, CB_ADDSTRING, 0, (LPARAM)L"Normal");
    SendMessageW(s_hGuiScaleCombo, CB_ADDSTRING, 0, (LPARAM)L"Large");
    y += spacing;

    // Checkboxes
    s_hInvertMouseCheck = CreateWindowW(L"BUTTON", L"Invert Mouse", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
        10, y, 200, 20, hwnd, (HMENU)ID_INVERTMOUSE_CHECK, NULL, NULL);
    y += spacing;

    s_hBobViewCheck = CreateWindowW(L"BUTTON", L"View Bobbing", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
        10, y, 200, 20, hwnd, (HMENU)ID_BOBVIEW_CHECK, NULL, NULL);
    y += spacing;

    s_hAnaglyphCheck = CreateWindowW(L"BUTTON", L"3D Anaglyph", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
        10, y, 200, 20, hwnd, (HMENU)ID_ANAGLYPH_CHECK, NULL, NULL);
    y += spacing;

    s_hAmbientOcclusionCheck = CreateWindowW(L"BUTTON", L"Smooth Lighting", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
        10, y, 200, 20, hwnd, (HMENU)ID_AMBIENTOCCLUSION_CHECK, NULL, NULL);
    y += spacing;

    s_hRenderCloudsCheck = CreateWindowW(L"BUTTON", L"Render Clouds", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
        10, y, 200, 20, hwnd, (HMENU)ID_RENDERCLOUDS_CHECK, NULL, NULL);
    y += spacing;

    s_hFancyGraphicsCheck = CreateWindowW(L"BUTTON", L"Fancy Graphics", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
        10, y, 200, 20, hwnd, (HMENU)ID_FANCYGRAPHICS_CHECK, NULL, NULL);
}

void DebugSettingsWindow::UpdateFromOptions()
{
    if (!s_pOptions || !s_hWnd) return;

    SendMessage(s_hMusicSlider, TBM_SETPOS, TRUE, (LPARAM)(s_pOptions->music * 100));
    SendMessage(s_hSoundSlider, TBM_SETPOS, TRUE, (LPARAM)(s_pOptions->sound * 100));
    SendMessage(s_hSensitivitySlider, TBM_SETPOS, TRUE, (LPARAM)(s_pOptions->sensitivity * 100));
    SendMessage(s_hFOVSlider, TBM_SETPOS, TRUE, (LPARAM)s_pOptions->fov);
    
    wchar_t fovText[10];
    swprintf_s(fovText, 10, L"%d", s_pOptions->fov);
    SetWindowTextW(s_hFOVEdit, fovText);
    
    SendMessage(s_hGammaSlider, TBM_SETPOS, TRUE, (LPARAM)(s_pOptions->gamma * 100));
    
    SendMessage(s_hViewDistanceCombo, CB_SETCURSEL, s_pOptions->viewDistance, 0);
    SendMessage(s_hFramerateLimitCombo, CB_SETCURSEL, s_pOptions->framerateLimit, 0);
    SendMessage(s_hDifficultyCombo, CB_SETCURSEL, s_pOptions->difficulty, 0);
    SendMessage(s_hGraphicsCombo, CB_SETCURSEL, s_pOptions->fancyGraphics ? 1 : 0, 0);
    SendMessage(s_hParticlesCombo, CB_SETCURSEL, s_pOptions->particles, 0);
    SendMessage(s_hGuiScaleCombo, CB_SETCURSEL, s_pOptions->guiScale, 0);
    
    SendMessage(s_hInvertMouseCheck, BM_SETCHECK, s_pOptions->invertYMouse ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(s_hBobViewCheck, BM_SETCHECK, s_pOptions->bobView ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(s_hAnaglyphCheck, BM_SETCHECK, s_pOptions->anaglyph3d ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(s_hAmbientOcclusionCheck, BM_SETCHECK, s_pOptions->ambientOcclusion ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(s_hRenderCloudsCheck, BM_SETCHECK, s_pOptions->renderClouds ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(s_hFancyGraphicsCheck, BM_SETCHECK, s_pOptions->fancyGraphics ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(s_hShowFPSCheck, BM_SETCHECK, s_pOptions->hideGui ? BST_UNCHECKED : BST_CHECKED, 0);
}

void DebugSettingsWindow::ApplyToOptions()
{
    if (!s_pOptions || !s_hWnd) return;

    float oldMusic = s_pOptions->music;
    float oldSound = s_pOptions->sound;
    
    s_pOptions->music = (float)SendMessage(s_hMusicSlider, TBM_GETPOS, 0, 0) / 100.0f;
    s_pOptions->sound = (float)SendMessage(s_hSoundSlider, TBM_GETPOS, 0, 0) / 100.0f;
    s_pOptions->sensitivity = (float)SendMessage(s_hSensitivitySlider, TBM_GETPOS, 0, 0) / 100.0f;
    
    // Update FOV from edit box
    wchar_t fovText[10];
    GetWindowTextW(s_hFOVEdit, fovText, 10);
    int fov = _wtoi(fovText);
    if (fov < 30) fov = 30;
    if (fov > 110) fov = 110;
    s_pOptions->fov = fov;
    SendMessage(s_hFOVSlider, TBM_SETPOS, TRUE, fov);
    
    s_pOptions->gamma = (float)SendMessage(s_hGammaSlider, TBM_GETPOS, 0, 0) / 100.0f;
    
    s_pOptions->viewDistance = (int)SendMessage(s_hViewDistanceCombo, CB_GETCURSEL, 0, 0);
    s_pOptions->framerateLimit = (int)SendMessage(s_hFramerateLimitCombo, CB_GETCURSEL, 0, 0);
    s_pOptions->difficulty = (int)SendMessage(s_hDifficultyCombo, CB_GETCURSEL, 0, 0);
    s_pOptions->particles = (int)SendMessage(s_hParticlesCombo, CB_GETCURSEL, 0, 0);
    s_pOptions->guiScale = (int)SendMessage(s_hGuiScaleCombo, CB_GETCURSEL, 0, 0);
    
    s_pOptions->invertYMouse = (SendMessage(s_hInvertMouseCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
    s_pOptions->bobView = (SendMessage(s_hBobViewCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
    s_pOptions->anaglyph3d = (SendMessage(s_hAnaglyphCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
    s_pOptions->ambientOcclusion = (SendMessage(s_hAmbientOcclusionCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
    s_pOptions->renderClouds = (SendMessage(s_hRenderCloudsCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
    s_pOptions->fancyGraphics = (SendMessage(s_hFancyGraphicsCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
    
    int graphicsMode = (int)SendMessage(s_hGraphicsCombo, CB_GETCURSEL, 0, 0);
    s_pOptions->fancyGraphics = (graphicsMode == 1);
    
    // Apply music/sound volume changes immediately
    if (oldMusic != s_pOptions->music || oldSound != s_pOptions->sound)
    {
        Minecraft* mc = Minecraft::GetInstance();
        if (mc && mc->soundEngine)
        {
            mc->soundEngine->updateMusicVolume(s_pOptions->music);
            mc->soundEngine->updateSoundEffectVolume(s_pOptions->sound);
        }
    }
    
    s_bNeedsUpdate = true;
}

void DebugSettingsWindow::ResetToDefaults()
{
    if (!s_pOptions) return;
    
    s_pOptions->music = 1.0f;
    s_pOptions->sound = 1.0f;
    s_pOptions->sensitivity = 0.5f;
    s_pOptions->invertYMouse = false;
    s_pOptions->viewDistance = 0;
    s_pOptions->bobView = true;
    s_pOptions->anaglyph3d = false;
    s_pOptions->advancedOpengl = false;
    s_pOptions->framerateLimit = 0;
    s_pOptions->fancyGraphics = true;
    s_pOptions->ambientOcclusion = true;
    s_pOptions->renderClouds = true;
    s_pOptions->fov = 70;
    s_pOptions->gamma = 0.0f;
    s_pOptions->guiScale = 0;
    s_pOptions->particles = 0;
    
    UpdateFromOptions();
    s_bNeedsUpdate = true;
}

LRESULT CALLBACK DebugSettingsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_RESET_BUTTON && HIWORD(wParam) == BN_CLICKED)
        {
            ResetToDefaults();
        }
        else if (LOWORD(wParam) == ID_FOV_EDIT && HIWORD(wParam) == EN_CHANGE)
        {
            ApplyToOptions();
        }
        else if (HIWORD(wParam) == CBN_SELCHANGE || HIWORD(wParam) == BN_CLICKED)
        {
            ApplyToOptions();
        }
        break;

    case WM_HSCROLL:
        ApplyToOptions();
        // Update FOV edit box when slider moves
        if ((HWND)lParam == s_hFOVSlider)
        {
            int fov = (int)SendMessage(s_hFOVSlider, TBM_GETPOS, 0, 0);
            wchar_t fovText[10];
            swprintf_s(fovText, 10, L"%d", fov);
            SetWindowTextW(s_hFOVEdit, fovText);
        }
        break;

    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        return 0;

    case WM_DESTROY:
        s_bRunning = false;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

DWORD WINAPI DebugSettingsWindow::ThreadProc(LPVOID lpParam)
{
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MinecraftDebugSettings";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    s_hWnd = CreateWindowExW(
        WS_EX_TOPMOST,
        L"MinecraftDebugSettings",
        L"Minecraft Debug Settings - Real-Time Control",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 420, 600,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    if (!s_hWnd)
    {
        s_bRunning = false;
        return 1;
    }

    CreateControls(s_hWnd);
    UpdateFromOptions();
    ShowWindow(s_hWnd, SW_SHOW);
    UpdateWindow(s_hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    s_bRunning = false;
    return 0;
}

void DebugSettingsWindow::Create(Options* pOptions)
{
    if (s_bRunning) return;

    s_pOptions = pOptions;
    s_bRunning = true;
    s_hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, &s_dwThreadId);
}

void DebugSettingsWindow::Destroy()
{
    if (s_hWnd)
    {
        PostMessage(s_hWnd, WM_CLOSE, 0, 0);
    }
    if (s_hThread)
    {
        WaitForSingleObject(s_hThread, 5000);
        CloseHandle(s_hThread);
        s_hThread = NULL;
    }
    s_bRunning = false;
}

void DebugSettingsWindow::Update()
{
    if (s_bRunning && s_hWnd && s_bNeedsUpdate)
    {
        UpdateFromOptions();
        s_bNeedsUpdate = false;
    }
}

void DebugSettingsWindow::UpdateFPS(float fps)
{
    if (s_hFPSLabel && s_hWnd)
    {
        wchar_t fpsText[32];
        swprintf_s(fpsText, 32, L"FPS: %.1f", fps);
        SetWindowTextW(s_hFPSLabel, fpsText);
    }
}

//#endif // _DEBUG
#endif // _WINDOWS64
