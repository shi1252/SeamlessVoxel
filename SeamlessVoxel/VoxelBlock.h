#pragma once
#include "Defined.h"
#include "VoxelCell.h"
#include "InstancingMesh.h"

#define BLOCKSIZE 4096 // 16*16*16

class VoxelChunk;

struct VoxelInstanceType
{
public:
	VoxelInstanceType(XMFLOAT3 position, UINT voxelType)
		: position(position), voxelType(voxelType) {}

public:
	XMFLOAT3 position;
	UINT voxelType;
};

struct VoxelBlock
{
public:
	VoxelBlock() = default;
	VoxelBlock(const VoxelBlock&) = delete;
	~VoxelBlock();

	//void Render();

	void Initialize();

	void Render(const int& block);

	bool SetCell(UINT index, const VoxelCellType& type);
	VoxelCell GetCell(UINT index) { return cells[index]; }

	static UINT Index(int x, int y, int z) { return (x + (16 * (y + 16 * z))); }
	static UINT Index(const XMINT3& position) { return (position.x + (16 * (position.y + 16 * position.z))); }
	float FillRate() { return ((float)fillCount / BLOCKSIZE); }

	bool CreateMesh(const int& block);
	void ReleaseMesh();

	char* GetBytesFromBlcok();

public:
	VoxelCell cells[BLOCKSIZE];
	unsigned int fillCount = 0;

	bool isChanged = true;
	InstancingMesh<VoxelInstanceType>* mesh = nullptr;
	VoxelChunk* parent = nullptr;
};