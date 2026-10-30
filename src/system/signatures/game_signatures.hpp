//
// Created by emi on 10/11/2026.
//
#pragma once

#include <system/signatures/basic_signature_system.hpp>
#include "basic_signature_system.hpp"
#include "sdk/mc/client/renderer/game/LevelRenderer.hpp"
#include "sig.hpp"

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

    struct local_player_t {
        // i think this is the name? either this or _applyTurnDelta
        // which it wouldn't be probably i don't think the game usually uses _applyTurnDelta
        // this was found by looking at things that access mYHeadRot in ActorHeadRotationComponent
        sig::reference<"E8 ? ? ? ? 48 8B ? ? ? 48 31 ? 48 3B ? ? ? ? ? 75 ? 0F 28 ? ? ? 0F 28 ? ? ? 44 0F ? ? ? ? 44 0F ? ? ? ? 48 83 C4", 1> localPlayerTurn;
    } LocalPlayer;

    struct level_renderer_player_t {
        // i sort of stumbled upon the function i needed while looking for something else lmfao
        sig::reference<"E8 ? ? ? ? E9 ? ? ? ? 31 C0 E9 ? ? ? ? 48 8B ? ? ? ? ? 48 8B", 1> getFov;
    } LevelRendererPlayer;

    struct client_input_handler_t {
        sig::field_offset<"49 8B ? ? 48 8B ? ? 48 85 ? 0F 84 ? ? ? ? 4C 8B ? ? 48 8B ? ? 48 C1 E9", 3, std::uint8_t> mMappingFactory;
    } ClientInputHandler;

    struct client_input_mapping_factory_t {
        sig::field_offset<"48 8B ? ? ? ? ? 48 89 ? ? 48 8B ? ? ? ? ? 48 89 ? ? EB ? 31 C9", 3> mKeyboardRemappingLayout;
    } ClientInputMappingFactory;

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

