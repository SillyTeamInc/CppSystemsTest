//
// Created by emi on 10/26/26.
//

#include "zoom.hpp"

#include "events/moused_event.hpp"
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
    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp) {
        disable();
        return;
    }

    m_target_fov = ZOOM_FOV;

    m_fov = static_cast<FloatOption*>(ci->getOptions().getIfValid(OptionID::GfxFieldOfView).value());
    m_fov->VALUE_MIN = 5.f;

    for (auto&& [id, component]: lp->mContext.mEnTTRegistry.view<MinecraftCamera::CameraComponent>().each()) {
        auto hasflag = lp->mContext.has_flag<MinecraftCamera::CameraRenderFirstPersonObjectsComponent>(id);
        if (hasflag) {
            m_restore_fpobj.push_back(id);
            lp->mContext.set_flag<MinecraftCamera::CameraRenderFirstPersonObjectsComponent>(id, false);
            log->info("removed CameraRenderFirstPersonObjectsComponent for camera id={}", id.rawId);
        }
    }
}


void zoom::on_disable() {
    dispatcher().deafen<moused_event, &zoom::on_mouse_event>(this);

    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp) {
        return;
    }

    for (auto&& [id, component]: lp->mContext.mEnTTRegistry.view<MinecraftCamera::CameraComponent>().each()) {
        if (std::ranges::find(m_restore_fpobj, id) == m_restore_fpobj.end()) {
            continue;
        }

        lp->mContext.set_flag<MinecraftCamera::CameraRenderFirstPersonObjectsComponent>(id, true);
    }
    m_restore_fpobj.clear();
}

void zoom::on_mouse_event(moused_event& event) {
    log->debug(event.to_string());


}

void zoom::on_render(render_event& event) {
    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp) {
        disable();
        return;
    }


}

void zoom::on_manager_init() { dispatcher().listen<render_event, &zoom::on_render>(this); }
void zoom::on_manager_exit() { dispatcher().deafen<render_event, &zoom::on_render>(this); }


/*
 *
void Zoom::onEnable()
{
    mPastFov = ClientInstance::get()->getOptions()->mGfxFieldOfView->mValue;
    mCurrentValue = mZoomValue.mValue;
    ClientInstance::get()->getOptions()->mGfxFieldOfView->mMinimum = 10.f;

    gFeatureManager->mDispatcher->listen<MouseEvent, &Zoom::onMouseEvent>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &Zoom::onRenderEvent>(this);
}

void Zoom::onDisable()
{
    gFeatureManager->mDispatcher->deafen<MouseEvent, &Zoom::onMouseEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &Zoom::onRenderEvent>(this);

    ClientInstance::get()->getOptions()->mGfxFieldOfView->mValue = mPastFov;
    ClientInstance::get()->getOptions()->mGfxFieldOfView->mMinimum = 30.f;
}

void Zoom::onMouseEvent(MouseEvent& event)
{
    if (ClientInstance::get()->getMouseGrabbed()) return;
    if (!mScroll.mValue) return;

    if (event.mActionButtonId == 4)
    {
        if(event.mButtonData == 0x78 || event.mButtonData == 0x7F)
        {
            mZoomValue.mValue -= mScrollIncrement.mValue;
            event.cancel();
        }
        else if (event.mButtonData == 0x88 || event.mButtonData == 0x80 || event.mButtonData == -0x78)
        {
            mZoomValue.mValue += mScrollIncrement.mValue;
            event.cancel();
        }
    }

    if (mZoomValue.mValue < 10.f) mZoomValue.mValue = 10.f;
    if (mPastFov < mZoomValue.mValue) mZoomValue.mValue = mPastFov;
    if (mCurrentValue < 10.f) mCurrentValue = 10.f;
}

void Zoom::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if(mSmooth.mValue)
    {
        mCurrentValue = MathUtils::lerp(mCurrentValue, mZoomValue.mValue, ImGui::GetIO().DeltaTime * 10.f);
        ClientInstance::get()->getOptions()->mGfxFieldOfView->mValue = mCurrentValue;
    }
    else
    {
        ClientInstance::get()->getOptions()->mGfxFieldOfView->mValue = mCurrentValue;
    }

}*/
