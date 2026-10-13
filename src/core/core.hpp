//
// Created by emi on 10/9/26.
//

#pragma once

namespace core {
    void init_logging(spdlog::level::level_enum level = spdlog::level::info);
    void shutdown_logging();
}