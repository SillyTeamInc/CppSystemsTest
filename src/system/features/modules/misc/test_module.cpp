//
// Created by emi on 10/16/26.
//

#include "test_module.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include "core/core.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/client/input/ClientInputHandler.hpp"
#include "sdk/mc/client/renderer/game/LevelRenderer.hpp"
#include "sdk/mc/entity/components/MoveInputComponent.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/Option.hpp"
#include "sdk/mc/options/option_types/OptionID.hpp"
#include "sdk/mc/world/actor/Actor.hpp"

#include <imgui.h>
#include "events/base_tick_event.hpp"
#include "events/render_event.hpp"
#include "sdk/mc/deps/minecraft_camera/CameraRegistry.hpp"
#include "sdk/mc/deps/minecraft_camera/components/ActiveCameraComponent.hpp"
#include "sdk/mc/entity/components/ActorHeadRotationComponent.hpp"
#include "sdk/mc/entity/components/CameraComponent.hpp"
#include "sdk/mc/entity/components/CameraDirectLookComponent.hpp"
#include "sdk/mc/options/option_types/FloatOption.hpp"
#include "system/features/modules/module_system.hpp"
#include "util/option_dumper/option_dumper.hpp"

#include "sdk/mc/options/option_types/IntOption.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"

#include "util/fonts.hpp"
#include "util/keyboard.hpp"

void test_module::on_enable() {
    log->info("test module enabled");

    dispatcher().listen<base_tick_event, &test_module::on_base_tick>(this);
    dispatcher().listen<render_event, &test_module::on_render>(this);

    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp)
        return;

    // option_dumper::dump_options();
    /*auto fovSetting = static_cast<FloatOption*>(ci->getOptions().getIfValid(OptionID::GfxFieldOfView).value());
    log->info("VALUE_MIN={} VALUE_MAX={} mValue={} mDefaultValue={} DELTA={}", fovSetting->VALUE_MIN,
    fovSetting->VALUE_MAX, fovSetting->mValue, fovSetting->mDefaultValue, fovSetting->DELTA);*/

    log->info("Dumping currently existing components for local player");
    lp->mContext.logComponents();
    log->info("Dumping currently existing components for mGameCamera");
    ci->getCameraRegistry()->mGameCamera->logComponents();
    if (ci->getCameraRegistry()->mDebugCamera) {
        log->info("Dumping currently existing components for mDebugCamera");
        ci->getCameraRegistry()->mDebugCamera->logComponents();
    }
    {
        MinecraftCamera::CameraComponent* gc = ci->getCameraRegistry()->mGameCamera->tryGetComponent<MinecraftCamera::CameraComponent>();
        if (gc) {
            log->info("GameCamera mode: {}", static_cast<int>(gc->getMode()));
        } else {
            log->info("GameCamera has no CameraComponent");
        }
    }

    /*int i = 0;
    for (auto& r : ci->getCameraRegistry()->mCameras) {
        log->info("RegisteredCamera{}: {:p}", i++, static_cast<void*>(&r));
        log->info("RegisteredCamera{} name: {}, id: {}", i, r.mName, r.mId.mStr);
    }*/

    for (auto cam : ci->getCameraRegistry()->mCameraEntities) {
        auto camComp = cam->tryGetComponent<MinecraftCamera::CameraComponent>();
        if (!camComp.has_value()) {
            continue;
        }

        std::string camName = camComp->mId.mStr;
        log->info("Dumping currently existing components for camera {}", camName);
        cam->logComponents();
        if (camComp->getMode() == CameraMode::Free) {
            auto& st = cam->getOrAddComponent<MinecraftCamera::StationaryCameraComponent>();
            st.mPos = glm::vec3(0, 70, 0);
            st.mRot = {0, 0};
            cam->set_flag<MinecraftCamera::OverrideCameraComponent>(true);
        }
    }


}

void test_module::on_disable() {
    log->info("test module disabled");

    dispatcher().deafen<base_tick_event, &test_module::on_base_tick>(this);
    dispatcher().deafen<render_event, &test_module::on_render>(this);
    auto ci = core::get_client_instance();
    if (!ci->getLocalPlayer()) return;

    for (auto cam : ci->getCameraRegistry()->mCameraEntities) {
        auto camComp = cam->tryGetComponent<MinecraftCamera::CameraComponent>();
        if (!camComp.has_value()) {
            continue;
        }

        cam->set_flag<MinecraftCamera::OverrideCameraComponent>(false);
    }
}

namespace {
    // Function to show a pointer and a button to copy the pointer in ImGui
    void show_pointer(const char* label, void* ptr) {
        ImGui::Text("%s: %p", label, ptr);
        ImGui::SameLine();
        if (ImGui::Button(("Copy " + std::string(label)).c_str())) {
            ImGui::SetClipboardText(fmt::format("{:p}", ptr).c_str());
        }
    }
} // namespace

