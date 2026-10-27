//
// Created by emi on 10/27/2026.
//

#pragma once

#include <sdk/mc/world/actor/Actor.hpp>

#include "sdk/mc/client/game/PlayerListener.hpp"
#include "sdk/mc/deps/core/utility/EnableNonOwnerReferences.hpp"


class Actor;
class BaseActorRenderContext;
class ClientFrameUpdateContext;
class Font;
class IClientInstance;
class RenderChunkShared;
class ScreenContext;
struct CrackRenderObjectCollection;
struct LevelRenderPreRenderUpdateParameters;
struct NameTagRenderObjectCollection;
struct ViewRenderObject;
enum class LevelCullerType : uchar;
enum class LevelRendererCameraType : uchar;

struct FogDefinition {
    enum class DistanceSettingType : int {
        Air = 0,
        Weather = 1,
        Water = 2,
        Lava = 3,
        LavaResist = 4,
        PowderSnow = 5,
    };

    enum class DensitySettingType : int {
        Air = 0,
        Weather = 1,
        Water = 2,
        Lava = 3,
        LavaResist = 4,
    };
};


class LevelRendererCamera {
public:
    // NOLINTBEGIN
    virtual ~LevelRendererCamera();
    virtual ::LevelRendererCameraType getCameraType() const;
    virtual void addCameraListenerToRenderChunkCoordinator();
    virtual void onAppSuspended();
    virtual void onAppResumed();
    virtual void onDeviceLost();
    virtual void onLowMemory();
    virtual void frameUpdate(::ClientFrameUpdateContext& clientFrameUpdateContext);
    virtual void tickLevelRendererCamera();
    virtual void tickRain();
    virtual void updateViewArea(::LevelRenderPreRenderUpdateParameters const& levelRenderPreRenderUpdateParameters);
    virtual void callRenderNameTags(::ScreenContext& screenContext, ::ViewRenderObject const& renderObj, ::Font& font);
    virtual ::NameTagRenderObjectCollection extractNameTags(::ScreenContext& screenContext) const;
    virtual void callRenderCracks(::BaseActorRenderContext& renderContext, ::ViewRenderObject const& renderObj);
    virtual ::CrackRenderObjectCollection extractCracks(::ScreenContext& screenContext) const;
    virtual void renderEntityEffects(::BaseActorRenderContext& renderContext);
    virtual void renderBlockEntities(::BaseActorRenderContext& renderContext, bool renderAlphaLayer);
    virtual void setViewArea(::LevelRenderPreRenderUpdateParameters const& levelRenderPreRenderUpdateParameters);
    virtual bool getForceFog(::Actor const& cameraActor) const;
    virtual void setupFog(::ScreenContext& screenContext, float const skyIntensityOverride);
    virtual float getAmbientBrightness() const;
    virtual void recalculateRenderDistance(float const renderDistanceScalar);
    virtual void preRenderUpdate(::ScreenContext& screenContext,
                                 ::LevelRenderPreRenderUpdateParameters& levelRenderPreRenderUpdateParameters);
    virtual void render(::BaseActorRenderContext& baseEntityRenderContext, ::ViewRenderObject const& renderObj,
                        ::IClientInstance& ci);
    virtual void postRenderUpdate();
    virtual void notifyGeoChangedForAffectedEntities(::RenderChunkShared& renderChunkShared, uchar version);
    virtual void updateLevelCullerType(::LevelCullerType const newLevelCullerType) = 0;
    virtual void queueRenderEntities(::LevelRenderPreRenderUpdateParameters const& preRenderUpdateParameters);
    virtual void _releaseRespectiveResources();
    virtual void setupViewArea();
    virtual ::FogDefinition::DistanceSettingType _getFogDistanceSettingType() const;
    virtual ::FogDefinition::DensitySettingType _getFogDensitySettingType() const;
    // NOLINTEND
};


class PushNotificationMessage;
enum class LowMemorySeverity : int;
enum class OperationMode : int;
enum class RefreshRate : int;


class AppPlatformListener {
public:
    // NOLINTBEGIN
    virtual ~AppPlatformListener();
    virtual void onAppPaused();
    virtual void onAppUnpaused();
    virtual void onAppPreSuspended();
    virtual void onAppSuspended();
    virtual void onAppResumed();
    virtual void onAppFocusLost();
    virtual void onAppFocusGained();
    virtual void onAppTerminated();
    virtual void onOperationModeChanged(::OperationMode const operationMode);
    virtual void onPerformanceModeChanged(bool const boost);
    virtual void onPushNotificationReceived(::PushNotificationMessage const& msg);
    virtual void onRefreshRateChanged(::RefreshRate const refreshRate);
    virtual void onResizeBegin();
    virtual void onResizeEnd();
    virtual void onDeviceLost();
    virtual void onAppSurfaceCreated();
    virtual void onAppSurfaceDestroyed();
    virtual void onClipboardCopy(::std::string const& clipString);
    virtual void onClipboardPaste(::std::string const& clipString);
    virtual void onLowMemory(::LowMemorySeverity);
    // NOLINTEND

