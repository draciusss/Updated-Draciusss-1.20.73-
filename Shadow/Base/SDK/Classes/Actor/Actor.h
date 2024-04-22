#pragma once
#include "../../../EnTT/EnTT.hpp" // Including entt here to avoid overriding.

#include "../../Components/StateVectorComponent.h"
#include "../../Components/AABBShapeComponent.h"
#include "../../Components/ActorHeadRotationComponent.h"
#include "../../Components/MobBodyRotationComponent.h"
#include "../../Components/RenderPositionComponent.h"
#include "../../Components/MovementInterpolatorComponent.h"

#include "ActorFlags.h"

#pragma region Classes & Structs

class EntityRegistry;
class ActorLocation;
class ActorDamageSource;
class NewInteractionModel;
class InputMode;
class UIProfanityContext;
class ActorUniqueID;
class ActorDamageCause;
class ItemStack;
class ActorEvent;
class ArmorSlot;
class EquipmentSlot;
class DataLoadHelper;
class ActorLink;
class LevelSoundEvent;
class AnimationComponent;
class RenderParams;
class HandSlot;
class EquipmentTableDefinition;
class Options;
class ActorInteraction;
class ItemStackBase;
class MobEffectInstance;
class Attribute;
class AnimationComponentGroupType;
class ItemUseMethod;
class ResolvedTextObject;
class INpcDialogueData;
class IConstBlockSource;
class ChalkboardBlockPlayer;
class BlockPlayer;
class Block;
class Tick;
class ChunkSource;
class LayeredAbilities;
class ChunkPos;
class MovementEventType;
class PlayerMovementSettings;
class Item;
class Container;
class EventPacket;
class GameType;

class ActorInitializationMethod;
class InitializationMethod;
class VariantParameterList;

class ChalkboardBlockActor;
class ResolvedTextObject;
struct TextObjectRoot;
class SubChunkPos;
class ChunkSource;
class LayeredAbilities;

class ContainerContentChangeListener;
class ItemStack;

// packets
class ChangeDimensionPacket;

#pragma endregion

/*struct EntityId final { // EntityID for entt
	using entity_type = std::uint32_t;
	static constexpr auto null = entt::null;

	constexpr EntityId(entity_type value = null) noexcept
		: entt{ value } {}

	~EntityId() noexcept = default;

	constexpr EntityId(const EntityId& other) = default;
	constexpr EntityId(EntityId&& other) noexcept = default;

	constexpr EntityId& operator=(const EntityId& other) = default;
	constexpr EntityId& operator=(EntityId&& other) noexcept = default;

	constexpr operator entity_type() const noexcept {
		return entt;
	}

private:
	entity_type entt;
};

class EntityContext { // EntityContext to register Components
public:
	EntityRegistry* mRegistry;
	entt::basic_registry<EntityId>* registry;
	EntityId id{};

	template<typename T>
	T* getComponent() {
		auto tryget = const_cast<T*>(registry->try_get<T>(id));
		if (tryget) {
			return tryget;
		}
		return nullptr;
	}
};*/

using EntityId = uint32_t;

class EntityRegistry {
	char pad0x0[0x30];
public:
	uintptr_t& registry;
};

class EntityContext {
public:
	EntityRegistry& registry;
	uintptr_t& basicReg;
	EntityId id;
};

