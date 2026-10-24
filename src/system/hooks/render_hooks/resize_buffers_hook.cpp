//
// Created by emi on 10/23/26.
//

#include "resize_buffers_hook.hpp"
#include <system/render/render_system.hpp>

bool resize_buffers_hook::applicable() {
    auto rs = manager->get<render_system>();
    if (rs == nullptr || !rs->ensure_kiero()) return true;

    return rs->render_type() == kiero::RenderType::D3D12;
}

uintptr_t resize_buffers_hook::resolve_target() {
    auto balls = manager->get<render_system>();
    if (balls == nullptr || !balls->ensure_kiero()) return 0;
    return balls->method_address<&IDXGISwapChain::ResizeBuffers>();
}

HRESULT resize_buffers_hook::detour(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height,
    DXGI_FORMAT format, UINT flags) {
    render_system::in_flight_guard guard;
    auto rs = render_system::active();
    if (rs != nullptr) rs->on_resize_begin();
    return original(swap_chain, buffer_count, width, height, format, flags);
}

bool resize_buffers1_hook::applicable() {
    auto rs = manager->get<render_system>();
    if (rs == nullptr || !rs->ensure_kiero()) return true;

    return rs->render_type() == kiero::RenderType::D3D12;
}

uintptr_t resize_buffers1_hook::resolve_target() {
    auto balls = manager->get<render_system>();
    if (balls == nullptr || !balls->ensure_kiero()) return 0;
    return balls->method_address<&IDXGISwapChain3::ResizeBuffers1>();
}

HRESULT resize_buffers1_hook::detour(IDXGISwapChain3 *swap_chain, UINT buffer_count, UINT width, UINT height,
    DXGI_FORMAT format, UINT flags, const UINT *pCreationNodeMask, IUnknown* const* ppPresentQueue) {
    render_system::in_flight_guard guard;
    auto rs = render_system::active();
    if (rs != nullptr) rs->on_resize_begin();
    return original(swap_chain, buffer_count, width, height, format, flags, pCreationNodeMask, ppPresentQueue);
}
