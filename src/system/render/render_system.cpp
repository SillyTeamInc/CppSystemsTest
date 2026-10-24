//
// Created by emi on 10/23/26.
//

#include "render_system.hpp"

#include <chrono>
#include <thread>

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <imgui.h>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>
#include "kiero.hpp"

#include "events/char_event.hpp"
#include "events/keyboard_event.hpp"
#include "events/moused_event.hpp"
#include "events/render_event.hpp"
#include "system/core_system.hpp"
#include "system/event_system.hpp"
#include "system/systems_manager.hpp"
#include "util/fonts.hpp"
#include "util/keyboard.hpp"

using namespace std::chrono_literals;

namespace {
    constexpr std::uint8_t mouse_move = 0;
    constexpr std::uint8_t mouse_left = 1;
    constexpr std::uint8_t mouse_right = 2;
    constexpr std::uint8_t mouse_middle = 3;
    constexpr std::uint8_t mouse_wheel = 4;
} // namespace

bool render_system::ensure_kiero() {
    if (m_kiero_ready)
        return true;
    if (GetTickCount64() < m_kiero_next_try)
        return false;

    bool has_dxgi = GetModuleHandleA("dxgi.dll") != nullptr;
    bool has_d3d12 = GetModuleHandleA("d3d12.dll") != nullptr;
    bool has_d3d11 = GetModuleHandleA("d3d11.dll") != nullptr;
    if (!has_dxgi || !(has_d3d11 || has_d3d12))
        return false;

    auto type = has_d3d12 ? kiero::RenderType::D3D12 : kiero::RenderType::D3D11;
    auto status = kiero::init(type);
    if (status != kiero::Status::Success) {
        if (!m_kiero_failure_logged) {
            spdlog::error("kiero failed to initialize: {}", std::string(magic_enum::enum_name(status)));
            m_kiero_failure_logged = true;
        }
        m_kiero_next_try = GetTickCount64() + 1000;
        return false;
    }

    m_kiero_type = type;
    return m_kiero_ready = true;
}

void render_system::on_present(IDXGISwapChain* swap_chain) {
    std::lock_guard lock(m_frame_mutex);

    try {
        if (m_shutdown_requested.load()) {
            cleanup();
            m_cleanup_done = true;
            return;
        }
        if (m_failed)
            return;

        if (!m_initialized) {
            if (!try_init(swap_chain))
                return;
        }

        run_frame(swap_chain);
    } catch (const std::exception& e) {
        spdlog::error("[render_system] exception in present: {}", e.what());
        m_failed = true;
    } catch (...) {
        spdlog::error("[render_system] unknown exception in present");
        m_failed = true;
    }
}

void render_system::set_queue(ID3D12CommandQueue* queue) {
    if (m_shutdown_requested.load())
        return;
    if (m_queue.load() != nullptr)
        return;

    D3D12_COMMAND_QUEUE_DESC desc = queue->GetDesc();
    if (desc.Type != D3D12_COMMAND_LIST_TYPE_DIRECT)
        return;

    queue->AddRef();
    ID3D12CommandQueue* expected = nullptr;
    if (!m_queue.compare_exchange_strong(expected, queue)) {
        queue->Release();
        return;
    }
    spdlog::info("[render_system] got command queue {:p}", static_cast<void*>(queue));
}

