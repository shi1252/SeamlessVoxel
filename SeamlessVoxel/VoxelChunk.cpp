#include "VoxelChunk.h"
#include "PrimitiveGenerator.h"
#include "SeamlessVoxelEngine.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Cube.h"

void VoxelChunk::Render()
{
	if (state != EChunkState::DONE)
		return;

	for (int i = 0; i < CHUNKSIZE; ++i)
	{
		blocks[i].Render(i);
	}
}

void VoxelChunk::CreateNewChunk(const XMINT2& position)
{
	state = EChunkState::LOADING;
	this->position = position;
	for (int i = 0; i < CHUNKSIZE; ++i)
	{
		if (i < 8)
		{
			for (int z = 0; z < 16; ++z)
			{
				for (int y = 0; y < 16; ++y)
				{
					for (int x = 0; x < 16; ++x)
					{
						blocks[i].SetCell(VoxelBlock::Index(x, y, z), VoxelCellType::DIRT);
					}
				}
			}
		}
	}
	state = EChunkState::DONE;
}

void VoxelChunk::SetCell(XMUINT3 position, const VoxelCellType& type)
{
	UINT block = position.y / 16;
	UINT cell = position.y % 16;

	if (blocks[block].SetCell(VoxelBlock::Index(position.x, cell, position.y), type))
		blocks[block].isChanged = true;
}

bool VoxelChunk::CreateMesh()
{
	int cnt = 0;
	for (int i = 0; i < CHUNKSIZE; ++i)
	{
		if (blocks[i].CreateMesh(i))
			++cnt;
	}

	if (cnt == CHUNKSIZE)
		return true;
	
	return false;
}

void VoxelChunk::SetParentForBlocks()
{
	for (int i = 0; i < CHUNKSIZE; ++i)
	{
		blocks[i].parent = this;
	}
}
