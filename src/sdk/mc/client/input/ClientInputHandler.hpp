#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>

#include "core/sdk_base.hpp"
#include "sdk/mc/client/input/ClientInputMappingFactory.hpp"
#include "util/memory.hpp"

class ClientInputHandler : SDK_CLASS {
public:
    SDK_FIELD(mMappingFactory, sigs().ClientInputHandler.mMappingFactory, ClientInputMappingFactory*);

    [[nodiscard]] ClientInputMappingFactory* getMappingFactory() const { return get_mMappingFactory(); }

    [[nodiscard]] std::shared_ptr<KeyboardRemappingLayout> getKeyboardLayout() const {
        const auto* factory = getMappingFactory();
        return factory ? factory->getKeyboardRemappingLayout() : nullptr;
    }

    [[nodiscard]] std::optional<Keymapping> get(std::string_view action) const {
        const auto layout = getKeyboardLayout();
        if (!layout)
            return std::nullopt;
        if (const auto* mapping = layout->get(action))
            return *mapping;
        return std::nullopt;
    }
};