bool render_system::try_init(IDXGISwapChain* swap_chain) {
    winrt::com_ptr<ID3D12Device> d3d12_device;
    HRESULT hr;

    if (SUCCEEDED(swap_chain->GetDevice(__uuidof(ID3D12Device), d3d12_device.put_void()))) {
        m_mode = kiero::RenderType::D3D12;
        auto queue = m_queue.load();
        if (queue == nullptr)
            return false; // retry next frame because we ddont have a queue yet

        UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED;
        IUnknown* command_queues[] = {queue};
        hr = D3D11On12CreateDevice(d3d12_device.get(), flags, nullptr, 0, command_queues, 1, 0, m_dev11.put(),
                                   m_ctx11.put(), nullptr);
        if (FAILED(hr))
            return fail("D3D11On12CreateDevice", hr);

        hr = m_dev11->QueryInterface(__uuidof(ID3D11On12Device), m_dev11on12.put_void());
        if (FAILED(hr))
            return fail("QueryInterface(ID3D11On12Device)", hr);
    } else {
        hr = swap_chain->GetDevice(__uuidof(ID3D11Device), m_dev11.put_void());
        if (FAILED(hr))
            return fail("GetDevice", hr);

        m_dev11->GetImmediateContext(m_ctx11.put());
        m_mode = kiero::RenderType::D3D11;
    }

    if (m_hwnd == nullptr)
        return fail("window handle", E_POINTER);

    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    m_imgui_ready = true;
    fonts::load_embedded();
    fonts::set_default(font::product_sans);

    if (!ImGui_ImplWin32_Init(m_hwnd))
        return fail("ImGui_ImplWin32_Init", E_FAIL);
    if (!ImGui_ImplDX11_Init(m_dev11.get(), m_ctx11.get())) {
        ImGui_ImplWin32_Shutdown();
        return fail("ImGui_ImplDX11_Init", E_FAIL);
    }

    m_initialized = true;
    m_imgui_live = true;
    spdlog::info("[render_system] initialized with mode = {}", std::string(magic_enum::enum_name(m_mode)));
    return true;
}

bool render_system::fail(const char* what, HRESULT hr) {
    spdlog::error("[render_system] {} failed: 0x{:X}", what, static_cast<unsigned long>(hr));
    m_failed = true;
    return false;
}

void render_system::run_frame(IDXGISwapChain* swap_chain) {
    ID3D11RenderTargetView* rtv = nullptr;
    ID3D11Resource* wrapped = nullptr;
    winrt::com_ptr<ID3D11RenderTargetView> d3d11_rtv;
    HRESULT hr;

    if (m_mode == kiero::RenderType::D3D12) {
        if (!ensure_buffers(swap_chain))
            return;

        winrt::com_ptr<IDXGISwapChain3> swap_chain3;
        hr = swap_chain->QueryInterface(__uuidof(IDXGISwapChain3), swap_chain3.put_void());
        if (FAILED(hr)) {
            fail("QueryInterface(IDXGISwapChain3)", hr);
            return;
        }

        const UINT index = swap_chain3->GetCurrentBackBufferIndex();
        if (index >= m_buffers.size())
            return;

        rtv = m_buffers[index].rtv.get();
        wrapped = m_buffers[index].wrapped.get();
    } else {
        winrt::com_ptr<ID3D11Texture2D> back_buffer;
        hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), back_buffer.put_void());
        if (FAILED(hr)) {
            fail("GetBuffer", hr);
            return;
        }

        hr = m_dev11->CreateRenderTargetView(back_buffer.get(), nullptr, d3d11_rtv.put());
        if (FAILED(hr)) {
            fail("CreateRenderTargetView", hr);
            return;
        }
        rtv = d3d11_rtv.get();
    }

    flush_input();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    publish_capture_state();

    if (m_events != nullptr) {
        m_events->dispatcher().post(render_event{});
    }

    ImGui::Render();

    if (wrapped != nullptr) {
        m_dev11on12->AcquireWrappedResources(&wrapped, 1);
    }

    ID3D11RenderTargetView* targets[] = {rtv};
    m_ctx11->OMSetRenderTargets(1, targets, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    if (wrapped != nullptr) {
        m_ctx11->OMSetRenderTargets(0, nullptr, nullptr);
        m_dev11on12->ReleaseWrappedResources(&wrapped, 1);
        m_ctx11->Flush();
    }
}

