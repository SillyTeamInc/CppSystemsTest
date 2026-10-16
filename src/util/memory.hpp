//
// Created by emi on 10/12/2026.
//
#pragma once

#define SDK_FIELD(name, offset, ...)                                                               \
    [[nodiscard]] __VA_ARGS__& get_##name() const {                                                \
        return *reinterpret_cast<__VA_ARGS__*>(reinterpret_cast<std::uintptr_t>(this) + (offset)); \
    }                                                                                              \
    void set_##name(__VA_ARGS__& value) const {                                              \
        get_##name() = value;                                                                      \
    }                                                                                              \
    __declspec(property(get = get_##name, put = set_##name)) __VA_ARGS__ name

#define SDK_FIELD_ADDR(name, offset, ...)                                                         \
    [[nodiscard]] __VA_ARGS__* name() {                                                           \
        return reinterpret_cast<__VA_ARGS__*>(reinterpret_cast<std::uintptr_t>(this) + (offset)); \
    }

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
