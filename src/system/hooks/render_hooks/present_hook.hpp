//
// Created by emi on 10/23/26.
//

#pragma once
#include "system/hooks/base/hook.hpp"
#include <dxgi.h>


class present_hook : public hook<present_hook, HRESULT(IDXGISwapChain*, UINT, UINT)> {
public:
    present_hook() : hook("IDXGISwapChain::Present") {}

    HOOK_RETRIES_UNTIL_RESOLVED

    uintptr_t resolve_target() override;
    HRESULT detour(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
};
