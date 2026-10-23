//
// Created by emi on 10/20/26.
//

#pragma once

#include <functional>
#include <string>

#include "IntOption.hpp"
#include "Option.hpp"


class EnumOption : public IntOption {
public:
    std::unordered_map<int, std::string> ValueNameMap;
};