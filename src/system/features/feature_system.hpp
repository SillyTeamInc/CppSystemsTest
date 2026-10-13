//
// Created by emi on 10/9/26.
//
#pragma once
#include "module_system.hpp"
#include "system/complex_system.hpp"
#include "system/generic_system.hpp"

class feature_system : public complex_system {
public:
    // in complex_systems, the complex_system base class overrides generic_system's
    // initialize and shutdown functions to manage the subsystems cleanly
    // this means all you need to do here is add the subsystem, and then it will be initialized.
    // keep in mind that this means the subsystems will be
    // a) shutdown in reverse order of init and b) the subsystems won't even be added let-alone initialized until
    // the feature system is initialized. this should never be an issue and if it is, it's a bug and you've done something stupidly wrong
    void initialize_subsystems() override {
        spdlog::info("Initializing feature system subsystems");
        // subsystems index into the main manager.
        // this means you can do smth like this:
        //   auto* moduleSys = manager->get<system_of_a_down>();
        // directly from the system_manager and it will return the subsystem.
        // this is because subsystems get attached to the main systems_manager when they are added to a complex_system.
        // (although for things like modules, you should probably make a private member function that returns the module_system* by doing that to save code)
        add_subsystem<module_system>();
    }
    void shutdown_subsystems() override {
        // Systems are removed after this call by the caller in the complex_system base class
        // so no action here is usually needed
        spdlog::info("Shutting down feature system subsystems");
    }
};
