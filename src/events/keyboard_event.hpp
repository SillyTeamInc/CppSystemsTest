//
// Created by emi on 10/12/2026.
//
#pragma once

class keyboard_event : public hvs::cancelable_event {
public:
    HWND window;
    uint32_t key_code;
    LPARAM lparam;
    uint16_t repeat_count;
    uint8_t scan_code;
    bool is_extended_key;
    uint8_t reserved;
    bool context_code;
    bool previous_key_state;
    bool transition_state;
    bool is_down;
    bool is_system_key;

    keyboard_event(
        HWND window,
        uint32_t key_code,
        LPARAM lparam,
        bool is_down,
        bool is_system_key
    )
        : window(window),
          key_code(key_code),
          lparam(lparam),
        repeat_count(static_cast<uint16_t>(lparam & 0xFFFF)),
        scan_code(static_cast<uint8_t>((lparam >> 16) & 0xFF)),
        is_extended_key((lparam & (1L << 24)) != 0),
        reserved(static_cast<uint8_t>((lparam >> 25) & 0x0F)),
        context_code((lparam & (1L << 29)) != 0),
        previous_key_state((lparam & (1L << 30)) != 0),
        transition_state((lparam & (1L << 31)) != 0),
        is_down(is_down),
        is_system_key(is_system_key) {

    }

    std::string to_string() {
        return fmt::format("KeyboardEvent: key_code = {}, repeat_count = {}, scan_code = {}, is_extended_key = {}, reserved = {}, context_code = {}, previous_key_state = {}, transition_state = {}, is_down = {}, is_system_key = {}",
            key_code, repeat_count, scan_code, is_extended_key, reserved, context_code, previous_key_state, transition_state, is_down, is_system_key);
    }
};