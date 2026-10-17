//
// Created by emi on 10/16/26.
//

#include "test_module.hpp"

void test_module::on_enable() {
    spdlog::info("test module enabled");
}

void test_module::on_disable() {
    spdlog::info("test module disabled");
}

void test_module::on_manager_init() {
    spdlog::info("test module initialized");
}

void test_module::on_manager_exit() {
    spdlog::info("test module exited");
}
