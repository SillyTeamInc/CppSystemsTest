//
// Created by emi on 10/9/26.
//
#pragma once

#include <system/generic_system.hpp>
#include <spdlog/spdlog.h> // For some reason the pch doesn't get included with intellisense or whatever very epic
#include "module.hpp"

class mod_tick_event;
class keyboard_event;

class module_system : public generic_system {
public:
    std::vector<std::unique_ptr<module>> modules;

    template<std::derived_from<module> T>
    [[nodiscard]] T* get() const
    {
        const auto& typeID = util::hash::type_id<T>();
        for (const auto& module : modules)
        {
            if (module->get_type_id() == typeID)
            {
                return static_cast<T*>(module.get());
            }
        }
        return nullptr;
    }

    template<std::derived_from<module> M>
    void add() {
        // making it unique this time as this is the only owner
        modules.push_back(std::make_unique<M>());
        spdlog::trace("{}::add()", modules.back()->get_name());
    }

    [[nodiscard]] hvs::event_dispatcher& dispatcher() const;
    void initialize() override;
    void shutdown() override;

private:
    void on_keyboard_event(keyboard_event& event) const;
    void on_mod_tick(const mod_tick_event& event) const;
};