//
// Created by emi on 10/20/26.
//

#include "option_dumper.hpp"

#include <fstream>

#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/entity/components/MoveInputComponent.hpp"
#include "core/core.hpp"
#include "sdk/mc/options/option_types/OptionID.hpp"
#include "sdk/mc/world/actor/Actor.hpp"
#include "sdk/mc/options/IOptionsReader.hpp"
#include "sdk/mc/options/option_types/Option.hpp"
#include <nlohmann/json.hpp>
#include <magic_enum.hpp>

namespace {
    nlohmann::json option_to_json(const Option* option) {
        nlohmann::json json;
        json["saveTag"] = option->mImpl->mSaveTag;
        json["optionId"] = option->mImpl->mID;
        json["captionId"] = option->mImpl->mCaptionId;
        json["type"] = std::string(magic_enum::enum_name((option->mImpl->mOptionType)));
        json["ownerType"] = std::string(magic_enum::enum_name((option->mImpl->mOwnerType)));
        json["resetFlags"] = static_cast<int>(option->mImpl->mOptionResetFlags);
        return json;
    }
}

void option_dumper::dump_options() {
    auto ci = core::get_client_instance();
    auto& options = ci->getOptions();

    nlohmann::json json = nlohmann::json::array();
    for (int i = 0; i < 1000; ++i) {
        auto option = options.getIfValid(static_cast<OptionID>(i));
        if (option) {
            spdlog::info("added option id {} to json", i);
            json.push_back(option_to_json(*option));
        } else {
            spdlog::warn("optionid {} isnt valid", i);
        }
    }

    std::ofstream file("options.json");
    if (file.is_open()) {
        file << json.dump(4);
        file.close();
        auto currentdir = std::filesystem::current_path();
        spdlog::info("options written to {}\\options.json", currentdir.string());
    } else {
        spdlog::error("failed to open options.json for writing..?");
    }
}
