//
// Created by emi on 10/12/2026.
//

#include "system/hooks/mouse_hook.hpp"

#include <kiero/detail/magic_vtable.hpp>
#include <winrt/base.h>

#include "system/event_system.hpp"
#include "events/moused_event.hpp"
#include "system/signatures/basic_signature_system.hpp"


std::uintptr_t mouse_hook::resolve_target() {
    return manager->get<signature_system>()->sigs().MouseDevice.feed;
}

void mouse_hook::on_initialize() {
    m_events = manager->get<event_system>();
}

void mouse_hook::detour(void* self, std::uint8_t actionButtonId, int buttonData, short x, short y, short dx, short dy,
                        std::uint8_t forceMotionlessPointer) {
    moused_event event(self, actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);
    m_events->dispatcher().post(event);

    if (event.is_canceled()) {
        return;
    }

    original(self, actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);
}
