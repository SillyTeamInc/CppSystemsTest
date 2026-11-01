//
// Created by emi on 11/1/26.
//

#pragma once
#include "hvs/hvs.hpp"

class cull_event : public hvs::event {
public:
    int m_type = 0;
    explicit cull_event(int type) : m_type(type) {}
};