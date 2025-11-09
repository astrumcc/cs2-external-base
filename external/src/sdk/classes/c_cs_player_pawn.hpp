#pragma once

#include <sdk/sdk.hpp>
#include "c_base_entity.hpp"

class c_cs_player_pawn : public c_base_entity
{
public:
	c_cs_player_pawn() : c_base_entity() {}
	c_cs_player_pawn(uintptr_t addr) : c_base_entity(addr) {}

	ADD_FIELD(vec3_t, m_last_clip_camera_pos, address + offsets::m_vecLastClipCameraPos);
	ADD_FIELD(vec3_t, m_old_origin, address + offsets::m_vOldOrigin);

	view_matrix_t view_matrix()
	{
		return memory->read<view_matrix_t>(addresses::view_matrix);
	}
};