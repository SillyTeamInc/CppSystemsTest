//
// Created by emi on 10/9/26.
//

#pragma once

#include "spdlog/spdlog.h"

class ClientInstance;
class MinecraftGame;

namespace core {
    namespace logging {
        extern std::shared_ptr<spdlog::logger> logger;
        constexpr std::string_view k_ansi_reset = "\033[0m";
        std::string ansi_fg(int r, int g, int b);
        std::string ansi_fg(float r, float g, float b);
        std::vector<spdlog::sink_ptr> make_sinks();
        void init(spdlog::level::level_enum level = spdlog::level::info);
        void shutdown();
    }
}
