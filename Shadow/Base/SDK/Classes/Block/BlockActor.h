#pragma once

class CompoundTag;

class BlockActor {
private:
	virtual void Destructor(); // BlockActor Destructor
	virtual __int64 load(__int64&, CompoundTag*, __int64&);

public:
	virtual bool save(CompoundTag*);
};