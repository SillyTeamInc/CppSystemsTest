//
// Created by emi on 10/24/26.
//

#pragma once
#include <cctype>
#include <cstdint>
#include <imgui.h>
#include <map>
#include <string_view>

#include "magic_enum.hpp"


namespace keyboard {
    ImGuiKey vk_to_imgui_key(std::uint32_t vk, std::uint8_t scan_code, bool extended);

    // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    // i'm sorry
    enum class key : std::uint32_t {
        left_mouse = 0x01,
        right_mouse = 0x02,
        cancel = 0x03,
        middle_mouse = 0x04,
        x1_mouse = 0x05,
        x2_mouse = 0x06,
        clear = 0x0C,
        return_key = 0x0D,
        shift = 0x10,
        control = 0x11,
        alt = 0x12,
        pause = 0x13,
        tab = 0x09,
        kana = 0x15,
        hangul = 0x15,
        ime_on = 0x16,
        junja = 0x17,
        final = 0x18,
        hanja = 0x19,
        kanji = 0x19,
        ime_off = 0x1A,
        convert = 0x1C,
        nonconvert = 0x1D,
        accept = 0x1E,
        mode_change = 0x1F,
        left_arrow = 0x25,
        up_arrow = 0x26,
        right_arrow = 0x27,
        down_arrow = 0x28,
        select = 0x29,
        print = 0x2A,
        execute = 0x2B,
        snapshot = 0x2C,
        page_up = 0x21,
        page_down = 0x22,
        home = 0x24,
        end = 0x23,
        insert = 0x2D,
        delete_key = 0x2E,
        backspace = 0x08,
        space = 0x20,
        enter = 0x0D,
        // capital = 0x14,
        escape = 0x1B,
        caps_lock = 0x14,
        left_shift = 0xA0,
        right_shift = 0xA1,
        left_ctrl = 0xA2,
        right_ctrl = 0xA3,
        left_alt = 0xA4,
        right_alt = 0xA5,
        left_super = 0x5B, // Windows key
        right_super = 0x5C, // Windows key
        apps = 0x5D,
        sleep = 0x5F,
        numpad_0 = 0x60,
        numpad_1 = 0x61,
        numpad_2 = 0x62,
        numpad_3 = 0x63,
        numpad_4 = 0x64,
        numpad_5 = 0x65,
        numpad_6 = 0x66,
        numpad_7 = 0x67,
        numpad_8 = 0x68,
        numpad_9 = 0x69,
        multiply = 0x6A,
        add = 0x6B,
        separator = 0x6C,
        subtract = 0x6D,
        decimal = 0x6E,
        divide = 0x6F,
        f1 = 0x70,
        f2 = 0x71,
        f3 = 0x72,
        f4 = 0x73,
        f5 = 0x74,
        f6 = 0x75,
        f7 = 0x76,
        f8 = 0x77,
        f9 = 0x78,
        f10 = 0x79,
        f11 = 0x7A,
        f12 = 0x7B,
        f13 = 0x7C,
        f14 = 0x7D,
        f15 = 0x7E,
        f16 = 0x7F,
        f17 = 0x80,
        f18 = 0x81,
        f19 = 0x82,
        f20 = 0x83,
        f21 = 0x84,
        f22 = 0x85,
        f23 = 0x86,
        f24 = 0x87,
        num_lock = 0x90,
        scroll_lock = 0x91,
        browser_back = 0xA6,
        browser_forward = 0xA7,
        browser_refresh = 0xA8,
        browser_stop = 0xA9,
        browser_search = 0xAA,
        browser_favorites = 0xAB,
        browser_home = 0xAC,
        volume_mute = 0xAD,
        volume_down = 0xAE,
        volume_up = 0xAF,
        media_next_track = 0xB0,
        media_prev_track = 0xB1,
        media_stop = 0xB2,
        media_play_pause = 0xB3,
        launch_mail = 0xB4,
        launch_media_select = 0xB5,
        launch_app1 = 0xB6,
        launch_app2 = 0xB7,
        oem_1 = 0xBA,
        oem_plus = 0xBB,
        oem_comma = 0xBC,
        oem_minus = 0xBD,
        oem_period = 0xBE,
        oem_2 = 0xBF,
        grave_accent = 0xC0,// oem_3
        gamepad_a = 0xC3,
        gamepad_b = 0xC4,
        gamepad_x = 0xC5,
        gamepad_y = 0xC6,
        gamepad_right_shoulder = 0xC7,
        gamepad_left_shoulder = 0xC8,
        gamepad_left_trigger = 0xC9,
        gamepad_right_trigger = 0xCA,
        gamepad_dpad_up = 0xCB,
        gamepad_dpad_down = 0xCC,
        gamepad_dpad_left = 0xCD,
        gamepad_dpad_right = 0xCE,
        gamepad_menu = 0xCF,
        gamepad_view = 0xD0,
        gamepad_left_thumbstick_button = 0xD1,
        gamepad_right_thumbstick_button = 0xD2,
        gamepad_left_thumbstick_up = 0xD3,
        gamepad_left_thumbstick_down = 0xD4,
        gamepad_left_thumbstick_right = 0xD5,
        gamepad_left_thumbstick_left = 0xD6,
        gamepad_right_thumbstick_up = 0xD7,
        gamepad_right_thumbstick_down = 0xD8,
        gamepad_right_thumbstick_right = 0xD9,
        gamepad_right_thumbstick_left = 0xDA,
        oem_4 = 0xDB,
        oem_5 = 0xDC,
        oem_6 = 0xDD,
        oem_7 = 0xDE,
        oem_8 = 0xDF,
        oem_102 = 0xE2,
        process_key = 0xE5,
        packet = 0xE7,
        attn = 0xF6,
        crsel = 0xF7,
        exsel = 0xF8,
        erase_eof = 0xF9,
        play = 0xFA,
        zoom = 0xFB,
        noname = 0xFC,
        pa1 = 0xFD,
        oem_clear = 0xFE,

