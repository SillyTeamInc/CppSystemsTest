//
// Created by emi on 10/16/26.
//
#pragma once
#include "hvs/hvs.hpp"

class module;

class module_state_change_event : public hvs::cancelable_event {
public:
    module* module = nullptr;
    // old state is always opposite because this event is never fired unless the state is changed
    bool m_new_state = false;

    module_state_change_event(::module* m, bool new_state) : module(m), m_new_state(new_state) {}
};
