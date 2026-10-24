//
// Created by emi on 10/23/26.
//

#pragma once
#include <atomic>
#include <bitset>
#include <cstdint>
#include <mutex>
#include <vector>

#include <d3d11.h>
#include <d3d11on12.h>
#include <d3d12.h>
#include <dxgi1_4.h>

#include <winrt/base.h>
#include <system/generic_system.hpp>
#include <kiero.hpp>

class event_system;
class moused_event;
class keyboard_event;
class char_event;

class render_system : public generic_system {
public:
    struct in_flight_guard {
        in_flight_guard() { s_in_flight.fetch_add(1); }
        ~in_flight_guard() { s_in_flight.fetch_sub(1); }
    };

    struct buffer_record {
        winrt::com_ptr<ID3D12Resource> native;
        winrt::com_ptr<ID3D11Resource> wrapped;
        winrt::com_ptr<ID3D11RenderTargetView> rtv;
    };

private:
    static inline std::atomic<render_system *> s_active{nullptr};
    static inline std::atomic<int> s_in_flight{0};

    kiero::RenderType m_mode{kiero::RenderType::None};
    winrt::com_ptr<ID3D11Device> m_dev11;
    winrt::com_ptr<ID3D11DeviceContext> m_ctx11;
    winrt::com_ptr<ID3D11On12Device> m_dev11on12;
    std::vector<buffer_record> m_buffers;
    bool m_initialized = false;
    bool m_imgui_ready = false;
    bool m_failed = false;
    std::recursive_mutex m_frame_mutex;

    std::atomic<ID3D12CommandQueue *> m_queue{nullptr};
    std::atomic<bool> m_shutdown_requested{false};
    std::atomic<bool> m_cleanup_done{false};

    enum class input_kind : std::uint8_t { mouse_button, mouse_wheel, key, text };

    struct input_item {
        input_kind kind;
        int value;
        bool down;
    };

    std::mutex m_input_mutex;
    std::vector<input_item> m_input_queue;
    std::vector<input_item> m_input_drain;
    std::bitset<256> m_imgui_keys;
    std::bitset<3> m_imgui_buttons;
    std::atomic<bool> m_imgui_live{false};
    std::atomic<bool> m_want_mouse{false};
    std::atomic<bool> m_want_keyboard{false};
    std::atomic<bool> m_want_text{false};

    void flush_input();

    void publish_capture_state();

    void reset_input_state();

    void handle_mouse(moused_event &e);

    void handle_key(keyboard_event &e);

    void handle_char(char_event &e);

    HWND m_hwnd{nullptr};
    event_system *m_events{nullptr};

    bool m_kiero_ready{false};
    bool m_kiero_failure_logged{false};
    ULONGLONG m_kiero_next_try{0};
    kiero::RenderType m_kiero_type{kiero::RenderType::None};

    bool try_init(IDXGISwapChain *swap_chain);

    bool fail(const char *what, HRESULT hr);

    void run_frame(IDXGISwapChain *swap_chain);

    bool ensure_buffers(IDXGISwapChain *swap_chain);

    void release_buffers();

    void cleanup();

    static void wait_for_detours();

public:
    static render_system *active() { return s_active.load(); }

    static void on_mouse_event(moused_event &e);

    static void on_keyboard_event(keyboard_event &e);

    static void on_char_event(char_event &e);

    bool ensure_kiero();

    [[nodiscard]] bool kiero_ready() const { return m_kiero_ready; }
    [[nodiscard]] kiero::RenderType render_type() const { return m_kiero_type; }

    template<auto Method>
    uintptr_t method_address() {
        return m_kiero_ready ? reinterpret_cast<uintptr_t>(kiero::getMethod<Method>()) : 0;
    }

    void on_present(IDXGISwapChain *swap_chain);

    void set_queue(ID3D12CommandQueue *queue);

    void on_resize_begin();

    void initialize() override;

    void shutdown() override;
};