void test_module::on_render(render_event& event) {

    ImGui::Begin("test_module");

    if (ImGui::Button("Don't click me :3")) {
        int* ptr = nullptr;
        *ptr = 0xB00B5; // haha funny
    }


    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();

    if (ImGui::CollapsingHeader("Modules")) {
        for (auto& module: get_module_system()->modules) {
            std::string name = std::string(module->get_name());
            ImGui::Text("%s: %s", name.c_str(), module->is_enabled() ? "Enabled" : "Disabled");
            ImGui::SameLine();
            if (ImGui::Button(("Toggle " + std::string(module->get_name())).c_str())) {
                module->toggle();
            }
        }
    }

    if (ImGui::CollapsingHeader("Stuff")) {
        ImGui::Spacing();

        if (ImGui::Button("Play Sound"))
            ci->playUI("random.orb", 1.f, 1.f);

        auto forward = keyboard::mapping["key.forward"];
        auto forwardState = keyboard::state["key.forward"];
        ImGui::TextUnformatted(fmt::format("Forward key: {:p} (state: {})", static_cast<void*>(forward), forwardState).c_str());

        CameraRegistry* cameraRegistry = ci->getCameraRegistry();
        if (cameraRegistry) {
            auto a = cameraRegistry->mGameCamera->mEntity.rawId;
            ImGui::TextUnformatted(fmt::format("GameCamera EntityId: {:X}", a).c_str());
        }
    }

    if (ImGui::CollapsingHeader("Font Tests")) {
        ImGui::Spacing();

        {
            fonts::scope font(font::comfortaa, 24);
            ImGui::Text("this is comfortaa, hello world");
        }

        {
            fonts::scope font(font::open_sans, 24);
            ImGui::Text("this is open sans, hello world");
        }
    }

    if (ImGui::CollapsingHeader("Pointers")) {
        ImGui::Spacing();

        LevelRenderer* lr = ci->getLevelRenderer();
        show_pointer("LevelRenderer", lr);
        if (lr) {
            auto lrp = lr->getLevelRendererPlayer();
            show_pointer("LevelRendererPlayer", lrp);
        }

        show_pointer("ClientInstance", ci);
        show_pointer("LocalPlayer", lp);
        if (lp != nullptr) {
            auto headRotComp = lp->mContext.tryGetComponent<ActorHeadRotationComponent>();
            if (headRotComp.has_value()) {
                show_pointer("ActorHeadRotationComponent", &headRotComp.value());
                ImGui::TextUnformatted(
                        fmt::format("mYHeadRot={} mYHeadRotO={}", headRotComp->mYHeadRot, headRotComp->mYHeadRotO)
                                .c_str());
            } else {
                ImGui::Text("ActorHeadRotationComponent: Not found");
            }

            auto renderRot = lp->mContext.tryGetComponent<RenderRotationComponent>();
            if (renderRot.has_value()) {
                show_pointer("RenderRotationComponent", &renderRot.value());
                ImGui::TextUnformatted(fmt::format("mRot=({}, {})", renderRot->mRot.x, renderRot->mRot.y).c_str());
            } else {
                ImGui::Text("RenderRotationComponent: Not found");
            }

            auto actorRot = lp->mContext.tryGetComponent<ActorRotationComponent>();
            if (actorRot.has_value()) {
                show_pointer("ActorRotationComponent", &actorRot.value());
                ImGui::TextUnformatted(fmt::format("mRot=({}, {})", actorRot->mRot.x, actorRot->mRot.y).c_str());
            } else {
                ImGui::Text("ActorRotationComponent: Not found");
            }


            auto fovSetting = static_cast<FloatOption*>(ci->getOptions().getIfValid(OptionID::GfxFieldOfView).value());
            show_pointer("GfxFieldOfView FloatOption", fovSetting);
            if (fovSetting)
                ImGui::TextUnformatted(fmt::format("VALUE_MIN={} VALUE_MAX={} mValue={}", fovSetting->VALUE_MIN,
                                                   fovSetting->VALUE_MAX, fovSetting->mValue)
                                               .c_str());

            int i = 0;
            for (auto&& [id, component]:
                 lp->mContext.mEnTTRegistry.view<MinecraftCamera::CameraDirectLookComponent>().each()) {
                MinecraftCamera::CameraDirectLookComponent* compPtr = &component;
                show_pointer(("CameraDirectLookComponent" + std::to_string(i++)).c_str(), compPtr);
                ImGui::TextUnformatted(fmt::format("mRotRads=({}, {}) mDelta={} mWrap=({}, {})", compPtr->mRotRads.x,
                                                   compPtr->mRotRads.y, compPtr->mDelta, compPtr->mWrap.x,
                                                   compPtr->mWrap.y)
                                               .c_str());
                /*log->info("id:{:X} CameraDirectLookComponent:{:X} mRotRads=({}, {}) mDelta={} mWrap=({}, {})",
                   id.rawId, reinterpret_cast<std::uintptr_t>(compPtr), compPtr->mRotRads.x, compPtr->mRotRads.y,
                             compPtr->mDelta, compPtr->mWrap.x, compPtr->mWrap.y);*/
            }

            int j = 0;
            for (auto&& [id, component]: lp->mContext.mEnTTRegistry.view<MinecraftCamera::CameraComponent>().each()) {
                MinecraftCamera::CameraComponent* compPtr = &component;
                show_pointer(("CameraComponent_" + std::string(magic_enum::enum_name(component.getMode())) +
                              std::to_string(j++))
                                     .c_str(),
                             compPtr);
                ImGui::TextUnformatted(compPtr->to_string().c_str());
                /*log->info("id:{:X} CameraDirectLookComponent:{:X} mRotRads=({}, {}) mDelta={} mWrap=({}, {})",
                   id.rawId, reinterpret_cast<std::uintptr_t>(compPtr), compPtr->mRotRads.x, compPtr->mRotRads.y,
                             compPtr->mDelta, compPtr->mWrap.x, compPtr->mWrap.y);*/
            }
        }
    }

    auto codes = keyboard::get_all_keys();
    if (ImGui::CollapsingHeader("Keys")) {
        ImGui::Spacing();
        if (ImGui::BeginTable("##keys", 3,
                              ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                                      ImGuiTableFlags_SizingStretchProp)) {
            ImGui::TableSetupColumn("Enum");
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Code", ImGuiTableColumnFlags_WidthFixed, 60.0f);
            ImGui::TableHeadersRow();

            for (const auto& [key, name]: codes) {
                std::string_view keyingIt = magic_enum::enum_name(key);
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted(keyingIt.data(), keyingIt.data() + keyingIt.size());
                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(name.data(), name.data() + name.size());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%d", static_cast<int>(key));
            }

            ImGui::EndTable();
        }
    }

    ImGui::End();
}

