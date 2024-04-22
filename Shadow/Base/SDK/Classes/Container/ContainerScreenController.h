#pragma once
class ItemStack;

namespace Containers
{
	static std::string Inventory = "inventory_items";
	static std::string Hotbar = "hotbar_items";
	static std::string Container = "container_items";
};

class ContainerScreenController { // indexes cuz im gonna make them methods that aren't virtual
private:
	virtual __int64 Destructor();
public:
	virtual void tick(void);
	virtual void handleEvent(uintptr_t ScreenEvent);
	virtual __int64 getRoute(void);
	virtual void setScreenState(std::vector<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::basic_string<char, std::char_traits<char>, std::allocator<char>>>, std::allocator<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::basic_string<char, std::char_traits<char>, std::allocator<char>>>>> const&);
	virtual void onOpen(void);
	virtual void onTerminate(void);
	virtual void onInit(void);
	virtual void onDelete(void);
private:
	virtual void Function1();
public:
	virtual bool canExit(void);
	virtual void tryExit(void);
	virtual bool areControllerTabsEnabled(void);
	virtual void onCreation(void);
	virtual void logCreationTime(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, double, double, UCHAR);
private:
	virtual void Function2();
	virtual void Function3();
public:
	virtual void onLeave(void);
	virtual void leaveScreen(void);
	virtual void handleGameEventNotification(uintptr_t uiGameEventNotification);
	virtual void bind(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, UINT, int, std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, UINT, std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, uintptr_t UIPropertyBag);
	virtual void bind(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, UINT, std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, uintptr_t UIPropertyBag);
	virtual void handleLicenseChanged(void);
	virtual void onDictationEvent(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual void setAssociatedBlockPos(Vector3<int> const&);
	virtual void setAssociatedEntityUniqueID(uint64_t);
	virtual void setSuspendInput(bool); // <- i think this causes disabled movement in containers
	virtual int getCallbackInterval(void);
	virtual void onRender(void);
	virtual void addStaticScreenVars(uintptr_t JsonValue); // what the fuck is Json::Value&?
	virtual __int64 getAdditionalScreenInfo(void);
	virtual __int64 getTelemetryOverride(void);
	virtual void addEventProperties(); //removed
	virtual int getSceneType(void);
	virtual int getScreenVersion(void);
	virtual bool screenHandlesGamepadMenuButton(void);
	virtual __int64 getProxy(void);
	virtual void onEntered(void);
	virtual std::string getNameId(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&);
	virtual bool _doesScreenHaveExitBehavior(void);
	virtual bool _isStillValid(void);
	virtual bool _getGamepadHelperVisible(void);
	virtual bool _getMixedHelperVisible(void);
	virtual bool _getKeyboardHelperVisible(void);
	virtual std::string _getButtonADescription(void);
	virtual std::string _getButtonBDescription(void);
	virtual std::string _getButtonXDescription(void);
	virtual std::string _getButtonYDescription(void);
	virtual std::string _getButtonKeyboardDescription(void);
	virtual void _handlePlaceAll(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual void _handlePlaceOne(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual void _handleSelectSlot(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual __int64 _getSelectedSlotInfo(void);
	virtual __int64 _getItemStack(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual __int64 _getVisualItemStack(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual __int64 _getTakeableItemStackBase(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual void _onContainerSlotHovered(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual void _onContainerSlotSelected(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual void _onContainerSlotPressed(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual bool _shouldSwap(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int, std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual std::string _getCollectionName(uintptr_t UIPropertyBagPtr);
	virtual bool _canSplit(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, int);
	virtual void _sendFlyingItem(std::string, int);
public:
	void stealItems(std::string containerName, int slots) {
		static void* shiftClick = Utils::findSig("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 45 8B E1 49 8B F0 44 8B FA 89 54 24 40 48 8B F9 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 8F ? ? ? ? 48 85 C9 74 6B E8 ? ? ? ? 84 C0 74 62 48 8D 8F ? ? ? ? 8B 91 ? ? ? ? 39 91 ? ? ? ? 74 13 44 89 64 24 ? 4C 8B CE");

		Utils::CallFunc<void, ContainerScreenController*, uintptr_t, TextHolder, int>(
			shiftClick, this, 0x7FFFFFFF, containerName, slots);
	}

	void closeContainer() {
		this->leaveScreen();
		this->canExit();
		this->tryExit();
	}
};

class ChestScreenController : public ContainerScreenController {
};