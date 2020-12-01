#define _CRT_SECURE_NO_WARNINGS
#include "RegionFileManager.h"
#include "VoxelBlock.h"
#include "VoxelChunk.h"

RegionFileManager::RegionFileManager(const std::string& directory)
    : saveDir(directory)
{
}

RegionFileManager::~RegionFileManager()
{
    for (auto&& rf : regionFiles)
    {
        delete rf.second;
        rf.second = nullptr;
    }
    regionFiles.clear();
}

bool RegionFileManager::LoadChunk(const XMINT2& position, VoxelChunk* chunk)
{
    chunk->state = EChunkState::LOADING;

    RegionFile* rf = GetRegionFile(position);

    UINT index = GetChunkIndexFromFile(position);

    UINT offset = rf->header.location[index] >> 8;
    UCHAR sectorCount = rf->header.location[index] & 0xFF;

    if (offset == 0 || sectorCount == 0)
        return false;

    std::unique_lock<std::mutex> lock(this->mutex);
    offset *= SECTORSIZE;
    int ret = fseek(rf->file, offset, SEEK_SET);
    if (ret != 0)
        return false;

    for (int i = 0; i < CHUNKSIZE; ++i)
    {
        ret = fread(&chunk->blocks[i], sizeof(VoxelCellType), BLOCKSIZE, rf->file);
        if (ret != BLOCKSIZE)
            return false;
        
        fflush(rf->file);
    }

    chunk->position = position;
    chunk->state = EChunkState::DONE;
    return true;
}

bool RegionFileManager::SaveChunk(VoxelChunk* chunk)
{
    if (chunk->state != EChunkState::DONE)
        return true;

    RegionFile* rf = GetRegionFile(chunk->position);

    UINT index = GetChunkIndexFromFile(chunk->position);
    
    //std::vector<Sector> sectors;
    //RLEEncoding(chunk, sectors);

    UCHAR sectorCount = 16;//sectors.size();
    UINT offset = 1 + index * 16;

    UINT lastOffset = rf->header.location[index] >> 8;
    UCHAR lastSectorCount = rf->header.location[index] & 0xFF;
    if (lastOffset != offset || lastSectorCount != sectorCount)
    {
        UINT head = (offset << 8) + sectorCount;
        rf->header.location[index] = head;
        rf->isDirty = true;
    }

    std::unique_lock<std::mutex> lock(this->mutex);
    offset *= SECTORSIZE;
    int ret = fseek(rf->file, offset, SEEK_SET);
    if (ret != 0)
        return false;

    for (int i = 0; i < CHUNKSIZE; ++i)
    {
        ret = fwrite(&chunk->blocks[i], sizeof(VoxelCellType), BLOCKSIZE, rf->file);
        if (ret != BLOCKSIZE)
            return false;

        fflush(rf->file);
    }

    return true;
}

void RegionFileManager::RLEEncoding(const VoxelChunk* chunk, std::vector<Sector>& outSectors)
{
    outSectors.push_back(Sector());

    UINT byteCnt = 0;
    UINT curSector = 0;

    UINT cnt = 0;
    VoxelCellType type = chunk->blocks[0].cells[0].type;
    for (int block = 0; block < 16; ++block)
    {
        for (int z = 0; z < 16; ++z)
        {
            for (int y = 0; y < 16; ++y)
            {
                for (int x = 0; x < 16; ++x)
                {
                    UINT index = VoxelBlock::Index(x, y, z);
                    if (type == chunk->blocks[block].cells[index].type)
                    {
                        ++cnt;
                    }
                    else
                    {
                        UINT rle = (cnt << 8) + type;
                        outSectors[byteCnt / 1024].uint[byteCnt % 1024] = rle;

                        if (curSector != (++byteCnt / 1024))
                        {
                            outSectors.push_back(Sector());
                            curSector = byteCnt / 1024;
                        }
                    }
                }
            }
        }
    }
}

UINT RegionFileManager::GetChunkIndexFromFile(const XMINT2& position)
{
    int x = position.x % 32;
    if (x < 0)
        x += 32;
    int y = position.y % 32;
    if (y < 0)
        y += 32;
    return x + y * 32;
}

RegionFile* RegionFileManager::GetRegionFile(const XMINT2& position)
{
    std::unique_lock<std::mutex> lock(this->mutex);

    std::string name = GetRegionFileName(position);
    if (regionFiles.find(name) == regionFiles.end())
    {
        std::string path = "./" + saveDir + "/" + name;

        RegionFile* newRF = new RegionFile;

        FILE* file = fopen(path.c_str(), "r+b");
        if (file == nullptr)
        {
            file = fopen(path.c_str(), "w+b");
            if (file != nullptr)
            {
                fseek(file, 0, SEEK_SET);
                fwrite(newRF->header.location, sizeof(UINT), REGIONSIZE, file);

                UCHAR buffer[SECTORSIZE] = { 0, };
                for (int r = 0; r < REGIONSIZE; ++r)
                {
                    for (int i = 0; i < CHUNKSIZE; ++i)
                    {
                        fwrite(buffer, sizeof(UCHAR), SECTORSIZE, file);
                    }
                }
                fflush(file);
            }
            else
            {
                delete newRF;
                return nullptr;
            }
        }
        else
        {
            fseek(file, 0, SEEK_END);
            long long size = ftell(file);

            if (size >= SECTORSIZE)
            {
                fseek(file, 0, SEEK_SET);
                fread(newRF->header.location, sizeof(UCHAR), SECTORSIZE, file);
                fflush(file);
            }
        }

        newRF->region = name;
        newRF->file = file;

        regionFiles[name] = newRF;
    }
    return regionFiles[name];
}

std::string RegionFileManager::GetRegionFileName(const XMINT2& position)
{
    return ("r." + std::to_string(position.x >> 5) + "." + std::to_string(position.y >> 5) + ".bin");
}
