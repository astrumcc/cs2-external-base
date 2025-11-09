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
	inline const char* entity_list = "48 8B 0D ? ? ? ? 48 89 7C 24 ? 8B FB C1 EB ? 81 E7 ? ? ? ? 81 E3 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 84 C0 74 ? 48 8B 0D ? ? ? ? 8B C7 48 C1 E8 ? 48 8B 54 C1 ? 48 85 D2 74 ? 8B C7 25 ? ? ? ? 8B CF 48 6B C0 ? 48 03 C2 74 ? 8B 40 ? 25 ? ? ? ? 3B C7 75 ? 81 E1 ? ? ? ? 48 6B C1 ? 8B 4C 10 ? 8B 44 10 ? 83 E0 ? C1 E9 ? 2B C8 C1 E1 ? C1 E9 ? 8B C1 25 ? ? ? ? 3B C3 0F 44 D9 48 8B 15 ? ? ? ? C1 E3 ? 0B DF 48 8B 7C 24 ? 48 85 D2 74 ? 8B C3 25 ? ? ? ? 8B C8 48 C1 E8 ? 48 8B 14 C2 48 85 D2 74 ? 81 E1 ? ? ? ? 48 6B C1 ? 48 03 C2 74 ? 39 58 ? 49 0F 45 C7";
	inline const char* local_player_controller = "48 8B 05 ? ? ? ? 41 89 BE";
	inline const char* view_matrix = "48 8D 0D ? ? ? ? 48 C1 E0 ? 48 03 C1 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC";
}

namespace addresses
{
	inline uintptr_t entity_list;
	inline uintptr_t local_player_controller;
	inline uintptr_t view_matrix;
}