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

class ICameraAPI {};

struct CameraAPIComponent {
    std::unique_ptr<ICameraAPI> mApi;
};
