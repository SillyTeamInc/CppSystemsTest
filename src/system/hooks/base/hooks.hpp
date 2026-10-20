//
// Created by emi on 10/12/2026.
//
#pragma once

#include "system/hooks/mob_base_tick_hook.hpp"
#include "system/hooks/base/hook_system.hpp"
#include "system/hooks/mouse_hook.hpp"

// insert da hooks here !!!
using hook_system = basic_hook_system<
    mouse_hook,
    mob_base_tick_hook
>;
