//
// Created by emi on 10/9/26.
//

#include "logging.hpp"

#include <cstdio>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/ansicolor_sink-inl.h>



#ifdef _WIN32
#include <windows.h>
#endif

namespace core::logging {
    namespace {
        std::shared_ptr<spdlog::logger> g_logger;
        FILE* g_log_file = nullptr;
        bool g_console_owned = false;

        // constexpr my beloved
        constexpr std::string_view k_ansi_reset = "\033[0m";
        constexpr const char* k_wine_log_path = "Z:\\tmp\\cppsystems.log";

        std::string ansi_fg(const int r, const int g, const int b) {
            return fmt::format("\033[38;2;{};{};{}m", r, g, b);
        }

        std::string ansi_fg(const float r, const float g, const float b) {
            return ansi_fg(static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255));
        }

        spdlog::sink_ptr make_stdout_sink() {
            return std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>(spdlog::color_mode::always);
        }

#ifdef _WIN32
        bool running_under_wine() {
            const HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
            return ntdll != nullptr && GetProcAddress(ntdll, "wine_get_version") != nullptr;
        }

        void setup_windows_console() {
#ifdef __MODE_LIBRARY
            AllocConsole();
            g_console_owned = true;
            SetConsoleTitleA("CppSystemsTest");

            HANDLE conOut = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE,
                                        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
            HANDLE conIn = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE,
                                       FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
            if (conOut != INVALID_HANDLE_VALUE) {
                SetStdHandle(STD_OUTPUT_HANDLE, conOut);
                SetStdHandle(STD_ERROR_HANDLE, conOut);
            }
            if (conIn != INVALID_HANDLE_VALUE) {
                SetStdHandle(STD_INPUT_HANDLE, conIn);
            }

            freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
            freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
            freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
#endif
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }

    // winegdk I fucking love and hate you
    void init(const spdlog::level::level_enum level) {
        std::vector<spdlog::sink_ptr> sinks;

#ifdef _WIN32
        if (running_under_wine()) {
            g_log_file = std::fopen(k_wine_log_path, "w");
            if (g_log_file != nullptr) {
                sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_sink<spdlog::details::console_mutex>>(
                    g_log_file, spdlog::color_mode::always));
            }
        } else {
            setup_windows_console();
            sinks.push_back(make_stdout_sink());
        }
#else
        sinks.push_back(make_stdout_sink());
#endif

        if (sinks.empty()) {
            sinks.push_back(make_stdout_sink());
        }

        g_logger = std::make_shared<spdlog::logger>("systems_test", sinks.begin(), sinks.end());
        g_logger->set_level(level);
        g_logger->flush_on(spdlog::level::trace);
        g_logger->set_pattern(fmt::format("[{}%H:%M:%S.%e{}] [%^%l%$] %v", ansi_fg(255, 135, 0), k_ansi_reset));

        spdlog::set_default_logger(g_logger);
    }


    void shutdown() {
        spdlog::debug("Deinitializing logging");
        spdlog::shutdown();
        g_logger.reset();

        if (g_log_file != nullptr) {
            std::fclose(g_log_file);
            g_log_file = nullptr;
        }

#ifdef _WIN32
        if (g_console_owned) {
            std::fclose(stdout);
            std::fclose(stdin);
            std::fclose(stderr);
            FreeConsole();
            g_console_owned = false;
        }
#endif
    }
}