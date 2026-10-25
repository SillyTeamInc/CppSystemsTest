//
// Created by emi on 10/17/26.
//
#pragma once
#include <bitset>

#include "ActorType.hpp"
#include "ActorUniqueID.hpp"
#include "glm/glm.hpp"
#include "sdk/mc/deps/ecs/EntityId.hpp"
#include "sdk/mc/input/NewInteractionModel.hpp"
#include "sdk/mc/server/commands/CommandPermissionLevel.hpp"


struct ActorHeadRotationComponent;
enum class InputMode;
class AABB;
class ActionEvent;
class ActionQueue;
class ActorDamageSource;
class ActorDefinitionDescriptor;
class ActorDefinitionDiffList;
class ActorDefinitionGroup;
class ActorHurtResult;
class ActorRuntimeID;
class AnimationComponent;
class Attribute;
class AttributeInstanceConstRef;
class BaseAttributeMap;
class Block;
class BlockPos;
class BlockSource;
class ChangeDimensionPacket;
class CompoundTag;
class DataLoadHelper;
class Dimension;
class DynamicProperties;
class EntityRegistry;
class FishingHook;
class GetCollisionShapeInterface;
class IConstBlockSource;
class ILevel;
class Interaction;
class ItemActor;
class ItemDescriptor;
class ItemStack;
class ItemStackBase;
class LootTable;
class Matrix;
class MerchantRecipeList;
class Mob;
class MobEffect;
class MobEffectInstance;
class Packet;
class Player;
class Random;
class RenderParams;
class RideableComponent;
class RopeSystem;
class SpatialActorNetworkData;
class StrictEntityContext;
class UpdateEquipPacket;
class UpdateTradePacket;
struct ActorDefinitionIdentifier;
struct ActorLink;
struct DimensionType;
struct DistanceSortedActor;
struct DynamicRenderOffsetComponent;
struct EquipmentTable;
struct HurtParameters;
struct InterpolationPair;
struct MutableAttributeWithContext;
struct SoundEventIdentifier;
struct StateVectorComponent;
namespace Bedrock::Safety { class RedactableString; }
namespace MovementDataExtractionUtility { class SnapshotAccessor; }
class IOptionsReader;
namespace mce { class Color; }
class HashedString;
class ActorInitializationMethod;
struct VariantParameterList;
namespace SharedTypes::Legacy {
    enum class ArmorSlot {};
    enum class LevelSoundEvent {};
    enum class ActorDamageCause {};
    enum class EquipmentSlot {};;
}

namespace SharedTypes::v1_26_20 {
    enum class MaterialType {
    };
}
struct HurtEffectsSettings;
enum class HandSlot {};
enum class ArmorMaterialType {};
enum class ActorResetRule {};
enum class EquipmentSlot {};
enum class ActorSwingSource {};
enum class ItemUseMethod {};
struct KnockbackParameters;
class BodyControl;
class ActorEvent;

// ReSharper disable once CppImplicitDefaultConstructorNotAvailable
class Actor {
public:
    // NOLINTBEGIN
    EntityContext mContext;