// Actor VTable
class Actor { // 1.20.71
public:
	virtual void Destructor(); // ~Actor(); // Actor Destructor
public:
	virtual bool getStatusFlag(ActorFlags);
	virtual void setStatusFlag(ActorFlags, bool);
	virtual void outOfWorld(void);
	virtual void reloadHardcoded(ActorInitializationMethod, VariantParameterList const&);
	virtual void reloadHardcodedClient(ActorInitializationMethod, VariantParameterList const&);
	virtual void initializeComponents(ActorInitializationMethod, VariantParameterList const&);
	virtual void reloadComponents(ActorInitializationMethod, VariantParameterList const&);
	virtual void _serverInitItemStackIds(void);
	virtual void _doInitialMove(void);
	virtual bool hasComponent(); // HashedString const&
	//virtual ~Actor(); // virtual void ~Actor();
	virtual void resetUserPos(bool);
	virtual void getOwnerEntityType(void);
	virtual void remove(void);
	virtual bool isRuntimePredictedMovementEnabled(void);
	virtual void getFiringPos(void);
	virtual void getInterpolatedBodyRot(float);
	virtual void getInterpolatedHeadRot(float);
	virtual void getInterpolatedBodyYaw(float);
	virtual void getYawSpeedInDegreesPerSecond(void);
	virtual void getInterpolatedRidingOffset(float, int);
	virtual void resetInterpolated(void);
	virtual bool isFireImmune(void);
	virtual void blockedByShield(ActorDamageSource const&, Actor&);
	virtual bool canDisableShield(void);
	virtual void teleportTo(Vector3<float> const&, bool, int, int, bool);
	virtual void lerpMotion(Vector3<float> const&);
	virtual void tryCreateAddActorPacket(void);
	virtual void normalTick(void);
	virtual void baseTick(void);
	virtual void passengerTick(void);
	virtual void startRiding(Actor&);
	virtual void addPassenger(Actor&);
	virtual void getExitTip(std::string const&, InputMode, NewInteractionModel);
	virtual void getEntityLocNameString(void);
	virtual bool isInWall(void);
	virtual bool isInvisible(void);
	virtual bool canShowNameTag(void);
	virtual std::string getFormattedNameTag(void);
	virtual void getNameTagTextColor(void);
	virtual void getShadowRadius(void);
	virtual void getHeadLookVector(float);
	virtual bool canInteractWithOtherEntitiesInGame(void);
	virtual void getBrightness(float, IConstBlockSource const&);
	virtual void playerTouch(); // Player&
	virtual bool isImmobile(void);
	virtual bool isSilentObserver(void);
	virtual bool isPickable(void);
	virtual bool isSleeping(void);
	virtual void setSleeping(bool);
	virtual void setSneaking(bool);
	virtual bool isBlocking(void);
	virtual bool isDamageBlocked(ActorDamageSource const&);
	virtual bool isAlive(void);
	virtual bool isOnFire(void);
	virtual bool isSurfaceMob(void);
	virtual bool isTargetable(void);
	virtual bool isLocalPlayer(void);
	virtual bool isPlayer(void);
	virtual bool canAttack(Actor*, bool);
	virtual void setTarget(Actor*);
	virtual bool isValidTarget(Actor*);
	virtual void attack(Actor&, ActorDamageCause const&);
	virtual void performRangedAttack(Actor&, float);
	virtual void setOwner(ActorUniqueID);
	virtual void setSitting(bool);
	virtual void onTame(void);
	virtual void onFailedTame(void);
	virtual void setStanding(bool);
	virtual bool canPowerJump(void);
	virtual bool isEnchanted(void);
	virtual void shouldRender(void);
	virtual void playAmbientSound(void);
	virtual void getAmbientSound(void);
	virtual bool isInvulnerableTo(ActorDamageSource const&);
	virtual void getBlockDamageCause(Block const&);
	virtual void doFireHurt(int);
	virtual void onLightningHit(void);
	virtual void feed(int);
	virtual void handleEntityEvent(ActorEvent, int);
	virtual void getPickRadius(void);
	virtual void getActorRendererId(void);
	virtual void despawn(void);
	virtual void setArmor(ArmorSlot, ItemStack const&);
	virtual void getArmorMaterialTypeInSlot(ArmorSlot);
	virtual void getArmorMaterialTextureTypeInSlot(ArmorSlot);
	virtual void getArmorColorInSlot(ArmorSlot, int);
	virtual void setEquippedSlot(int, ItemStack const&);
	virtual void setCarriedItem(ItemStack const&);
	virtual void getCarriedItem(void);
	virtual void setOffhandSlot(ItemStack const&);
	virtual void getEquippedTotem(void);
	virtual void consumeTotem(void);
	virtual void save(CompoundTag&);
	virtual void load(CompoundTag const&, DataLoadHelper&);
	virtual void queryEntityRenderer(void);
	virtual void getSourceUniqueID(void);
	virtual bool canFreeze(void);
	virtual void getLiquidAABB(MaterialType);
	virtual void handleInsidePortal(Vector3<int> const&);
	virtual bool canChangeDimensionsUsingPortal(void);
	virtual void changeDimension(int, int);
	virtual void changeDimension(ChangeDimensionPacket const&);
	virtual void getControllingPlayer(void);
	virtual void causeFallDamageToActor(float, float, ActorDamageSource);
	virtual void onSynchedDataUpdate(int);
	virtual bool canAddPassenger(Actor&);
	virtual bool canPickupItem(ItemStack const&);
	virtual bool canBePulledIntoVehicle(void);
	virtual void inCaravan(void);
	virtual void sendMotionPacketIfNeeded(PlayerMovementSettings const&);
	virtual bool canSynchronizeNewEntity(void);
	virtual void startSwimming(void);
	virtual void stopSwimming(void);
	virtual void buildDebugInfo(std::string&);
	virtual void getCommandPermissionLevel(void);
	virtual void getDeathTime(void);
	virtual bool canBeAffected(int);
	virtual bool canBeAffectedByArrow(MobEffectInstance const&);
	virtual void onEffectRemoved(MobEffectInstance&);
	virtual bool canObstructSpawningAndBlockPlacement(void);
	virtual void getAnimationComponent(void);
	virtual void openContainerComponent(); // Player&
	virtual void swing(void);
	virtual void useItem(ItemStackBase&, ItemUseMethod, bool);
	virtual void getDebugText(std::vector<std::string>&);
	virtual void getMapDecorationRotation(void);
	virtual void getPassengerYRotation(Actor const&);
	virtual void add(ItemStack&);
	virtual void drop(ItemStack const&, bool);
	virtual void getInteraction();
	virtual bool canDestroyBlock(Block const&);
	virtual void setAuxValue(int);
	virtual void startSpinAttack(void);
	virtual void stopSpinAttack(void);
	virtual void renderDebugServerState(Options const&);
	virtual void kill(void);
	virtual void die(ActorDamageSource const&);
	virtual void shouldDropDeathLoot(void);
	virtual void applySnapshot(EntityContext const&, EntityContext const&);
	virtual void getNextStep(float);
	virtual void onPush(Actor&);
	virtual void getLastDeathPos(void);
	virtual void getLastDeathDimension(void);
	virtual bool hasDiedBefore(void);
	virtual void doEnterWaterSplashEffect(void);
	virtual void doExitWaterSplashEffect(void);
	virtual void doWaterSplashEffect(void);
	virtual void _shouldProvideFeedbackOnHandContainerItemSet(int, ItemStack const&);
	virtual void _shouldProvideFeedbackOnArmorSet(ArmorSlot, ItemStack const&);
	virtual void updateEntitySpecificMolangVariables(RenderParams&);
	virtual void shouldTryMakeStepSound(void);
	virtual void _hurt(ActorDamageSource const&, float, bool, bool);
	virtual void readAdditionalSaveData(CompoundTag const&, DataLoadHelper&);
	virtual void addAdditionalSaveData(CompoundTag&);
	virtual void _playStepSound(Vector3<int> const&, Block const&);
	virtual void _doAutoAttackOnTouch(Actor&);
public:
	PlayerInventory* getSupplies() {
		uintptr_t address = reinterpret_cast<uintptr_t>(this);
		return *reinterpret_cast<PlayerInventory**>(address + 0x7B0);
	}

