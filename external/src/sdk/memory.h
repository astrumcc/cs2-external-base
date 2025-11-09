#pragma once
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <vector>

struct pattern_byte {
    unsigned char value;
    bool wildcard;
};

class c_memory {
public:
    uintptr_t client_dll;
    HANDLE process_handle;
    DWORD process_id;

    bool attach(const char* process_name) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE)
            return false;

        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &entry)) {
            do {
                if (strcmp(entry.szExeFile, process_name) == 0) {
                    process_id = entry.th32ProcessID;
                    process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
                    CloseHandle(snapshot);
                    return process_handle != nullptr;
                }
            } while (Process32Next(snapshot, &entry));
        }

        CloseHandle(snapshot);
        return false;
    }

    template<typename t>
    t read(uintptr_t address) {
        t buffer;
        ReadProcessMemory(process_handle, (void*)address, &buffer, sizeof(t), nullptr);
        return buffer;
    }

    template<typename t>
    bool write(uintptr_t address, t value) {
        return WriteProcessMemory(process_handle, (void*)address, &value, sizeof(t), nullptr);
    }

    bool write_bytes(uintptr_t address, const void* data, size_t size) {
        return WriteProcessMemory(process_handle, (void*)address, data, size, nullptr);
    }

    std::string read_string(uintptr_t address, size_t max_length = 256) {
        char buffer[256];
        ReadProcessMemory(process_handle, (void*)address, buffer, max_length, nullptr);
        buffer[max_length - 1] = 0;
        return std::string(buffer);
    }

    bool write_string(uintptr_t address, const std::string& str) {
        return WriteProcessMemory(process_handle, (void*)address, str.c_str(), str.length() + 1, nullptr);
    }

    uintptr_t get_module_base(const char* module_name) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
        if (snapshot == INVALID_HANDLE_VALUE)
            return 0;

        MODULEENTRY32 entry;
        entry.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(snapshot, &entry)) {
            do {
                if (strcmp(entry.szModule, module_name) == 0) {
                    CloseHandle(snapshot);
                    return (uintptr_t)entry.modBaseAddr;
                }
            } while (Module32Next(snapshot, &entry));
        }

        CloseHandle(snapshot);
        return 0;
    }

    std::vector<pattern_byte> parse_pattern(const char* pattern) {
        std::vector<pattern_byte> bytes;
        const char* p = pattern;

        while (*p) {
            if (*p == ' ') {
                p++;
                continue;
            }

            if (*p == '?') {
                bytes.push_back({ 0, true });
                p++;
                if (*p == '?') p++;
            }
            else {
                char byte_str[3] = { p[0], p[1], 0 };
                bytes.push_back({ (unsigned char)strtoul(byte_str, nullptr, 16), false });
                p += 2;
            }
        }

        return bytes;
    }

    uintptr_t find_pattern(uintptr_t base, size_t size, const char* pattern_str) {
        auto pattern = parse_pattern(pattern_str);
        char* buffer = new char[size];
        ReadProcessMemory(process_handle, (void*)base, buffer, size, nullptr);

        for (size_t i = 0; i < size - pattern.size(); i++) {
            bool found = true;
            for (size_t j = 0; j < pattern.size(); j++) {
                if (!pattern[j].wildcard && pattern[j].value != (unsigned char)buffer[i + j]) {
                    found = false;
                    break;
                }
            }

            if (found) {
                delete[] buffer;
                return base + i;
            }
        }

        delete[] buffer;
        return 0;
    }

    uintptr_t resolve_rip(uintptr_t address, int offset, int instruction_length) {
        int rel_offset = read<int>(address + offset);
        return address + rel_offset + instruction_length;
    }
};

inline const auto memory = std::make_unique<c_memory>();