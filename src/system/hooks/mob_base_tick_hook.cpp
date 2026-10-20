//
// Created by emi on 10/17/26.
//

#include "mob_base_tick_hook.hpp"

#include "kiero/detail/magic_vtable.hpp"
#include "events/base_tick_event.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "system/signatures/basic_signature_system.hpp"

std::uintptr_t mob_base_tick_hook::resolve_target() {
    const ClientInstance* ci = core::get_client_instance();
    auto* lp = ci ? ci->getLocalPlayer() : nullptr;
    if (!lp) return 0;
    return util::memory::get_virtual_func(lp, kiero::detail::magic_vft::vtable_index<&Mob::baseTick>());
}

void mob_base_tick_hook::on_initialize() {
    m_events = manager->get<event_system>();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void mob_base_tick_hook::detour(Mob* self) {
    // By the time this gets evaluated ci should be resolved
    static const ClientInstance* ci = core::get_client_instance();
    if (self != ci->getLocalPlayer()) {
        original(self);
        return;
    }

    base_tick_event e(self);
    m_events->dispatcher().post(e);

    original(self);
}
