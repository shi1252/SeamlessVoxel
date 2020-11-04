#pragma once
#include "Defined.h"
#include <queue>
#include <unordered_map>
#include <utility>
#include <mutex>

class VoxelChunk;
class RegionFileManager;
class ThreadPool;

struct pair_hash
{
public:
	template<typename T, typename U>
	std::size_t operator()(const std::pair<T, U>& x) const
	{
		return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
	}
};

class ChunkMemoryPool
{
public:
	ChunkMemoryPool(unsigned int size = 8);
	~ChunkMemoryPool();

	void Update(const XMFLOAT3& position);

	void Render();

	void LoadChunk(std::pair<int, int> position);
	void ReleaseChunk(int index);
	void ReleaseChunk(std::pair<int, int> position);

private:
	void SaveAllChunks();

private:
	VoxelChunk* pool = nullptr;
	UINT poolSize;
	UINT halfSize;

	std::queue<VoxelChunk*> waitingChunks;
	std::unordered_map<std::pair<int, int>, VoxelChunk*, pair_hash> map;

	RegionFileManager* rfm = nullptr;
	ThreadPool* tpool = nullptr;
	std::mutex mutex;
};