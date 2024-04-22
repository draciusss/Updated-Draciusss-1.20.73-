#pragma once

class Actor;
class Mob;
class Player;
class Level;
class Container;
class CompoundTag;
class SemVersion;
class Experiments;
struct RenderParams;
class BaseGameVersion;
class ItemDescriptor;
class ItemInstance;
class ItemStackBase;
class ItemStack;
class IDataInput;
class IDataOutput;
class ReadOnlyBinaryStream;

class Item
{
public:
	/*std::string getNameId()
	{
		return *(std::string*)((uintptr_t)(this) + 0x110);
	}*/

	bool isPickaxe(void) {
		if (((std::string)name).find("pickaxe") != std::string::npos) return true;
		return false;
	}

	bool isShooter(void) {
		//if ((name).find("bow") != std::string::npos) return true;
		//if ((name).find("arrow") != std::string::npos) return true;
		if (itemId == 300) return true;
		return false;
	}

	bool isThrowablePotion() {
		if ((name).find("splash_potion") != std::string::npos || (name).find("lingering_potion") != std::string::npos) return true;
		return false;
	}

	bool nameContains(const std::string& itemName) {
		if ((name).find(itemName) != std::string::npos) return true;
		return false;
	}

	BUILD_ACCESS(int, itemId, 0xA2);
	BUILD_ACCESS(uint16_t, maxStackSize, 0xA0);
	BUILD_ACCESS(int16_t, MaxUseDuration, 0x14C);
	BUILD_ACCESS(std::string, name, 0xD0); //this->mRawNameId in china db (find in constructor of Item class)
public:
	virtual ~Item();
	virtual void initServer(__int64, SemVersion const&, bool, Experiments const&);
	virtual void tearDown(void);
	virtual void setDescriptionId(std::string const&);
	virtual void getDescriptionId(void);
	virtual void getMaxUseDuration(ItemStack const*);
	virtual bool isMusicDisk(void);
	virtual void executeEvent(ItemStackBase&, std::string const&, RenderParams&);
	virtual bool isComponentBased(void);
	virtual bool isArmor(void);
	virtual bool isBlockPlanterItem(void);
	virtual bool isBucket(void);
	virtual bool isCandle(void);
	virtual bool isDamageable(void);
	virtual bool isDyeable(void);
	virtual bool isDye(void);
	virtual void getItemColor(void);
	virtual bool isFertilizer(void);
	virtual bool isFood(void);
	virtual bool isThrowable(void);
	virtual bool isUseable(void);
	virtual bool isTrimAllowed(void);
	virtual void getComponent();
	virtual void getFood(void);
	virtual void setMaxDamage(int);
	virtual void setMaxUseDuration(int);
	virtual void buildNetworkTag(void);
	virtual void initializeFromNetwork(CompoundTag const&);
	virtual void validateFromNetwork(CompoundTag const&);
	virtual void getBlockShape(void);
	virtual bool canBeDepleted(void);
	virtual bool canDestroySpecial(Block const&);
	virtual void getLevelDataForAuxValue(int);
	virtual bool isStackedByData(void);
	virtual void getMaxDamage(void);
	virtual void getAttackDamage(void);
	virtual bool isHandEquipped(void);
	virtual bool isGlint(ItemStackBase const&);
	virtual bool isPattern(void);
	virtual void getPatternIndex(void);
	virtual void showsDurabilityInCreative(void);
	virtual bool isWearableThroughLootTable(CompoundTag const*);
	virtual bool canDestroyInCreative(void);
	virtual bool isDestructive(int);
	virtual bool isLiquidClipItem(void);
	virtual void shouldInteractionWithBlockBypassLiquid(Block const&);
	virtual void requiresInteract(void);
	virtual void appendFormattedHovertext(ItemStackBase const&, Level&, std::string&, bool);
	virtual bool isValidRepairItem(ItemStackBase const&, ItemStackBase const&, BaseGameVersion const&);
	virtual void getEnchantSlot(void);
	virtual void getEnchantValue(void);
	virtual void getArmorValue(void);
	virtual void getToughnessValue(void);
	virtual bool isComplex(void);
	virtual bool isValidAuxValue(int);
	virtual void getDamageChance(int);
	virtual void getViewDamping(void);
	virtual void uniqueAuxValues(void);
	virtual bool isActorPlacerItem(void);
	virtual bool isMultiColorTinted(ItemStack const&);
	virtual void getColor(CompoundTag const*, ItemDescriptor const&);
	virtual bool hasCustomColor(ItemStackBase const&);
	virtual bool hasCustomColor(CompoundTag const*);
	virtual void clearColor(ItemStackBase&);
	virtual void clearColor(CompoundTag*);
	virtual void setColor(ItemStackBase&, UIColor const&);
	virtual void getBaseColor(ItemStack const&);
	virtual void getSecondaryColor(ItemStack const&);
	virtual void getActorIdentifier(ItemStack const&);
	virtual void buildIdAux(short, CompoundTag const*);
	virtual bool canUseOnSimTick(void);
	virtual void use(ItemStack&, Player&);
	virtual void dispense(BlockSource&, Container&, int, Vector3<float> const&, uint8_t);
	virtual void useTimeDepleted(ItemStack&, Level*, Player*);
	virtual void releaseUsing(ItemStack&, Player*, int);
	virtual void getDestroySpeed(ItemStackBase const&, Block const&);
	virtual void hurtActor(ItemStack&, Actor&, Mob&);
	virtual void hitActor(ItemStack&, Actor&, Mob&);
	virtual void hitBlock(ItemStack&, Block const&, Vector3<int> const&, Mob&);
	virtual void mineBlock(ItemStack&, Block const&, int, int, int, Actor*);
	virtual void buildDescriptionName(ItemStackBase const&);
	virtual void buildDescriptionId(ItemDescriptor const&, CompoundTag const*);
	virtual void buildEffectDescriptionName(ItemStackBase const&);
	virtual void readUserData(ItemStackBase&, IDataInput&, ReadOnlyBinaryStream&);
	virtual void writeUserData(ItemStackBase const&, IDataOutput&);
	virtual void getMaxStackSize(ItemDescriptor const&);
	virtual void inventoryTick(ItemStack&, Level&, Actor&, int, bool);
	virtual void refreshedInContainer(ItemStackBase const&, Level&);
	virtual void getCooldownType(void);
	virtual void getCooldownTime(void);
	virtual void fixupCommon(ItemStackBase&);
	virtual void fixupCommon(ItemStackBase&, Level&);
	virtual void getInHandUpdateType(Player const&, ItemInstance const&, ItemInstance const&, bool, bool);
	virtual void getInHandUpdateType(Player const&, ItemStack const&, ItemStack const&, bool, bool);
	virtual void validFishInteraction(int);
	virtual void enchantProjectile(ItemStackBase const&, Actor&);
	virtual void getEquipLocation(void);
	virtual void getEquipSound(void);
	virtual void shouldSendInteractionGameEvents(void);
	virtual void useInterruptedByAttacking(void);
	virtual bool hasSameRelevantUserData(ItemStackBase const&, ItemStackBase const&);
	virtual void initClient(__int64, SemVersion const&, bool, Experiments const&);
	virtual void setIconInfo(std::string const&, int);
	virtual void getIconInfo(ItemStackBase const&, int, bool);
	virtual void getInteractText(Player const&);
	virtual void getAnimationFrameFor(Mob*, bool, ItemStack const*, bool);
	virtual bool isEmissive(int);
	virtual void getLightEmission(int);
	virtual void getIconYOffset(void);
	virtual bool canBeCharged(void);
	virtual void playSoundIncrementally(ItemStack const&, Mob&);
	virtual void getFurnaceXPmultiplier(ItemStackBase const&);
	virtual void getAuxValuesDescription(void);
	virtual void calculatePlacePos(ItemStackBase&, Actor&, uint8_t&, Vector3<int>&);
	virtual void _checkUseOnPermissions(Actor&, ItemStackBase&, uint8_t const&, Vector3<int> const&);
	virtual void _calculatePlacePos(ItemStackBase&, Actor&, uint8_t&, Vector3<int>&);
	virtual void _shouldAutoCalculatePlacePos(void);
	virtual void _useOn(ItemStack&, Actor&, Vector3<int>, uint8_t, Vector3<float> const&);
	virtual void PacketSendingEvent(ItemStack&, Actor&, Vector3<int>, uint8_t, Vector3<float> const&);
};