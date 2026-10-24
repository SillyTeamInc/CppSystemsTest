//
// Created by emi on 10/24/26.
//

#pragma once

#include <embedded/font.hpp>
#include <imgui.h>

using font = embedded::font;

namespace fonts {
    constexpr float default_size = 20.0f;

    void load_embedded();

    void clear();

    [[nodiscard]] ImFont* get(font id);

    void set_default(font id);

    class scope {
    public:
        explicit scope(font id, float size = 0.0f) { ImGui::PushFont(fonts::get(id), size); }

        ~scope() { ImGui::PopFont(); }

        scope(const scope&) = delete;
        scope& operator=(const scope&) = delete;
    };
} // namespace fonts
