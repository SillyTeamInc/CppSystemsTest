//
// Created by emi on 10/23/26.
//

#include "present_hook.hpp"
#include <system/render/render_system.hpp>
#include <system/generic_system.hpp>


uintptr_t present_hook::resolve_target() {
    // why is clangd bitching about this it compiles perfectly fine
    auto balls = manager->get<render_system>();
    if (balls == nullptr || !balls->ensure_kiero()) return 0;
    return balls->method_address<&IDXGISwapChain::Present>();
}

HRESULT present_hook::detour(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags) {
    render_system::in_flight_guard guard;
    auto rs = render_system::active();
    if (rs != nullptr) rs->on_present(swap_chain);

    return original(swap_chain, sync_interval, flags);
}
