//
// Created by emi on 10/23/26.
//

#include "execute_command_lists_hook.hpp"
#include <d3d12.h>
#include <system/render/render_system.hpp>

bool execute_command_lists_hook::applicable() {
    auto rs = manager->get<render_system>();
    if (rs == nullptr || !rs->ensure_kiero()) return true;

    return rs->render_type() == kiero::RenderType::D3D12;
}

uintptr_t execute_command_lists_hook::resolve_target() {
    auto balls = manager->get<render_system>();
    if (balls == nullptr || !balls->ensure_kiero()) return 0;
    return balls->method_address<&ID3D12CommandQueue::ExecuteCommandLists>();
}

void execute_command_lists_hook::detour(ID3D12CommandQueue *command_queue, UINT num_command_lists,
    ID3D12CommandList * const *command_lists) {
    render_system::in_flight_guard guard;
    auto rs = render_system::active();
    if (rs != nullptr) rs->set_queue(command_queue);
    original(command_queue, num_command_lists, command_lists);
}
