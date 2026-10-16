//
// Created by emi on 10/9/26.
//

#pragma once

class ClientInstance;
class MinecraftGame;
namespace core {
    MinecraftGame* get_minecraft_game();
    ClientInstance* get_client_instance();
    void init_logging(spdlog::level::level_enum level = spdlog::level::info);
    void shutdown_logging();
}
