//
// Created by emi on 10/15/26.
//
#pragma once
#include "../../../../../core/sdk_base.hpp"
#include "util/memory.hpp"

class MinecraftGame;

namespace Bedrock {
    class GameHolder : SDK_CLASS {
    public:
        SDK_FIELD(mGame, (sigs().Bedrock.GameHolder.mGame.v()), MinecraftGame*); // (this is a shared_ptr but for this it doesnt matter)
    };
}