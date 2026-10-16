//
// Created by emi on 10/11/2026.
//
#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <libhat.hpp>

namespace sig {
    struct field_tag {};
    // shouldn't be used directly
    template<typename value_t, hat::fixed_string Sig>
    class basic : public field_tag {
        value_t m_value{};
        bool m_found = false;

    protected:
        void set(value_t value) {
            m_value = value;
            m_found = true;
        }

    public:
        using value_type = value_t;

        static hat::signature_view pattern() {
            static constexpr auto compiled = hat::compile_signature<Sig>();
            return hat::signature_view{compiled};
        }

        [[nodiscard]] bool found() const {return m_found;}
        [[nodiscard]] value_t value() const {return m_value;}
        [[nodiscard]] value_t v() const {return m_value;}
        // implicit conversion makes this so epic
        operator value_t() const { return m_value; }

        template<typename Fn> requires (std::is_pointer_v<Fn> && std::is_same_v<value_t, std::uintptr_t>)
        [[nodiscard]] Fn as() const {
            return reinterpret_cast<Fn>(m_value);
        }

        void reset() {
            m_value = value_t{};
            m_found = false;
        }
    };

    template<hat::fixed_string Sig, std::size_t Offset = 0>
    struct address : public basic<std::uintptr_t, Sig> {
        void resolve(const hat::scan_result& result) {
            this->set(reinterpret_cast<std::uintptr_t>(result.get() + Offset));
        }
    };

    template<hat::fixed_string Sig, std::size_t Offset = 0, std::size_t Remaining = 0>
    struct reference : public basic<std::uintptr_t, Sig> {
        void resolve(const hat::scan_result& result) {
            this->set(reinterpret_cast<std::uintptr_t>(result.rel(Offset, Remaining)));
        }
    };

    template<hat::fixed_string Sig, std::size_t Offset = 0>
    struct vtable_index : public basic<std::uintptr_t, Sig> {
        void resolve(const hat::scan_result& result) {
            size_t r = result.index<std::int32_t, void*>(Offset);
            this->set(r);
        }
    };

    template<hat::fixed_string Sig, std::size_t Offset = 0, std::integral TheFunny = std::int32_t>
    struct field_offset : basic<TheFunny, Sig> {
        void resolve(const hat::scan_result& result) {
            // i didn't know .read was a thing. brady you're so based
            this->set(result.read<TheFunny>(Offset));
        }
    };

    static_assert(std::is_base_of_v<field_tag, address<"00">>, "signature fields must derive from sig::field_tag");
}