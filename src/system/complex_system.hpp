//
// Created by emi on 2/23/2026.
//

#pragma once

#include <memory>
#include <vector>
#include "generic_system.hpp"
#include "systems_manager.hpp"

class complex_system : public generic_system {
    std::vector<std::unique_ptr<generic_system>> subsystems;

    void for_each_child(const std::function<void(generic_system&)>& fn) override {
        for (auto& child: subsystems) {
            fn(*child);
        }
    }

public:
    template<std::derived_from<generic_system> T>
    T& get() {
        for (auto& subsystem: subsystems) {
            if (auto* casted = dynamic_cast<T*>(subsystem.get())) {
                return *casted;
            }
        }
        throw std::runtime_error("Subsystem not found");
    }

    virtual void initialize_subsystems() = 0;
    virtual void shutdown_subsystems() = 0;

    template<std::derived_from<generic_system> T, typename... Args>
    T& add_subsystem(Args&&... args) {
        auto owned = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *owned;
        subsystems.push_back(std::move(owned));
        if (manager) {
            manager->attach(ref);
        }
        return ref;
    }

    void initialize() override {
        initialize_subsystems();

        for (auto& child: subsystems) {
            child->initialize();
            // don't attach because add_subsystem already does that you dumbass
        }
    }


    void shutdown() override {
        shutdown_subsystems();

        while (!subsystems.empty()) {
            generic_system& child = *subsystems.back();
            child.shutdown();
            if (manager) {
                manager->detach(child);
            }
            subsystems.pop_back();
        }
    }
};
