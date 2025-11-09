#pragma once
#include <sdk/sdk.hpp>
#include <sdk/classes/c_cs_player_controller.hpp>
#include <sdk/classes/c_cs_player_pawn.hpp>

class c_entity
{
public:
	uintptr_t get_list()
	{
		return memory->read<uintptr_t>(addresses::entity_list);
	}

	c_cs_player_pawn get_local_player()
	{
		uintptr_t controller = memory->read<uintptr_t>(addresses::local_player_controller);
		uintptr_t pawn_index = memory->read<uintptr_t>(controller + offsets::m_hPlayerPawn);
		uintptr_t entry = memory->read<uintptr_t>(get_list() + 0x8 * ((pawn_index & 0x7FFF) >> 9) + 16);
		return c_cs_player_pawn(memory->read<uintptr_t>(entry + 112 * (pawn_index & 0x1FF)));
	}

	c_cs_player_controller get_controller(int index)
	{
		uintptr_t list = get_list();
		if (!list) return c_cs_player_controller(0);

		uintptr_t entry = memory->read<uintptr_t>(list + (8 * (index & 0x7FFF) >> 9) + 16);
		if (!entry) return c_cs_player_controller(0);

		uintptr_t controller = memory->read<uintptr_t>(entry + 112 * (index & 0x7FFF));
		return c_cs_player_controller(controller);
	}

	c_cs_player_pawn get_pawn_from_handle(uint32_t handle)
	{
		if (!handle || handle == 0xFFFFFFFF) return c_cs_player_pawn(0);

		uintptr_t list = get_list();
		if (!list) return c_cs_player_pawn(0);

		uintptr_t entry = memory->read<uintptr_t>(list + 0x8 * ((handle & 0x7FFF) >> 9) + 16);
		if (!entry) return c_cs_player_pawn(0);

		uintptr_t pawn = memory->read<uintptr_t>(entry + 112 * (handle & 0x1FF));
		return c_cs_player_pawn(pawn);
	}

	c_cs_player_pawn get_player_pawn(int index)
	{
		c_cs_player_controller controller = get_controller(index);
		if (!controller.is_valid()) return c_cs_player_pawn(0);

		uint32_t pawn_handle = controller.get_player_pawn_handle();
		if (!pawn_handle || pawn_handle == 0xFFFFFFFF) return c_cs_player_pawn(0);

		bool is_pawn_alive = controller.is_pawn_alive();
		if (!is_pawn_alive) return c_cs_player_pawn(0);

		return get_pawn_from_handle(pawn_handle);
	}
};

inline const auto entity = std::make_unique<c_entity>();