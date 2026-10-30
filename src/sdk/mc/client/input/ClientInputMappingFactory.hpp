#pragma once
#include <cstdint>
#include <memory>

#include "core/sdk_base.hpp"
#include "sdk/mc/client/input/RemappingLayout.hpp"
#include "util/memory.hpp"

class ClientInputMappingFactory : SDK_CLASS {
public:
    SDK_FIELD(mKeyboardRemappingLayout, sigs().ClientInputMappingFactory.mKeyboardRemappingLayout, std::weak_ptr<KeyboardRemappingLayout>);

    [[nodiscard]] std::shared_ptr<KeyboardRemappingLayout> getKeyboardRemappingLayout() const {
        return get_mKeyboardRemappingLayout().lock();
    }
};
