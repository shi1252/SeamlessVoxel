#include "VoxelChunk.h"
#include "PrimitiveGenerator.h"
#include "SeamlessVoxelEngine.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Cube.h"
#include "PerlinNoise.h"
#include "SimplexNoise.h"

void VoxelChunk::Render()
{
	if (state != EChunkState::DONE)
		return;

	for (int i = 0; i < CHUNKSIZE; ++i)
	{
		XMFLOAT3 lbb = XMFLOAT3(position.x * 16.f, 0.f, position.y * 16.f);
		XMFLOAT3 rtf = XMFLOAT3((position.x + 1.f) * 16.f, 0.f, (position.y + 1.f) * 16.f);
		SVMath::Cube cube(lbb, rtf);

		if (//!cam.ViewSpaceFrustumCulling(cube))
			//!Camera::mainCam->ViewSpaceFrustumCulling(cube)
			//cam.ClipSpaceFrustumCulling(cube))
			!Camera::mainCam->ClipSpaceFrustumCullingXZ(cube))
			return;

		blocks[i].Render(i);
	}
}

void VoxelChunk::CreateNewChunk(const XMINT2& position)
{
	state = EChunkState::LOADING;
	this->position = position;
	
	SimplexNoise simplex;
	for (int z = 0; z < 16; ++z)
	{
		for (int x = 0; x < 16; ++x)
		{
			float noise = (simplex.fractal(24u, x * 0.00625 + position.x * 0.1, z * 0.00625 + position.y * 0.1) + 1) * 90;
				//perlin.Noise(x * 0.00625 + position.x * 0.1, z * 0.00625 + position.y * 0.1, 0) * 180;
				//(SimplexNoise::noise(x * 0.00625 + position.x * 0.1, z * 0.00625 + position.y * 0.1) + 1) * 90;
			while (noise > 0)
			{
				blocks[(int)(noise) / 16].SetCell(VoxelBlock::Index(x, (int)(noise) % 16, z), VoxelCellType::DIRT);
				noise -= 1;
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
