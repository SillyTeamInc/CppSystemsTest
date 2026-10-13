//
// Created by emi on 10/12/2026.
//
#pragma once

#include "util/memory.hpp"
#include <algorithm>
#include <cstddef>
#include <execution>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "libhat.hpp"
#include "sig.hpp"
#include "system/generic_system.hpp"
#include <boost/pfr.hpp>
#include <spdlog/spdlog.h>

#include "sig.hpp"
#include "system/generic_system.hpp"
#include "game_signatures.hpp"

namespace sig::detail {
    template<typename field_t>
    inline constexpr bool is_field_v = std::is_base_of_v<sig::field_tag, field_t>;

    struct leaf {
        std::string name;
        void* object;
        void (*scan)(void*, const hat::process::module&, std::string_view);
        bool (*found)(const void*);
        std::string (*describe)(const void*);
        void (*reset)(void*);
    };

    template<typename field_t>
    leaf make_leaf(std::string name, field_t& f) {
        return {
            std::move(name),
            &f,
            [](void* p, const hat::process::module& mod, std::string_view section) {
                auto& target = *static_cast<field_t*>(p);
                const auto result = hat::find_pattern(field_t::pattern(), section, mod);
                if (result.has_result()) {
                    target.resolve(result);
                }
            },
            [](const void* p) {
                return static_cast<const field_t*>(p)->found();
            },
            [](const void* p) -> std::string {
                const auto value = static_cast<const field_t*>(p)->value();
                if constexpr (std::is_same_v<typename field_t::value_type, std::uintptr_t>) {
                    return util::memory::get_memory_string(value);
                } else {
                    return std::to_string(value);
                }
            },
            [](void* p) { static_cast<field_t*>(p)->reset(); }
        };
    }

    template<typename Group>
    void collect(Group& group, const std::string& prefix, std::vector<leaf>& out);

    template<std::size_t index_t, typename Group>
    void visit(Group& group, const std::string& prefix, std::vector<leaf>& out) {
        using member_t = std::remove_cvref_t<boost::pfr::tuple_element_t<index_t, Group>>;
        auto& member = boost::pfr::get<index_t>(group);
        const std::string name = prefix + std::string(boost::pfr::get_name<index_t, Group>());

        if constexpr (is_field_v<member_t>) {
            out.push_back(make_leaf(name, member));
        } else {
            static_assert(std::is_aggregate_v<member_t>, "signature groups must be plain aggregates");
            collect(member, name + ".", out);
        }
    }

    template<typename Group>
    void collect(Group& group, const std::string& prefix, std::vector<leaf>& out) {
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            (visit<I>(group, prefix, out), ...);
        }(std::make_index_sequence<boost::pfr::tuple_size_v<Group>>{});
    }


}

template<typename Signatures>
class basic_signature_system : public generic_system {
private:
    Signatures m_sigs{};
    std::vector<sig::detail::leaf> m_leaves;
    std::string m_section;
public:
    explicit basic_signature_system(std::string section = ".text") : m_section(std::move(section)) {}

    [[nodiscard]] const Signatures& sigs() const {
        return m_sigs;
    }

    [[nodiscard]] std::size_t total_count() const {
        return m_leaves.size();
    }

    [[nodiscard]] std::size_t resolved_count() const {
        return static_cast<std::size_t>(std::ranges::count_if(m_leaves, [](const sig::detail::leaf& l) {
            return l.found(l.object);
        }));
    }

    void initialize() override {
        m_leaves.clear();
        sig::detail::collect(m_sigs, "", m_leaves);
        spdlog::info("Scanning {} signatures in {}", m_leaves.size(), m_section);

        const auto procmod = hat::process::get_process_module();
        std::for_each(std::execution::par, m_leaves.begin(), m_leaves.end(), [&](sig::detail::leaf& l) {
            l.scan(l.object, procmod, m_section);
        });

        for (const auto& l : m_leaves) {
            if (l.found(l.object)) {
                spdlog::info("Found '{}' = {}", l.name, l.describe(l.object));
            } else {
                spdlog::error("Sig {} was not found", l.name);
            }
        }

        spdlog::info("{}/{} sigs found", resolved_count(), m_leaves.size());
    }

    void shutdown() override {
        for (auto& l : m_leaves) {
            l.reset(l.object);
        }
        m_leaves.clear();
    }
};

using signature_system = basic_signature_system<game_signatures>;