#include <windows.h>
#include <WinUser.h>
#include "c_menu.hpp"
#include "imgui.h"
#include <process.h>

c_menu& c_menu::get() {
    static c_menu instance;
    return instance;
}

void c_menu::draw() {
    toggle_menu();

    if (!m_is_menu_open)
        return;

    ImGui::Begin("Window", &m_is_menu_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    {
        if(ImGui::Button("Exit"))
            exit(0);
    }
    ImGui::End();
}

void c_menu::toggle_menu() {
    if(GetAsyncKeyState(VK_INSERT) & 1)
        m_is_menu_open = !m_is_menu_open;
}

bool c_menu::is_menu_open() const {
    return m_is_menu_open;
}