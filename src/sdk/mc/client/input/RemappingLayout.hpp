#pragma once
#include <string_view>
#include <vector>

#include "core/sdk_base.hpp"
#include "sdk/mc/client/input/Keymapping.hpp"
#include "util/memory.hpp"

class RemappingLayout : SDK_CLASS {
public:
    SDK_FIELD(mKeymappings, 0x8, std::vector<Keymapping>);
    SDK_FIELD(mDefaultMappings, 0x20, std::vector<Keymapping>);

    [[nodiscard]] const Keymapping* get(std::string_view action) const {
        return find(get_mKeymappings(), action);
    }

    [[nodiscard]] const Keymapping* operator[](std::string_view action) const {
        return get(action);
    }

    [[nodiscard]] const Keymapping* getDefault(std::string_view action) const {
        return find(get_mDefaultMappings(), action);
    }

private:
    static const Keymapping* find(const std::vector<Keymapping>& mappings, std::string_view action) {
        for (const auto& mapping: mappings) {
            if (mapping.mAction == action)
                return &mapping;
        }
        return nullptr;
    }
};

class KeyboardRemappingLayout : public RemappingLayout {};
