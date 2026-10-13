//
// Created by emi on 10/12/2026.
//
#pragma once

namespace util::memory {
    template<typename Ret, typename... Args>
    static Ret call_func(uintptr_t func_addr, Args... args) {
        using func_t = Ret(__fastcall*)(Args...);
        auto func = reinterpret_cast<func_t>(func_addr);
        return func(std::forward<Args>(args)...);
    }

    template<typename Ret, typename... Args>
    static auto call_virtual(int index, void* _this, Args... args)
    {
        using Fn = Ret(__thiscall*)(void*, Args...);
        const auto vtable = *static_cast<uintptr_t**>(_this);
        return reinterpret_cast<Fn>(vtable[index])(_this, args...);
    }

    std::string get_memory_string(uintptr_t addr);
}
