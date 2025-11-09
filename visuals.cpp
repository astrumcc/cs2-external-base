#include "visuals.h"
#include <sdk/entity.hpp>

void c_visuals::update()
{
    c_cs_player_pawn local_pawn = entity->get_local_player();
    if (!local_pawn.is_valid()) return;
    
    int local_team = local_pawn.get_team();
    view_matrix_t view_mat = local_pawn.view_matrix();
    
    for (int i = 0; i < 64; i++)
    {
        c_cs_player_pawn pawn = entity->get_player_pawn(i);
        if (!pawn.is_valid()) continue;
    
        int health = pawn.get_health();
        if (health <= 0) continue;
    
        int team = pawn.get_team();
        if (team == local_team) continue;
    
        vec3_t origin_pos = pawn.m_old_origin();
        vec3_t camera_pos = pawn.m_last_clip_camera_pos();
    
        vec2_t screen_top, screen_bottom;
        if (!camera_pos.world_to_screen(screen_top, view_mat) || !origin_pos.world_to_screen(screen_bottom, view_mat))
            continue;
    
        float box_height = screen_bottom.y - screen_top.y;
        float box_width = box_height * 0.5f;
    
        ImVec2 box_min = ImVec2((screen_top.x + screen_bottom.x) * 0.5f - box_width * 0.5f, screen_top.y);
        ImVec2 box_max = ImVec2((screen_top.x + screen_bottom.x) * 0.5f + box_width * 0.5f, screen_bottom.y);
    
        ImGui::GetBackgroundDrawList()->AddRect(box_min, box_max, IM_COL32(255, 255, 255, 255), 0.0f, 0, 1.0f);
    }
}