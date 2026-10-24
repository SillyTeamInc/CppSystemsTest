#pragma once

#include "hvs/hvs.hpp"

class char_event : public hvs::cancelable_event {
public:
    wchar_t character;

    explicit char_event(wchar_t character) : character(character) {}
};
