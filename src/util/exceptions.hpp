//
// Created by emi on 10/12/2026.
//
#pragma once

namespace util::exceptions {
    static void throw_error(const std::string& message) {
        spdlog::error(message);
        throw std::runtime_error(message);
    }
}