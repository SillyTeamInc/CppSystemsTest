//
// Created by emi on 10/9/26.
//

#pragma once

class ClientInstance;
class MinecraftGame;

namespace core {
    namespace logging {
        void init(spdlog::level::level_enum level = spdlog::level::info);
        void shutdown();
    }
}
