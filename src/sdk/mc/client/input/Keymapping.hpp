//
// Created by emi on 10/28/26.
//

#pragma once
#include <algorithm>
#include <string>
#include <vector>

class Keymapping {
public:
    std::string mAction;
    std::vector<int> mKeys;
    bool mAllowRemap;
    bool mIsSharedKey;

    [[nodiscard]] bool isAssigned() const { return !mKeys.empty() && mKeys[0] != 0; }

    [[nodiscard]] bool isAltKey() const {
        return isAssigned() && std::ranges::any_of(mKeys, [](int key) { return key < 0; });
    }

    [[nodiscard]] bool isRemappable() const { return mAllowRemap; }
    [[nodiscard]] bool isSharedKey() const { return mIsSharedKey; }

    [[nodiscard]] bool matching(uint32 key) const {
        return std::ranges::any_of(mKeys, [key](int k) { return k == key; });
    }
};

static_assert(sizeof(Keymapping) == 0x40);
