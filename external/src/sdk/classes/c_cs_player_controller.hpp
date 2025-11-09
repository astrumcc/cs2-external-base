#pragma once

#include <sdk/sdk.hpp>
#include "c_base_entity.hpp"

class c_cs_player_controller : public c_base_entity
{
public:
	c_cs_player_controller() : c_base_entity() {}
	c_cs_player_controller(uintptr_t addr) : c_base_entity(addr) {}

	ADD_FIELD(uint32_t, get_player_pawn_handle, address + offsets::m_hPlayerPawn);
	ADD_FIELD(bool, is_pawn_alive, address + offsets::m_bPawnIsAlive);
};