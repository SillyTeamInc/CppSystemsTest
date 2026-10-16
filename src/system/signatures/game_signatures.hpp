//
// Created by emi on 10/11/2026.
//
#pragma once

#include "sig.hpp"
#include <system/signatures/basic_signature_system.hpp>
#include "basic_signature_system.hpp"

// sigs should be accurate to their naming convention as well excluding the struct names
// all sigs here get scanned and don't need to be manually added anywhere else.
// this is my love letter to boost.pfr
struct game_signatures {
    struct client_instance_t {
        // This is fucking stupid generating a sig on the func directly gave me an obese ass sig
        sig::address<"31 C0 C3 CC CC CC CC 48 83 EC", 7> getLocalPlayer;

        struct v_table_t {
            sig::vtable_index<"48 8B ? ? ? ? ? 48 89 ? FF 15 ? ? ? ? 48 89 ? 48 8B ? 48 8B ? ? ? ? ? 48 89 ? FF 15 ? ? ? ? 48 85 ? 74 ? 49 89", 3> getLocalPlayer;
        } vTable;
    } ClientInstance;

    struct mouse_device_t {
        sig::reference<"E8 ? ? ? ? E9 ? ? ? ? 48 8D ? ? ? ? ? 48 89 8D ? ? ? ? C6 85 ? ? ? ? 01 48", 1> feed;
    } MouseDevice;

    struct minecraft_game_t {
        sig::field_offset<"48 8B ? ? ? ? ? 48 8B ? ? 80 7? ? ? 48 89 ? 75 ? 66 66", 3> mClientInstances;
    } MinecraftGame;

    struct bedrock_t {
        struct platform_gamecore_t {
            sig::field_offset<"48 8B ? ? 48 8B ? 48 8B ? ? ? ? ? FF 15 ? ? ? ? 48 8B ? ? 66 66", 3, std::uint8_t> mPlatform_Shim;
            sig::field_offset<"84 DB 74 ? 48 8B ? ? ? ? ? FF 15", 7> mHwnd;
            sig::field_offset<"48 8B ? ? ? ? ? 48 8B ? ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? E8", 3> mHolder;
        } Platform_GameCore;

        struct game_holder_t {
            sig::field_offset<"48 8B ? ? 48 8B ? 48 8B ? ? FF 15 ? ? ? ? E8 ? ? ? ? 48 8B", 3, std::uint8_t> mGame;
        } GameHolder;

    } Bedrock;

    struct main_game_core_t {
        struct win_main_t {
            sig::reference<"4C 8B ? ? ? ? ? 4D 85 ? 0F 95 ? 83 FA", 3> instance;
            sig::field_offset<"8B 6F ? 81 FE", 2, std::uint8_t> mPlatform;
        } WinMain; // WinMain type name is normally reserved by Windows.h so keep that in mind lol
    } MainGameCore;
};

