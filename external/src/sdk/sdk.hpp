#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include <tlhelp32.h>

#include "memory.h"
#include "offsets.h"

#include <sdk/math/vector.hpp>

#define ADD_FIELD(type, name, addr) \
 __forceinline type name() { return memory->read<type>(addr); } \
 __forceinline void name(type value) { memory->write<type>(addr, value); }

#define ADD_CLASS(type, name, addr) __forceinline type name() { return type(memory->read<uintptr_t>(addr)); }