bool render_system::ensure_buffers(IDXGISwapChain* swap_chain) {
    winrt::com_ptr<IDXGISwapChain3> swap_chain3;
    HRESULT hr = swap_chain->QueryInterface(__uuidof(IDXGISwapChain3), swap_chain3.put_void());
    if (FAILED(hr)) {
        return fail("QueryInterface(IDXGISwapChain3)", hr);
    }

    DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
    hr = swap_chain->GetDesc(&swap_chain_desc);
    if (FAILED(hr)) {
        return fail("GetDesc", hr);
    }

    const auto buffer_count = swap_chain_desc.BufferCount;
    const auto current_index = swap_chain3->GetCurrentBackBufferIndex();

    if (m_buffers.size() == buffer_count && current_index < m_buffers.size()) {
        winrt::com_ptr<ID3D12Resource> current;
        hr = swap_chain->GetBuffer(current_index, __uuidof(ID3D12Resource), current.put_void());
        if (SUCCEEDED(hr) && current.get() == m_buffers[current_index].native.get()) {
            return true;
        }
    }

    release_buffers();

    for (UINT i = 0; i < buffer_count; ++i) {
        buffer_record record;

        hr = swap_chain->GetBuffer(i, __uuidof(ID3D12Resource), record.native.put_void());
        if (FAILED(hr)) {
            release_buffers();
            return fail("GetBuffer", hr);
        }

        D3D11_RESOURCE_FLAGS flags{};
        flags.BindFlags = D3D11_BIND_RENDER_TARGET;
        hr = m_dev11on12->CreateWrappedResource(record.native.get(), &flags, D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                D3D12_RESOURCE_STATE_PRESENT, __uuidof(ID3D11Resource),
                                                record.wrapped.put_void());
        if (FAILED(hr)) {
            release_buffers();
            return fail("CreateWrappedResource", hr);
        }

        hr = m_dev11->CreateRenderTargetView(record.wrapped.get(), nullptr, record.rtv.put());
        if (FAILED(hr)) {
            release_buffers();
            return fail("CreateRenderTargetView", hr);
        }

        m_buffers.push_back(std::move(record));
    }

    return true;
}

void render_system::release_buffers() { m_buffers.clear(); }

void render_system::on_resize_begin() {
    std::lock_guard lock(m_frame_mutex);
    if (!m_initialized || m_mode != kiero::RenderType::D3D12)
        return;

    m_ctx11->ClearState();
    release_buffers();
    m_ctx11->Flush();
}

void render_system::cleanup() {
    // safe to call more than once
    m_imgui_live = false;
    reset_input_state();
    if (m_ctx11) {
        m_ctx11->ClearState();
        m_ctx11->Flush();
    }
    release_buffers();
    if (m_ctx11)
        m_ctx11->Flush();

    if (m_initialized) {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
    }
    if (m_imgui_ready) {
        fonts::clear();
        ImGui::DestroyContext();
        m_imgui_ready = false;
    }

    m_dev11on12 = nullptr;
    m_ctx11 = nullptr;
    m_dev11 = nullptr;
    m_initialized = false;
}

void render_system::initialize() {
    m_events = manager->get<event_system>();
    if (auto* core = manager->get<core_system>()) {
        m_hwnd = core->m_window_handle;
    }

    s_active.store(this);

    if (m_events != nullptr) {
        auto& dispatcher = m_events->dispatcher();
        dispatcher.listen<moused_event, &render_system::on_mouse_event, hvs::priority::highest>();
        dispatcher.listen<keyboard_event, &render_system::on_keyboard_event, hvs::priority::highest>();
        dispatcher.listen<char_event, &render_system::on_char_event, hvs::priority::highest>();
    }

    ensure_kiero();
}

void render_system::shutdown() {
    if (m_events != nullptr) {
        auto& dispatcher = m_events->dispatcher();
        dispatcher.deafen<moused_event, &render_system::on_mouse_event>();
        dispatcher.deafen<keyboard_event, &render_system::on_keyboard_event>();
        dispatcher.deafen<char_event, &render_system::on_char_event>();
    }

    bool needs_cleanup;
    {
        std::lock_guard lock(m_frame_mutex);
        m_shutdown_requested = true;
        needs_cleanup = m_initialized || m_imgui_ready || m_dev11 || m_ctx11;
    }

    if (needs_cleanup) {
        const auto deadline = std::chrono::steady_clock::now() + 2s;
        while (!m_cleanup_done.load() && std::chrono::steady_clock::now() < deadline) {
            std::this_thread::sleep_for(1ms);
        }

        if (!m_cleanup_done.load()) {
            spdlog::warn("[render_system] present was not called, cleaning up from the unload thread!!!");
            std::lock_guard lock(m_frame_mutex);
            cleanup();
            m_cleanup_done = true;
        }
    }

    s_active.store(nullptr);
    wait_for_detours();

    if (auto* queue = m_queue.exchange(nullptr)) {
        queue->Release();
    }

    // hooks never call kiero so should be safe
    kiero::shutdown();
    m_kiero_ready = false;
}

