#pragma once
#include "Defined.h"
#include "VoxelCell.h"

#define BLOCKSIZE 4096 // 16*16*16

struct VoxelBlock
{
public:
	VoxelBlock() = default;
	VoxelBlock(const VoxelBlock&) = delete;

	//void Render();

	void Initialize();

	bool SetCell(UINT index, const VoxelCellType& type);
	VoxelCell GetCell(UINT index) { return cells[index]; }

	static UINT Index(int x, int y, int z) { return (x + (16 * (y + 16 * z))); }
	static UINT Index(const XMINT3& position) { return (position.x + (16 * (position.y + 16 * position.z))); }
	float FillRate() { return ((float)fillCount / BLOCKSIZE); }

public:
	unsigned int fillCount = 0;

private:
	VoxelCell cells[BLOCKSIZE];
};