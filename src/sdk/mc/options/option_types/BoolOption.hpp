//
// Created by emi on 10/20/26.
//

#pragma once
#include <functional>
#include <string>

#include "Option.hpp"

class BoolOption : public Option {
public:
    bool mValue;
    bool mDefaultValue;
    std::function<bool(bool)> mCoerceValueCallback;

    // NOLINTBEGIN
    virtual void save(::std::vector<::std::pair<::std::string, ::std::string> > &propertyVector) /*override*/;
    virtual void load(::std::string const &valueString) /*override*/;
    // NOLINTEND
};