    virtual bool hasComponent(::HashedString const& name) const;
    virtual void outOfWorld();
    virtual void reloadHardcoded(::ActorInitializationMethod method, VariantParameterList const& params);
    virtual void reloadHardcodedClient(::ActorInitializationMethod method);
    virtual void initializeComponents(::ActorInitializationMethod method, VariantParameterList const& params);
    virtual void reloadComponents(::ActorInitializationMethod, VariantParameterList const&);
    virtual void _serverInitItemStackIds();
    virtual void _doInitialMove();
    virtual ~Actor();
    virtual void resetUserPos(ActorResetRule resetRule);
    virtual ActorType getOwnerEntityType();
    virtual void remove();
    virtual ::glm::vec3 getFiringPos() const;
    virtual float getInterpolatedBodyRot(float a = 0.0f) const;
    virtual float getInterpolatedHeadRot(float a = 0.0f) const;
    virtual float getInterpolatedBodyYaw(float a = 0.0f) const;
    virtual float getYawSpeedInDegreesPerSecond() const;
    virtual ::glm::vec3 getInterpolatedRidingOffset(float, int const) const;
    virtual bool isFireImmune() const;
    virtual void blockedByShield(::ActorDamageSource const& source, ::Actor& blocker);
    virtual bool canDisableShield();
    virtual void teleportTo(::glm::vec3 const& pos, bool shouldStopRiding, int cause, int sourceEntityType, bool keepVelocity);
    virtual void lerpMotion(::glm::vec3 const& delta);
    virtual ::std::unique_ptr<::Packet> tryCreateAddActorPacket();
    virtual void normalTick();
    virtual void baseTick();
    virtual void passengerTick();
    virtual bool startRiding(::Actor& vehicle, bool forceRiding);
    virtual void addPassenger(::Actor& passenger);
    virtual ::std::string getExitTip(::std::string const& kind, InputMode mode, NewInteractionModel scheme) const;
    virtual ::std::string getEntityLocNameString() const;
    virtual bool isInvisible() const;
    virtual bool canShowNameTag() const;
    virtual ::mce::Color getNameTagTextColor() const;
    virtual float getShadowRadius() const;
    virtual ::glm::vec3 getHeadLookVector(float a = 0.0f) const;
    virtual bool canInteractWithOtherEntitiesInGame() const;
    virtual float getBrightness(float a, ::IConstBlockSource const& region) const;
    virtual void playerTouch(::Player& player);
    virtual bool isImmobile() const;
    virtual bool isSilentObserver() const;
    virtual bool isSleeping() const;
    virtual void setSleeping(bool val);
    virtual void setSneaking(bool value);
    virtual bool isBlocking() const;
    virtual bool isDamageBlocked(::ActorDamageSource const& source) const;
    virtual bool isAlive() const;
    virtual bool isOnFire() const;
    virtual bool isSurfaceMob() const;
    virtual bool isTargetable() const;
    virtual void setTarget(::Actor* entity);
    virtual bool isValidTarget(::Actor* attacker) const;
    virtual ::ActorHurtResult attack(Actor& target, SharedTypes::Legacy::ActorDamageCause const& cause);
    virtual void performRangedAttack(Actor& target, float);
    virtual void setOwner(ActorUniqueID const ownerId);
    virtual void setSitting(bool value);
    virtual void onTame();
    virtual void onFailedTame();
    virtual void setStanding(bool value);
    virtual bool canPowerJump() const;
    virtual bool isEnchanted() const;
    virtual void playAmbientSound();
    virtual ::SharedTypes::Legacy::LevelSoundEvent getAmbientSound() const;
    virtual bool isInvulnerableTo(::ActorDamageSource const& source) const;
    virtual ::SharedTypes::Legacy::ActorDamageCause getBlockDamageCause(::Block const& block) const;
    virtual ::ActorHurtResult doFireHurt(int amount);
    virtual void onLightningHit();
    virtual void feed(int itemId);
    virtual void handleEntityEvent(::ActorEvent eventId, int data);
    virtual ::HashedString const& getActorRendererId() const;
    virtual void despawn();
    virtual void setArmor(::SharedTypes::Legacy::ArmorSlot slot, ::ItemStack const& item);
    virtual ::ArmorMaterialType getArmorMaterialTypeInSlot(::SharedTypes::Legacy::ArmorSlot slot) const;
    virtual int getArmorTextureIndexInSlot(::SharedTypes::Legacy::ArmorSlot slot) const;
    virtual float getArmorColorInSlot(::SharedTypes::Legacy::ArmorSlot slot, int channelRGBA) const;
    virtual void setEquippedSlot(::SharedTypes::Legacy::EquipmentSlot slot, ::ItemStack const& item);
    virtual void setCarriedItem(::ItemStack const& item);
    virtual ::ItemStack const& getCarriedItem() const;
    virtual void setOffhandSlot(::ItemStack const& item);
    virtual ::ItemStack const& getEquippedTotem() const;
    virtual bool consumeTotem();
    virtual bool load(::CompoundTag const& tag, ::DataLoadHelper& dataLoadHelper);
    virtual ::HashedString const& queryEntityRenderer() const;
    virtual ActorUniqueID getSourceUniqueID() const;
    virtual ::AABB getLiquidAABB(::SharedTypes::v1_26_20::MaterialType const liquidType) const;
    virtual void handleInsidePortal(::BlockPos const& portalPos);
    virtual bool canChangeDimensionsUsingPortal() const;
    virtual void changeDimension(::DimensionType toId);
    virtual void changeDimension(::ChangeDimensionPacket const& packet);
    virtual ActorUniqueID getControllingPlayer() const;
    virtual float causeFallDamageToActor(float distance, float multiplier, ::ActorDamageSource source);
    virtual void onSynchedDataUpdate(int dataId);
    virtual bool canAddPassenger(::Actor& passenger) const;
    virtual bool canPickupItem(::ItemStack const& item) const;
    virtual bool canBePulledIntoVehicle() const;
    virtual bool inCaravan() const;
    virtual void sendMotionPacketIfNeeded();
    virtual bool canSynchronizeNewEntity() const;
    virtual void startSwimming();
    virtual void stopSwimming();
    virtual void buildDebugInfo(::std::string& out) const;
    virtual ::CommandPermissionLevel getCommandPermissionLevel() const;
    virtual int getDeathTime() const;
    virtual bool canBeAffected(uint id) const;
    virtual bool canBeAffectedByArrow(::MobEffectInstance const& effect) const;
    virtual void onEffectRemoved(::MobEffectInstance& effect);
    virtual bool canObstructSpawningAndBlockPlacement() const;
    virtual ::AnimationComponent& getAnimationComponent();
    virtual void openContainerComponent(::Player& player);
    virtual bool swing(::ActorSwingSource swingSource, ::HandSlot handSlot);
    virtual void useItem(::ItemStackBase& item, ::ItemUseMethod itemUseMethod, bool consumeItem);
    virtual void getDebugText(::std::vector<::std::string>& outputInfo);
    virtual float getMapDecorationRotation() const;
    virtual float getPassengerYRotation(::Actor const& passenger) const;
    virtual bool add(::ItemStack& item);
    virtual bool drop(::ItemStack const& item, bool const randomly);
    virtual ::Interaction getInteraction(::Player& player, ::glm::vec3 const& location);
    virtual bool canDestroyBlock(::Block const& block) const;
    virtual void setAuxValue(int aux);
    virtual void renderDebugServerState(::IOptionsReader const& options);
    virtual void kill();
    virtual void die(::ActorDamageSource const& source);
    virtual bool shouldDropDeathLoot() const;
    virtual void applySnapshot(::EntityContext const &snapshotEntity,::MovementDataExtractionUtility::SnapshotAccessor const &originalSnapshotEntity);
    virtual void onPush(::Actor& source);
    virtual ::std::optional<::BlockPos> getLastDeathPos() const;
    virtual ::std::optional<::DimensionType> getLastDeathDimension() const;
    virtual bool hasDiedBefore() const;
    virtual void doEnterWaterSplashEffect();
    virtual void doExitWaterSplashEffect();
    virtual void doWaterSplashEffect();
    virtual bool _shouldProvideFeedbackOnHandContainerItemSet(HandSlot handSlot, ::ItemStack const& item) const;
    virtual bool _shouldProvideFeedbackOnArmorSet(::SharedTypes::Legacy::ArmorSlot slot, ::ItemStack const& item) const;
    virtual ::ActorHurtResult _hurt(::ActorDamageSource const& source, float damage, ::HurtParameters const& hurtParameters);
    virtual void readAdditionalSaveData(::CompoundTag const& tag, ::DataLoadHelper& dataLoadHelper);
    virtual void addAdditionalSaveData(::CompoundTag& tag) const;

