//
// Created by emi on 10/9/26.
//
#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>
#include <vector>

// Horrible eVent System
// based on DisabledMallis/ASM's NES (Nuvola Event System)
// that event system isn't actually thread safe due to a bug
// but the repo is archived now :c
namespace hvs {
    enum class priority : int {
        lowest = -2,
        low = -1,
        normal = 0,
        high = 1,
        highest = 2
    };

    enum class on_cancel {
        skip,
        receive
    };

    struct listen_options {
        priority prio = priority::normal;
        on_cancel cancel = on_cancel::skip;
        const void* owner = nullptr;
    };

    class event {};

    class cancelable_event : public event {
    public:
        void cancel() { canceled = true; }
        [[nodiscard]] bool is_canceled() const { return canceled; }

    private:
        bool canceled = false;
    };

    template<typename E>
    concept cancelable = requires(const E& e) {
        { e.is_canceled() } -> std::convertible_to<bool>;
    };

    struct listener_id {
        std::size_t type = static_cast<std::size_t>(-1);
        std::uint64_t value = 0;

        explicit operator bool() const { return value != 0; }
    };

    namespace detail {
        inline constexpr std::size_t max_event_types = 256;

        inline std::size_t next_event_type_id() {
            static std::atomic<std::size_t> counter{0};
            const auto id = counter.fetch_add(1);
            if (id >= max_event_types) {
                std::abort();
            }
            return id;
        }

        template<typename E>
        std::size_t event_type_id() {
            static const std::size_t id = next_event_type_id();
            return id;
        }

        template<auto Fn>
        struct fn_key {
            static constexpr char tag = 0;
        };
    }

    class scoped_listener;

    class event_dispatcher {
        struct entry {
            std::uint64_t id;
            const void* fn_key;
            const void* owner;
            priority prio;
            on_cancel cancel;
            std::function<void(void*)> invoke;
        };

        using entry_list = std::vector<entry>;

        struct channel {
            std::mutex mutex;
            std::shared_ptr<const entry_list> entries = std::make_shared<const entry_list>();
            std::atomic<std::size_t> count{0};
        };

        std::array<std::atomic<channel*>, detail::max_event_types> m_slots{};
        std::atomic<std::uint64_t> m_next_id{1};

        std::mutex m_queue_mutex;
        std::vector<std::function<void()>> m_queue;

        [[nodiscard]] channel* find(std::size_t type) const {
            return m_slots[type].load(std::memory_order_acquire);
        }

        channel& get_or_create(std::size_t type) {
            if (auto* existing = find(type)) {
                return *existing;
            }
            auto fresh = std::make_unique<channel>();
            channel* expected = nullptr;
            if (m_slots[type].compare_exchange_strong(expected, fresh.get(), std::memory_order_acq_rel)) {
                return *fresh.release();
            }
            return *expected;
        }

        static std::shared_ptr<const entry_list> snapshot(channel& ch) {
            std::lock_guard lock(ch.mutex);
            return ch.entries;
        }

        listener_id add(std::size_t type, entry e) {
            const auto id = m_next_id.fetch_add(1, std::memory_order_relaxed);
            e.id = id;

            auto& ch = get_or_create(type);
            std::lock_guard lock(ch.mutex);
            auto next = std::make_shared<entry_list>(*ch.entries);
            const auto pos = std::ranges::find_if(*next, [&](const entry& other) {
                return other.prio < e.prio;
            });
            next->insert(pos, std::move(e));
            ch.count.store(next->size(), std::memory_order_relaxed);
            ch.entries = std::move(next);
            return {type, id};
        }

        template<typename Pred>
        static std::size_t remove_where(channel& ch, Pred pred) {
            std::lock_guard lock(ch.mutex);
            auto next = std::make_shared<entry_list>(*ch.entries);
            const auto removed = std::erase_if(*next, pred);
            if (removed != 0) {
                ch.count.store(next->size(), std::memory_order_relaxed);
                ch.entries = std::move(next);
            }
            return removed;
        }

    public:
        event_dispatcher() = default;
        event_dispatcher(const event_dispatcher&) = delete;
        event_dispatcher& operator=(const event_dispatcher&) = delete;

        ~event_dispatcher() {
            for (auto& slot : m_slots) {
                delete slot.load(std::memory_order_relaxed);
            }
        }

        template<typename E, auto Fn, priority P = priority::normal, on_cancel C = on_cancel::skip, typename T>
            requires std::is_member_function_pointer_v<decltype(Fn)>
        listener_id listen(T* instance) {
            static_assert(std::is_invocable_v<decltype(Fn), T&, E&>, "handler must be callable with the event type");
            return add(detail::event_type_id<E>(), entry{
                .id = 0,
                .fn_key = &detail::fn_key<Fn>::tag,
                .owner = instance,
                .prio = P,
                .cancel = C,
                .invoke = [instance](void* e) { (instance->*Fn)(*static_cast<E*>(e)); }
            });
        }

