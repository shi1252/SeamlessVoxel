#include "VoxelChunk.h"
#include "PrimitiveGenerator.h"
#include "SeamlessVoxelEngine.h"
#include "ShaderManager.h"

void VoxelChunk::Render()
{
	if (state != EChunkState::DONE)
		return;

	mesh.Render(SVEngine::svEngine->GetD3DDC());
	SVEngine::svEngine->GetShaderManager()->Render("Nothing", mesh.GetIndexCount(), mesh.GetInstCount());
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
	CreateMesh();
	state = EChunkState::DONE;
}

void VoxelChunk::SaveChunk()
{
}

bool VoxelChunk::LoadChunkFromFile(const XMINT2& position)
{
	return false;
}

void VoxelChunk::SetCell(XMUINT3 position, const VoxelCellType& type)
{
	UINT block = position.y / 16;
	UINT cell = position.y % 16;

	if (blocks[block].SetCell(VoxelBlock::Index(position.x, cell, position.y), type))
		isChanged = true;
}

bool VoxelChunk::CreateMesh()
{
	PrimitiveGenerator::CreateBox(mesh.vertices, mesh.indices);

	for (int block = 0; block < 16; ++block)
	{
		for (int z = 0; z < 16; ++z)
		{
			for (int y = 0; y < 16; ++y)
			{
				for (int x = 0; x < 16; ++x)
				{
					VoxelCellType type = blocks[block].GetCell(VoxelBlock::Index(x, y, z)).type;
					if (type != VoxelCellType::NONE)
					{
						mesh.instances.push_back(VoxelInstanceType(XMFLOAT3(position.x * 16 + x, block * 16 + y, position.y * 16 + z), type));
					}
				}
			}
		}
	}

	return mesh.CreateBuffers(SVEngine::svEngine->GetD3DDevice());
}