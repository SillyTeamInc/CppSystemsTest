//
// Created by emi on 10/9/26.
//
#pragma once

#ifdef max // FUCK YOU
#undef max
#endif
#include "entt/entt.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks-inl.h>
#include <typeindex>
#include <functional>
#include <type_traits>
#include "libhat.hpp"
#include "MinHook.h"
#include "util/types.hpp"
