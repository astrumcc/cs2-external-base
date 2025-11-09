#pragma once

#include <windows.h>
#include <sdk/sdk.hpp>

class c_visuals
{
public:
	void update();
};

inline const auto g_visuals = std::make_unique<c_visuals>();