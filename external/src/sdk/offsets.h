#pragma once

namespace offsets
{
	//C_BaseEntity
	constexpr std::uintptr_t m_iHealth = 0x34C;
	constexpr std::uintptr_t m_iTeamNum = 0x3EB;

	//CCSPlayerController
	constexpr std::uintptr_t m_hPlayerPawn = 0x8FC;
	constexpr std::uintptr_t m_bPawnIsAlive = 0x904;

	//C_CSPlayerPawn
	constexpr std::uintptr_t m_flViewmodelOffsetX = 0x2438; 
	constexpr std::uintptr_t m_flViewmodelOffsetY = 0x243C; 
	constexpr std::uintptr_t m_flViewmodelOffsetZ = 0x2440; 
	constexpr std::uintptr_t m_flViewmodelFOV = 0x2444; 
	constexpr std::uintptr_t m_bIsScoped = 0x2718;
	constexpr std::uintptr_t m_bIsDefusing = 0x271A;
	constexpr std::uintptr_t m_vecLastClipCameraPos = 0x3DC4;
	constexpr std::uintptr_t m_pWeaponServices = 0x13F0;
	constexpr std::uintptr_t m_vOldOrigin = 0x15A0;

	//CPlayer_WeaponServices
	constexpr std::uintptr_t m_hMyWeapons = 0x40;
	constexpr std::uintptr_t m_hActiveWeapon = 0x58;
	constexpr std::uintptr_t m_hLastWeapon = 0x5C;
	constexpr std::uintptr_t m_iAmmo = 0x60;

	//C_BasePlayerWeapon
	constexpr std::uintptr_t m_iClip1 = 0x18F0; 
	constexpr std::uintptr_t m_iClip2 = 0x18F4; 
	constexpr std::uintptr_t m_pReserveAmmo = 0x18F8; 
}

namespace signatures
{
	inline const char* entity_list = "48 89 35 ? ? ? ? 48 85 F6";
	inline const char* local_player_controller = "48 8B 05 ? ? ? ? 41 89 BE";
	inline const char* view_matrix = "48 8D 0D ? ? ? ? 48 C1 E0 06";
}

namespace addresses
{
	inline uintptr_t entity_list;
	inline uintptr_t local_player_controller;
	inline uintptr_t view_matrix;
}