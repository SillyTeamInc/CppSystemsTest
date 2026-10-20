//
// Created by emi on 10/12/2026.
//
#pragma once

#include <excpt.h>

#define SDK_FIELD(name, offset, ...)                                                               \
    [[nodiscard]] __VA_ARGS__& get_##name() const {                                                \
        return *reinterpret_cast<__VA_ARGS__*>(reinterpret_cast<std::uintptr_t>(this) + (offset)); \
    }                                                                                              \
    void set_##name(__VA_ARGS__& value) const {                                              \
        get_##name() = value;                                                                      \
    }                                                                                              \
    __declspec(property(get = get_##name, put = set_##name)) __VA_ARGS__ name

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define VIRT_PAD private: \
    virtual void TOKENPASTE2(padding_, __LINE__)() = 0; \
    public: \

#define PAD(size) \
        private: \
            char TOKENPASTE2(padding_, __LINE__) [size] = {0}; \
        public:

template<typename Func, typename... Args>
bool TryCallWrapper(Func func, Args&&... args) {
    __try
    {
        func(std::forward<Args>(args)...);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return false;
    }
}

// Refactored TRY_CALL using lambdas and spdlog
#define TRY_CALL(func, ...) \
[&]() { \
bool result = TryCallWrapper([&]() { func(__VA_ARGS__); }); \
if (!result) { \
spdlog::error("Exception thrown in {} at line {} in {}", __FUNCTION__, __LINE__, __FILE__); \
} \
return result; \
}()
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

    uintptr_t get_virtual_func(void* _this, int index);

    std::string get_memory_string(uintptr_t addr);
}
