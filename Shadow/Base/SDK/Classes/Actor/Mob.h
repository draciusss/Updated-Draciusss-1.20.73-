#pragma once

// This should be in Actor but using Mob for a clean base since Mob is a child of Actor.
// Mob VTable
class Mob : public Actor { // 1.20.71
public:
	virtual void knockback(Actor*, int, float, float, float, float, float);
	virtual void spawnAnim(void);
	virtual void setSprinting(bool);
	virtual void getHurtSound(void);
	virtual void getDeathSound(void);
	virtual void getSpeed(void);
	virtual void setSpeed(float);
	virtual void hurtEffects(ActorDamageSource const&, float, bool, bool);
	virtual void aiStep(void);
	virtual void pushActors(void);
	virtual void checkSpawnRules(bool);
	virtual void checkSpawnObstruction(void);
	virtual void getItemUseDuration(void);
	virtual void getItemUseStartupProgress(void);
	virtual void getItemUseIntervalProgress(void);
	virtual void getMaxHeadXRot(void);
	virtual bool isAlliedTo(Mob*);
	virtual void doHurtTarget(Actor*, ActorDamageCause const&);
	virtual void getArmorValue(void);
	virtual void hurtArmorSlots(ActorDamageSource const&, int, __int64);
	virtual void setDamagedArmor(ArmorSlot, ItemStack const&);
	virtual void sendArmorDamage(__int64);
	virtual void sendArmor(__int64);
	virtual void getAllHand(void);
	virtual void getAllEquipment(void);
	virtual void dropEquipmentOnDeath(ActorDamageSource const&, int);
	virtual void dropEquipmentOnDeath(void);
	virtual void clearVanishEnchantedItemsOnDeath(void);
	virtual void sendInventory(bool);
	virtual void getDamageAfterEnchantReduction(ActorDamageSource const&, float);
	virtual void createAIGoals(void);
	virtual void onBorn(Actor&, Actor&);
	virtual void setItemSlot(int, ItemStack const&);
	virtual void setTransitioningSitting(bool);
	virtual void _getWalkTargetValue(Vector3<int> const&);
	virtual bool canExistWhenDisallowMob(void);
	virtual void initBodyControl(void);
	virtual void newServerAiStep(void);
	virtual void tickDeath(void);
	virtual void _getAdjustedAABBForSpawnCheck(float, Vector3<float> const&);
};
