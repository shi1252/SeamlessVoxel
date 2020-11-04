#pragma once
#include "Defined.h"
#include "VoxelBlock.h"
#include "InstancingMesh.h"

#define CHUNKSIZE 16// Total cell size by chunk: 16 * 256 * 16

enum class EChunkState
{
	NONE,
	LOADING,
	DONE
};

struct VoxelInstanceType
{
public:
	VoxelInstanceType(XMFLOAT3 position, UINT voxelType)
		: position(position), voxelType(voxelType) {}

public:
	XMFLOAT3 position;
	UINT voxelType;
};

struct VoxelChunk
{
public:
	VoxelChunk() : state(EChunkState::NONE) {}
	VoxelChunk(XMINT2 position) : position(position) {}

	void Render();

	void CreateNewChunk(const XMINT2& position);

	void SetCell(XMUINT3 position, const VoxelCellType& type);
	bool CreateMesh();

public:
	VoxelBlock blocks[CHUNKSIZE];
	XMINT2 position = XMINT2(0, 0);
	EChunkState state = EChunkState::NONE;

	bool isChanged = false;
	InstancingMesh<VoxelInstanceType>* mesh = nullptr;
};