	std::string* getNametag() {
		void* signature = Utils::findSig("48 83 EC ? 48 8B 81 ? ? ? ? 48 85 C0 74 3B 48 8B 08 BA ? ? ? ? 48 8B 40 ? 48 2B C1 48 C1 F8 ? 66 3B D0 73 17");

		auto getNameTag = reinterpret_cast<std::string*(__thiscall*)(Actor*)>(signature); // NameTags signature address
		return getNameTag(this); // Return FunctionCall as Actor
	}

	/*Vector3<float> getRenderPos() { // getRenderPosition Inside RenderPositionComponent
		auto component = EntityContext.getComponent<RenderPositionComponent>(); // get the Component using entt
		if (component != nullptr) { return component->renderPos; } // If the component isn't nullptr return the Vector3
		return { 0, 0, 0 }; // If component is nullptr return 0, 0, 0 for Render Position
	}*/

	float getBlocksPerSecond() {
		if (this != nullptr) {
			Vector3<float> targetPos = this->getStateVector()->Position;
			Vector3<float> targetPosOld = this->getStateVector()->PrevPosition;
			float hVel = sqrtf(((targetPos.x - targetPosOld.x) * (targetPos.x - targetPosOld.x)) + ((targetPos.z - targetPosOld.z) * (targetPos.z - targetPosOld.z)));
			return hVel * 20;
		}
	}

