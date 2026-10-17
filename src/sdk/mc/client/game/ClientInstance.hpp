//
// Created by emi on 10/15/26.
//
#pragma once
#include "core/sdk_base.hpp"
#include "util/memory.hpp"

class Actor;



class IClientInstance {
    // NOLINTBEGIN
private:
    VIRT_PAD // Accounting for the base class
public:
    // i want client symbols so bad >:c i feel bad referencing levi
    // this isn't supposed to be a cheat so whatevers
    // https://discord.com/users/921648530769182740 please!!!!
    virtual ~IClientInstance() = default;
    VIRT_PAD // virtual void onInitMinecraftGame() = 0;
    VIRT_PAD // virtual void onDestroyMinecraftGame() = 0;
    VIRT_PAD // virtual void init(::ClientInstanceInitArguments&& args) = 0;
    VIRT_PAD // virtual void initSceneFactory(::std::unique_ptr<::SceneFactory> sceneFactory,::std::unique_ptr<::OreUI::SceneProvider> sceneProvide) = 0;
    VIRT_PAD // virtual void setUiRouter(::std::unique_ptr<::OreUI::Router> uiRouter) = 0;
    VIRT_PAD // virtual void initGraphics() = 0;
    VIRT_PAD // virtual void stop() = 0;
    VIRT_PAD // virtual void teardown() = 0;
    VIRT_PAD // virtual void preShutDown() = 0;
    VIRT_PAD // virtual void setGameConnectionInfo(::Social::GameConnectionInfo const& gameConnection) = 0;
    VIRT_PAD // virtual ::std::optional<::Social::GameConnectionInfo> getGameConnectionInfo() = 0;
    VIRT_PAD // virtual void onStartJoinGame(bool isLocalServer,::std::string const &multiplayerCorrelationId,::std::string const &serverName,::std::string const &worldName,::NetworkType networkType,::Social::MultiplayerServiceIdentifier service,::PlayerJoinWorldContext context) = 0;
    VIRT_PAD // virtual void onCancelJoinGame() = 0;
    VIRT_PAD // virtual void requestLeaveGameAsync() = 0;
    VIRT_PAD // virtual void requestLeaveGame(bool switchScreen, bool sync) = 0;
    VIRT_PAD // virtual void stopPlayScreen() = 0;
    VIRT_PAD // virtual bool isLeaveGameDone() const = 0;
    VIRT_PAD // virtual void setupPlayScreenForLeaveGame() = 0;
    VIRT_PAD // virtual void preCacheOutOfGameViews() = 0;
    VIRT_PAD // virtual void resetPrimaryClient() = 0;
    VIRT_PAD // virtual void resetGameSession() = 0;
    virtual void tick() = 0;
    VIRT_PAD // virtual void frameUpdate(::FrameUpdateContext& frameUpdateContext) = 0;
    VIRT_PAD // virtual bool update(bool isInitFinished) = 0;
    VIRT_PAD // virtual void preFrameTick() = 0;
    VIRT_PAD // virtual void endFrame() = 0;
    VIRT_PAD // virtual float getFrameAlpha() = 0;
    VIRT_PAD // virtual void startSubClientLateJoin(bool hasXBLBroadcast, ::std::unique_ptr<::GameModuleClient> gameModuleClient,::std::optional<::ConnectionContextInfo> primaryConnectionInfo) = 0;
    VIRT_PAD // virtual ::Bedrock::Threading::Async<::ClientGameSetupResult> setupClientGame(bool joiningLocalServer, ::ServerSupportedAuthenticationTypes supportedAuth,::std::unique_ptr<::GameModuleClient> gameModuleClient) = 0;
    virtual class BlockSource* getRegion() = 0;
    virtual class LocalPlayer* getLocalPlayer() const = 0;
    VIRT_PAD
    // NOLINTEND
};

// note that this doesn't include all of the base classes
class ClientInstance : public IClientInstance, SDK_CLASS {
public:
    Actor* getLocalPlayer() {
        return util::memory::call_func<Actor*>(sigs().ClientInstance.getLocalPlayer.v(), reinterpret_cast<std::uintptr_t>(this));
    }
};
