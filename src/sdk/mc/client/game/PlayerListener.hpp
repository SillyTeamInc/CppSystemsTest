//
// Created by emi on 10/27/2026.
//

#pragma once

class Player;

class PlayerListener {
public:
    // NOLINTBEGIN
    virtual ~PlayerListener() = default;
    virtual void onWillChangeDimension(Player& player);
    virtual void onDimensionChanged(Player& player);
    virtual void onPlayerDestruction(Player& player);
    // NOLINTEND
};