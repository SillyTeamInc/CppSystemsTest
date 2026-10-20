//
// Created by emi on 10/20/26.
//

#pragma once
#include "Option.hpp"

class IntOption : public Option {
public:
    int const VALUE_MAX;
    int const VALUE_MIN;
    int mValue{};
    int mDefaultValue{};
    bool mClampToRange{};
    std::vector<int> mPossibleValues;
    std::function<int(int)> mCoerceSaveValueCallback;

    // NOLINTBEGIN
    virtual ~IntOption() /*override*/ = default;
    virtual void save(std::vector<std::pair<std::string, std::string>>& propertyVector) /*override*/;
    virtual void load(std::string const& valueString) /*override*/;
    // NOLINTEND
};