	bool isOnGround() { // ActorCollision::isOnGround
		static auto isOnGroundFunc = Utils::findSig("4C 8B 41 ? 4C 8B D1 48 8B 41 ? 4C 8B 49 ? 49 2B C0 48 C1 F8 ? 48 FF C8 25 ? ? ? ? 49 8B 0C C0 48 83 F9 ? 74 ? 0F 1F 84 00 ? ? ? ? 48 C1 E1 ? 42 81 7C 09 ? ? ? ? ? 4A 8D 04 09 74 ? 48 8B 08 48 83 F9 ? 75 ? 49 8B 42 ? 49 3B 42 ? 74 ? 48 8B 48 ? 48 85 C9 74 ? 4C 8B 49 ? 8B 02 8B D0 48 8B 49 ? 81 E2 ? ? ? ? 49 2B C9 44 8B C2 49 C1 E8 ? 48 C1 F9 ? 4C 3B C1 73 ? 4B 8B 0C C1 48 85 C9 74 ? 81 E2 ? ? ? ? 4C 8D 04 91 4D 85 C0 74 ? 25 ? ? ? ? 41 33 00 3D ? ? ? ? 73 ? B0 ? C3 32 C0 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 4C 8B 41 ? 41 BB ? ? ? ? 48 8B 41 ? 4C 8B D1 4C 8B 49 ? 49 2B C0 48 C1 F8 ? 48 FF C8 49 23 C3 49 8B 04 C0 48 83 F8 ? 74 ? 0F 1F 40 ? 48 C1 E0 ? 46 39 5C 08 ? 4A 8D 0C 08 74 ? 48 8B 01 48 83 F8 ? 75 ? 49 8B 4A ? 49 3B 4A ? 74 ? 48 8B 49 ? 48 85 C9 74 ? 4C 8B 49 ? 8B 02 8B D0 48 8B 49 ? 81 E2 ? ? ? ? 49 2B C9 44 8B C2 49 C1 E8 ? 48 C1 F9 ? 4C 3B C1 73 ? 4B 8B 0C C1 48 85 C9 74 ? 81 E2 ? ? ? ? 4C 8D 04 91 4D 85 C0 74 ? 25 ? ? ? ? 41 33 00 3D ? ? ? ? 73 ? B0 ? C3 32 C0 C3 CC CC CC 4C 8B 41 ? 4C 8B D1 48 8B 41 ? 4C 8B 49 ? 49 2B C0 48 C1 F8 ? 48 FF C8 25 ? ? ? ? 49 8B 0C C0 48 83 F9 ? 74 ? 0F 1F 84 00 ? ? ? ? 48 C1 E1 ? 42 81 7C 09 ? ? ? ? ? 4A 8D 04 09 74 ? 48 8B 08 48 83 F9 ? 75 ? 49 8B 42 ? 49 3B 42 ? 74 ? 48 8B 48 ? 48 85 C9 74 ? 4C 8B 49 ? 8B 02 8B D0 48 8B 49 ? 81 E2 ? ? ? ? 49 2B C9 44 8B C2 49 C1 E8 ? 48 C1 F9 ? 4C 3B C1 73 ? 4B 8B 0C C1 48 85 C9 74 ? 81 E2 ? ? ? ? 4C 8D 04 91 4D 85 C0 74 ? 25 ? ? ? ? 41 33 00 3D ? ? ? ? 73 ? B0 ? C3 32 C0 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 4C 8B 41 ? 41 BB");
		void* rcx = *(void**)((__int64)this + 0x10);
		int rda = *(int*)((__int64)this + 0x18);
		return Utils::CallFunc<bool, void*, int*>((void*)isOnGroundFunc, rcx, &rda);
	}

