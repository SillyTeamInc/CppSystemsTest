//
// Created by emi on 10/28/26.
//

#pragma once
#include <string>
#include <vector>

class Keymapping {
public:
    std::string mAction;
    std::vector<int> mKeys;
    bool mAllowRemap;
    bool mIsSharedKey;
};
