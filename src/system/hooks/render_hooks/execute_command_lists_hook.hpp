//
// Created by emi on 10/23/26.
//

#pragma once
#include "system/hooks/base/hook.hpp"
#include <dxgi.h>
#include <d3d12.h>

class execute_command_lists_hook : public hook<execute_command_lists_hook, void(ID3D12CommandQueue*, UINT, ID3D12CommandList* const*)> {
public:
    execute_command_lists_hook() : hook("ID3D12CommandQueue::ExecuteCommandLists") {}

    HOOK_RETRIES_UNTIL_RESOLVED

    bool applicable() override;
    uintptr_t resolve_target() override;
    void detour(ID3D12CommandQueue* command_queue, UINT num_command_lists, ID3D12CommandList* const* command_lists);
};
