#include "c_renderer.hpp"
#include "../menu/c_menu.hpp"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

c_renderer& c_renderer::get() {
    static c_renderer instance;
    return instance;
}

bool c_renderer::initialize(const wchar_t* window_title, const wchar_t* window_class_name) {
    if (!create_window(window_title, window_class_name))
        return false;

    if (!create_device_d3d()) {
        shutdown();
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(m_pd3d_device, m_pd3d_device_context);

    return true;
}

void c_renderer::shutdown() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    cleanup_device_d3d();
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        UnregisterClassW(m_wc.lpszClassName, m_wc.hInstance);
    }
}

void c_renderer::begin_scene() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            m_is_running = false;
    }

    update_overlay_position();
    toggle_input_passthrough(!c_menu::get().is_menu_open());

    if (GetForegroundWindow() != m_target_hwnd || !IsWindowVisible(m_target_hwnd)) {
        ShowWindow(m_hwnd, SW_HIDE);
        m_should_render = false;
        return;
    } else {
        ShowWindow(m_hwnd, SW_SHOW);
        m_should_render = true;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void c_renderer::render() {
    if (m_should_render)
        c_menu::get().draw();
}

void c_renderer::end_scene() {
    if (!m_should_render)
        return;

    ImGui::Render();
    const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
    m_pd3d_device_context->OMSetRenderTargets(1, &m_main_render_target_view, nullptr);
    m_pd3d_device_context->ClearRenderTargetView(m_main_render_target_view, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    m_p_swap_chain->Present(1, 0);
}

bool c_renderer::is_running() const {
    return m_is_running;
}

HWND c_renderer::get_window_handle() const {
    return m_hwnd;
}

HWND c_renderer::find_game_window() {
    return FindWindowW(nullptr, L"Counter-Strike 2");
}

bool c_renderer::create_window(const wchar_t* title, const wchar_t* class_name) {
    m_target_hwnd = find_game_window();
    if (!m_target_hwnd)
        return false;

    RECT rect;
    GetWindowRect(m_target_hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    m_wc = { sizeof(WNDCLASSEX), CS_CLASSDC, wnd_proc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, class_name, nullptr };
    RegisterClassExW(&m_wc);

    m_hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, m_wc.lpszClassName, title, WS_POPUP, rect.left, rect.top, width, height, nullptr, nullptr, m_wc.hInstance, nullptr);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(m_hwnd, &margins);
    SetLayeredWindowAttributes(m_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    return m_hwnd != nullptr;
}

void c_renderer::update_overlay_position() {
    if (!IsWindow(m_target_hwnd)) return;

    RECT rect;
    GetWindowRect(m_target_hwnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    MoveWindow(m_hwnd, rect.left, rect.top, width, height, TRUE);
}

bool c_renderer::create_device_d3d() {
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT create_device_flags = 0;
    D3D_FEATURE_LEVEL feature_level;
    const D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, feature_level_array, 2, D3D11_SDK_VERSION, &sd, &m_p_swap_chain, &m_pd3d_device, &feature_level, &m_pd3d_device_context);

    if (res != S_OK)
        return false;

    create_render_target();
    return true;
}

void c_renderer::cleanup_device_d3d() {
    cleanup_render_target();
    if (m_p_swap_chain) { m_p_swap_chain->Release(); m_p_swap_chain = nullptr; }
    if (m_pd3d_device_context) { m_pd3d_device_context->Release(); m_pd3d_device_context = nullptr; }
    if (m_pd3d_device) { m_pd3d_device->Release(); m_pd3d_device = nullptr; }
}

void c_renderer::create_render_target() {
    ID3D11Texture2D* p_back_buffer;
    m_p_swap_chain->GetBuffer(0, IID_PPV_ARGS(&p_back_buffer));
    m_pd3d_device->CreateRenderTargetView(p_back_buffer, nullptr, &m_main_render_target_view);
    p_back_buffer->Release();
}

void c_renderer::cleanup_render_target() {
    if (m_main_render_target_view) { m_main_render_target_view->Release(); m_main_render_target_view = nullptr; }
}

void c_renderer::toggle_input_passthrough(bool enable) {
    LONG_PTR style = GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
    if (enable) {
        SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, style | WS_EX_TRANSPARENT);
    } else {
        SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, style & ~WS_EX_TRANSPARENT);
    }
}

LRESULT __stdcall c_renderer::wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}