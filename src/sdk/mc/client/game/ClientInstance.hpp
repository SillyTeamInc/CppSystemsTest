//
// Created by emi on 10/15/26.
//
#pragma once
#include <set>

#include "core/sdk_base.hpp"
#include "glm/vec2.hpp"
#include "sdk/mc/deps/core/utility/EnableNonOwnerReferences.hpp"
#include "util/memory.hpp"
#include "util/types.hpp"

class AbstractScene;
class Actor;
class ActorAnimationGroup;
class ActorBlockRenderer;
class ActorRenderDispatcher;
class ActorResourceDefinitionGroup;
class BlockActorRenderDispatcher;
class BlockCullingGroup;
class BlockSource;
class BlockTessellator;
class BuildActionIntention;
class CachedScenes;
class CameraRegistry;
class ClientHitDetectCoordinator;
class ClientInputHandler;
class ClientInstanceEventCoordinator;
class ClientLevel;
class ClientMoveInputHandler;
class ClientNetworkEventCoordinator;
class ClientNetworkSystem;
class ClientRequirementVerifier;
class ClientScriptEventCoordinator;
class ClientScriptManager;
class DateManager;
class DeferredLighting;
class DevConsoleLogger;
class DisconnectionRequestHandler;
class EDUSystems;
class EntityContext;
class EntitySystems;
class FileDataRequest;
class FogDefinitionRegistry;
class FogManager;
class FontHandle;
class FrameUpdateContext;
class GameCallbacks;
class GameModuleClient;
class GameRenderer;
class GeometryGroup;
class GuiData;
class GuidedFlowManager;
class HitDetectSystem;
class HitResult;
class HudIconActorRenderer;
class IClientInstances;
class IConnectionEventing;
class IContentKeyProvider;
class IGameConnectionListener;
class IMinecraftEventing;
class IMinecraftGame;
class IOptionRegistry;
class IReadWriteOptions;
class IResourcePackRepository;
class ISceneStack;
class ITTSEventManager;
class IUIRepository;
class ItemInHandRenderer;
class ItemRegistryRef;
class ItemRenderer;
class KeyboardManager;
class LatencyGraphDisplay;
class Level;
class LevelRenderer;
class LevelRendererCameraProxy;
class LightTexture;
class LinkedAssetValidator;
class LocalPlayer;
class MarketplaceServicesManager;
class Minecraft;
class MinecraftGraphics;
class MinecraftInputHandler;
class MobEffectsLayout;
class MusicManager;
class Option;
class OptionRegistry;
class PackManifestFactory;
class PacketSender;
class PersonaClient;
class PersonaRepository;
class PixelCalc;
class Player;
class PlayerAuthentication;
class PlayerReportHandler;
class ProfanityContext;
class ResourcePackManager;
class SceneFactory;
class ScreenContext;
class ScreenLoadTimeTracker;
class ShaderColor;
class SkinRepository;
class SoundEngine;
class StoreCatalogItem;
class StoreCatalogRepository;
class TaskGroup;
class TextToSpeechClient;
class ToastManager;
class TrialManager;
class UIEventCoordinator;
class Vec2;
class Vec3;
class WeakEntityRef;
class WorldTransferAgent;
template <typename T> class OwnerPtr;
template <typename TCopy, typename TPaste> class ClipboardProxy;
template <typename T> class WeakRef;
struct ActorUniqueID;
struct ClientInstanceInitArguments;
class ClientGameSetupResult;
struct ConnectionContextInfo;
struct DisconnectionScreenParams;
struct ListenerState;
struct LocalPlayerChangedConnector;
struct PacksInfoData;
struct PlayerJoinWorldContext;
struct ScreenshotOptions;
struct ServerSupportedAuthenticationTypes;
struct SplitScreenInfo;
class ProfanityFilterContext;
enum class SplitScreenDirection;
enum class BehaviorStatus;
enum class ClientInstanceState;
enum class ControlOptionType;
enum class GameType;
enum class InputMode;
enum class InventoryTabIndex;
enum class MarketplacePassTabIndex;
enum class NetworkType;
enum class OptionID;
enum class PlayScreenDefaultTab;
enum class StoreErrorCodes;
enum class StoreNavigationOrigin;
enum class SubClientId;
enum class WYSIWYGState;
namespace ApplicationSignal { class ClipboardCopy; }
namespace ApplicationSignal { class ClipboardPasteRequest; }
namespace Automation { class AutomationClient; }
namespace Bedrock {
template <typename T> class NonOwnerPointer;
template <typename T> class NotNullNonOwnerPtr;
template <typename T> class UniqueOwnerPointer;
namespace Input { enum class PointerType; }
namespace Threading { template <typename T> class Async; }
}
namespace Bedrock::Http { class Status; }
namespace Bedrock::Profiling { struct IProfilingOrchestratorProvider; }
namespace Core { class FileStorageArea; }
namespace Core { template <typename T> class PathBuffer; }
namespace Connection { enum class DisconnectFailReason; }
namespace MatrixStack { class MatrixStackRef; }
namespace Core { class Path; }
namespace Editor { class IEditorManager; }
namespace Editor { class IEditorPlayer; }
namespace cg::math { template <typename T> struct Rect; }
namespace OreUI { class ITelemetry; }
namespace OreUI { class Router; }
namespace OreUI { class SceneProvider; }
namespace OreUI { class UIBlockThumbnailAtlasManager; }
namespace PlayerCapabilities { struct IClientController; }
namespace Realms { struct World; }
namespace Scripting { class ScriptEngine; }
namespace Settings { class IRegistry; }
namespace Social { class GameConnectionInfo; }
namespace Social { class IUserManager; }
namespace Social { class MultiplayerServiceManager; }
namespace Social { enum class MultiplayerServiceIdentifier; }
namespace Social { class User; }
namespace ui { enum class GameEventNotification; }
namespace ui { enum class SceneType; }
namespace World { class WorldSystem; }
namespace mce { class Camera; }
namespace mce { class Texture; }
namespace mce { class TextureGroup; }
namespace mce { class TexturePtr; }
namespace mce { struct ViewportInfo; }
namespace ui { class ScreenTechStackSelector; }
namespace ui { enum class SceneType; }
enum class ClientPlayMode {};
namespace Core { class LevelStorageState; };

class IClientInstance : public ::Bedrock::EnableNonOwnerReferences {
public:
    // i want client symbols so bad >:c i feel bad referencing levi
    // this isn't supposed to be a cheat so whatevers
    // https://discord.com/users/921648530769182740 please!!!!