	void jumpFromGround() {
		static auto jumpFromGroundFunc = Utils::findSig("40 53 48 83 EC ? 48 8B DA BA ? ? ? ? E8 ? ? ? ? 8B 0B 4C 8B D0 44 8B C1 41 81 E0 ? ? ? ? 41 8B C0 4D 8B 4A ? 49 8B 52 ? 49 2B D1 48 C1 E8 ? 48 C1 FA ? 48 3B C2 73 ? 49 8B 04 C1 48 85 C0 74 ? 41 81 E0 ? ? ? ? 4A 8D 14 80 48 85 D2 74 ? 8B C1 25 ? ? ? ? 33 02 3D ? ? ? ? 72 ? 89 4C 24 ? 4C 8D 44 24 ? 49 8B CA 48 C7 44 24 ? ? ? ? ? 45 33 C9 48 8D 54 24 ? E8 ? ? ? ? 48 8D 05 ? ? ? ? 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC CC CC CC CC 40 55 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B E9 89 54 24 ? 48 83 C1 ? 48 8D 54 24 ? 4C 8D 44 24 ? E8 ? ? ? ? 48 8B 30 48 83 7E ? ? 0F 85 ? ? ? ? 48 89 9C 24 ? ? ? ? 48 8D 4C 24 ? 48 89 BC 24 ? ? ? ? 4C 89 B4 24 ? ? ? ? E8 ? ? ? ? 45 33 F6 48 8B 08 48 8B 50 ? 4C 89 30 4C 89 70 ? 48 89 4E ? 48 8B 4E ? 48 89 56 ? 48 85 C9 74 ? E8 ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? E8 ? ? ? ? 48 8B 7E ? 48 8D 4C 24 ? 48 8B D5 48 8B 07 4C 89 74 24 ? 4C 89 74 24 ? 4C 89 74 24 ? 48 8B 58 ? 44 88 74 24 ? E8 ? ? ? ? 48 8D 54 24 ? 48 8B CF 48 8B C3 FF 15 ? ? ? ? 4C 8B B4 24 ? ? ? ? 48 8B BC 24 ? ? ? ? 48 8B 9C 24 ? ? ? ? 48 8B 46 ? 48 8B 4C 24 ? 48 33 CC E8 ? ? ? ? 48 81 C4 ? ? ? ? 5E 5D C3 CC CC CC CC CC CC CC 48 89 5C 24 ? 57 48 83 EC ? 48 8B 01");
		void* rcx = *(void**)((__int64)this + 0x10);
		int rda = *(int*)((__int64)this + 0x18);
		Utils::CallFunc<void, void*, int*>((void*)jumpFromGroundFunc, rcx, &rda);
	}

	ActorCollision* getActorCollision() {
		return (ActorCollision*)((uintptr_t)this + 8);
	}
public:
	// EntityContext
	//BUILD_ACCESS(class EntityContext, EntityContext, 0x8); // 1.20.71
	// Level Actor::getLevel(void)
	BUILD_ACCESS(class Level*, Level, 0x258); // 1.20.71
	// StateVectorComponent Actor::getPosition(void)
	BUILD_ACCESS(StateVectorComponent*, StateVector, 0x298); // 1.20.71
	// AABBShapeComponent Actor::getAABB(void) or StateVector + 8
	BUILD_ACCESS(AABBShapeComponent*, AABBShape, 0x2A0); // 1.20.71
	// MovementInterpolatorComponent Actor::getRotation(void) or StateVector + 16
	BUILD_ACCESS(MovementInterpolatorComponent*, MovementInterpolator, 0x2A8); // 1.20.71
};