        digit_0 = 0x30,
        digit_1 = 0x31,
        digit_2 = 0x32,
        digit_3 = 0x33,
        digit_4 = 0x34,
        digit_5 = 0x35,
        digit_6 = 0x36,
        digit_7 = 0x37,
        digit_8 = 0x38,
        digit_9 = 0x39,
        a = 0x41,
        b = 0x42,
        c = 0x43,
        d = 0x44,
        e = 0x45,
        f = 0x46,
        g = 0x47,
        h = 0x48,
        i = 0x49,
        j = 0x4A,
        k = 0x4B,
        l = 0x4C,
        m = 0x4D,
        n = 0x4E,
        o = 0x4F,
        p = 0x50,
        q = 0x51,
        r = 0x52,
        s = 0x53,
        t = 0x54,
        u = 0x55,
        v = 0x56,
        w = 0x57,
        x = 0x58,
        y = 0x59,
        z = 0x5A,
    };

    struct key_name {
        std::array<char, 64> characters{};
        std::uint8_t size{};

        constexpr operator std::string_view() const { return {characters.data(), size}; }
    };

    constexpr key_name format_key_name(std::string_view name) {
        key_name result;
        bool word_start = true;

        for (const char c: name) {
            if (c == '_') {
                result.characters[result.size++] = ' ';
                word_start = true;
            } else {
                char formatted = c;

                if (word_start && c >= 'a' && c <= 'z') {
                    formatted = static_cast<char>(c - ('a' - 'A'));
                } else if (!word_start && c >= 'A' && c <= 'Z') {
                    formatted = static_cast<char>(c + ('a' - 'A'));
                }

                result.characters[result.size++] = formatted;
                word_start = false;
            }
        }

        return result;
    }

    // i kinda like this style of docs compared to c# its kinda nicer in a way

    /**
     * Returns key string for given key value
     * @remarks every string for keys are computed at compile time
     * @param k The key to convert to a string
     * @return A string_view representing the key name
     */
    std::string_view key_to_string(key k);

    std::map<key, std::string_view> get_all_keys();
} // namespace keyboard
