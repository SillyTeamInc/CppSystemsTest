//
// Created by emi on 10/20/26.
//

#pragma once
#include "Option.hpp"

class FloatOption : public Option {
public:
    float const VALUE_MIN{};
    float const VALUE_MAX{};
    float mValue{};
    float mDefaultValue{};
    float const DELTA{};

    // NOLINTBEGIN
    virtual void save(::std::vector<::std::pair<::std::string, ::std::string> > &propertyVector) /*override*/;
    virtual void load(::std::string const &valueString) /*override*/;
    // NOLINTEND
};