    // NOLINTBEGIN
    virtual ~IClientInstance() /*override*/ = default;
    virtual void onInitMinecraftGame() = 0;
    virtual void onDestroyMinecraftGame() = 0;
    virtual void init(::ClientInstanceInitArguments&& args) = 0;
    virtual void initSceneFactory(::std::unique_ptr<::SceneFactory> sceneFactory,::std::unique_ptr<::OreUI::SceneProvider> sceneProvider) = 0;
    virtual void setUiRouter(::std::unique_ptr<::OreUI::Router> uiRouter) = 0;
    virtual void initGraphics() = 0;
    virtual void stop() = 0;
    virtual void teardown() = 0;
    virtual void preShutDown() = 0;
    virtual void setGameConnectionInfo(::Social::GameConnectionInfo const& gameConnection) = 0;
    virtual ::std::optional<::Social::GameConnectionInfo> getGameConnectionInfo() = 0;
    virtual void onStartJoinGame(bool isLocalServer,::std::string const& multiplayerCorrelationId,::std::string const& serverName,::std::string const& worldName,::NetworkType networkType,::Social::MultiplayerServiceIdentifier service,::PlayerJoinWorldContext context) = 0;
    virtual void onCancelJoinGame() = 0;
    virtual void requestLeaveGameAsync() = 0;
    virtual void requestLeaveGame(bool switchScreen, bool sync) = 0;
    virtual void stopPlayScreen() = 0;
    virtual bool isLeaveGameDone() const = 0;
    virtual void setupPlayScreenForLeaveGame() = 0;
    virtual void preCacheOutOfGameViews() = 0;
    virtual void resetPrimaryClient() = 0;
    virtual void resetGameSession() = 0;
    virtual void tick() = 0;
    virtual void frameUpdate(::FrameUpdateContext& frameUpdateContext) = 0;
    virtual bool update(bool isInitFinished) = 0;
    virtual void preFrameTick() = 0;
    virtual void endFrame() = 0;
    virtual float getFrameAlpha() = 0;
    virtual void startSubClientLateJoin(bool hasXBLBroadcast,::std::unique_ptr<::GameModuleClient> gameModuleClient,::std::optional<::ConnectionContextInfo> primaryConnectionInfo) = 0;
    virtual ::Bedrock::Threading::Async<::ClientGameSetupResult> setupClientGame(bool joiningLocalServer,::ServerSupportedAuthenticationTypes supportedAuth,::std::unique_ptr<::GameModuleClient> gameModuleClient) = 0;
    virtual ::BlockSource* getRegion() = 0;
    virtual ::LocalPlayer* getLocalPlayer() const = 0;
    virtual void setupPrimaryClientEditorManager() = 0;
    virtual ::Bedrock::NonOwnerPointer<::Editor::IEditorPlayer> getLocalEditorPlayer() const = 0;
    virtual ::Bedrock::NonOwnerPointer<::Editor::IEditorManager> getEditorManager() const = 0;
    virtual bool isPlayerInEditor() const = 0;
    virtual ::LocalPlayerChangedConnector getLocalPlayerChangedConnector() = 0;
    virtual ::WeakEntityRef getLocalUser() const = 0;
    virtual ::Actor* getCameraActor() const = 0;
    virtual void setCameraActor(::Actor* cameraActor) = 0;
    virtual ::ListenerState const* getListenerState() const = 0;
    virtual ::Actor* getCameraTargetActor() const = 0;
    virtual void setCameraTargetActor(::Actor* cameraTargetActor) = 0;
    virtual ::WeakEntityRef getCameraEntity() const = 0;
    virtual void getRawCameraEntities(::WeakEntityRef& cameraEntity, ::WeakEntityRef& cameraTargetEntity) const = 0;
    virtual void setRawCameraEntities(::WeakEntityRef cameraEntity, ::WeakEntityRef cameraTargetEntity) = 0;
    virtual ::HitResult const& getLatestHitResult() const = 0;
    virtual bool isLeavingGame() const = 0;
    virtual bool isDestroyingGame() const = 0;
    virtual bool isShuttingDown() const = 0;
    virtual bool useLowFrequencyUIRender() const = 0;
    virtual bool isSplitScreenActive() const = 0;
    virtual bool isExitingLevel() const = 0;
    virtual bool isInBedScreen() const = 0;
    virtual bool isInDeathScreen() const = 0;
    virtual bool isKeyboardEnabled() const = 0;
    virtual bool hasCommands() const = 0;
    virtual int getSplitScreenCount() const = 0;
    virtual bool isShowingLoadingScreen() const = 0;
    virtual bool shouldDisconnectOnAppSuspended() const = 0;
    virtual bool isGamePlayTipsEnabled() const = 0;
    virtual bool isPlatformNX() const = 0;
    virtual bool isLocalSplitscreenWith(::ActorUniqueID const& id) const = 0;
    virtual bool isValidCrossPlatformSkin(bool valueIfNull) const = 0;
    virtual bool isSelectedSkinInitialized() const = 0;
    virtual ::SplitScreenInfo getSplitScreenInfo() const = 0;
    virtual bool getHandlingControllerDisconnect() = 0;
    virtual void setOpenControllerDisconnectScreen(bool open) = 0;
    virtual ::ClientPlayMode getClientPlayMode() const = 0;
    virtual void setClientPlayMode(::ClientPlayMode const& mode) = 0;
    virtual ::std::function<void()> getCreditsCallback() = 0;
    virtual void setCreditsCallback(::std::function<void()> callback) = 0;
    virtual void setupTransitionForCredits(::std::function<void()> callback) = 0;
    virtual void refreshScreenSizeData() = 0;
    virtual void onScreenSizeChanged(int width, int height) = 0;
    virtual void onGuiScaleOffsetChanged() = 0;
    virtual void onSafeZoneChanged() = 0;
    virtual bool hasDismissedNewPlayerFlow() const = 0;
    virtual void quit(::std::string const& src, ::std::string const& reason) = 0;
    virtual ::IMinecraftGame& getMinecraftGame_DEPRECATED() const = 0;
    virtual ::IClientInstances& getClientInstances() const = 0;
    virtual ::World::WorldSystem& getWorldSystem() const = 0;
    virtual bool isWorldSystemReady() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Automation::AutomationClient> getAutomationClient() const = 0;
    virtual ::Bedrock::NonOwnerPointer<::EDUSystems> getEDUSystems() = 0;
    virtual ::Bedrock::NonOwnerPointer<::EDUSystems const> getEDUSystems() const = 0;
    virtual ::IMinecraftEventing& getEventing() const = 0;
    virtual ::IConnectionEventing& getConnectionEventing() const = 0;
    virtual ::Bedrock::Profiling::IProfilingOrchestratorProvider& getProfilingOrchestratorProvider() const = 0;
    virtual ::FontHandle getFontHandle() const = 0;
    virtual ::FontHandle getRuneFontHandle() const = 0;
    virtual ::FontHandle getUnicodeFontHandle() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::GeometryGroup> getGeometryGroup() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::BlockCullingGroup> getBlockCullingGroup() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Social::MultiplayerServiceManager> getMultiplayerServiceManager() const = 0;
    virtual ::IResourcePackRepository& getResourcePackRepository() const = 0;
    virtual ::ResourcePackManager& getResourcePackManager() const = 0;
    virtual ::PackManifestFactory& getPackManifestFactory() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::IContentKeyProvider const> getKeyProvider() const = 0;
    virtual ::PacksInfoData const& getHostSpecifiedPacks() const = 0;
    virtual ::std::shared_ptr<::SkinRepository> getSkinRepository() const = 0;
    virtual ::PersonaClient& getPersonaClient() const = 0;
    virtual ::PersonaRepository& getPersonaRepository() const = 0;
    virtual ::MarketplaceServicesManager& getMarketplaceServicesManager() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::StoreCatalogRepository> getStoreCatalogRepository() = 0;
    virtual ::std::shared_ptr<::mce::TextureGroup> getTextureGroup() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::mce::TextureGroup> getStoreCacheTextures() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::MinecraftGraphics> getMinecraftGraphics() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::IUIRepository> getUIRepository() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Social::IUserManager> getUserManager() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::TrialManager> getTrialManager() const = 0;
    virtual bool wantToQuit() const = 0;
    virtual bool isPrimaryLevelCrossPlatformMultiplayer() const = 0;
    virtual bool isPrimaryLevelMultiplayer() const = 0;
    virtual bool isAdhocEnabled() const = 0;
    virtual ::std::shared_ptr<::ActorAnimationGroup> getActorAnimationGroup() const = 0;
    virtual bool isMarketplaceDisabled() const = 0;
    virtual ::std::pair<::StoreErrorCodes, ::std::string> const getMarketplaceDisabledReasonWithErrorCode() const = 0;
    virtual void linkToOffer(::std::string const& productId, bool allowWhileInGame) = 0;
    virtual void linkToPage(::std::string const& pageId) = 0;
    virtual void linkTo3PServerOffers(::std::string const& pageId, ::std::string const& creatorName) = 0;
    virtual void navigateToMarketplaceInventoryScreen(::InventoryTabIndex tabIndex) = 0;
    virtual void navigateToStoreHomeScreen() = 0;
    virtual void navigateToCoinPurchaseScreen(int neededCoins, ::std::function<void(bool, int)> callback) = 0;
    virtual void navigateToPurchaseOfferScreen(::StoreCatalogItem&, ::StoreNavigationOrigin, bool const) = 0;
    virtual void navigateToDressingRoomOfferScreen(::std::string const& offerId) = 0;
    virtual bool navigateToProfileScreen(::std::string const&, bool const) = 0;
    virtual void navigateToServersScreen(bool const calledFromHyperlink) = 0;
    virtual void navigateToHowToPlayScreen(::std::string const& startTopic) = 0;
    virtual void navigateToMarketplacePassPDPScreen(::MarketplacePassTabIndex tabIndex) = 0;
    virtual void navigateToRealmsStoriesTransitionScreen(::Realms::World const& world) = 0;
    virtual void tryPushLeaveGameScreen() = 0;
    virtual void tryStartDayOneExperience() = 0;
    virtual bool isReadyToRender() const = 0;
    virtual void onDimensionChangedEvent() = 0;
    virtual void onGameEventNotification(::ui::GameEventNotification notification) = 0;
    virtual ::std::string getTopScreenName() const = 0;
    virtual void setLeaveGameInProgressAsReadyToContinue() = 0;
    virtual void stopDestroying() = 0;
    virtual void onClientCreatedLevel(::std::pair<::std::unique_ptr<::Level>, ::OwnerPtr<::EntityContext>> levelEntity,::OwnerPtr<::EntityContext> userEntity) = 0;
    virtual ::PlayerAuthentication& getPlayerAuthentication() = 0;
    virtual void createPlayerAuthentication() = 0;
    virtual void createPlayerAuthentication(uint64_t clientRandomId) = 0;
    virtual ::std::string getPlatformId() const = 0;
    virtual ::std::string getPlatformOnlineId() const = 0;
    virtual bool useController() const = 0;
    virtual bool useTouchscreen() const = 0;
    virtual bool getMouseGrabbed() const = 0;
    virtual bool currentInputModeIsMouseAndKeyboard() const = 0;
    virtual bool allowPicking() const = 0;
    virtual bool isShowingMenu() const = 0;
    virtual bool isShowingPauseScreen() const = 0;
    virtual bool isShowingProgressScreen() const = 0;
    virtual bool isShowingWorldProgressScreen() const = 0;
    virtual bool isShowingRealmsProgressScreen() const = 0;
    virtual bool isShowingDeathScreen() const = 0;
    virtual bool isShowingServerForm() const = 0;
    virtual bool isScreenReplaceable() const = 0;
    virtual bool isInWorldAndNotShowingAnyMenuScreens() const = 0;
    virtual bool isWorldActive() const = 0;
    virtual bool isInRealm() = 0;
    virtual bool readyForShutdown() const = 0;
    virtual bool isPrimaryClient() const = 0;
    virtual bool isEduMode() const = 0;
    virtual bool isGamepadCursorEnabled() const = 0;
    virtual bool isInControlCustomization() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Minecraft> getServerData() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Minecraft const> getServerData() const = 0;
    virtual ::ClientLevel* getLevel() = 0;
    virtual ::ClientLevel const* getLevel() const = 0;
    virtual bool hasLevel() const = 0;
    virtual bool isPreGame() const = 0;
    virtual bool isInMultiplayerGame() const = 0;
    virtual bool isMultiPlayerClient() const = 0;
    virtual ::IOptionRegistry& getOptions() = 0;
    virtual ::IOptionRegistry const& getOptions() const = 0;
    virtual ::IReadWriteOptions const& getReadWriteOptions() const = 0;
    virtual ::std::shared_ptr<::OptionRegistry> getOptionsPtr() = 0;
    virtual ::std::shared_ptr<::OptionRegistry const> const getOptionsPtr() const = 0;
    virtual ::std::shared_ptr<::Social::User> const& getUser() const = 0;
    virtual ::std::shared_ptr<::Settings::IRegistry> getSettingsRegistry() = 0;
    virtual ::Option const& getShowLearningPromptsOption() const = 0;
    virtual ::Option& getShowControlTipsOption() = 0;
    virtual ::Option& getShowControlTipsOverrideOption() = 0;
    virtual double getControlTipsTimeElapsed() const = 0;
    virtual void setControlTipsTimeElapsedStart() = 0;
    virtual ::GameRenderer& getGameRenderer() const = 0;
    virtual ::LevelRenderer* getLevelRenderer() const = 0;
    virtual ::LevelRendererCameraProxy* getLevelRendererCameraProxy() const = 0;
    virtual ::Bedrock::NonOwnerPointer<::CameraRegistry> getCameraRegistry() = 0;
    virtual ::Bedrock::NonOwnerPointer<::CameraRegistry const> getCameraRegistry() const = 0;
    virtual ::Bedrock::NonOwnerPointer<::EntitySystems> getCameraSystems() = 0;
    virtual ::LightTexture* getLightTexture() = 0;
    virtual void setupLevelRendering(::ClientLevel& level, ::WeakEntityRef cameraTargetEntity) = 0;
    virtual ::mce::ViewportInfo const& getViewportInfo() const = 0;
    virtual void setViewportInfo(::mce::ViewportInfo const& viewportInfo) = 0;
    virtual ::Vec2 getNormalizedViewportSize() const = 0;
    virtual void updateChunkRadius() = 0;
    virtual void setUITexture(::mce::Texture* tex) = 0;
    virtual ::mce::Texture* getUITexture() = 0;
    virtual void setLevelTexture(::mce::Texture* tex) = 0;
    virtual ::mce::Texture* getLevelTexture() = 0;
    virtual void setUICursorTexture(::mce::TexturePtr tex) = 0;
    virtual ::mce::TexturePtr getUICursorTexture() const = 0;
    virtual ::mce::Camera& getCamera() = 0;
    virtual ::ShaderColor& getShaderColor() = 0;
    virtual ::ShaderColor& getDarkShaderColor() = 0;
    virtual void clearGraphicsCache() = 0;
    virtual void getNormalizedUICursorTransform(::MatrixStack::MatrixStackRef& matrix, float cursorHalfSizeTexels) = 0;
    virtual bool shouldRenderUICursor() const = 0;
    virtual bool getRenderPlayerModel() const = 0;
    virtual ::DeferredLighting& getDeferredLighting() = 0;
    virtual float getGuiScale() const = 0;
    virtual ::Option const& getGuiScaleOption() const = 0;
    virtual int getGuiScaleOffset() const = 0;
    virtual void setGuiScaleOffset(int guiScale) = 0;
    virtual void renderImGui(::ScreenContext& screenContext, bool drawMenuBar) = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::GuiData> getGuiData() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::GuiData const> getGuiData() const = 0;
    virtual ::GuidedFlowManager& getGuidedFlowManager() = 0;
    virtual ::PixelCalc const& getDpadScale() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::DateManager const> getDateManager() const = 0;
    virtual void addOverrideHoursToDateTime(unsigned int const hoursToAdd) = 0;
    virtual ::ToastManager& getToastManager() = 0;
    virtual ::ClipboardProxy<::ApplicationSignal::ClipboardCopy, ::ApplicationSignal::ClipboardPasteRequest>&getClipboardManager() = 0;
    virtual ::AbstractScene* getTopScene() = 0;
    virtual ::AbstractScene const* getTopScene() const = 0;
    virtual ::AbstractScene* getActiveScene() = 0;
    virtual ::AbstractScene const* getActiveScene() const = 0;
    virtual ::SceneFactory& getSceneFactory() const = 0;
    virtual ::OreUI::SceneProvider& getSceneProvider() const = 0;
    virtual ::WeakRef<::OreUI::UIBlockThumbnailAtlasManager> getOreUIBlockThumbnailAtlasManager() const = 0;
    virtual ::ui::ScreenTechStackSelector& getScreenTechStackSelector() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack const> getClientSceneStack() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack> getClientSceneStack() = 0;
    virtual ::OreUI::Router& getClientUIRouter() const = 0;
    virtual ::ISceneStack& getMainSceneStackInterface() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack> getMainSceneStack() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack const> getMainSceneStack() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack const> getCurrentSceneStack() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack> getCurrentSceneStack() = 0;
    virtual ::OreUI::Router& getCurrentUIRouter() = 0;
    virtual ::OreUI::Router const& getCurrentUIRouter() const = 0;
    virtual ::OreUI::ITelemetry& getOreUITelemetry() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack> prepareSceneFor(::ui::SceneType const sceneTypeToFind,::std::function<::std::shared_ptr<::AbstractScene>()> createScreenCallback) = 0;
    virtual ::CachedScenes& getCachedScenes() = 0;
    virtual ::std::string getScreenName() const = 0;
    virtual ::std::string getScreenTelemetry() const = 0;
    virtual ::ui::SceneType getTopSceneType() const = 0;
    virtual ::MobEffectsLayout& getMobEffectsLayout() = 0;
    virtual ::std::string emoticonifyText(::std::string const& text) const = 0;
    virtual ::std::string emoticonifyText(::std::string const& text, bool isGameTip) = 0;
    virtual void onMobEffectsChange() = 0;
    virtual void setUISize(int w, int h) = 0;
    virtual void forEachVisibleScreen(/*::brstd::function_ref<void(::AbstractScene&)> callback*/) = 0;
    virtual void forEachScreen(/*::brstd::function_ref<bool(::AbstractScene&)> callback, bool topDown*/) = 0;
    virtual void forEachScreenConst(/*::brstd::function_ref<bool(::AbstractScene const&)> callback, bool topDown*/) const = 0;
    virtual bool updateSceneStack() = 0;
    virtual void forEachAlwaysAcceptInputScreen(::std::function<void(::AbstractScene&)> callback,::AbstractScene const* ignoreScreen) = 0;
    virtual void forEachAlwaysAcceptInputScreenWithTop(::std::function<void(::AbstractScene&)> callback) = 0;
    virtual void showPlayerProfile(::std::string const& xuid, ::std::string const& platformId) = 0;
    virtual bool isInGameInputEnabled() const = 0;
    virtual void setInGameInputEnabled(bool isEnabled) = 0;
    virtual ::Vec2 getSafeZoneScale() const = 0;
    virtual void verifySkinApproval(::std::function<void(::std::string)> const& notApprovedCallback,::std::function<void()> const& approvedCallback) const = 0;
    virtual ::InputMode getCurrentInputMode() const = 0;
    virtual bool isTouchGameplayAllowed() const = 0;
    virtual ::Bedrock::NonOwnerPointer<::MinecraftInputHandler> getMinecraftInput() const = 0;
    virtual ::KeyboardManager& getKeyboardManager() = 0;
    virtual void setLastPointerLocation(float x, float y, float z) = 0;
    virtual ::Vec3 getLastPointerLocation() = 0;
    virtual void clearTouchPointerLocations() = 0;
    virtual void clearTouchPointerLocation(int id) = 0;
    virtual void updateControlOptionState(uint id, bool active) = 0;
    virtual void clearActiveControlOptions() = 0;
    virtual std::set<uint> const& getActiveControlOptions() const = 0;
    virtual ::std::vector<::OptionID> getActiveOptionIDs(::ControlOptionType controlOptionType) const = 0;
    virtual void setNumberOfActiveConfigs(int numberOfActiveConfigs) = 0;
    virtual int getNumberOfActiveConfigs() const = 0;
    virtual bool isNoConfigSelected() const = 0;
    virtual void setNumberOfEnabledConfigs(int numberOfEnabledConfigs) = 0;
    virtual int getNumberOfEnabledConfigs() const = 0;
    virtual void setWYSIWYGState(::WYSIWYGState wysiwygState) = 0;
    virtual ::WYSIWYGState getWYSIWYGState() const = 0;
    virtual void setOtherConfigsExistInThisCategory(bool otherConfigsExistInThisCategory) = 0;
    virtual bool getOtherConfigsExistInThisCategory() const = 0;
    virtual void updateTouchPointerLocation(int id, float x, float y) = 0;
    virtual ::Vec2 getTouchPointerLocation(int id) const = 0;
    virtual void updateActionPointerId(int id) = 0;
    virtual int getActionPointerId() const = 0;
    virtual bool shouldUseLastPointerLocationOnFocusChange() = 0;
    virtual bool currentScreenShouldStealMouse() = 0;
    virtual ::BuildActionIntention& getInProgressBAI() const = 0;
    virtual ::PacketSender& getPacketSender() = 0;
    virtual ::ClientNetworkSystem& getClientNetworkSystem() = 0;
    virtual ::ClientNetworkSystem const& getClientNetworkSystem() const = 0;
    virtual void setMoveTurnInput(::std::unique_ptr<::ClientMoveInputHandler> pClientMoveInputHandler) = 0;
    virtual void setupPersistentControls(::InputMode currentMode) = 0;
    virtual void resetPlayerMovement() = 0;
    virtual void suspendPredictedMovement() = 0;
    virtual void onClientInputInitComplete() = 0;
    virtual ::glm::vec2 getGamepadCursorPosition() const = 0;
    virtual void setClientInputHandler(::std::unique_ptr<::ClientInputHandler> pClientInputHandler) = 0;
    virtual ::ClientInputHandler* getInput() const = 0;
    virtual int getControllerId() const = 0;
    virtual bool hasConnectedController() const = 0;
    virtual ::SubClientId getClientSubId() const = 0;
    virtual void setSuspendInput(bool suspendInput) = 0;
    virtual void setSuspendDirectionalInput(bool suspendDirectionalInput) = 0;
    virtual void setDisableInput(bool disableInput) = 0;
    virtual void grabMouse() = 0;
    virtual void releaseMouse() = 0;
    virtual void refocusMouse(bool lostMouse) = 0;
    virtual void setMouseType(::Bedrock::Input::PointerType type) = 0;
    virtual void resetBai(int baiFlags) = 0;
    virtual void clearInProgressBAI() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::SoundEngine> getSoundEngine() const = 0;
    virtual ::MusicManager const* getMusicManager() const = 0;
    virtual ::MusicManager* getMusicManagerNonConst() const = 0;
    virtual void play(::std::string const& name, ::Vec3 const& pos, float volume, float pitch) = 0;
    virtual void playUI(::std::string const& name, float volume, float pitch) = 0;
    virtual void muteAudio() = 0;
    virtual void unMuteAudio() = 0;
    virtual void fadeOutMusic() const = 0;
    virtual ::TaskGroup& getTaskGroup() = 0;
    virtual void onFullVanillaPackOnStack() = 0;
    virtual bool isFullVanillaPackOnStack() const = 0;
    virtual void onPlayerLoaded(::Player& player) = 0;
    virtual void setClientGameMode(::GameType gameType) = 0;
    virtual void resetToDefaultGameMode() = 0;
    virtual ::IGameConnectionListener& getGameConnectionListener() = 0;
    virtual void connectToThirdPartyServer(::std::string const& ipAddress, int port) = 0;
    virtual void startExternalNetworkWorld(::Social::GameConnectionInfo connection,::std::string const& serverName,::PlayerJoinWorldContext context) = 0;
    virtual bool isReadyToReconnect() const = 0;
    virtual bool checkForPiracy() = 0;
    virtual void updateChatFilterStatus(::ProfanityContext& profanityContext) = 0;
    virtual void updateControllerHandling() = 0;
    virtual void setBehaviorCommandCallback(::std::function<void(::std::string const&, ::BehaviorStatus)> callback) = 0;
    virtual void setBehaviorCommandStatus(::std::string const& treeName, ::BehaviorStatus status) = 0;
    virtual void setConnectGamepadScreenActive(bool active) = 0;
    virtual ::Bedrock::Threading::Async<::Core::PathBuffer<::std::string>>requestScreenshot(::ScreenshotOptions& screenshotOptions) = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::DevConsoleLogger> getDevConsoleLogger() const = 0;
    virtual ::std::shared_ptr<::FileDataRequest> requestImageFromUrl(::std::string const& imageUrl,::std::function<void(::Bedrock::Http::Status, ::Core::Path const&, uint64)> callback) = 0;
    virtual void initializeRenderResources() = 0;
    virtual void postInitRenderResources() = 0;
    virtual void onAppSuspended() = 0;
    virtual void onAppResumed() = 0;
    virtual void onActiveResourcePacksChanged(::Bedrock::NotNullNonOwnerPtr<::ActorResourceDefinitionGroup> const& entityResourceDefGroup) = 0;
    virtual void reloadEntityRenderers(::Bedrock::NotNullNonOwnerPtr<::ActorResourceDefinitionGroup> const& actorResourceDefinitionGroup);
    virtual ::BlockTessellator& getBlockTessellator() = 0;
    virtual ::BlockActorRenderDispatcher& getBlockEntityRenderDispatcher() = 0;
    virtual ::std::shared_ptr<::ActorRenderDispatcher> getEntityRenderDispatcher() = 0;
    virtual ::ActorBlockRenderer& getEntityBlockRenderer() = 0;
    virtual ::ItemInHandRenderer* getItemInHandRenderer() = 0;
    virtual ::ItemRenderer* getItemRenderer() = 0;
    virtual ::HudIconActorRenderer* getHudIconActorRenderer() = 0;
    virtual ::std::deque<::std::string>& getSentMessageHistory() = 0;
    virtual ::std::deque<::std::string>& getDevConsoleMessageHistory() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ProfanityContext> getProfanityContext() const = 0;
    virtual void initTTSClient(::std::shared_ptr<::TextToSpeechClient> textToSpeechClient) = 0;
    virtual ::std::shared_ptr<::TextToSpeechClient> getTTSClient() = 0;
    virtual ::std::shared_ptr<::TextToSpeechClient const> getTTSClient() const = 0;
    virtual ::std::shared_ptr<::ITTSEventManager> getTTSEventManager() = 0;
    virtual void addTTSMessage(::std::string const& message,::ProfanityFilterContext profanityFilterContext,bool interruptible,::std::string const& interruptibleId,bool required,bool disregardAppFocus) = 0;
    virtual void initCommands() = 0;
    virtual uint getUserId() const = 0;
    virtual bool isPrimaryUser() const = 0;
    virtual double getServerConnectionTime() const = 0;
    virtual void setServerPingTime(::std::chrono::milliseconds pingTime) = 0;
    virtual ::std::chrono::milliseconds getServerPingTime() const = 0;
    virtual void setDefaultPlayscreenTab(::PlayScreenDefaultTab defaultTab) = 0;
    virtual void setClientInstanceState(::ClientInstanceState const& newstate) = 0;
    virtual void setUIEventCoordinator(::Bedrock::UniqueOwnerPointer<::UIEventCoordinator>&& coordinator) = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::UIEventCoordinator> getUIEventCoordinator() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ClientInstanceEventCoordinator> getEventCoordinator() = 0;
    virtual ::ClientNetworkEventCoordinator& getClientNetworkEventCoordinator() = 0;
    virtual ::ClientScriptEventCoordinator& getClientScriptEventCoordinator() = 0;
    virtual ::std::chrono::steady_clock::time_point getNoBlockBreakUntil() = 0;
    virtual void setNoBlockBreakUntil(::std::chrono::steady_clock::time_point timePoint) = 0;
    virtual ::GameModuleClient* getGameModule() = 0;
    virtual ::ClientHitDetectCoordinator& getHitEventCoordinator() = 0;
    virtual ::GameCallbacks& getGameCallbacks() = 0;
    virtual void setActiveFileStorageArea(::std::shared_ptr<::Core::FileStorageArea> storageArea) = 0;
    virtual void sendClientEnteredLevel() = 0;
    virtual ::HitDetectSystem* getHitDetectSystem() = 0;
    virtual bool isPlaying() const = 0;
    virtual ::ClientInstanceState getClientInstanceState() const = 0;
    virtual ::LatencyGraphDisplay* getLatencyGraphDisplay() const = 0;
    virtual ::PlayerCapabilities::IClientController const& getClientCapabilities() const = 0;
    virtual ::cg::math::Rect<float> calculateViewPortModifiers(::SubClientId const clientId,uint64 const clientCount,::SplitScreenDirection const splitScreenConfiguration) const = 0;
    virtual ::std::weak_ptr<::IClientInstance> getWeakPtrToThis() = 0;
    virtual ::ClientRequirementVerifier const& getClientRequirementVerifier() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::FogDefinitionRegistry const> getFogDefinitionRegistry() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::FogDefinitionRegistry> getFogDefinitionRegistry() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::FogManager const> getFogManager() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::FogManager> getFogManager() = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ScreenLoadTimeTracker> getScreenLoadTimeTracker() = 0;
    virtual ::ItemRegistryRef getItemRegistry() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::DisconnectionRequestHandler> getDisconnectionRequestHandler() const = 0;
    virtual float getRemoteServerTimeMs() const = 0;
    virtual void setRemoteServerTimeMs(float time) = 0;
    virtual float getRemoteServerNetworkTimeMs() const = 0;
    virtual void setRemoteServerNetworkTimeMs(float time) = 0;
    virtual ::Bedrock::NonOwnerPointer<::ClientScriptManager> getClientScriptManager() = 0;
    virtual ::Scripting::ScriptEngine* getScriptingEngine() = 0;
    virtual ::Bedrock::NonOwnerPointer<::WorldTransferAgent> const getWorldTransferAgent() const = 0;
    virtual ::Bedrock::NotNullNonOwnerPtr<::PlayerReportHandler> getPlayerReportHandler() = 0;
    virtual void flagDisconnectionAndNotify(::Connection::DisconnectFailReason disconnectReason) = 0;
    virtual void flagDisconnectionAndNotifyWithParams(::DisconnectionScreenParams const& params) = 0;
    virtual void disconnectSubClient(::Connection::DisconnectFailReason disconnectReason) = 0;
    virtual void setClientUpdateAndRenderThrottling(bool enabled, int threshold, float scalar) = 0;
    virtual bool isClientUpdateAndRenderThrottlingEnabled() const = 0;
    virtual int getClientUpdateAndRenderThrottlingThreshold() const = 0;
    virtual float getClientUpdateAndRenderThrottlingScalar() const = 0;
    virtual bool isUserBanned() const = 0;
    virtual void setupPauseManagers() = 0;
    virtual bool isEligibleForPauseFeature() const = 0;
    virtual bool requestInGamePause(bool status) = 0;
    virtual void openContentLogHistory() = 0;
    virtual double getGameUpdateDurationInSeconds() const = 0;
    virtual ::std::optional<::ConnectionContextInfo> getConnectionContextInfo() const = 0;
    virtual ::Bedrock::NonOwnerPointer<::LinkedAssetValidator> getLinkedAssetValidator() = 0;
    // NOLINTEND
};

