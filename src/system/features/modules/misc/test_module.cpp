//
// Created by emi on 10/16/26.
//

#include "test_module.hpp"

#include <fstream>

#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/entity/components/MoveInputComponent.hpp"
#include "core/core.hpp"
#include "sdk/mc/options/option_types/OptionID.hpp"
#include "sdk/mc/world/actor/Actor.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/Option.hpp"
#include <nlohmann/json.hpp>

#include "sdk/mc/options/option_types/FloatOption.hpp"
#include "util/option_dumper/option_dumper.hpp"


void test_module::on_enable() {
    spdlog::info("test module enabled");
    auto ci = core::get_client_instance();
    auto lp = ci->getLocalPlayer();
    if (!lp) return;

    //option_dumper::dump_options();
    auto fovSetting = static_cast<FloatOption*>(ci->getOptions().getIfValid(OptionID::GfxFieldOfView).value());
    spdlog::info("VALUE_MIN={} VALUE_MAX={} mValue={} mDefaultValue={} DELTA={}", fovSetting->VALUE_MIN, fovSetting->VALUE_MAX, fovSetting->mValue, fovSetting->mDefaultValue, fovSetting->DELTA);
}

void test_module::on_disable() {
    spdlog::info("test module disabled");
}

void test_module::on_manager_init() {
    spdlog::info("test module initialized");
}

void test_module::on_manager_exit() {
    spdlog::info("test module exited");
}
