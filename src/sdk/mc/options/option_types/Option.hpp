//
// Created by emi on 10/20/26.
//

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "OptionID.hpp"
#include "OptionOwnerType.hpp"
#include "OptionResetFlags.hpp"
#include "OptionType.hpp"
#include "sdk/mc/world/actor/Actor.hpp"
#include "util/memory.hpp"

namespace Bedrock::PubSub::ThreadModel {
    struct MultiThreaded;
}

namespace Json {
    class Value;
}

class Option {
public:
    class Impl {
    public:
        PAD(0x188);
        std::string mSaveTag{};
        std::string mTelemetryProperty{};
        uchar mTelemetryTag{};
        OptionID const mID{};
        OptionOwnerType const mOwnerType{};
        OptionType mOptionType{};
        std::string const mCaptionId{};
        PAD(0x38);
        OptionResetFlags const mOptionResetFlags{};
        Option *mOverrideSource{};
        PAD(0x20);
        std::function<void(bool)> mRequestSaveCallback{};
    };

    std::unique_ptr<Impl> mImpl;

    virtual ~Option();
    virtual void save(std::vector<std::pair<std::string, std::string> > &propertyVector) = 0;
    virtual void load(std::string const &valueString) = 0;
    virtual void load(std::map<std::string, std::string> &propertyMap);
    virtual void load(Json::Value const &valueJson);
};
