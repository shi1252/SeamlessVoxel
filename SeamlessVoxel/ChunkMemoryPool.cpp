#include "ChunkMemoryPool.h"
#include "VoxelChunk.h"

ChunkMemoryPool::ChunkMemoryPool(unsigned int size)
{
	if (poolSize % 2 == 0)
		poolSize += 1;
	poolSize = size;

	pool = new VoxelChunk[poolSize * poolSize];

	halfSize = poolSize * 0.5f;
	int i = 0;
	for (int z = 0; z < poolSize; ++z)
	{
		for (int x = 0; x < poolSize; ++x)
		{
			XMINT2 position = XMINT2(x - halfSize, z - halfSize);
			if (!pool[i].LoadChunkFromFile(position))
			{
				pool[i].CreateNewChunk(position);
			}
			++i;
		}
	}
}

ChunkMemoryPool::~ChunkMemoryPool()
{
	SaveAllChunks();
	delete[] pool;
}

void ChunkMemoryPool::Render()
{
	for (int i = 0; i < poolSize * poolSize; ++i)
	{
		pool[i].Render();
	}
}

void ChunkMemoryPool::ReleaseChunk(int index)
{
	pool[index].SaveChunk();
	pool[index].state = EChunkState::NONE;
}

void ChunkMemoryPool::SaveAllChunks()
{
	for (int i = 0; i < poolSize * poolSize; ++i)
	{
		pool[i].SaveChunk();
	}
}
