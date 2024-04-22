#pragma once

#include <functional>

class GameMode
{
public:
    BUILD_ACCESS(Player*, player, 0x8);
    BUILD_ACCESS(float, lastBreakProgress, 0x20);
private:
    virtual void Destructor(); // 0 // GameMode Destructor
public:
    virtual void startDestroyBlock(Vector3<int> const&, unsigned char blockSide, bool auth); // 1
    virtual void destroyBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 2
    virtual void continueDestroyBlock(Vector3<int> const& a1, unsigned char a2, Vector3<float> const& a3, bool& a4); // 3
    virtual void stopDestroyBlock(Vector3<int> const& a1); // 4
    virtual void startBuildBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 5
    virtual void buildBlock(Vector3<int> const& a1, unsigned char a2, bool auth); // 6
    virtual void continueBuildBlock(Vector3<int> const& a1, unsigned char a2); // 7
    virtual void stopBuildBlock(); // 8
    virtual void tick(); // 9
    virtual void getPickRange(class InputMode const& a1, bool a2); // 10
    virtual void useItem(class ItemStack& a1); // 11
    virtual void useItemOn(ItemStack& a1, Vector3<int> const& a2, unsigned char a3, Vector3<float> const& a4, class Block const* a5); // 12
    virtual void interact(Actor* a1, Vector3<float> const& a2); // 13
    virtual void attack(Actor&); // 14
    virtual void releaseUsingItem(); // 15
    virtual void setTrialMode(bool a1); // 16
    virtual void isInTrialMode(); // 17
    virtual void registerUpsellScreenCallback(std::function<void> a3); // 18
};