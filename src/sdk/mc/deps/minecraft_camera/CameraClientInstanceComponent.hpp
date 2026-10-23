//
// Created by emi on 10/21/26.
//

#pragma once

#include <memory>

class ICameraClientInstance;

struct CameraClientInstanceComponent {
public:
    std::unique_ptr<ICameraClientInstance> mClientInstance;
};
