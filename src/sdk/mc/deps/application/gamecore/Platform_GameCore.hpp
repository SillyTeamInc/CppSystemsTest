//
// Created by emi on 10/14/26.
//
#pragma once
#include "../../../../../core/sdk_base.hpp"

class AppPlatform_GameCore;

namespace Bedrock {
    class GameHolder;
    class Platform_GameCore : SDK_CLASS {
    public:
        SDK_FIELD(mPlatformShim, (sigs().Bedrock.Platform_GameCore.mPlatform_Shim.v()), AppPlatform_GameCore*);
        SDK_FIELD(mHwnd, (sigs().Bedrock.Platform_GameCore.mHwnd.v()), HWND);
        SDK_FIELD(mHolder, (sigs().Bedrock.Platform_GameCore.mHolder.v()), Bedrock::GameHolder*);
    };
}
