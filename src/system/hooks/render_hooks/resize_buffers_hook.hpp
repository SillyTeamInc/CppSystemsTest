//
// Created by emi on 10/23/26.
//

#pragma once
#include "system/hooks/base/hook.hpp"
#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d12.h>
class resize_buffers_hook : public hook<resize_buffers_hook, HRESULT(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT)> {
public:
    resize_buffers_hook() : hook("IDXGISwapChain::ResizeBuffers") {}

    HOOK_RETRIES_UNTIL_RESOLVED

    bool applicable() override;
    uintptr_t resolve_target() override;
    HRESULT detour(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags);
};

class resize_buffers1_hook : public hook<resize_buffers1_hook, HRESULT(IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT, const UINT*, IUnknown* const*)> {
public:
    resize_buffers1_hook() : hook("IDXGISwapChain3::ResizeBuffers1") {}

    HOOK_RETRIES_UNTIL_RESOLVED

    bool applicable() override;
    uintptr_t resolve_target() override;
    HRESULT detour(IDXGISwapChain3* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT format, UINT flags, const UINT* pCreationNodeMask, IUnknown* const* ppPresentQueue);
};