    // NOLINTEND
};

class Mob : public ::Actor {
public:
    // NOLINTBEGIN
    virtual void initializeComponents(::ActorInitializationMethod method, ::VariantParameterList const& params) /*override*/;
    virtual void reloadHardcodedClient(::ActorInitializationMethod method) /*override*/;
    virtual void reloadHardcoded(::ActorInitializationMethod method, ::VariantParameterList const& params) /*override*/;
    virtual ~Mob() /*override*/;
    virtual void knockback(::Actor* source, float damage, float xd, float zd, ::KnockbackParameters const& parameters);
    virtual void kill() /*override*/;
    virtual void die(::ActorDamageSource const& source) /*override*/;
    virtual bool shouldDropDeathLoot() const /*override*/;
    virtual void spawnAnim();
    virtual bool isAlive() const /*override*/;
    virtual bool isSleeping() const /*override*/;
    virtual void setSleeping(bool val) /*override*/;
    virtual bool isBlocking() const /*override*/;
    virtual void setSprinting(bool shouldSprint);
    virtual bool canBePulledIntoVehicle() const /*override*/;
    virtual float getSpeed() const;
    virtual void setSpeed(float speed);
    virtual float getYawSpeedInDegreesPerSecond() const /*override*/;
    virtual void normalTick() /*override*/;
    virtual void baseTick() /*override*/;
    virtual void hurtEffects(::ActorDamageSource const& source, float damage, ::HurtEffectsSettings const& settings);
    virtual void damageCarriedItemOnAttack(::Actor& target, float damageDealtToTarget);
    virtual ::ActorHurtResult doFireHurt(int amount) /*override*/;
    virtual void aiStep();
    virtual void pushActors();
    virtual bool checkSpawnRules(bool fromSpawner);
    virtual bool checkSpawnObstruction() const;
    virtual void addPassenger(::Actor& newPassenger) /*override*/;
    virtual bool startRiding(::Actor& vehicle, bool forceRiding) /*override*/;
    virtual void handleEntityEvent(::ActorEvent id, int data) /*override*/;
    virtual int getItemUseDuration() const;
    virtual float getItemUseStartupProgress() const;
    virtual float getItemUseIntervalProgress() const;
    virtual bool swing(::ActorSwingSource swingSource, ::HandSlot handSlot) /*override*/;
    virtual float getMaxHeadXRot();
    virtual bool isImmobile() const /*override*/;
    virtual void blockedByShield(::ActorDamageSource const& source, ::Actor& blocker) /*override*/;
    virtual void setTarget(::Actor* target) /*override*/;
    virtual ::ActorHurtResult
    attack(::Actor& target, ::SharedTypes::Legacy::ActorDamageCause const& cause) /*override*/;
    virtual bool isAlliedTo(::Mob* other);
    virtual ::ActorHurtResult doHurtTarget(::Actor* target, ::SharedTypes::Legacy::ActorDamageCause const& cause);
    virtual ::SharedTypes::Legacy::ActorDamageCause getBlockDamageCause(::Block const& block) const /*override*/;
    virtual bool inCaravan() const /*override*/;
    virtual int getArmorValue() const;
    virtual void hurtArmorSlots(::ActorDamageSource const& source, int damage, ::std::bitset<5> const hurtSlots);
    virtual void setDamagedArmor(::SharedTypes::Legacy::ArmorSlot slot, ::ItemStack const& item);
    virtual void sendArmorDamage(::std::bitset<5> const damagedSlots);
    virtual void sendArmor(std::bitset<5> const armorSlots);
    virtual ::ArmorMaterialType getArmorMaterialTypeInSlot(::SharedTypes::Legacy::ArmorSlot slot) const /*override*/;
    virtual int getArmorTextureIndexInSlot(::SharedTypes::Legacy::ArmorSlot slot) const /*override*/;
    virtual float getArmorColorInSlot(::SharedTypes::Legacy::ArmorSlot slot, int channelRGBA) const /*override*/;
    virtual ::std::vector<::ItemStack const*> getAllHandEquipment() const;
    virtual ::std::vector<::ItemStack const*> getAllEquipment() const;
    virtual int getDeathTime() const /*override*/;
    virtual void dropEquipmentOnDeath(::ActorDamageSource const& source);
    virtual void dropEquipmentOnDeath();
    virtual void clearVanishEnchantedItemsOnDeath();
    virtual void sendInventory(bool shouldSelectSlot);
    virtual void buildDebugInfo(::std::string& out) const /*override*/;
    virtual float getInterpolatedBodyRot(float a) const /*override*/;
    virtual float getInterpolatedHeadRot(float a) const /*override*/;
    virtual float getInterpolatedBodyYaw(float a) const /*override*/;
    virtual bool isSurfaceMob() const /*override*/;
    virtual float getDamageAfterEnchantReduction(::ActorDamageSource const& source, float damage) const;
    virtual bool createAIGoals();
    virtual void onBorn(::Actor&, ::Actor&);
    virtual bool setItemSlot(::SharedTypes::Legacy::EquipmentSlot slot, ::ItemStack const& item);
    virtual void setTransitioningSitting(bool value);
    virtual bool hasComponent(::HashedString const& name) const /*override*/;
    virtual bool canChangeDimensionsUsingPortal() const /*override*/;
    virtual void teleportTo(
        ::glm::vec3 const& pos,
        bool          shouldStopRiding,
        int           cause,
        int           sourceEntityType,
        bool          keepVelocity
    ) /*override*/;
    virtual float _getWalkTargetValue(::BlockPos const& pos);
    virtual bool canExistWhenDisallowMob() const;
    virtual bool canPowerJump() const /*override*/;
    virtual void setEquippedSlot(::SharedTypes::Legacy::EquipmentSlot slot, ::ItemStack const& item) /*override*/;
    virtual void renderDebugServerState(::IOptionsReader const& options) /*override*/;
    virtual void addAdditionalSaveData(::CompoundTag& entityTag) const /*override*/;
    virtual void readAdditionalSaveData(::CompoundTag const& tag, ::DataLoadHelper& dataLoadHelper) /*override*/;
    virtual ::std::unique_ptr<::BodyControl> initBodyControl();
    virtual float causeFallDamageToActor(float distance, float multiplier, ::ActorDamageSource source) /*override*/;
    virtual void outOfWorld() /*override*/;
    virtual ::ActorHurtResult
    _hurt(::ActorDamageSource const& source, float damage, ::HurtParameters const& hurtParameters) /*override*/;
    virtual void newServerAiStep();
    virtual void _doInitialMove() /*override*/;
    virtual ::AABB _getAdjustedAABBForSpawnCheck(::AABB const& aabb, ::glm::vec3 const& mobPos) const;
    // NOLINTEND
};

