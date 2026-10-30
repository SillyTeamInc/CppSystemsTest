//
// Created by emi on 10/14/26.
//
#pragma once
#include "system/signatures/basic_signature_system.hpp"

// provides easy access to functions without fucking up vtable
class sdk_base {
protected:
    static systems_manager* sysman();

    static signature_system* sigsys();

    static auto& sigs() { return sigsys()->sigs(); }
};

#define SDK_CLASS protected sdk_base