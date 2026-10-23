//
// Created by emi on 10/21/26.
//

#pragma once
#include <memory>

class IVanillaCameraAPI;

struct VanillaCameraAPIComponent {
public:
    std::unique_ptr<IVanillaCameraAPI> mApi;
};