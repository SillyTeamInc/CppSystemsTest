//
// Created by emi on 10/24/26.
//

#include "keyboard.hpp"
#include <imgui.h>
#include <map>
#include <windows.h>

#include "spdlog/spdlog.h"

ImGuiKey keyboard::vk_to_imgui_key(std::uint32_t vk, std::uint8_t scan_code, bool extended) {
    if (vk >= 'A' && vk <= 'Z')
        return static_cast<ImGuiKey>(ImGuiKey_A + (vk - 'A'));
    if (vk >= '0' && vk <= '9')
        return static_cast<ImGuiKey>(ImGuiKey_0 + (vk - '0'));
    if (vk >= VK_F1 && vk <= VK_F12)
        return static_cast<ImGuiKey>(ImGuiKey_F1 + (vk - VK_F1));
    if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9)
        return static_cast<ImGuiKey>(ImGuiKey_Keypad0 + (vk - VK_NUMPAD0));

    switch (vk) {
        case VK_TAB:
            return ImGuiKey_Tab;
        case VK_LEFT:
            return ImGuiKey_LeftArrow;
        case VK_RIGHT:
            return ImGuiKey_RightArrow;
        case VK_UP:
            return ImGuiKey_UpArrow;
        case VK_DOWN:
            return ImGuiKey_DownArrow;
        case VK_PRIOR:
            return ImGuiKey_PageUp;
        case VK_NEXT:
            return ImGuiKey_PageDown;
        case VK_HOME:
            return ImGuiKey_Home;
        case VK_END:
            return ImGuiKey_End;
        case VK_INSERT:
            return ImGuiKey_Insert;
        case VK_DELETE:
            return ImGuiKey_Delete;
        case VK_BACK:
            return ImGuiKey_Backspace;
        case VK_SPACE:
            return ImGuiKey_Space;
        case VK_RETURN:
            return extended ? ImGuiKey_KeypadEnter : ImGuiKey_Enter;
        case VK_ESCAPE:
            return ImGuiKey_Escape;
        case VK_CAPITAL:
            return ImGuiKey_CapsLock;
        case VK_SHIFT:
            return scan_code == 0x36 ? ImGuiKey_RightShift : ImGuiKey_LeftShift;
        case VK_CONTROL:
            return extended ? ImGuiKey_RightCtrl : ImGuiKey_LeftCtrl;
        case VK_MENU:
            return extended ? ImGuiKey_RightAlt : ImGuiKey_LeftAlt;
        case VK_LWIN:
            return ImGuiKey_LeftSuper;
        case VK_RWIN:
            return ImGuiKey_RightSuper;
        case VK_OEM_7:
            return ImGuiKey_Apostrophe;
        case VK_OEM_COMMA:
            return ImGuiKey_Comma;
        case VK_OEM_MINUS:
            return ImGuiKey_Minus;
        case VK_OEM_PERIOD:
            return ImGuiKey_Period;
        case VK_OEM_2:
            return ImGuiKey_Slash;
        case VK_OEM_1:
            return ImGuiKey_Semicolon;
        case VK_OEM_PLUS:
            return ImGuiKey_Equal;
        case VK_OEM_4:
            return ImGuiKey_LeftBracket;
        case VK_OEM_5:
            return ImGuiKey_Backslash;
        case VK_OEM_6:
            return ImGuiKey_RightBracket;
        case VK_OEM_3:
            return ImGuiKey_GraveAccent;
        default:
            return ImGuiKey_None;
    }
}
std::string_view keyboard::key_to_string(key k) {
    static constexpr auto cached_names = [] {
        std::array<key_name, 0x100> names{};

        for (const key enum_key: magic_enum::enum_values<key>()) {
            names[static_cast<std::uint32_t>(enum_key)] = format_key_name(magic_enum::enum_name(enum_key));
        }

        return names;
    }();

    return cached_names[static_cast<std::uint32_t>(k)];
}
std::map<keyboard::key, std::string_view> keyboard::get_all_keys() {
    std::map<key, std::string_view> result {};
    for (const key k: magic_enum::enum_values<key>()) {
        result[k] = key_to_string(k);
    }
    return result;
}


// this is fucking epic