class IPersonaClientProvider {
public:
    // virtual functions
    // NOLINTBEGIN
    virtual ::Bedrock::NonOwnerPointer<::PersonaClient> tryGetPersonaClient() const = 0;
    // NOLINTEND
};

namespace Core {
    class StorageAreaStateListener {
    public:
        // NOLINTBEGIN
        virtual ~StorageAreaStateListener();
        virtual void onExtendDiskSpace(
            bool const bSet,
            ::std::weak_ptr<::Core::FileStorageArea> const &storageAreaWeakPtr,
            uint64 freeSpace,
            ::std::function<void()> onHandledEventCallback
        );
        virtual void onLowDiskSpace(bool const bSet, uint64 freeSpace);
        virtual void onOutOfDiskSpace(bool const bSet, uint64 freeSpace);
        virtual void onCriticalDiskError(bool const bSet, ::Core::LevelStorageState const &errorCode);
        // NOLINTEND
    };
}

class PlayerListener {
public:
    // NOLINTBEGIN
    virtual ~PlayerListener() = default;
    virtual void onWillChangeDimension(::Player& player);
    virtual void onDimensionChanged(::Player& player);
    virtual void onPlayerDestruction(::Player& player);
    // NOLINTEND
};




class GameCallbacks {
public:
    // virtual functions
    // NOLINTBEGIN
    virtual ~GameCallbacks() = default;
    virtual void onLevelCorrupt() = 0;
    virtual void onGameModeChanged() = 0;
    virtual void onBeforeSimTick();
    virtual void onTick(int nTick, int maxTick) = 0;
    virtual void onInternetUpdate() = 0;
    virtual void onGameSessionReset() = 0;
    virtual void onLevelExit() = 0;
    virtual void onRequestResourceReload() = 0;
    virtual void updateScreens() = 0;
    // NOLINTEND
};