class Agent;
class BlockActor;
class ChalkboardBlockActor;
class ChunkPos;
class ChunkSource;
class ChunkViewSource;
class ComplexInventoryTransaction;
class Container;
class ContainerManagerModel;
class CreativeItemRegistry;
class EnderChestContainer;
class EntityContext;
class GameMode;
class HudContainerManagerModel;
class IContainerManager;
class IMinecraftEventing;
class ISparseContainerSetListener;
class InteractionResult;
class InventoryTransaction;
class Item;
class ItemStackNetManagerBase;
class LayeredAbilities;
class LegacyTelemetryEventPacket;
class NetworkChunkPublisher;
class NetworkIdentifier;
class PacketSender;
class PersonaPiece;
class PlayerEventCoordinator;
class PlayerInventory;
class PlayerRespawnRandomizer;
class ResolvedTextObject;
class SerializedSkinRef;
class SubChunkPos;
class TextObjectRoot;
struct AABBShapeComponent;
struct ActorRotationComponent;
struct BiomeIdType;
struct INpcDialogueData;
struct PlayerAuthenticationInfo;
struct PlayerDestroyProgressCacheComponent;
struct PlayerMovementSettings;
struct PlayerPartyInfo;
struct Tick;
namespace Bedrock::PubSub::ThreadModel { struct SingleThreaded; }
namespace Editor { class IEditorPlayer; }
namespace mce { class UUID; }
class FrameUpdateContextBase;
class PlayerListener;
enum class BedSleepingResult {};
enum class GameType;
enum class BehaviorStatus;
// clang-format on