    PAD(0x11);
};

class Block;
class BlockActor;
class BlockPos;
class BlockSource;
struct ActorBlockSyncMessage;
enum class BlockChangedEventTarget : uchar;


class BlockSourceListener {
public:
    // NOLINTBEGIN
    virtual ~BlockSourceListener() = default;
    virtual void onSourceCreated(::BlockSource& source);
    virtual void onSourceDestroyed(::BlockSource& source);
    virtual void onAreaChanged(::BlockSource& source, ::BlockPos const& min, ::BlockPos const& max);
    virtual void onBlockChanged(::BlockSource& source, ::BlockPos const& pos, uint layer, ::Block const& block,
                                ::Block const& oldBlock, int updateFlags, ::ActorBlockSyncMessage const* syncMsg,
                                ::BlockChangedEventTarget eventTarget, ::Actor* blockChangeSource);
    virtual void onBrightnessChanged(::BlockSource& source, ::BlockPos const& pos);
    virtual void onBlockEntityChanged(::BlockSource& source, ::BlockActor& te);
    virtual void onEntityChanged(::BlockSource& source, ::Actor& entity);
    virtual void onBlockEvent(::BlockSource& source, int x, int y, int z, int b0, int b1);
    // NOLINTEND
};


class Actor;
class Block;
class BlockPos;
class ChunkSource;
class CompoundTag;
class HashedString;
class LevelChunk;
class MolangVariableMap;
class Vec3;
struct BreakingItemParticleData;
struct ResolvedItemIconInfo;
struct ScreenshotOptions;
enum class ParticleType : int;
namespace SharedTypes::Legacy {
    enum class LevelEvent : short;
}
namespace cg {
    class ImageBuffer;
}
namespace mce {
    struct Color;
}

class LevelListener : public ::BlockSourceListener {
public:
    // NOLINTBEGIN
    virtual void allChanged();
    virtual void sendServerLegacyParticle(::ParticleType, ::Vec3 const&, ::Vec3 const&, int);
    virtual void addParticleEffect(::HashedString const&, ::Actor const&, ::HashedString const&, ::Vec3 const&,
                                   ::MolangVariableMap const&);
    virtual void addTerrainParticleEffect(::BlockPos const& pos, ::Block const& block, ::Vec3 const& emitterPosition,
                                          float particleCount, float velocityScalar, float emitterRadius);
    virtual void addTerrainSlideEffect(::BlockPos const& pos, ::Block const& block, ::Vec3 const& emitterPosition,
                                       float particleCount, float velocityScalar, float emitterRadius);
    virtual void addBreakingItemParticleEffect(::Vec3 const& pos, ::BreakingItemParticleData const& data,
                                               ::ResolvedItemIconInfo const& textureInfo);
    virtual void addBiomeTintedParticleEffect(::HashedString const& effect, ::BlockPos const& pos, ::Block const& block,
                                              ::std::optional<::mce::Color> overrideColor);
    virtual void playMusic(::std::string const&, ::Vec3 const&, float, float);
    virtual void playStreamingMusic(::std::string const&, int, int, int);
    virtual void onEntityAdded(::Actor&);
    virtual void onEntityRemoved(::Actor& entity);
    virtual void onChunkLoaded(::ChunkSource& source, ::LevelChunk& lc);
    virtual void onChunkReloaded(::ChunkSource&, ::LevelChunk&);
    virtual void onSubChunkLoaded(::ChunkSource& source, ::LevelChunk& lc, short absoluteSubChunkIndex,
                                  bool subChunkVisibilityChanged);
    virtual void onChunkUnloaded(::LevelChunk& lc);
    virtual void onLevelDestruction(::std::string const& levelId);
    virtual void levelEvent(::SharedTypes::Legacy::LevelEvent type, ::Vec3 const& pos, int data);
    virtual void levelEvent(::SharedTypes::Legacy::LevelEvent type, ::CompoundTag const& data);
    virtual void
    takePicture(::cg::ImageBuffer& outImage, ::Actor* camera, ::Actor* target, ::ScreenshotOptions& screenshotOptions,
                ::std::function<void(::cg::ImageBuffer&, ::ScreenshotOptions&)> completedScreenshotCallback);
    virtual void playerListChanged();
    virtual void onLevelDataCommandsChanged(bool commandsEnabeld);
    virtual void onLevelDataWorldTemplateOptionsUnlocked();
    // NOLINTEND
};

class RegionListener {
public:
    // NOLINTBEGIN
    virtual ~RegionListener() = default;

    virtual void onRegionDestroyed() = 0;
    // NOLINTEND
};

