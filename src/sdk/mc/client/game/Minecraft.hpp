//
// Created by emi on 10/31/2026.
//

#pragma once
#include "core/sdk_base.hpp"

class Timer {
public:
    float mTicksPerSecond;
    int mTicks;
    float mPartialTicks;
    float mTimeScale;
    float mPassedTime;
    float mFrameStepAlignmentRemainder;
    float mLastTimestep;
    float mOverflowTime;
    int64 mLastMs;
    int64 mLastMsSysTime;
    int64 mLastTimeMs;
    float mAdjustTime;
    float mSteppingTick;
    std::function<int64()> mGetTimeMSCallback;
};

class Minecraft : SDK_CLASS {
public:
    SDK_FIELD(mSimTimer, sigs().Minecraft.mSimTimer, Timer*);
    SDK_FIELD(mRealTimer, sigs().Minecraft.mSimTimer.value() + 8, Timer*);
};
