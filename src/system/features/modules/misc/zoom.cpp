//
// Created by emi on 10/26/26.
//

#include "zoom.hpp"

#include <imgui.h>

#include "events/fov_check_event.hpp"
#include "events/local_player_turn_event.hpp"
#include "events/moused_event.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/FloatOption.hpp"
#include "sdk/mc/world/actor/Actor.hpp"

void zoom::on_enable() {
    dispatcher().listen<moused_event, &zoom::on_mouse_event>(this);
    dispatcher().listen<local_player_turn_event, &zoom::on_lp_turn>(this);

    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp) {
        disable();
        return;
    }

    m_target_mult = TARGET;
}

void zoom::on_disable() {
    dispatcher().deafen<moused_event, &zoom::on_mouse_event>(this);
    dispatcher().deafen<local_player_turn_event, &zoom::on_lp_turn>(this);

    m_target_mult = 1.f;
}


void zoom::on_fov_event(fov_check_event::post& event) {
    if (!is_enabled() && m_current_mult == 1.f)
        return;
    event.result *= m_current_mult;
}

void zoom::on_mouse_event(moused_event& event) {
    if (event.action_button_id == 4) {
        constexpr float step = 0.075f;

        const auto delta = static_cast<int8_t>(event.button_data);

        if (delta != 0) {
            const float scale = std::clamp(m_target_mult * 4.f, 0.001f, 1.f);

            m_target_mult += (delta > 0 ? -step : step) * scale;
            event.cancel();
        }
    }

    m_target_mult = std::clamp(m_target_mult, 0.01f, 1.f);
}

void zoom::on_lp_turn(local_player_turn_event& event) {
    if (!is_enabled() && m_current_mult == 1.f)
        return;

    // i almost forgot i had this event i can't lie

    float chipsAndMult = std::clamp(m_current_mult, 0.05f, 1.f);

    event.deltaRots->x *= chipsAndMult;
    event.deltaRots->y *= chipsAndMult;
}

void zoom::on_render(render_event& event) {
    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp) {
        disable();
        return;
    }

    float joker = 1.f - std::exp(-12.f * ImGui::GetIO().DeltaTime);
    m_current_mult += (m_target_mult - m_current_mult) * joker;
    // floats make me angry sometimes
    if (std::abs(m_current_mult - m_target_mult) < 0.0001f)
        m_current_mult = m_target_mult;
}
bool zoom::can_be_toggled() { return core::get_client_instance()->getMouseGrabbed() || m_enabled; }

void zoom::on_manager_init() {
    dispatcher().listen<render_event, &zoom::on_render>(this);
    dispatcher().listen<fov_check_event::post, &zoom::on_fov_event>(this);
}
void zoom::on_manager_exit() {
    dispatcher().deafen<render_event, &zoom::on_render>(this);
    dispatcher().deafen<fov_check_event::post, &zoom::on_fov_event>(this);
}