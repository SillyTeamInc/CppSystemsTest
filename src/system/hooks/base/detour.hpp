//
// Created by emi on 10/12/2026.
//
#pragma once

class detour {
public:
    std::string name;
    void* func;
    void* originalFunc;
    ~detour();
    detour(const std::string& name, void* target, void* detour);
    detour(const std::string& name, uintptr_t target, void* detour);

    void enable() const;
    void restore() const;

    template<auto T>
    auto get_original() const {
        return reinterpret_cast<decltype(T)>(originalFunc);
    }
};
