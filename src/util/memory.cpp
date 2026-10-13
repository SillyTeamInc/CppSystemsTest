//
// Created by emi on 10/12/2026.
//
#include "memory.hpp"
#include <Windows.h>
//converts raw pointer to base+offset, if the provided address is not in a module, it will return the address in hex
namespace util::memory {
    std::string get_memory_string(uintptr_t addr) {
        MEMORY_BASIC_INFORMATION mbi;
        if(VirtualQuery(reinterpret_cast<void*>(addr), &mbi, sizeof(mbi)))
        {
            char moduleName[MAX_PATH];
            if (GetModuleFileNameA(static_cast<HMODULE>(mbi.AllocationBase), moduleName, MAX_PATH)) {
                std::string addrHex = fmt::format("{:X}", addr - reinterpret_cast<uintptr_t>(mbi.AllocationBase));
                std::string result = std::string(moduleName) + "+" + addrHex;
                return result.substr(result.find_last_of('\\') + 1);
            }
        }

        return fmt::format("{:X}", addr);
    }
}