class Player : public ::Mob {
    // NOLINTBEGIN
    virtual ~Player() /*override*/;
    virtual void initializeComponents(::ActorInitializationMethod method, ::VariantParameterList const& params) /*override*/;
    virtual void reloadHardcoded(::ActorInitializationMethod method, ::VariantParameterList const& params) /*override*/;
    virtual void prepareRegion(::ChunkSource& mainChunkSource);
    virtual void destroyRegion();
    virtual void suspendRegion();
    virtual void _fireDimensionChanged();
    virtual bool canChangeDimensionsUsingPortal() const /*override*/;
    virtual void changeDimensionWithCredits(::DimensionType dimension);
    virtual void tickWorld(::Tick const& currentTick);
    virtual void frameUpdate(::FrameUpdateContextBase& frameUpdateContextBase) = 0;
    virtual ::std::vector<::ChunkPos> const& getTickingOffsets() const;
    virtual void normalTick() /*override*/;
    virtual void moveView();
    virtual void moveSpawnView(::glm::vec3 const& spawnPosition, ::DimensionType dimensionType);
    virtual void onSynchedDataUpdate(int dataId) /*override*/;
    virtual void aiStep() /*override*/;
    virtual bool isFireImmune() const /*override*/;
    virtual void checkMovementStats(::glm::vec3 const& d);
    virtual ::HashedString getCurrentStructureFeature() const;
    virtual bool isAutoJumpEnabled() const;
    virtual ::glm::vec3 getInterpolatedRidingOffset(float, int const) const /*override*/;
    virtual void passengerTick() /*override*/;
    virtual void die(::ActorDamageSource const& source) /*override*/;
    virtual void remove() /*override*/;
    virtual void respawn();
    virtual bool shouldDropDeathLoot() const /*override*/;
    virtual void dropEquipmentOnDeath(::ActorDamageSource const& source) /*override*/;
    virtual void dropEquipmentOnDeath() /*override*/;
    virtual void clearVanishEnchantedItemsOnDeath() /*override*/;
    virtual bool drop(::ItemStack const& item, bool const randomly) /*override*/;
    virtual void resetRot();
    virtual void resetUserPos(::ActorResetRule resetRule) /*override*/;
    virtual bool isInTrialMode();
    virtual void setSpeed(float _speed) /*override*/;
    virtual int getItemUseDuration() const /*override*/;
    virtual float getItemUseStartupProgress() const /*override*/;
    virtual float getItemUseIntervalProgress() const /*override*/;
    virtual bool isBlocking() const /*override*/;
    virtual bool isDamageBlocked(::ActorDamageSource const& source) const /*override*/;
    virtual void handleEntityEvent(::ActorEvent id, int data) /*override*/;
    virtual ::std::vector<::ItemStack const*> getAllHandEquipment() const /*override*/;
    virtual ::std::vector<::ItemStack const*> getAllEquipment() const /*override*/;
    virtual bool add(::ItemStack& item) /*override*/;
    virtual ::CommandPermissionLevel getCommandPermissionLevel() const /*override*/;
    virtual ::ActorHurtResult attack(::Actor& actor, ::SharedTypes::Legacy::ActorDamageCause const& cause) /*override*/;
    virtual ::ItemStack const& getCarriedItem() const /*override*/;
    virtual void setCarriedItem(::ItemStack const& item) /*override*/;
    virtual void damageCarriedItemOnAttack(::Actor& target, float damageDealtToTarget) /*override*/;
    virtual void openPortfolio();
    virtual void openBook(int bookSlot, bool editable, int page, ::BlockActor* lectern);
    virtual void openTrading(ActorUniqueID const& uniqueID, bool useNewScreen);
    virtual void openChalkboard(::ChalkboardBlockActor& chalkboard, bool showLockToggle);
    virtual void openNpcInteractScreen(::std::shared_ptr<::INpcDialogueData> npc);
    virtual void openInventory();
    virtual void displayChatMessage(
        ::std::string const&                 author,
        ::std::string const&                 message,
        ::std::optional<::std::string> const filteredMessage
    );
    virtual void
    displayClientMessage(::std::string const& message, ::std::optional<::std::string> const filteredMessage);
    virtual void displayTextObjectMessage(
        ::TextObjectRoot const& textObject,
        ::std::string const&    fromXuid,
        ::std::string const&    fromPlatformId
    );
    virtual void displayTextObjectWhisperMessage(
        ::ResolvedTextObject const& resolvedTextObject,
        ::std::string const&        xuid,
        ::std::string const&        platformId
    );
    virtual void displayTextObjectWhisperMessage(
        ::std::string const& message,
        ::std::string const& xuid,
        ::std::string const& platformId
    );
    virtual void displayWhisperMessage(
        ::std::string const&                 author,
        ::std::string const&                 message,
        ::std::optional<::std::string> const filteredMessage,
        ::std::string const&                 xuid,
        ::std::string const&                 platformId
    );
    virtual float getShadowRadius() const /*override*/;
    virtual bool isSleeping() const /*override*/;
    virtual void setSleeping(bool val) /*override*/;
    virtual ::BedSleepingResult startSleepInBed(::BlockPos const& bedPos, bool setsRespawn, float sleepOffset);
    virtual void stopSleepInBed(bool forcefulWakeUp, bool updateLevelList);
    virtual bool canStartSleepInBed();
    virtual void sendInventory(bool shouldSelectSlot) /*override*/;
    virtual void openSign(::BlockPos const& position, bool isFrontSide);
    virtual void playEmote(::std::string const& pieceId, bool const playChatMessage);
    virtual bool isSilentObserver() const /*override*/;
    virtual void useItem(::ItemStackBase& instance, ::ItemUseMethod itemUseMethod, bool consumeItem) /*override*/;
    virtual bool isLoading() const;
    virtual bool isPlayerInitialized() const;
    virtual void stopLoading();
    virtual float getSpeed() const /*override*/;
    virtual void setPlayerGameType(GameType gameType);
    virtual void initHUDContainerManager();
    virtual void emitCriticalHitParticles(::Actor const& target, int particleCount);
    virtual bool isImmobile() const /*override*/;
    virtual void sendMotionPacketIfNeeded() /*override*/;
    virtual ::IMinecraftEventing* getEventing() const;
    virtual uint getUserId() const;
    virtual void addExperience(int xp);
    virtual void addLevels(int levels);
    virtual void setArmor(::SharedTypes::Legacy::ArmorSlot slot, ::ItemStack const& item) /*override*/;
    virtual void setOffhandSlot(::ItemStack const& item) /*override*/;
    virtual ::std::unique_ptr<::Packet> tryCreateAddActorPacket() /*override*/;
    virtual bool isInvulnerableTo(::ActorDamageSource const& source) const /*override*/;
    virtual void setContainerData(::IContainerManager& menu, int id, int value) = 0;
    virtual void slotChanged(
        ::IContainerManager& menu,
        ::Container&         container,
        int                  slot,
        ::ItemStack const&   oldItem,
        ::ItemStack const&   newItem,
        bool                 isResultSlot
    ) = 0;
    virtual void refreshContainer(::IContainerManager& menu) = 0;
    virtual void deleteContainerManager();
    virtual ::ItemStack const& getEquippedTotem() const /*override*/;
    virtual bool consumeTotem() /*override*/;
    virtual bool isActorRelevant(::Actor const& actor);
    virtual float getMapDecorationRotation() const /*override*/;
    virtual void teleportTo(
        ::glm::vec3 const& pos,
        bool          shouldStopRiding,
        int           cause,
        int           sourceEntityType,
        bool          keepVelocity
    ) /*override*/;
    virtual bool startRiding(::Actor& vehicle, bool forceRiding) /*override*/;
    virtual void startSwimming() /*override*/;
    virtual void stopSwimming() /*override*/;
    virtual bool isTeacher() const = 0;
    virtual bool isExternalCommunicationAllowed() const = 0;
    virtual void onSuspension();
    virtual void onLinkedSlotsChanged();
    virtual bool canBePulledIntoVehicle() const /*override*/;
    virtual void feed(int itemId) /*override*/;
    virtual void sendInventoryTransaction(::InventoryTransaction const& transaction) const = 0;
    virtual void
    sendComplexInventoryTransaction(::std::unique_ptr<::ComplexInventoryTransaction> transaction) const = 0;
    virtual void sendNetworkPacket(::Packet& packet) const;
    virtual bool canExistWhenDisallowMob() const /*override*/;
    virtual ::mce::Color getNameTagTextColor() const /*override*/;
    virtual bool canAddPassenger(::Actor& passenger) const /*override*/;
    virtual ::PlayerEventCoordinator& getPlayerEventCoordinator() = 0;
    virtual bool isSimulated() const;
    virtual ::std::string getXuid() const;
    virtual ::PlayerMovementSettings const& getMovementSettings() const;
    virtual bool canInteractWithOtherEntitiesInGame() const /*override*/;
    virtual bool canObstructSpawningAndBlockPlacement() const /*override*/;
    virtual ::std::optional<::BlockPos> getLastDeathPos() const /*override*/;
    virtual ::std::optional<::DimensionType> getLastDeathDimension() const /*override*/;
    virtual bool hasDiedBefore() const /*override*/;
    virtual void doEnterWaterSplashEffect() /*override*/;
    virtual void doExitWaterSplashEffect() /*override*/;
    virtual ::std::optional<::PlayerPartyInfo> getPartyInfo_UNTRUSTED() const;
    virtual void requestMissingSubChunk(::SubChunkPos const& scp);
    virtual uchar getMaxChunkBuildRadius() const;
    virtual float causeFallDamageToActor(float distance, float multiplier, ActorDamageSource source) /*override*/;
    virtual void setBehaviorCommandStatus(::std::string const& name, BehaviorStatus status);
    virtual void setRemotePlayerTicked(bool ticked);
    virtual ::std::unique_ptr<::ISparseContainerSetListener> createSparseContainerListener();
    virtual void addChunksToQueue(::std::vector<::ChunkPos> const& chunkPostions);
    virtual ::ActorHurtResult
    _hurt(::ActorDamageSource const& source, float damage, ::HurtParameters const& hurtParameters) /*override*/;
    virtual void readAdditionalSaveData(::CompoundTag const& tag, ::DataLoadHelper& dataLoadHelper) /*override*/;
    virtual void addAdditionalSaveData(::CompoundTag& entityTag) const /*override*/;
    virtual void onMovePlayerPacketNormal(::glm::vec3 const& pos, ::glm::vec2 const& rot, float yHeadRot);
    virtual bool _shouldProvideFeedbackOnHandContainerItemSet(::HandSlot handSlot, ::ItemStack const& item) const
        /*override*/;
    virtual bool _shouldProvideFeedbackOnArmorSet(::SharedTypes::Legacy::ArmorSlot slot, ::ItemStack const& item) const
        /*override*/;
    virtual ::std::shared_ptr<::ChunkViewSource> _createChunkSource(::ChunkSource& mainChunkSource);
    virtual void setAbilities(::LayeredAbilities const& newAbilities);
    virtual ::AnimationComponent& getAnimationComponent() /*override*/;
    virtual ::HashedString const& getActorRendererId() const /*override*/;
    virtual /*::Bedrock::NonOwnerPointer<::Editor::IEditorPlayer>*/ void getEditorPlayer() const = 0;
    virtual void destroyEditorPlayer() = 0;
    virtual void _serverInitItemStackIds() /*override*/;
    virtual ::std::unique_ptr<::BodyControl> initBodyControl() /*override*/;
    // NOLINTEND
};

