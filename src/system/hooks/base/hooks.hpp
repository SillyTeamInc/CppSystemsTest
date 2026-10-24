//
// Created by emi on 10/12/2026.
//
#pragma once

#include "system/hooks/mob_base_tick_hook.hpp"
#include "system/hooks/base/hook_system.hpp"
#include "system/hooks/mouse_hook.hpp"
#include "system/hooks/render_hooks/present_hook.hpp"
#include "system/hooks/render_hooks/execute_command_lists_hook.hpp"
#include "system/hooks/render_hooks/resize_buffers_hook.hpp"

// insert da hooks here !!!
using hook_system = basic_hook_system<
    mouse_hook,
    mob_base_tick_hook,
    present_hook,
    execute_command_lists_hook,
    resize_buffers_hook,
    resize_buffers1_hook
>;
