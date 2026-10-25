//
// Created by emi on 10/12/2026.
//
#pragma once
#include <exception>

#include "spdlog/spdlog.h"



namespace util::exceptions {
    void attach_handler();
    void remove_handler();
    void throw_error(const std::string& message);
} // namespace util::exceptions
