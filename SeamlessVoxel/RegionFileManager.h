#pragma once
#include "Defined.h"
#include <unordered_map>

#define REGIONSIZE 1024u // 32 * 32

#define SECTORSIZE 4096u

union Sector
{
	UINT uint[SECTORSIZE / 4] = { 0, };
	UCHAR uchar[SECTORSIZE];
};

struct RegionFileHeader
{
	UINT location[REGIONSIZE] = { 0, };
};

struct RegionFile
{
public:
	~RegionFile()
	{
		if (file != nullptr)
		{
			if (isDirty)
			{
				fseek(file, 0, SEEK_SET);
				fwrite(header.location, sizeof(UCHAR), SECTORSIZE, file);
			}
			fclose(file);
		}
	}

public:
	RegionFileHeader header;
	std::string region;
	FILE* file = nullptr;
	bool isDirty = false;
};

class VoxelChunk;

class RegionFileManager
{
public:
	RegionFileManager(const std::string& directory);
	RegionFileManager(const RegionFileManager&) = delete;
	~RegionFileManager();

	bool LoadChunk(const XMINT2& position, VoxelChunk* chunk);
	bool SaveChunk(VoxelChunk* chunk);

private:
	void RLEEncoding(const VoxelChunk* chunk, std::vector<Sector>& outSectors);
	void RLEDecoding(VoxelChunk& chunk);

	UINT GetChunkIndexFromFile(const XMINT2& position);

	RegionFile* GetRegionFile(const XMINT2& position);
	std::string GetRegionFileName(const XMINT2& position);

private:
	std::unordered_map<std::string, RegionFile*> regionFiles;
	std::string saveDir;
};