#include "hooks.hpp"
#include "../render/c_renderer.hpp"
#include "../menu/c_menu.hpp"
#include "imgui_impl_win32.h"

WNDPROC hooks::original_wndproc = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool hooks::initialize() {
    original_wndproc = (WNDPROC)SetWindowLongPtr(c_renderer::get().get_window_handle(), GWLP_WNDPROC, (LONG_PTR)wndproc_hook);
    return original_wndproc != nullptr;
}

void hooks::shutdown() {
    SetWindowLongPtr(c_renderer::get().get_window_handle(), GWLP_WNDPROC, (LONG_PTR)original_wndproc);
}

LRESULT __stdcall hooks::wndproc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
        c_menu::get().toggle_menu();
    }

    if (c_menu::get().is_menu_open() && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
        return true;
    }

    return CallWindowProc(original_wndproc, hWnd, uMsg, wParam, lParam);
}