        template<typename E, auto Fn, priority P = priority::normal, on_cancel C = on_cancel::skip>
            requires std::is_function_v<std::remove_pointer_t<decltype(Fn)>>
        listener_id listen() {
            static_assert(std::is_invocable_v<decltype(Fn), E&>, "handler must be callable with the event type");
            return add(detail::event_type_id<E>(), entry{
                .id = 0,
                .fn_key = &detail::fn_key<Fn>::tag,
                .owner = nullptr,
                .prio = P,
                .cancel = C,
                .invoke = [](void* e) { Fn(*static_cast<E*>(e)); }
            });
        }

        template<typename E, typename F>
        listener_id listen(F&& fn, listen_options options = {}) {
            static_assert(std::is_invocable_v<std::decay_t<F>&, E&>, "handler must be callable with the event type");
            return add(detail::event_type_id<E>(), entry{
                .id = 0,
                .fn_key = nullptr,
                .owner = options.owner,
                .prio = options.prio,
                .cancel = options.cancel,
                .invoke = [f = std::forward<F>(fn)](void* e) mutable { f(*static_cast<E*>(e)); }
            });
        }

        template<typename E, auto Fn>
        std::size_t deafen() {
            if (auto* ch = find(detail::event_type_id<E>())) {
                return remove_where(*ch, [](const entry& e) { return e.fn_key == &detail::fn_key<Fn>::tag; });
            }
            return 0;
        }

        template<typename E, auto Fn, typename T>
        std::size_t deafen(const T* instance) {
            if (auto* ch = find(detail::event_type_id<E>())) {
                return remove_where(*ch, [instance](const entry& e) {
                    return e.fn_key == &detail::fn_key<Fn>::tag && e.owner == instance;
                });
            }
            return 0;
        }

        std::size_t deafen(const listener_id& id) {
            if (!id || id.type >= detail::max_event_types) {
                return 0;
            }
            if (auto* ch = find(id.type)) {
                return remove_where(*ch, [&id](const entry& e) { return e.id == id.value; });
            }
            return 0;
        }

        std::size_t deafen_all(const void* owner) {
            std::size_t removed = 0;
            for (std::size_t type = 0; type < detail::max_event_types; ++type) {
                if (auto* ch = find(type)) {
                    removed += remove_where(*ch, [owner](const entry& e) { return e.owner == owner; });
                }
            }
            return removed;
        }

        [[nodiscard]] scoped_listener scope(listener_id id);

        template<typename E>
        [[nodiscard]] std::size_t listener_count() const {
            const auto* ch = find(detail::event_type_id<E>());
            return ch ? ch->count.load(std::memory_order_relaxed) : 0;
        }

        template<typename E>
        [[nodiscard]] bool has_listeners() const {
            return listener_count<E>() != 0;
        }

        template<typename E>
        bool post(E&& event) {
            using event_t = std::remove_cvref_t<E>;
            static_assert(!std::is_const_v<std::remove_reference_t<E>>, "events must be non-const");

            event_t& ref = event;
            if (auto* ch = find(detail::event_type_id<event_t>());
                ch && ch->count.load(std::memory_order_relaxed) != 0) {
                const auto list = snapshot(*ch);
                for (const auto& listener : *list) {
                    if constexpr (cancelable<event_t>) {
                        if (ref.is_canceled() && listener.cancel == on_cancel::skip) {
                            continue;
                        }
                    }
                    listener.invoke(&ref);
                }
            }

            if constexpr (cancelable<event_t>) {
                return !ref.is_canceled();
            } else {
                return true;
            }
        }

        template<typename E>
        void queue(E event) {
            static_assert(std::is_copy_constructible_v<E>, "queued events must be copyable");
            std::lock_guard lock(m_queue_mutex);
            m_queue.push_back([this, event = std::move(event)]() mutable { post(event); });
        }

        std::size_t process_queue() {
            std::vector<std::function<void()>> pending;
            {
                std::lock_guard lock(m_queue_mutex);
                pending.swap(m_queue);
            }
            for (auto& dispatch : pending) {
                dispatch();
            }
            return pending.size();
        }
    };

    class scoped_listener {
        event_dispatcher* m_dispatcher = nullptr;
        listener_id m_id;

    public:
        scoped_listener() = default;
        scoped_listener(event_dispatcher& dispatcher, listener_id id) : m_dispatcher(&dispatcher), m_id(id) {}

        scoped_listener(scoped_listener&& other) noexcept
            : m_dispatcher(std::exchange(other.m_dispatcher, nullptr)), m_id(other.m_id) {}

        scoped_listener& operator=(scoped_listener&& other) noexcept {
            if (this != &other) {
                reset();
                m_dispatcher = std::exchange(other.m_dispatcher, nullptr);
                m_id = other.m_id;
            }
            return *this;
        }

        scoped_listener(const scoped_listener&) = delete;
        scoped_listener& operator=(const scoped_listener&) = delete;

        ~scoped_listener() { reset(); }

        void reset() {
            if (m_dispatcher) {
                m_dispatcher->deafen(m_id);
                m_dispatcher = nullptr;
            }
        }
    };

    inline scoped_listener event_dispatcher::scope(listener_id id) {
        return scoped_listener(*this, id);
    }
}
