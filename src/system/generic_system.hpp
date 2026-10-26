//
// Created by emi on 2/23/2026.
//

#pragma once
#include <functional>

class systems_manager;

class generic_system {
    friend class systems_manager;
    friend class complex_system;
    virtual void for_each_child(const std::function<void(generic_system&)> &) {
    }
public:
    systems_manager* manager = nullptr;
    virtual ~generic_system() = default;

    generic_system(const generic_system&) = delete;
    generic_system& operator=(const generic_system&) = delete;

    virtual void initialize() = 0;

    virtual void shutdown() = 0;

protected:
    generic_system() = default;

    [[nodiscard]] systems_manager& systems() const { return *manager; }
};
