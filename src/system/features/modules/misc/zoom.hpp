//
// Created by emi on 10/26/26.
//

#pragma once
#include "events/render_event.hpp"
#include "sdk/mc/deps/ecs/EntityId.hpp"
#include "system/features/modules/module.hpp"


class moused_event;
class FloatOption;
class zoom : public module_base<zoom> {
public:
    static constexpr float ZOOM_FOV = 60.f;

    static constexpr module_info info {"Zoom", "Zooming everywhere", category::misc, static_cast<int>(keyboard::key::c), false, false};

    explicit zoom() : module_base(info) {}

    float m_target_fov = ZOOM_FOV;
    float m_current_fov = ZOOM_FOV;
    float m_prev_fov = 0;
    FloatOption* m_fov = nullptr;
    std::vector<EntityId> m_restore_fpobj{};

    void on_enable() override;
    void on_disable() override;
    void on_mouse_event(moused_event& event);
    void on_render(render_event& event);

    void on_manager_init() override;
    void on_manager_exit() override;
};
