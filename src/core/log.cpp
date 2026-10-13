//
// Created by emi on 10/9/26.
//

#include "log.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

namespace core {
    namespace {
        std::shared_ptr<spdlog::logger> g_logger;
        // constexpr my beloved
        constexpr std::string_view k_ansi_reset = "\033[0m";

        std::string ansi_fg(const int r, const int g, const int b) {
            return fmt::format("\033[38;2;{};{};{}m", r, g, b);
        }

        std::string ansi_fg(const float r, const float g, const float b) {
            return ansi_fg(static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255));
        }
    }

    void init_logging(const spdlog::level::level_enum level) {
    #ifdef _WIN32
    #ifdef __MODE_LIBRARY
        AllocConsole();

        SetConsoleTitle("CppSystemsTest");

        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
        freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);


    #endif
        // windows is fucking annoying
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    #endif
        const auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::always);
        g_logger = std::make_shared<spdlog::logger>("systems_test", sink);

        g_logger->set_level(level);
        g_logger->set_pattern(fmt::format("[{}%H:%M:%S.%e{}] [%^%l%$] %v", ansi_fg(255, 135, 0), k_ansi_reset));

        spdlog::set_default_logger(g_logger);
    }

    void shutdown_logging() {
        spdlog::debug("Deinitializing logging");
        g_logger.reset();
        fclose(stdout);
        fclose(stdin);
        fclose(stderr);
        FreeConsole();
    }
}