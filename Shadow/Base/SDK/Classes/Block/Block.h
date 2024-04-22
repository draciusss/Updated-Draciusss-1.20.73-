#pragma once

#include "MaterialType.h"
#include "Material.h"
#include "BlockLegacy.h"

class Block
{
public:
	BlockLegacy* GetBlockLegacy() {
		return *reinterpret_cast<BlockLegacy**>((uintptr_t)this + 0x30); // 1.20.71
	}

	TextHolder* GetTileName()
	{
		return GetBlockLegacy()->GetTileName();
	}

	Material* GetMaterial()
	{
		return GetBlockLegacy()->GetMaterial();
	}

	MaterialType GetMaterialType()
	{
		return GetBlockLegacy()->GetMaterial()->type;
	}
};
