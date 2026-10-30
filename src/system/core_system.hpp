//
// Created by emi on 10/12/2026.
//
#pragma once

#include <kiero.hpp>
#include <unordered_set>
#include <windows.h>
#include <winrt/base.h>
#include "generic_system.hpp"

#include "event_system.hpp"
#include "events/char_event.hpp"
#include "events/keyboard_event.hpp"
#include "spdlog/spdlog.h"

class core_system : public generic_system {
    std::string m_old_window_title{};
public:
    core_system(HMODULE hModule) : m_dll_module(hModule) {}

    HMODULE m_dll_module{};
    bool m_eject_requested = false;
    HWND m_window_handle = nullptr;
    std::unordered_set<uint32_t> m_pressed_keys;

    static inline LONG_PTR wndProcOld{};
    static inline systems_manager* system_of_a_down{};

    static LRESULT wndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void initialize() override;
    void shutdown() override;
};