class LocalPlayer : public ::Player {
public:
    // NOLINTBEGIN
    virtual ~LocalPlayer() /*override*/ = 0;
    virtual void reloadHardcodedClient(::ActorInitializationMethod method) /*override*/;
    virtual void prepareRegion(::ChunkSource& mainChunkSource) /*override*/;
    virtual void suspendRegion() /*override*/;
    virtual void destroyRegion() /*override*/;
    virtual void tickWorld(::Tick const& currentTick) /*override*/;
    virtual void frameUpdate(::FrameUpdateContextBase& frameUpdateContextBase) /*override*/;
    virtual ::std::vector<::ChunkPos> const& getTickingOffsets() const /*override*/;
    virtual void normalTick() /*override*/;
    virtual bool startRiding(::Actor& vehicle, bool forceRiding) /*override*/;
    virtual void aiStep() /*override*/;
    virtual void addAdditionalSaveData(::CompoundTag& entityTag) const /*override*/;
    virtual void readAdditionalSaveData(::CompoundTag const& tag, ::DataLoadHelper& dataLoadHelper) /*override*/;
    virtual void deleteContainerManager() /*override*/;
    virtual void openPortfolio() /*override*/;
    virtual void openBook(int bookSlot, bool editable, int page, ::BlockActor* lectern) /*override*/;
    virtual void openChalkboard(::ChalkboardBlockActor& chalkboard, bool showLockToggle) /*override*/;
    virtual void openNpcInteractScreen(::std::shared_ptr<::INpcDialogueData> data) /*override*/;
    virtual void openTrading(ActorUniqueID const& uniqueID, bool useNewScreen) /*override*/;
    virtual void openInventory() /*override*/;
    virtual void setContainerData(::IContainerManager& menu, int id, int value) /*override*/;
    virtual void slotChanged(
        ::IContainerManager& menu,
        ::Container&         container,
        int                  slot,
        ::ItemStack const&   oldItem,
        ::ItemStack const&   newItem,
        bool                 isResultSlot
    ) /*override*/;
    virtual void refreshContainer(::IContainerManager& menu) /*override*/;
    virtual bool isLoading() const /*override*/;
    virtual void stopLoading() /*override*/;
    virtual void respawn() /*override*/;
    virtual void displayClientMessage(
        ::std::string const&                 message,
        ::std::optional<::std::string> const filteredMessage
    ) /*override*/;

