#pragma once

#include <windows.h>

namespace hooks {
    bool initialize();
    void shutdown();

    extern WNDPROC original_wndproc;
    LRESULT __stdcall wndproc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}