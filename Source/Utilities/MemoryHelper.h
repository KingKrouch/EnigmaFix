//
// Created by bryce on 3/21/25.
//

#ifndef MEMORY_HELPER_HPP
#define MEMORY_HELPER_HPP

#include <windows.h>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <initializer_list>

namespace Memory
{
    template<typename T>
    void Write(uintptr_t writeAddress, T value)
    {
        DWORD oldProtect;
        VirtualProtect((LPVOID)(writeAddress), sizeof(T), PAGE_EXECUTE_WRITECOPY, &oldProtect);
        *(reinterpret_cast<T*>(writeAddress)) = value;
        VirtualProtect((LPVOID)(writeAddress), sizeof(T), oldProtect, &oldProtect);
    }

    void PatchBytes(uintptr_t address, const char* pattern, unsigned int numBytes);
    HMODULE GetThisDllHandle();
    uint32_t ModuleTimestamp(void* module);
    uint8_t* PatternScan(void* module, const char* signature);
    uintptr_t GetAbsolute(uintptr_t address) noexcept;
    BOOL HookIAT(HMODULE callerModule, char const* targetModule, const void* targetFunction, void* detourFunction);
}

namespace Util
{
    int findStringInVector(std::string& str, const std::initializer_list<std::string>& search);
    std::wstring utf8_decode(const std::string& str);
}

#endif // MEMORY_HELPER_HPP
