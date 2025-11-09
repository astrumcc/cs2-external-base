/*ASTRUM EXTERNAL BASE*/

#include <windows.h>
#include <sdk/sdk.hpp>
#include "render/c_renderer.hpp"
#include "hooks/hooks.hpp"

#include <features/visuals.h>

#include <thread>

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    if (!memory->attach("cs2.exe")) {
        return 1;
    }

    memory->client_dll = memory->get_module_base("client.dll");
    if (!memory->client_dll) {
        return 1;
    }

    addresses::entity_list = memory->resolve_rip(memory->find_pattern(memory->client_dll, 0x5000000, signatures::entity_list), 3, 7);
    addresses::view_matrix = memory->resolve_rip(memory->find_pattern(memory->client_dll, 0x5000000, signatures::view_matrix), 3, 7);
    addresses::local_player_controller = memory->resolve_rip(memory->find_pattern(memory->client_dll, 0x5000000, signatures::local_player_controller), 3, 7);

    if (!c_renderer::get().initialize(L"External_CS2", L"External_CS2_Class")) {
        return 1;
    }

    if (!hooks::initialize()) {
        c_renderer::get().shutdown();
        return 1;
    }

    while (c_renderer::get().is_running()) {
        c_renderer::get().begin_scene();  

        g_visuals->update();              
        c_renderer::get().render();       

        c_renderer::get().end_scene();    
    }

    hooks::shutdown();
    c_renderer::get().shutdown();

    return 0;
}