#pragma once
#include <sdk/sdk.hpp>

class c_base_entity
{
protected:
	uintptr_t address;

public:
	c_base_entity() : address(0) {}
	c_base_entity(uintptr_t addr) : address(addr) {}

	uintptr_t get_address() const { return address; }
	bool is_valid() const { return address != 0; }

	ADD_FIELD(int, get_health, address + offsets::m_iHealth);
	ADD_FIELD(uint8_t, get_team, address + offsets::m_iTeamNum);
};                      