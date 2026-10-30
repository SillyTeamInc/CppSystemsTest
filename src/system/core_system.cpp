//
// Created by emi on 10/29/2026.
//
#include "core_system.hpp"
#include "event_system.hpp"
#include "systems_manager.hpp"

LRESULT core_system::wndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN || uMsg == WM_KEYUP || uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP) {
        auto* coreSys = system_of_a_down->get<core_system>();

        const auto key = static_cast<uint32_t>(wParam);
        const bool keyDown = uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN;

        if (keyDown)
            coreSys->m_pressed_keys.insert(key);
        else
            coreSys->m_pressed_keys.erase(key);


        if (wParam == VK_END && uMsg == WM_KEYDOWN) {
            spdlog::info("End pressed, requesting eject");
            coreSys->m_eject_requested = true;
            return 0;
        }

        hvs::event_dispatcher& dispatcher = coreSys->manager->get<event_system>()->dispatcher();
        keyboard_event event{hWnd, static_cast<uint32_t>(wParam), lParam, uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN,
                             uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP};

        dispatcher.post(event);

        if (event.is_canceled()) {
            return 0;
        }
    }


    if (uMsg == WM_CHAR) {
        char_event event{static_cast<wchar_t>(wParam)};
        system_of_a_down->get<event_system>()->dispatcher().post(event);

        if (event.is_canceled()) {
            return 0;
        }
    }

    return CallWindowProc(reinterpret_cast<WNDPROC>(wndProcOld), hWnd, uMsg, wParam, lParam);
}
void core_system::initialize() {
    system_of_a_down = manager;
    while (m_window_handle == nullptr) {
        m_window_handle = FindWindowA("Bedrock", "Minecraft");
        if (m_window_handle == nullptr) {
            spdlog::info("waiting for mc window!!!!");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    spdlog::info("Initializing core system, window handle: {:X}", reinterpret_cast<std::uintptr_t>(m_window_handle));

    char old_title[256];
    GetWindowTextA(m_window_handle, old_title, sizeof(old_title));
    m_old_window_title = old_title;
    SetWindowTextA(m_window_handle, "CppSystemsTest - Minecraft");

    wndProcOld = SetWindowLongPtrA(m_window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&wndProcHook));
    spdlog::info("wndproc hooked for keyboard input");
}

void core_system::shutdown() {
    SetWindowLongPtrA(m_window_handle, GWLP_WNDPROC, wndProcOld);
    spdlog::info("wndproc restored");

    SetWindowTextA(m_window_handle, m_old_window_title.c_str());
}
