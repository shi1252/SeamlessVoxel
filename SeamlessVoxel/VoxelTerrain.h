#pragma once
#include "Defined.h"
#include "VoxelChunk.h"
#include "ChunkMemoryPool.h"

class VoxelTerrain
{
public:
	VoxelTerrain() = default;
	VoxelTerrain(const VoxelTerrain&) = delete;
	~VoxelTerrain() = default;

	void Render();
private:

public:

private:
	ChunkMemoryPool chunkPool = ChunkMemoryPool(8);

	UINT chunkSize = 64;
	UINT viewDistance = 8;
};