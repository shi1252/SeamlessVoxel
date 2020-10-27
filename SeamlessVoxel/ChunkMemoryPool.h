#pragma once
#include "Defined.h"

class VoxelChunk;
class RegionFileManager;

class ChunkMemoryPool
{
public:
	ChunkMemoryPool(unsigned int size = 32);
	~ChunkMemoryPool();

	void Render();

	//void LoadChunk();
	void ReleaseChunk(int index);

private:
	void SaveAllChunks();

private:
	VoxelChunk* pool = nullptr;
	UINT poolSize;
	UINT halfSize;

	RegionFileManager* rfm = nullptr;
};