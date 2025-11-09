#pragma once

class c_menu {
public:
    static c_menu& get();
    void draw();
    void toggle_menu();
    bool is_menu_open() const;

private:
    bool m_is_menu_open = true;
};