void test_module::on_base_tick(base_tick_event& event) {

    /*auto lp = static_cast<LocalPlayer *>(event.actor);
    auto headRotComp = lp->mContext.tryGetComponent<ActorHeadRotationComponent>();
    if (!headRotComp.has_value()) {
        log->warn("[test] ActorHeadRotationComponent not found..?");
        return;
    }

    log->info("[test] lp:{:X} headRotComponent:{:X} mYHeadRot={} mYHeadRotO={}",
                 reinterpret_cast<std::uintptr_t>(lp), reinterpret_cast<std::uintptr_t>(&headRotComp.value()),
                 headRotComp->mYHeadRot, headRotComp->mYHeadRotO);


    for (auto &&[id, component]: lp->mContext.mEnTTRegistry.view<MinecraftCamera::CameraDirectLookComponent>().each()) {
        MinecraftCamera::CameraDirectLookComponent *compPtr = &component;
        log->info("id:{:X} CameraDirectLookComponent:{:X} mRotRads=({}, {}) mDelta={} mWrap=({}, {})", id.rawId,
                     reinterpret_cast<std::uintptr_t>(compPtr), compPtr->mRotRads.x, compPtr->mRotRads.y,
                     compPtr->mDelta, compPtr->mWrap.x, compPtr->mWrap.y);
    }

    // CameraComponent
    for (auto&&[i, component]: lp->mContext.mEnTTRegistry.view<MinecraftCamera::CameraComponent>().each()) {
        MinecraftCamera::CameraComponent* compPtr = &component;
        log->info("id:{:X} CameraComponent:{:X} mId={} mOrientation=({}, {}, {}, {}) mPosition=({}, {}, {})
    mAspectRatio={} mFieldOfView={} mNearPlane={} mFarPlane={}", i.rawId, reinterpret_cast<std::uintptr_t>(compPtr),
    compPtr->mId.mStr, compPtr->mOrientation.x, compPtr->mOrientation.y, compPtr->mOrientation.z,
    compPtr->mOrientation.w, compPtr->mPosition.x, compPtr->mPosition.y, compPtr->mPosition.z, compPtr->mAspectRatio,
    compPtr->mFieldOfView, compPtr->mNearPlane, compPtr->mFarPlane);
    }

    log->info("[test] ok");*/
}

void test_module::on_manager_init() { log->info("test module initialized"); }

void test_module::on_manager_exit() { log->info("test module exited"); }
