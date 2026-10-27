//
// Created by emi on 10/26/26.
//

#include "zoom.hpp"

#include <imgui.h>

#include "events/fov_check_event.hpp"
#include "events/moused_event.hpp"
#include "events/local_player_turn_event.hpp"

#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/deps/minecraft_camera/components/CameraBlendStateComponent.hpp"
#include "sdk/mc/deps/minecraft_camera/components/CameraRenderFirstPersonObjectsComponent.hpp"
#include "sdk/mc/deps/vanilla_camera/VanillaCameraApiComponent.hpp"
#include "sdk/mc/entity/components/CameraComponent.hpp"
#include "sdk/mc/entity/components/CameraCustomFovComponent.hpp"

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

    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp)
        return;

    m_target_mult = 1.f;
}


void zoom::on_fov_event(fov_check_event::post& event) {
    if (!is_enabled() && m_current_mult == 1.f)
        return;
    event.result *= m_current_mult;
}

void zoom::on_mouse_event(moused_event& event) {
    if (event.action_button_id == 4)
    {
        float chipsAndMult = m_target_mult * 4.f;
        chipsAndMult = std::clamp(chipsAndMult, 0.001f, 1.f);

        if(event.button_data == 0x78 || event.button_data == 0x7F)
        {
            m_target_mult -= 0.05f * chipsAndMult;
            event.cancel();
        }
        else if (event.button_data == 0x88 || event.button_data == 0x80 || event.button_data == -0x78 || event.button_data == 0xFF80) // am i stupid?
        {
            m_target_mult += 0.05f * chipsAndMult;
            event.cancel();
        }
    }

    m_target_mult = std::clamp(m_target_mult, 0.01f, 1.f);
}

void zoom::on_lp_turn(local_player_turn_event& event) {
    if (!is_enabled() && m_current_mult == 1.f)
        return;

    // i almost forgot i had this event i can't lie

    float chipsAndMult = m_target_mult * 3.75f;
    chipsAndMult = std::clamp(chipsAndMult, 0.001f, 1.f);

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

    m_target_mult = std::clamp(m_target_mult, 0.01f, 1.f);
    float joker = ImGui::GetIO().DeltaTime;
    m_current_mult = std::lerp(m_current_mult, m_target_mult, joker * 8.f);
}

void zoom::on_manager_init() {
    dispatcher().listen<render_event, &zoom::on_render>(this);
    dispatcher().listen<fov_check_event::post, &zoom::on_fov_event>(this);

}
void zoom::on_manager_exit() {
    dispatcher().deafen<render_event, &zoom::on_render>(this);
    dispatcher().deafen<fov_check_event::post, &zoom::on_fov_event>(this);
}