#pragma once

#include <windows.h>
#include <d3d11.h>

class c_renderer {
public:
    static c_renderer& get();
    bool initialize(const wchar_t* window_title, const wchar_t* window_class_name);
    void shutdown();

    void begin_scene();
    void render();
    void end_scene();

    bool is_running() const;
    HWND get_window_handle() const;

private:
    bool create_window(const wchar_t* title, const wchar_t* class_name);
    bool create_device_d3d();
    void cleanup_device_d3d();
    void create_render_target();
    void cleanup_render_target();

    HWND find_game_window();
    void update_overlay_position();
    void toggle_input_passthrough(bool enable);

    static LRESULT __stdcall wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd = nullptr;
    HWND m_target_hwnd = nullptr;
    WNDCLASSEXW m_wc = {};
    bool m_is_running = true;
    bool m_should_render = false;

    ID3D11Device* m_pd3d_device = nullptr;
    ID3D11DeviceContext* m_pd3d_device_context = nullptr;
    IDXGISwapChain* m_p_swap_chain = nullptr;
    ID3D11RenderTargetView* m_main_render_target_view = nullptr;
};
