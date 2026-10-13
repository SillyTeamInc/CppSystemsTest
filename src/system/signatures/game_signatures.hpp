//
// Created by emi on 10/11/2026.
//
#pragma once

#include "sig.hpp"
// sigs should be accurate to their naming convention as well excluding the struct names
// all sigs here get scanned and don't need to be manually added anywhere else.
// this is my love letter to boost.pfr
struct game_signatures {
    struct client_instance_t {
        sig::address<"48 83 EC ? 48 8D ? ? ? ? ? 48 8D ? ? ? E8 ? ? ? ? 80 7C 24 40"> getLocalPlayer;
    } ClientInstance;

    struct mouse_device_t {
        sig::reference<"E8 ? ? ? ? E9 ? ? ? ? 48 8D ? ? ? ? ? 48 89 8D ? ? ? ? C6 85 ? ? ? ? 01 48", 1> feed;
    } MouseDevice;
};
