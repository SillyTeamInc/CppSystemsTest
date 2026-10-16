//
// Created by emi on 10/14/26.
//
#pragma once
#include <core/sdk_base.hpp>

namespace Bedrock {
    class Platform_GameCore;
}

namespace MainGameCore {
    class WinMain : SDK_CLASS {
    public:
        static inline WinMain* get() {
            return *reinterpret_cast<WinMain**>(sigs().MainGameCore.WinMain.instance.v());
        }

        SDK_FIELD(mPlatform, (sigs().MainGameCore.WinMain.mPlatform.v()), Bedrock::Platform_GameCore*);
    };
}