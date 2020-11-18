#include "ChunkMemoryPool.h"
#include "VoxelBlock.h"
#include "VoxelChunk.h"
#include "RegionFileManager.h"
#include "ThreadPool.h"
#include "Rectangle.h"

ChunkMemoryPool::ChunkMemoryPool(unsigned int size)
{
	rfm = new RegionFileManager("Map");
	tpool = new ThreadPool;

	poolSize = size;
	if (poolSize % 2 == 0)
		poolSize += 1;

	pool = new VoxelChunk[poolSize * poolSize];

	halfSize = poolSize * 0.5f;
	//int i = 0;
	//for (int z = 0; z < poolSize; ++z)
	//{
	//	for (int x = 0; x < poolSize; ++x)
	//	{
	//		XMINT2 position = XMINT2(x - halfSize, z - halfSize);
	//		if (!rfm->LoadChunk(position, &pool[i]))
	//		{
	//			pool[i].CreateNewChunk(position);
	//		}
	//		pool[i].CreateMesh();
	//		++i;
	//	}
	//}

	for (int i = 0; i < poolSize * poolSize; ++i)
	{
		waitingChunks.push(&pool[i]);
	}
}

ChunkMemoryPool::~ChunkMemoryPool()
{
	if (pool)
	{
		SaveAllChunks();
		delete[] pool;
		pool = nullptr;
	}

	if (rfm)
	{
		delete rfm;
		rfm = nullptr;
	}

	if (tpool)
	{
		delete tpool;
		tpool = nullptr;
	}
}

void ChunkMemoryPool::Update(const XMFLOAT3& position)
{
	int x = position.x / 16.0f;
	int y = position.z / 16.0f;
	SVMath::RectangleInt renderRect(x - (int)halfSize, y - (int)halfSize, x + (int)halfSize, y + (int)halfSize);

	//std::vector<std::pair<int, int>> releaseList;
	//for (auto&& pair : map)
	//{
	//	if (!renderRect.IntersectionAABB(XMINT2(pair.second->position.x, pair.second->position.y)))
	//	{
	//		releaseList.push_back(pair.first);
	//	}
	//}
	//for (int i = 0; i < releaseList.size(); ++i)
	//{
	//	ReleaseChunk(releaseList[i]);
	//}

	for (int i = 0; i < poolSize * poolSize; ++i)
	{
		if (pool[i].state == EChunkState::DONE)
			continue;
	}

	int n = 1;
	int mapX = x;
	int mapY = y;
	int sign = -1;
	{
		std::pair<int, int> pos = std::make_pair(mapX, mapY);
		if (map.find(pos) == map.end())
		{
			LoadChunk(pos);
		}
	}
	bool stop = false;
	while (n <= poolSize)
	{
		for (int i = 0; i < n; ++i)
		{
			mapX += sign;
			std::pair<int, int> pos = std::make_pair(mapX, mapY);
			if (map.find(pos) == map.end())
			{
				LoadChunk(pos);
			}
			if (mapX == (x - (int)halfSize) && mapY == (y - (int)halfSize))
			{
				stop = true;
				break;
			}
		}
		if (stop)
			break;
		sign *= -1;
		for (int i = 0; i < n; ++i)
		{
			mapY += sign;
			std::pair<int, int> pos = std::make_pair(mapX, mapY);
			if (map.find(pos) == map.end())
			{
				LoadChunk(pos);
			}
		}
		++n;
	}
}

void ChunkMemoryPool::Render()
{
	for (int i = 0; i < poolSize * poolSize; ++i)
	{
		pool[i].Render();
	}
}

void ChunkMemoryPool::LoadChunk(std::pair<int, int> position)
{
	tpool->Enqueue(
		[=]
		{
			XMINT2 pos = XMINT2(position.first, position.second);
			VoxelChunk* chunk;
			{
				std::unique_lock<std::mutex> lock(this->mutex);

				if (waitingChunks.empty() || map.find(position) != map.end())
					return;

				chunk = waitingChunks.front();
				waitingChunks.pop();
			}

			if (!rfm->LoadChunk(pos, chunk))
			{
				chunk->CreateNewChunk(pos);
			}
			for (int i = 0; i < CHUNKSIZE; ++i)
			{
				chunk->blocks[i].isChanged = true;
			}
			chunk->CreateMesh();

			{
				std::unique_lock<std::mutex> lock(this->mutex);
				map[position] = chunk;
			}
		});
}

void ChunkMemoryPool::ReleaseChunk(int index)
{
	rfm->SaveChunk(&pool[index]);
	pool[index].state = EChunkState::NONE;
	waitingChunks.push(&pool[index]);
}

void ChunkMemoryPool::ReleaseChunk(std::pair<int, int> position)
{
	rfm->SaveChunk(map[position]);
	//for (int i = 0; i < CHUNKSIZE; ++i)
	//{
	//	map[position]->blocks[i].ReleaseMesh();
	//}
	map[position]->state = EChunkState::NONE;
	{
		std::unique_lock<std::mutex> lock(this->mutex);
		waitingChunks.push(map[position]);
	}
	map.erase(position);
}

void ChunkMemoryPool::SaveAllChunks()
{
	for (int i = 0; i < poolSize * poolSize; ++i)
	{
		rfm->SaveChunk(&pool[i]);
	}
}