class LevelRendererCameraListeners : public ::LevelRendererCamera,
                                     public ::LevelListener,
                                     public ::PlayerListener,
                                     public ::LocalPlayer::RegionListener,
                                     public ::std::enable_shared_from_this<::LevelRendererCamera> {
public:
    LevelRendererCameraListeners();

    // NOLINTBEGIN
    virtual ~LevelRendererCameraListeners() /*override*/ = default;
    virtual void onWillChangeDimension(::Player& player) /*override*/;
    virtual void onDimensionChanged(::Player& player) /*override*/;
    virtual void onRegionDestroyed() /*override*/;
    virtual void onChunkReloaded(::ChunkSource&, ::LevelChunk& lc) /*override*/;
    virtual void updateLevelCullerType(::LevelCullerType const newLevelCullerType) /*override*/;
    // NOLINTEND
};

class LevelRendererPlayer : public ::LevelRendererCameraListeners, public ::Bedrock::EnableNonOwnerReferences {

    // NOLINTBEGIN
    virtual ~LevelRendererPlayer() /*override*/ = default;

    virtual ::LevelRendererCameraType getCameraType() const /*override*/;

    virtual void frameUpdate(::ClientFrameUpdateContext& clientFrameUpdateContext) /*override*/;

    virtual void tickLevelRendererCamera() /*override*/;

    virtual void tickRain() /*override*/;

    virtual void onWillChangeDimension(::Player& player) /*override*/;

    virtual void onDimensionChanged(::Player& player) /*override*/;

    virtual void recalculateRenderDistance(float const renderDistanceScalar) /*override*/;

    virtual void preRenderUpdate(
        ::ScreenContext&                        screenContext,
        ::LevelRenderPreRenderUpdateParameters& levelRenderPreRenderUpdateParameters
    ) /*override*/;

    virtual void renderBlockEntities(::BaseActorRenderContext& renderContext, bool renderAlphaLayer) /*override*/;

    virtual void
    updateViewArea(::LevelRenderPreRenderUpdateParameters const& levelRenderPreRenderUpdateParameters) /*override*/;

    virtual ::NameTagRenderObjectCollection extractNameTags(::ScreenContext& screenContext) const /*override*/;

    virtual void
    callRenderNameTags(::ScreenContext& screenContext, ::ViewRenderObject const& renderObj, ::Font& font) /*override*/;

    virtual ::CrackRenderObjectCollection extractCracks(::ScreenContext& screenContext) const /*override*/;

    virtual void
    callRenderCracks(::BaseActorRenderContext& renderContext, ::ViewRenderObject const& renderObj) /*override*/;

    virtual bool getForceFog(::Actor const& cameraActor) const /*override*/;

    virtual void setupFog(::ScreenContext& screenContext, float const skyIntensityOverride) /*override*/;

    virtual void levelEvent(::SharedTypes::Legacy::LevelEvent type, ::Vec3 const& pos, int data) /*override*/;

    virtual void levelEvent(::SharedTypes::Legacy::LevelEvent type, ::CompoundTag const& data) /*override*/;

    virtual void addCameraListenerToRenderChunkCoordinator() /*override*/;

    virtual void
    notifyGeoChangedForAffectedEntities(::RenderChunkShared& renderChunkShared, uchar version) /*override*/;

    virtual void addTerrainParticleEffect(
        ::BlockPos const& pos,
        ::Block const&    block,
        ::Vec3 const&     emitterPosition,
        float             particleCount,
        float             velocityScalar,
        float             emitterRadius
    ) /*override*/;

    virtual void addTerrainSlideEffect(
        ::BlockPos const& pos,
        ::Block const&    block,
        ::Vec3 const&     emitterPosition,
        float             particleCount,
        float             velocityScalar,
        float             emitterRadius
    ) /*override*/;

    virtual void addBreakingItemParticleEffect(
        ::Vec3 const&                     pos,
        ::BreakingItemParticleData const& data,
        ::ResolvedItemIconInfo const&     textureInfo
    ) /*override*/;


    virtual void addBiomeTintedParticleEffect(
        ::HashedString const&         effect,
        ::BlockPos const&             pos,
        ::Block const&                block,
        ::std::optional<::mce::Color> overrideColor
    ) /*override*/;

    virtual float getUnderwaterVisionClarity();

    virtual ::FogDefinition::DistanceSettingType _getFogDistanceSettingType() const /*override*/;

    virtual ::FogDefinition::DensitySettingType _getFogDensitySettingType() const /*override*/;

    virtual void _releaseRespectiveResources() /*override*/;

    virtual void queueRenderEntities(
        ::LevelRenderPreRenderUpdateParameters const& levelRenderPreRenderUpdateParameters
    ) /*override*/;

    virtual void renderEntityEffects(::BaseActorRenderContext& renderContext) /*override*/;
    // NOLINTEND
};

class LevelRenderer {
public:
    SDK_FIELD(mClientInstance, 0x408, IClientInstance*);
    SDK_FIELD(mLevelRendererPlayer, 0x468, std::shared_ptr<LevelRendererPlayer>);

    [[nodiscard]] LevelRendererPlayer* getLevelRendererPlayer() const { return mLevelRendererPlayer.get(); }
};
