//
// Created by emi on 10/12/2026.
//

#include "detour.hpp"
#include "util/exceptions.hpp"

detour::~detour() = default;

detour::detour(const std::string& name, void* target, void* detour) {
    this->func = target;
    this->name = name;

    const MH_STATUS result = MH_CreateHook(func, detour, &originalFunc);
    if (result != MH_OK) {
        util::exceptions::throw_error("Failed to create detour for " + name + ": " + std::to_string(result));
    }
}

detour::detour(const std::string &name, uintptr_t target, void* detour) {
    this->func = reinterpret_cast<void*>(target);
    this->name = name;

    const MH_STATUS result = MH_CreateHook(func, detour, &originalFunc);
    if (result != MH_OK) {
        util::exceptions::throw_error("Failed to create detour for " + name + ": " + std::to_string(result));
    }

    spdlog::info("Detour created for {} at address 0x{:X}", name, target);
}

void detour::enable() const {
    const MH_STATUS result = MH_EnableHook(func);
    if (result != MH_OK) {
        util::exceptions::throw_error("Failed to enable hook for " + name + ": " + std::to_string(result));
    }

    spdlog::info("Detour enabled for {}", name);
}

void detour::restore() const {
    const MH_STATUS result = MH_DisableHook(func);
    if (result != MH_OK) {
        util::exceptions::throw_error("Failed to disable detour for " + name + ": " + std::to_string(result));
    }

    spdlog::info("Detour disabled for {}", name);
}