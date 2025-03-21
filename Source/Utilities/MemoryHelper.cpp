/**
Display Utilities code that is from Lyall's C++ mods (https://github.com/Lyall/). Simply borrowing them here.
At least on the MGSHDFix repo, it's licensed under MIT. So I figured I'd mention that here.
**/

#include "MemoryHelper.h"

namespace Memory
{

    void PatchBytes(uintptr_t address, const char* pattern, unsigned int numBytes)
    {
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<LPVOID>(address), numBytes, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(reinterpret_cast<LPVOID>(address), pattern, numBytes);
        VirtualProtect(reinterpret_cast<LPVOID>(address), numBytes, oldProtect, &oldProtect);
    }

    HMODULE GetThisDllHandle()
    {
        MEMORY_BASIC_INFORMATION info;
        size_t len = VirtualQueryEx(GetCurrentProcess(), reinterpret_cast<void*>(GetThisDllHandle), &info, sizeof(info));
        assert(len == sizeof(info));
        return len ? static_cast<HMODULE>(info.AllocationBase) : nullptr;
    }

    uint32_t ModuleTimestamp(void* module)
    {
        auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
        auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(module) + dosHeader->e_lfanew);
        return ntHeaders->FileHeader.TimeDateStamp;
    }

    uint8_t* PatternScan(void* module, const char* signature)
    {
        auto pattern_to_byte = [](const char* pattern) {
            std::vector<int> bytes;
            auto start = const_cast<char*>(pattern);
            auto end = start + strlen(pattern);

            for (auto current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?') ++current;
                    bytes.push_back(-1);
                } else {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
        auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(module) + dosHeader->e_lfanew);
        auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = pattern_to_byte(signature);
        auto scanBytes = reinterpret_cast<uint8_t*>(module);

        auto s = patternBytes.size();
        auto d = patternBytes.data();

        for (size_t i = 0; i < sizeOfImage - s; ++i) {
            bool found = true;
            for (size_t j = 0; j < s; ++j) {
                if (scanBytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return &scanBytes[i];
            }
        }
        return nullptr;
    }

    uintptr_t GetAbsolute(uintptr_t address) noexcept
    {
        return (address + 4 + *reinterpret_cast<int32_t*>(address));
    }

    BOOL HookIAT(HMODULE callerModule, const char* targetModule, const void* targetFunction, void* detourFunction)
    {
        auto* base = reinterpret_cast<uint8_t*>(callerModule);
        auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
        auto ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dosHeader->e_lfanew);
        auto* imports = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

        for (int i = 0; imports[i].Characteristics; i++) {
            const char* name = reinterpret_cast<const char*>(base + imports[i].Name);
            if (lstrcmpiA(name, targetModule) != 0) continue;

            void** thunk = reinterpret_cast<void**>(base + imports[i].FirstThunk);
            for (; *thunk; ++thunk) {
                if (*thunk != targetFunction) continue;

                DWORD oldState;
                if (!VirtualProtect(thunk, sizeof(void*), PAGE_READWRITE, &oldState))
                    return FALSE;

                *thunk = detourFunction;
                VirtualProtect(thunk, sizeof(void*), oldState, &oldState);
                return TRUE;
            }
        }
        return FALSE;
    }
}

namespace Util
{
    std::wstring utf8_decode(const std::string& str)
    {
        if (str.empty()) return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstrTo[0], size_needed);
        return wstrTo;
    }
}
