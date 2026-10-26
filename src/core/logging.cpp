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
    std::shared_ptr<spdlog::logger> logger;
    namespace {
        FILE* g_log_file = nullptr;
        bool g_console_owned = false;

        // constexpr my beloved

        constexpr const char* k_wine_log_path = "Z:\\tmp\\cppsystems.log";

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

        constexpr std::string_view k_main_name = "systems_test";

        std::string name_color(const std::string_view name) {
            unsigned hash = 2166136261u;
            for (const unsigned char c : name) {
                hash ^= c;
                hash *= 16777619u;
            }
            hash = (hash >> 16) ^ hash;

            const float h = static_cast<float>(hash % 360) / 60.0f, s = 0.55f, v = 1.0f;
            const float c = v * s, x = c * (1.0f - std::fabs(std::fmod(h, 2.0f) - 1.0f)), m = v - c;
            float r = 0, g = 0, b = 0;
            switch (static_cast<int>(h)) {
                case 0: r = c; g = x; break;
                case 1: r = x; g = c; break;
                case 2: g = c; b = x; break;
                case 3: g = x; b = c; break;
                case 4: r = x; b = c; break;
                default: r = c; b = x; break;
            }
            return ansi_fg(r + m, g + m, b + m);
        }

        class obj_name_flag : public spdlog::custom_flag_formatter {
        public:
            void format(const spdlog::details::log_msg& msg, const std::tm&, spdlog::memory_buf_t& dest) override {
                const std::string_view name{msg.logger_name.data(), msg.logger_name.size()};
                if (name == k_main_name) return;
                fmt::format_to(std::back_inserter(dest), " [{}{}{}]", name_color(name), name, k_ansi_reset);
            }

            [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override {
                return std::make_unique<obj_name_flag>();
            }
        };
    }



    std::string ansi_fg(const int r, const int g, const int b) {
        return fmt::format("\033[38;2;{};{};{}m", r, g, b);
    }

    std::string ansi_fg(const float r, const float g, const float b) {
        return ansi_fg(static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255));
    }


    // winegdk I fucking love and hate you
    std::vector<spdlog::sink_ptr> make_sinks() {
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

        return sinks;
    }

    void init(const spdlog::level::level_enum level) {
        const auto sinks = make_sinks();
        logger = std::make_shared<spdlog::logger>(std::string(k_main_name), sinks.begin(), sinks.end());
        logger->set_level(level);
        logger->flush_on(spdlog::level::trace);

        auto formatter = std::make_unique<spdlog::pattern_formatter>();
        formatter->add_flag<obj_name_flag>('N').set_pattern(
            fmt::format("[{}%H:%M:%S.%e{}] [%^%l%$]%N %v", ansi_fg(255, 135, 0), k_ansi_reset));
        logger->set_formatter(std::move(formatter));

        spdlog::set_default_logger(logger);
    }

    std::shared_ptr<spdlog::logger> make_logger(const std::string_view name) {
        if (auto existing = spdlog::get(std::string(name))) return existing;

        auto l = logger->clone(std::string(name));
        spdlog::register_logger(l);
        return l;
    }

    void shutdown() {
        spdlog::debug("Deinitializing logging");
        spdlog::shutdown();
        logger.reset();

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