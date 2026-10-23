//
// Created by emi on 10/21/26.
//

#pragma once
#include <vector>

#include "CameraRegistry.hpp"
#include "sdk/mc/client/game/ClientInstance.hpp"
#include "sdk/mc/deps/game_refs/OwnerPtr.hpp"
#include "util/memory.hpp"
#include "sdk/mc/deps/ecs/EntityId.hpp"

class CameraPresets;
class EcsEventDispatcher;
class EntityContext;
class EntityRegistry;
class Experiments;
class IMinecraftEventing;
class ResourcePackManager;

namespace MinecraftCamera {
    struct RegisteredCamera;
}

namespace SharedTypes::v1_21_100 {
    struct CameraDefinitions;
}

namespace SharedTypes::v1_26_50 {
    struct CameraPreset;
}


class CameraRegistry : public Bedrock::EnableNonOwnerReferences {
public:
    using RegisteredCamera = ::MinecraftCamera::RegisteredCamera;
    using ComponentInitializer = void (*)(::EntityContext &, ::SharedTypes::v1_21_100::CameraDefinitions const &);

public:
    // these sizes and the offset here doesn't match the game and i have no clue why
    // NOLINTBEGIN
    PAD(0x80);
    PAD(0x110);
    PAD(0x24);
    std::vector<::MinecraftCamera::RegisteredCamera> mCameras;
    std::vector<::OwnerPtr<::EntityId> > mCameraEntities;
    OwnerPtr<EntityId> mGameCamera;
    OwnerPtr<EntityId> mDebugCamera;

    virtual ~CameraRegistry() /*override*/ = default;

    // NOLINTEND
};