    virtual void displayTextObjectMessage(
        ::TextObjectRoot const& textObject,
        ::std::string const&    fromXuid,
        ::std::string const&    fromPlatformId
    ) /*override*/;
    virtual void displayWhisperMessage(
        ::std::string const&                 author,
        ::std::string const&                 message,
        ::std::optional<::std::string> const filteredMessage,
        ::std::string const&                 xuid,
        ::std::string const&                 platformId
    ) /*override*/;
    virtual ::BedSleepingResult
    startSleepInBed(::BlockPos const& pos, bool setsRespawn, float sleepOffset) /*override*/;
    virtual void stopSleepInBed(bool forcefulWakeUp, bool updateLevelList) /*override*/;
    virtual bool canStartSleepInBed() /*override*/;
    virtual void handleInsidePortal(::BlockPos const& portalPos) /*override*/;
    virtual bool swing(::ActorSwingSource swingSource, ::HandSlot handSlot) /*override*/;
    virtual void setSneaking(bool _isSneaking) /*override*/;
    virtual void setSprinting(bool _isSprinting) /*override*/;
    virtual void playEmote(::std::string const& pieceId, bool const playChatMessage) /*override*/;
    virtual void resetRot() /*override*/;
    virtual void resetUserPos(::ActorResetRule resetRule) /*override*/;
    virtual void teleportTo(
        ::glm::vec3 const& pos,
        bool          shouldStopRiding,
        int           cause,
        int           sourceEntityType,
        bool          keepVelocity
    ) /*override*/;
    virtual void changeDimension(::DimensionType toId) /*override*/;
    virtual void changeDimension(::ChangeDimensionPacket const& packet) /*override*/;
    virtual void setPlayerGameType(GameType gameType) /*override*/;
    virtual ActorUniqueID getControllingPlayer() const /*override*/;
    virtual void _fireDimensionChanged() /*override*/;
    virtual bool isAutoJumpEnabled() const /*override*/;
    virtual bool setItemSlot(::SharedTypes::Legacy::EquipmentSlot slot, ::ItemStack const& item) /*override*/;
    virtual void setOffhandSlot(::ItemStack const& item) /*override*/;
    virtual void setArmor(::SharedTypes::Legacy::ArmorSlot slot, ::ItemStack const& item) /*override*/;
    virtual ::IMinecraftEventing* getEventing() const /*override*/;
    virtual uint getUserId() const /*override*/;
    virtual bool isInTrialMode() /*override*/;
    virtual void setAbilities(::LayeredAbilities const& newAbilities) /*override*/;
    virtual void die(::ActorDamageSource const& source) /*override*/;
    virtual void onEffectRemoved(::MobEffectInstance& effect) /*override*/;
    virtual void addExperience(int xp) /*override*/;
    virtual void addLevels(int levels) /*override*/;
    virtual bool isActorRelevant(::Actor const& actor) /*override*/;
    virtual void handleEntityEvent(::ActorEvent id, int data) /*override*/;
    virtual void checkMovementStats(::glm::vec3 const& d) /*override*/;
    virtual ::HashedString getCurrentStructureFeature() const /*override*/;
    virtual bool isTeacher() const /*override*/;
    virtual bool isExternalCommunicationAllowed() const /*override*/;
    virtual void sendInventoryTransaction(::InventoryTransaction const& transaction) const /*override*/;
    virtual void sendComplexInventoryTransaction(::std::unique_ptr<::ComplexInventoryTransaction> transaction) const
        /*override*/;
    virtual void sendNetworkPacket(::Packet& packet) const /*override*/;
    virtual ::PlayerEventCoordinator& getPlayerEventCoordinator() /*override*/;
    virtual void applySnapshot(::EntityContext const &snapshotEntity,::MovementDataExtractionUtility::SnapshotAccessor const &originalSnapshotEntity) /*override*/;
    virtual void destroyEditorPlayer() /*override*/;
    virtual /*::Bedrock::NonOwnerPointer<::Editor::IEditorPlayer>*/ void getEditorPlayer() const /*override*/;
    virtual void requestMissingSubChunk(::SubChunkPos const& scp) /*override*/;
    virtual uchar getMaxChunkBuildRadius() const /*override*/;
    virtual void setBehaviorCommandStatus(::std::string const& name, BehaviorStatus status) /*override*/;
    virtual ::std::unique_ptr<::ISparseContainerSetListener> createSparseContainerListener() /*override*/;
    virtual void emitCriticalHitParticles(::Actor const& target, int particleCount) /*override*/;
    virtual ::ActorHurtResult _hurt(::ActorDamageSource const& source, float damage, ::HurtParameters const& hurtParameters) /*override*/;
    // NOLINTEND


    void displayClientMessage(const std::string& msg) {
        displayClientMessage(msg, std::nullopt);
    }

    ActorHeadRotationComponent* getActorHeadRotationComponent();
};