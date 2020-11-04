#include "VoxelTerrain.h"

void VoxelTerrain::Update(const XMFLOAT3& position)
{
	chunkPool.Update(position);
}

void VoxelTerrain::Render()
{
	chunkPool.Render();
}
