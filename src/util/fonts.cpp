//
// Created by emi on 10/24/26.
//

#include "fonts.hpp"

#include <array>
#include <cstdio>

#include <spdlog/spdlog.h>

namespace {
    constexpr std::size_t font_count = static_cast<std::size_t>(font::count);

    std::array<ImFont*, font_count> loaded{};
} // namespace

void fonts::load_embedded() {
    ImGuiIO& io = ImGui::GetIO();

    ImGuiStyle& style = ImGui::GetStyle();
    if (style.FontSizeBase <= 0.0f)
        style.FontSizeBase = default_size;

    std::size_t added = 0;
    for (std::size_t i = 0; i < font_count; ++i) {
        const auto id = static_cast<font>(i);
        const embedded::blob& blob = embedded::get(id);
        const std::string_view name = embedded::name(id);

        if (blob.empty()) {
            spdlog::warn("[fonts] {} is empty?? skipping!", name);
            continue;
        }

        ImFontConfig config;

        config.FontDataOwnedByAtlas = false;
        std::snprintf(config.Name, sizeof(config.Name), "%.*s", static_cast<int>(name.size()), name.data());

        loaded[i] = io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(blob.data()),
                                                   static_cast<int>(blob.size()), 0.0f, &config);
        if (loaded[i] == nullptr) {
            spdlog::warn("[fonts] imgui could not load {}", name);
            continue;
        }
        ++added;
    }

    spdlog::info("[fonts] loaded {}/{} fonts", added, font_count);
}

void fonts::clear() { loaded.fill(nullptr); }

ImFont* fonts::get(font id) {
    const auto index = static_cast<std::size_t>(id);
    return index < font_count ? loaded[index] : nullptr;
}

void fonts::set_default(font id) {
    ImFont* target = fonts::get(id);
    if (target == nullptr) {
        spdlog::warn("[fonts] cannot make {} the default, it is not loaded", embedded::name(id));
        return;
    }
    ImGui::GetIO().FontDefault = target;
}