// note that this doesn't include all of the base classes
class ClientInstance : public ::IClientInstance,
                       public ::IPersonaClientProvider,
                       public ::Core::StorageAreaStateListener,
                       public ::GameCallbacks,
                       public ::PlayerListener,
                       public ::std::enable_shared_from_this<::ClientInstance>, SDK_CLASS {
public:
    // NOLINTBEGIN
    virtual ~ClientInstance() /*override*/ = default;
    virtual void onInitMinecraftGame() /*override*/;
    virtual void onDestroyMinecraftGame() /*override*/;
    virtual void init(::ClientInstanceInitArguments&& args) /*override*/;
    virtual void initSceneFactory(
        ::std::unique_ptr<::SceneFactory>         sceneFactory,
        ::std::unique_ptr<::OreUI::SceneProvider> sceneProvider
    ) /*override*/;
    virtual void setUiRouter(::std::unique_ptr<::OreUI::Router> uiRouter) /*override*/;
    virtual void initGraphics() /*override*/;
    virtual void stop() /*override*/;
    virtual void teardown() /*override*/;
    virtual void preShutDown() /*override*/;
    virtual void setGameConnectionInfo(::Social::GameConnectionInfo const& gameConnection) /*override*/;
    virtual ::std::optional<::Social::GameConnectionInfo> getGameConnectionInfo() /*override*/;
    virtual void onStartJoinGame(
        bool                                   isLocalServer,
        ::std::string const&                   multiplayerCorrelationId,
        ::std::string const&                   serverName,
        ::std::string const&                   worldName,
        ::NetworkType                          networkType,
        ::Social::MultiplayerServiceIdentifier service,
        ::PlayerJoinWorldContext               context
    ) /*override*/;
    virtual void onCancelJoinGame() /*override*/;
    virtual void requestLeaveGameAsync() /*override*/;
    virtual void requestLeaveGame(bool switchScreen, bool sync) /*override*/;
    virtual void stopPlayScreen() /*override*/;
    virtual bool isLeaveGameDone() const /*override*/;
    virtual void setupPlayScreenForLeaveGame() /*override*/;
    virtual void preCacheOutOfGameViews() /*override*/;
    virtual void resetPrimaryClient() /*override*/;
    virtual void resetGameSession() /*override*/;
    virtual void tick() /*override*/;
    virtual void frameUpdate(::FrameUpdateContext& frameUpdateContext) /*override*/;
    virtual void preFrameTick() /*override*/;
    virtual bool update(bool isInitFinished) /*override*/;
    virtual void endFrame() /*override*/;
    virtual float getFrameAlpha() /*override*/;
    virtual void startSubClientLateJoin(
        bool                                     hasXBLBroadcast,
        ::std::unique_ptr<::GameModuleClient>    gameModuleClient,
        ::std::optional<::ConnectionContextInfo> primaryConnectionInfo
    ) /*override*/;
    virtual ::Bedrock::Threading::Async<::ClientGameSetupResult> setupClientGame(
        bool                                  joiningLocalServer,
        ::ServerSupportedAuthenticationTypes  supportedAuth,
        ::std::unique_ptr<::GameModuleClient> gameModuleClient
    ) /*override*/;
    virtual ::BlockSource* getRegion() /*override*/;
    virtual ::LocalPlayer* getLocalPlayer() const /*override*/;
    virtual void setupPrimaryClientEditorManager() /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::Editor::IEditorPlayer> getLocalEditorPlayer() const /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::Editor::IEditorManager> getEditorManager() const /*override*/;
    virtual bool isPlayerInEditor() const /*override*/;
    virtual ::LocalPlayerChangedConnector getLocalPlayerChangedConnector() /*override*/;
    virtual ::WeakEntityRef getLocalUser() const /*override*/;
    virtual ::Actor* getCameraActor() const /*override*/;
    virtual void setCameraActor(::Actor* cameraActor) /*override*/;
    virtual ::ListenerState const* getListenerState() const /*override*/;
    virtual ::Actor* getCameraTargetActor() const /*override*/;
    virtual void setCameraTargetActor(::Actor* cameraTargetActor) /*override*/;
    virtual ::WeakEntityRef getCameraEntity() const /*override*/;
    virtual void getRawCameraEntities(::WeakEntityRef& cameraEntity, ::WeakEntityRef& cameraTargetEntity) const
        /*override*/;
    virtual void setRawCameraEntities(::WeakEntityRef cameraEntity, ::WeakEntityRef cameraTargetEntity) /*override*/;
    virtual ::HitResult const& getLatestHitResult() const /*override*/;
    virtual bool isLeavingGame() const /*override*/;
    virtual bool isDestroyingGame() const /*override*/;
    virtual bool isShuttingDown() const /*override*/;
    virtual bool useLowFrequencyUIRender() const /*override*/;
    virtual bool isSplitScreenActive() const /*override*/;
    virtual bool isExitingLevel() const /*override*/;
    virtual bool isInBedScreen() const /*override*/;
    virtual bool isInDeathScreen() const /*override*/;
    virtual bool isKeyboardEnabled() const /*override*/;
    virtual bool hasCommands() const /*override*/;
    virtual int getSplitScreenCount() const /*override*/;
    virtual bool isShowingLoadingScreen() const /*override*/;
    virtual bool shouldDisconnectOnAppSuspended() const /*override*/;
    virtual bool isGamePlayTipsEnabled() const /*override*/;
    virtual bool isPlatformNX() const /*override*/;
    virtual bool isLocalSplitscreenWith(::ActorUniqueID const& id) const /*override*/;
    virtual bool isValidCrossPlatformSkin(bool valueIfNull) const /*override*/;
    virtual bool isSelectedSkinInitialized() const /*override*/;
    virtual ::SplitScreenInfo getSplitScreenInfo() const /*override*/;
    virtual bool getHandlingControllerDisconnect() /*override*/;
    virtual void setOpenControllerDisconnectScreen(bool open) /*override*/;
    virtual ::ClientPlayMode getClientPlayMode() const /*override*/;
    virtual void setClientPlayMode(::ClientPlayMode const& mode) /*override*/;
    virtual ::std::function<void()> getCreditsCallback() /*override*/;
    virtual void setCreditsCallback(::std::function<void()> callback) /*override*/;
    virtual void setupTransitionForCredits(::std::function<void()> callback) /*override*/;
    virtual void refreshScreenSizeData() /*override*/;
    virtual void onScreenSizeChanged(int width, int height) /*override*/;
    virtual void onGuiScaleOffsetChanged() /*override*/;
    virtual void onSafeZoneChanged() /*override*/;
    virtual bool hasDismissedNewPlayerFlow() const /*override*/;
    virtual void quit(::std::string const& src, ::std::string const& reason) /*override*/;
    virtual ::IMinecraftGame& getMinecraftGame_DEPRECATED() const /*override*/;
    virtual ::IClientInstances& getClientInstances() const /*override*/;
    virtual ::World::WorldSystem& getWorldSystem() const /*override*/;
    virtual bool isWorldSystemReady() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Automation::AutomationClient> getAutomationClient() const /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::EDUSystems> getEDUSystems() /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::EDUSystems const> getEDUSystems() const /*override*/;
    virtual ::IMinecraftEventing& getEventing() const /*override*/;
    virtual ::IConnectionEventing& getConnectionEventing() const /*override*/;
    virtual ::Bedrock::Profiling::IProfilingOrchestratorProvider& getProfilingOrchestratorProvider() const /*override*/;
    virtual ::FontHandle getFontHandle() const /*override*/;
    virtual ::FontHandle getRuneFontHandle() const /*override*/;
    virtual ::FontHandle getUnicodeFontHandle() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::GeometryGroup> getGeometryGroup() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::BlockCullingGroup> getBlockCullingGroup() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Social::MultiplayerServiceManager> getMultiplayerServiceManager() const
        /*override*/;
    virtual ::IResourcePackRepository& getResourcePackRepository() const /*override*/;
    virtual ::ResourcePackManager& getResourcePackManager() const /*override*/;
    virtual ::PackManifestFactory& getPackManifestFactory() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::IContentKeyProvider const> getKeyProvider() const /*override*/;
    virtual ::PacksInfoData const& getHostSpecifiedPacks() const /*override*/;
    virtual ::std::shared_ptr<::SkinRepository> getSkinRepository() const /*override*/;
    virtual ::PersonaRepository& getPersonaRepository() const /*override*/;
    virtual ::MarketplaceServicesManager& getMarketplaceServicesManager() const /*override*/;
    virtual ::PersonaClient& getPersonaClient() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::StoreCatalogRepository> getStoreCatalogRepository() /*override*/;
    virtual ::std::shared_ptr<::mce::TextureGroup> getTextureGroup() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::mce::TextureGroup> getStoreCacheTextures() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::MinecraftGraphics> getMinecraftGraphics() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::IUIRepository> getUIRepository() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Social::IUserManager> getUserManager() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::TrialManager> getTrialManager() const /*override*/;
    virtual bool wantToQuit() const /*override*/;
    virtual bool isPrimaryLevelCrossPlatformMultiplayer() const /*override*/;
    virtual bool isPrimaryLevelMultiplayer() const /*override*/;
    virtual bool isAdhocEnabled() const /*override*/;
    virtual ::std::shared_ptr<::ActorAnimationGroup> getActorAnimationGroup() const /*override*/;
    virtual bool isMarketplaceDisabled() const /*override*/;
    virtual ::std::pair<::StoreErrorCodes, ::std::string> const getMarketplaceDisabledReasonWithErrorCode() const
        /*override*/;
    virtual void linkToOffer(::std::string const& productId, bool allowWhileInGame) /*override*/;
    virtual void linkToPage(::std::string const& pageId) /*override*/;
    virtual void linkTo3PServerOffers(::std::string const& pageId, ::std::string const& creatorName) /*override*/;
    virtual void navigateToMarketplaceInventoryScreen(::InventoryTabIndex tabIndex) /*override*/;
    virtual void navigateToStoreHomeScreen() /*override*/;
    virtual void navigateToCoinPurchaseScreen(int neededCoins, ::std::function<void(bool, int)> callback) /*override*/;
    virtual void
    navigateToPurchaseOfferScreen(::StoreCatalogItem& item, ::StoreNavigationOrigin, bool const) /*override*/;
    virtual void navigateToDressingRoomOfferScreen(::std::string const& offerId) /*override*/;
    virtual bool navigateToProfileScreen(::std::string const&, bool const preventProgressScreen) /*override*/;
    virtual void navigateToServersScreen(bool const calledFromHyperlink) /*override*/;
    virtual void navigateToHowToPlayScreen(::std::string const& startTopic) /*override*/;
    virtual void navigateToMarketplacePassPDPScreen(::MarketplacePassTabIndex tabIndex) /*override*/;
    virtual void navigateToRealmsStoriesTransitionScreen(::Realms::World const& world) /*override*/;
    virtual void tryPushLeaveGameScreen() /*override*/;
    virtual void tryStartDayOneExperience() /*override*/;
    virtual bool isReadyToRender() const /*override*/;
    virtual void onDimensionChangedEvent() /*override*/;
    virtual void onGameEventNotification(::ui::GameEventNotification notification) /*override*/;
    virtual ::std::string getTopScreenName() const /*override*/;
    virtual void setLeaveGameInProgressAsReadyToContinue() /*override*/;
    virtual void stopDestroying() /*override*/;
    virtual void onClientCreatedLevel(
        ::std::pair<::std::unique_ptr<::Level>, ::OwnerPtr<::EntityContext>> levelEntity,
        ::OwnerPtr<::EntityContext>                                          userEntity
    ) /*override*/;
    virtual ::PlayerAuthentication& getPlayerAuthentication() /*override*/;
    virtual void createPlayerAuthentication() /*override*/;
    virtual void createPlayerAuthentication(uint64 clientRandomId) /*override*/;
    virtual ::std::string getPlatformId() const /*override*/;
    virtual ::std::string getPlatformOnlineId() const /*override*/;
    virtual bool useController() const /*override*/;
    virtual bool useTouchscreen() const /*override*/;
    virtual bool getMouseGrabbed() const /*override*/;
    virtual bool currentInputModeIsMouseAndKeyboard() const /*override*/;
    virtual bool allowPicking() const /*override*/;
    virtual bool isShowingMenu() const /*override*/;
    virtual bool isShowingPauseScreen() const /*override*/;
    virtual bool isShowingProgressScreen() const /*override*/;
    virtual bool isShowingWorldProgressScreen() const /*override*/;
    virtual bool isShowingRealmsProgressScreen() const /*override*/;
    virtual bool isShowingDeathScreen() const /*override*/;
    virtual bool isShowingServerForm() const /*override*/;
    virtual bool isScreenReplaceable() const /*override*/;
    virtual bool isInWorldAndNotShowingAnyMenuScreens() const /*override*/;
    virtual bool isWorldActive() const /*override*/;
    virtual bool isInRealm() /*override*/;
    virtual bool readyForShutdown() const /*override*/;
    virtual bool isPrimaryClient() const /*override*/;
    virtual bool isEduMode() const /*override*/;
    virtual bool isGamepadCursorEnabled() const /*override*/;
    virtual bool isInControlCustomization() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Minecraft> getServerData() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::Minecraft const> getServerData() const /*override*/;
    virtual ::ClientLevel* getLevel() /*override*/;
    virtual ::ClientLevel const* getLevel() const /*override*/;
    virtual bool hasLevel() const /*override*/;
    virtual bool isPreGame() const /*override*/;
    virtual bool isInMultiplayerGame() const /*override*/;
    virtual bool isMultiPlayerClient() const /*override*/;
    virtual ::IOptionRegistry& getOptions() /*override*/;
    virtual ::IOptionRegistry const& getOptions() const /*override*/;
    virtual ::IReadWriteOptions const& getReadWriteOptions() const /*override*/;
    virtual ::std::shared_ptr<::OptionRegistry> getOptionsPtr() /*override*/;
    virtual ::std::shared_ptr<::OptionRegistry const> const getOptionsPtr() const /*override*/;
    virtual ::std::shared_ptr<::Social::User> const& getUser() const /*override*/;
    virtual ::std::shared_ptr<::Settings::IRegistry> getSettingsRegistry() /*override*/;
    virtual ::Option const& getShowLearningPromptsOption() const /*override*/;
    virtual ::Option& getShowControlTipsOption() /*override*/;
    virtual ::Option& getShowControlTipsOverrideOption() /*override*/;
    virtual double getControlTipsTimeElapsed() const /*override*/;
    virtual void setControlTipsTimeElapsedStart() /*override*/;
    virtual ::GameRenderer& getGameRenderer() const /*override*/;
    virtual ::LevelRenderer* getLevelRenderer() const /*override*/;
    virtual ::LevelRendererCameraProxy* getLevelRendererCameraProxy() const /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::CameraRegistry> getCameraRegistry() /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::CameraRegistry const> getCameraRegistry() const /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::EntitySystems> getCameraSystems() /*override*/;
    virtual ::LightTexture* getLightTexture() /*override*/;
    virtual void setupLevelRendering(::ClientLevel& level, ::WeakEntityRef cameraTargetEntity) /*override*/;
    virtual ::mce::ViewportInfo const& getViewportInfo() const /*override*/;
    virtual void setViewportInfo(::mce::ViewportInfo const& viewportInfo) /*override*/;
    virtual ::Vec2 getNormalizedViewportSize() const /*override*/;
    virtual void updateChunkRadius() /*override*/;
    virtual void setUITexture(::mce::Texture* tex) /*override*/;
    virtual ::mce::Texture* getUITexture() /*override*/;
    virtual void setLevelTexture(::mce::Texture* tex) /*override*/;
    virtual ::mce::Texture* getLevelTexture() /*override*/;
    virtual void setUICursorTexture(::mce::TexturePtr tex) /*override*/;
    virtual ::mce::TexturePtr getUICursorTexture() const /*override*/;
    virtual ::mce::Camera& getCamera() /*override*/;
    virtual ::ShaderColor& getShaderColor() /*override*/;
    virtual ::ShaderColor& getDarkShaderColor() /*override*/;
    virtual void clearGraphicsCache() /*override*/;
    virtual void
    getNormalizedUICursorTransform(::MatrixStack::MatrixStackRef& matrix, float cursorHalfSizeTexels) /*override*/;
    virtual bool shouldRenderUICursor() const /*override*/;
    virtual bool getRenderPlayerModel() const /*override*/;
    virtual ::DeferredLighting& getDeferredLighting() /*override*/;
    virtual float getGuiScale() const /*override*/;
    virtual ::Option const& getGuiScaleOption() const /*override*/;
    virtual int getGuiScaleOffset() const /*override*/;
    virtual void setGuiScaleOffset(int guiScale) /*override*/;
    virtual void renderImGui(::ScreenContext& screenContext, bool drawMenuBar) /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::GuiData> getGuiData() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::GuiData const> getGuiData() const /*override*/;
    virtual ::GuidedFlowManager& getGuidedFlowManager() /*override*/;
    virtual ::PixelCalc const& getDpadScale() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::DateManager const> getDateManager() const /*override*/;
    virtual void addOverrideHoursToDateTime(uint const hoursToAdd) /*override*/;
    virtual ::ToastManager& getToastManager() /*override*/;
    virtual ::ClipboardProxy<::ApplicationSignal::ClipboardCopy, ::ApplicationSignal::ClipboardPasteRequest>&
    getClipboardManager() /*override*/;
    virtual ::AbstractScene* getTopScene() /*override*/;
    virtual ::AbstractScene const* getTopScene() const /*override*/;
    virtual ::AbstractScene* getActiveScene() /*override*/;
    virtual ::AbstractScene const* getActiveScene() const /*override*/;
    virtual ::SceneFactory& getSceneFactory() const /*override*/;
    virtual ::OreUI::SceneProvider& getSceneProvider() const /*override*/;
    virtual ::ui::ScreenTechStackSelector& getScreenTechStackSelector() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack const> getClientSceneStack() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack> getClientSceneStack() /*override*/;
    virtual ::OreUI::Router& getClientUIRouter() const /*override*/;
    virtual ::ISceneStack& getMainSceneStackInterface() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack> getMainSceneStack() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack const> getMainSceneStack() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack const> getCurrentSceneStack() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack> getCurrentSceneStack() /*override*/;
    virtual ::OreUI::Router& getCurrentUIRouter() /*override*/;
    virtual ::OreUI::Router const& getCurrentUIRouter() const /*override*/;
    virtual ::OreUI::ITelemetry& getOreUITelemetry() /*override*/;
    virtual ::WeakRef<::OreUI::UIBlockThumbnailAtlasManager> getOreUIBlockThumbnailAtlasManager() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ISceneStack> prepareSceneFor(
        ::ui::SceneType const                                 sceneTypeToFind,
        ::std::function<::std::shared_ptr<::AbstractScene>()> createScreenCallback
    ) /*override*/;
    virtual ::CachedScenes& getCachedScenes() /*override*/;
    virtual ::std::string getScreenName() const /*override*/;
    virtual ::std::string getScreenTelemetry() const /*override*/;
    virtual ::ui::SceneType getTopSceneType() const /*override*/;
    virtual ::MobEffectsLayout& getMobEffectsLayout() /*override*/;
    virtual ::std::string emoticonifyText(::std::string const& text) const /*override*/;
    virtual ::std::string emoticonifyText(::std::string const& text, bool isGameTip) /*override*/;
    virtual void onMobEffectsChange() /*override*/;
    virtual void setUISize(int w, int h) /*override*/;
    virtual void forEachVisibleScreen(/*::brstd::function_ref<void(::AbstractScene&)> callback*/) /*override*/;
    virtual void forEachScreen(/*::brstd::function_ref<bool(::AbstractScene&)> callback, bool topDown*/) /*override*/;
    virtual void forEachScreenConst(/*::brstd::function_ref<bool(::AbstractScene const&)> callback, bool topDown*/) const
        /*override*/;
    virtual bool updateSceneStack() /*override*/;
    virtual void forEachAlwaysAcceptInputScreen(
        ::std::function<void(::AbstractScene&)> callback,
        ::AbstractScene const*                  ignoreScreen
    ) /*override*/;
    virtual void forEachAlwaysAcceptInputScreenWithTop(::std::function<void(::AbstractScene&)> callback) /*override*/;
    virtual void showPlayerProfile(::std::string const& xuid, ::std::string const& platformId) /*override*/;
    virtual bool isInGameInputEnabled() const /*override*/;
    virtual void setInGameInputEnabled(bool isEnabled) /*override*/;
    virtual ::Vec2 getSafeZoneScale() const /*override*/;
    virtual void verifySkinApproval(
        ::std::function<void(::std::string)> const& notApprovedCallback,
        ::std::function<void()> const&              approvedCallback
    ) const /*override*/;
    virtual ::InputMode getCurrentInputMode() const /*override*/;
    virtual bool isTouchGameplayAllowed() const /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::MinecraftInputHandler> getMinecraftInput() const /*override*/;
    virtual ::KeyboardManager& getKeyboardManager() /*override*/;
    virtual void setLastPointerLocation(float x, float y, float z) /*override*/;
    virtual void clearTouchPointerLocations() /*override*/;
    virtual void clearTouchPointerLocation(int id) /*override*/;
    virtual void updateTouchPointerLocation(int id, float x, float y) /*override*/;
    virtual ::Vec3 getLastPointerLocation() /*override*/;
    virtual ::Vec2 getTouchPointerLocation(int id) const /*override*/;
    virtual void updateActionPointerId(int id) /*override*/;
    virtual int getActionPointerId() const /*override*/;
    virtual bool shouldUseLastPointerLocationOnFocusChange() /*override*/;
    virtual bool currentScreenShouldStealMouse() /*override*/;
    virtual ::BuildActionIntention& getInProgressBAI() const /*override*/;
    virtual ::PacketSender& getPacketSender() /*override*/;
    virtual ::ClientNetworkSystem& getClientNetworkSystem() /*override*/;
    virtual ::ClientNetworkSystem const& getClientNetworkSystem() const /*override*/;
    virtual ::glm::vec2 getGamepadCursorPosition() const /*override*/;
    virtual void updateControlOptionState(uint id, bool active) /*override*/;
    virtual void clearActiveControlOptions() /*override*/;
    virtual ::std::set<uint> const& getActiveControlOptions() const /*override*/;
    virtual ::std::vector<::OptionID> getActiveOptionIDs(::ControlOptionType controlOptionType) const /*override*/;
    virtual void setNumberOfActiveConfigs(int numberOfActiveConfigs) /*override*/;
    virtual int getNumberOfActiveConfigs() const /*override*/;
    virtual bool isNoConfigSelected() const /*override*/;
    virtual void setNumberOfEnabledConfigs(int numberOfEnabledConfigs) /*override*/;
    virtual int getNumberOfEnabledConfigs() const /*override*/;
    virtual void setWYSIWYGState(::WYSIWYGState wysiwygState) /*override*/;
    virtual ::WYSIWYGState getWYSIWYGState() const /*override*/;
    virtual void setOtherConfigsExistInThisCategory(bool otherConfigsExistInThisCategory) /*override*/;
    virtual bool getOtherConfigsExistInThisCategory() const /*override*/;
    virtual void setMoveTurnInput(::std::unique_ptr<::ClientMoveInputHandler> pClientMoveInputHandler) /*override*/;
    virtual void setupPersistentControls(::InputMode currentMode) /*override*/;
    virtual void resetPlayerMovement() /*override*/;
    virtual void suspendPredictedMovement() /*override*/;
    virtual void onClientInputInitComplete() /*override*/;
    virtual void setClientInputHandler(::std::unique_ptr<::ClientInputHandler> pClientInputHandler) /*override*/;
    virtual ::ClientInputHandler* getInput() const /*override*/;
    virtual int getControllerId() const /*override*/;
    virtual bool hasConnectedController() const /*override*/;
    virtual ::SubClientId getClientSubId() const /*override*/;
    virtual void setSuspendInput(bool suspendInput) /*override*/;
    virtual void setSuspendDirectionalInput(bool suspendDirectionalInput) /*override*/;
    virtual void setDisableInput(bool disableInput) /*override*/;
    virtual void grabMouse() /*override*/;
    virtual void releaseMouse() /*override*/;
    virtual void refocusMouse(bool lostMouse) /*override*/;
    virtual void setMouseType(::Bedrock::Input::PointerType type) /*override*/;
    virtual void resetBai(int baiFlags) /*override*/;
    virtual void clearInProgressBAI() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::SoundEngine> getSoundEngine() const /*override*/;
    virtual ::MusicManager const* getMusicManager() const /*override*/;
    virtual ::MusicManager* getMusicManagerNonConst() const /*override*/;
    virtual void play(::std::string const& name, ::Vec3 const& pos, float volume, float pitch) /*override*/;
    virtual void playUI(::std::string const& name, float volume, float pitch) /*override*/;
    virtual void muteAudio() /*override*/;
    virtual void unMuteAudio() /*override*/;
    virtual void fadeOutMusic() const /*override*/;
    virtual ::TaskGroup& getTaskGroup() /*override*/;
    virtual void onFullVanillaPackOnStack() /*override*/;
    virtual bool isFullVanillaPackOnStack() const /*override*/;
    virtual void onPlayerLoaded(::Player& player) /*override*/;
    virtual void setClientGameMode(::GameType gameType) /*override*/;
    virtual void resetToDefaultGameMode() /*override*/;
    virtual ::IGameConnectionListener& getGameConnectionListener() /*override*/;
    virtual void connectToThirdPartyServer(::std::string const& ipAddress, int port) /*override*/;
    virtual void startExternalNetworkWorld(
        ::Social::GameConnectionInfo connection,
        ::std::string const&         serverName,
        ::PlayerJoinWorldContext     context
    ) /*override*/;
    virtual bool isReadyToReconnect() const /*override*/;
    virtual bool checkForPiracy() /*override*/;
    virtual void updateChatFilterStatus(::ProfanityContext& profanityContext) /*override*/;
    virtual void updateControllerHandling() /*override*/;
    virtual void onPlayerDestruction(::Player& player) /*override*/;
    virtual void
    setBehaviorCommandCallback(::std::function<void(::std::string const&, ::BehaviorStatus)> callback) /*override*/;
    virtual void setBehaviorCommandStatus(::std::string const& treeName, ::BehaviorStatus status) /*override*/;
    virtual void setConnectGamepadScreenActive(bool active) /*override*/;
    virtual ::Bedrock::Threading::Async<::Core::PathBuffer<::std::string>>
    requestScreenshot(::ScreenshotOptions& screenshotOptions) /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::DevConsoleLogger> getDevConsoleLogger() const /*override*/;
    virtual ::std::shared_ptr<::FileDataRequest> requestImageFromUrl(
        ::std::string const&                                                        imageUrl,
        ::std::function<void(::Bedrock::Http::Status, ::Core::Path const&, uint64)> callback
    ) /*override*/;
    virtual void setActiveFileStorageArea(::std::shared_ptr<::Core::FileStorageArea> storageArea) /*override*/;
    virtual void onExtendDiskSpace(
        bool const                                      bSet,
        ::std::weak_ptr<::Core::FileStorageArea> const& storageAreaWeakPtr,
        uint64                                          freeSpace,
        ::std::function<void()>                         onHandledEventCallback
    ) /*override*/;
    virtual void onLowDiskSpace(bool const bSet, uint64 freeSpace) /*override*/;
    virtual void onOutOfDiskSpace(bool const bSet, uint64 freeSpace) /*override*/;
    virtual void onCriticalDiskError(bool const bSet, ::Core::LevelStorageState const& errorCode) /*override*/;
    virtual void onLevelCorrupt() /*override*/;
    virtual void onGameModeChanged() /*override*/;
    virtual void onBeforeSimTick() /*override*/;
    virtual void onTick(int nTick, int maxTick) /*override*/;
    virtual void onInternetUpdate() /*override*/;
    virtual void onGameSessionReset() /*override*/;
    virtual void onLevelExit() /*override*/;
    virtual void onRequestResourceReload() /*override*/;
    virtual void updateScreens() /*override*/;
    virtual void initializeRenderResources() /*override*/;
    virtual void postInitRenderResources() /*override*/;
    virtual void onAppSuspended() /*override*/;
    virtual void onAppResumed() /*override*/;
    virtual void onActiveResourcePacksChanged(
        ::Bedrock::NotNullNonOwnerPtr<::ActorResourceDefinitionGroup> const& entityResourceDefGroup
    ) /*override*/;
    virtual void reloadEntityRenderers(
        ::Bedrock::NotNullNonOwnerPtr<::ActorResourceDefinitionGroup> const& actorResourceDefinitionGroup
    ) /*override*/;
    virtual ::BlockTessellator& getBlockTessellator() /*override*/;
    virtual ::BlockActorRenderDispatcher& getBlockEntityRenderDispatcher() /*override*/;
    virtual ::std::shared_ptr<::ActorRenderDispatcher> getEntityRenderDispatcher() /*override*/;
    virtual ::ActorBlockRenderer& getEntityBlockRenderer() /*override*/;
    virtual ::ItemInHandRenderer* getItemInHandRenderer() /*override*/;
    virtual ::ItemRenderer* getItemRenderer() /*override*/;
    virtual ::HudIconActorRenderer* getHudIconActorRenderer() /*override*/;
    virtual ::std::deque<::std::string>& getSentMessageHistory() /*override*/;
    virtual ::std::deque<::std::string>& getDevConsoleMessageHistory() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ProfanityContext> getProfanityContext() const /*override*/;
    virtual void initTTSClient(::std::shared_ptr<::TextToSpeechClient> textToSpeechClient) /*override*/;
    virtual ::std::shared_ptr<::TextToSpeechClient> getTTSClient() /*override*/;
    virtual ::std::shared_ptr<::TextToSpeechClient const> getTTSClient() const /*override*/;
    virtual ::std::shared_ptr<::ITTSEventManager> getTTSEventManager() /*override*/;
    virtual void addTTSMessage(
        ::std::string const&     message,
        ::ProfanityFilterContext profanityFilterContext,
        bool                     interruptible,
        ::std::string const&     interruptibleId,
        bool                     required,
        bool                     disregardAppFocus
    ) /*override*/;
    virtual void initCommands() /*override*/;
    virtual uint getUserId() const /*override*/;
    virtual bool isPrimaryUser() const /*override*/;
    virtual double getServerConnectionTime() const /*override*/;
    virtual void setServerPingTime(::std::chrono::milliseconds pingTime) /*override*/;
    virtual ::std::chrono::milliseconds getServerPingTime() const /*override*/;
    virtual void setDefaultPlayscreenTab(::PlayScreenDefaultTab defaultTab) /*override*/;
    virtual void setClientInstanceState(::ClientInstanceState const& newstate) /*override*/;
    virtual void setUIEventCoordinator(::Bedrock::UniqueOwnerPointer<::UIEventCoordinator>&& coordinator) /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::UIEventCoordinator> getUIEventCoordinator() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ClientInstanceEventCoordinator> getEventCoordinator() /*override*/;
    virtual ::ClientNetworkEventCoordinator& getClientNetworkEventCoordinator() /*override*/;
    virtual ::ClientScriptEventCoordinator& getClientScriptEventCoordinator() /*override*/;
    virtual ::std::chrono::steady_clock::time_point getNoBlockBreakUntil() /*override*/;
    virtual void setNoBlockBreakUntil(::std::chrono::steady_clock::time_point timePoint) /*override*/;
    virtual ::GameCallbacks& getGameCallbacks() /*override*/;
    virtual ::GameModuleClient* getGameModule() /*override*/;
    virtual ::ClientHitDetectCoordinator& getHitEventCoordinator() /*override*/;
    virtual void sendClientEnteredLevel() /*override*/;
    virtual ::HitDetectSystem* getHitDetectSystem() /*override*/;
    virtual bool isPlaying() const /*override*/;
    virtual ::ClientInstanceState getClientInstanceState() const /*override*/;
    virtual ::LatencyGraphDisplay* getLatencyGraphDisplay() const /*override*/;
    virtual ::PlayerCapabilities::IClientController const& getClientCapabilities() const /*override*/;
    virtual ::cg::math::Rect<float> calculateViewPortModifiers(
        ::SubClientId const          clientId,
        uint64 const                 clientCount,
        ::SplitScreenDirection const splitScreenConfiguration
    ) const /*override*/;
    virtual ::std::weak_ptr<::IClientInstance> getWeakPtrToThis() /*override*/;
    virtual ::ClientRequirementVerifier const& getClientRequirementVerifier() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::FogDefinitionRegistry const> getFogDefinitionRegistry() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::FogDefinitionRegistry> getFogDefinitionRegistry() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::FogManager const> getFogManager() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::FogManager> getFogManager() /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::ScreenLoadTimeTracker> getScreenLoadTimeTracker() /*override*/;
    virtual ::ItemRegistryRef getItemRegistry() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::DisconnectionRequestHandler> getDisconnectionRequestHandler() const
        /*override*/;
    virtual float getRemoteServerTimeMs() const /*override*/;
    virtual void setRemoteServerTimeMs(float time) /*override*/;
    virtual float getRemoteServerNetworkTimeMs() const /*override*/;
    virtual void setRemoteServerNetworkTimeMs(float time) /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::ClientScriptManager> getClientScriptManager() /*override*/;
    virtual ::Scripting::ScriptEngine* getScriptingEngine() /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::WorldTransferAgent> const getWorldTransferAgent() const /*override*/;
    virtual ::Bedrock::NotNullNonOwnerPtr<::PlayerReportHandler> getPlayerReportHandler() /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::LinkedAssetValidator> getLinkedAssetValidator() /*override*/;
    virtual void flagDisconnectionAndNotify(::Connection::DisconnectFailReason disconnectReason) /*override*/;
    virtual void flagDisconnectionAndNotifyWithParams(::DisconnectionScreenParams const& params) /*override*/;
    virtual void disconnectSubClient(::Connection::DisconnectFailReason disconnectReason) /*override*/;
    virtual void setClientUpdateAndRenderThrottling(bool enabled, int threshold, float scalar) /*override*/;
    virtual bool isClientUpdateAndRenderThrottlingEnabled() const /*override*/;
    virtual int getClientUpdateAndRenderThrottlingThreshold() const /*override*/;
    virtual float getClientUpdateAndRenderThrottlingScalar() const /*override*/;
    virtual bool isUserBanned() const /*override*/;
    virtual bool isEligibleForPauseFeature() const /*override*/;
    virtual void setupPauseManagers() /*override*/;
    virtual bool requestInGamePause(bool status) /*override*/;
    virtual void openContentLogHistory() /*override*/;
    virtual double getGameUpdateDurationInSeconds() const /*override*/;
    virtual ::std::optional<::ConnectionContextInfo> getConnectionContextInfo() const /*override*/;
    virtual ::Bedrock::NonOwnerPointer<::PersonaClient> tryGetPersonaClient() const /*override*/;

    // NOLINTEND
    /*Actor* getLocalPlayer() {
        return util::memory::call_func<Actor*>(sigs().ClientInstance.getLocalPlayer.v(), reinterpret_cast<std::uintptr_t>(this));
    }
};*/
};