void render_system::wait_for_detours() {
    const auto deadline = std::chrono::steady_clock::now() + 2s;
    while (s_in_flight.load() != 0 && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(1ms);
    }
}

void render_system::on_mouse_event(moused_event& e) {
    in_flight_guard guard;
    if (auto* rs = active())
        rs->handle_mouse(e);
}

void render_system::on_keyboard_event(keyboard_event& e) {
    in_flight_guard guard;
    if (auto* rs = active())
        rs->handle_key(e);
}

void render_system::on_char_event(char_event& e) {
    in_flight_guard guard;
    if (auto* rs = active())
        rs->handle_char(e);
}

void render_system::handle_mouse(moused_event& e) {
    if (!m_imgui_live.load())
        return;
    const bool want = m_want_mouse.load();

    std::lock_guard lock(m_input_mutex);

    switch (e.action_button_id) {
        case mouse_move:
            if (want)
                e.cancel();
            return;

        case mouse_left:
        case mouse_right:
        case mouse_middle: {
            const int button = e.action_button_id - mouse_left;
            const bool down = e.button_data != 0;
            m_input_queue.push_back({input_kind::mouse_button, button, down});

            if (down && want)
                m_imgui_buttons.set(button);
            if (m_imgui_buttons.test(button)) {
                e.cancel();
                if (!down)
                    m_imgui_buttons.reset(button);
            }
            return;
        }

        case mouse_wheel:
            m_input_queue.push_back({input_kind::mouse_wheel, e.button_data > 0 ? 1 : -1, false});
            if (want)
                e.cancel();
            return;

        default:
            return;
    }
}

void render_system::handle_key(keyboard_event& e) {
    if (!m_imgui_live.load())
        return;
    const bool want = m_want_keyboard.load();
    const std::size_t vk = e.key_code & 0xFF;

    std::lock_guard lock(m_input_mutex);

    const ImGuiKey key = keyboard::vk_to_imgui_key(e.key_code, e.scan_code, e.is_extended_key);
    if (key != ImGuiKey_None && !e.is_repeated()) {
        m_input_queue.push_back({input_kind::key, static_cast<int>(key), e.is_down});
    }

    if (e.is_down) {
        if (!e.is_repeated() && want)
            m_imgui_keys.set(vk);
        if (m_imgui_keys.test(vk))
            e.cancel();
    } else if (m_imgui_keys.test(vk)) {
        m_imgui_keys.reset(vk);
        e.cancel();
    }
}

void render_system::handle_char(char_event& e) {
    if (!m_imgui_live.load() || !m_want_text.load())
        return;

    std::lock_guard lock(m_input_mutex);
    m_input_queue.push_back({input_kind::text, static_cast<int>(e.character), true});
    e.cancel();
}

void render_system::flush_input() {
    {
        std::lock_guard lock(m_input_mutex);
        m_input_drain.clear();
        m_input_drain.swap(m_input_queue);
    }

    ImGuiIO& io = ImGui::GetIO();
    for (const input_item& item: m_input_drain) {
        switch (item.kind) {
            case input_kind::mouse_button:
                io.AddMouseButtonEvent(item.value, item.down);
                break;
            case input_kind::mouse_wheel:
                io.AddMouseWheelEvent(0.0f, static_cast<float>(item.value));
                break;
            case input_kind::key:
                io.AddKeyEvent(static_cast<ImGuiKey>(item.value), item.down);
                break;
            case input_kind::text:
                io.AddInputCharacterUTF16(static_cast<ImWchar16>(item.value));
                break;
        }
    }
}

void render_system::publish_capture_state() {
    const ImGuiIO& io = ImGui::GetIO();
    m_want_mouse.store(io.WantCaptureMouse);
    m_want_keyboard.store(io.WantCaptureKeyboard);
    m_want_text.store(io.WantTextInput);
}

void render_system::reset_input_state() {
    m_want_mouse = false;
    m_want_keyboard = false;
    m_want_text = false;

    std::lock_guard lock(m_input_mutex);
    m_input_queue.clear();
    m_imgui_keys.reset();
    m_imgui_buttons.reset();
}
