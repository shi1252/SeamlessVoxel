#include "VoxelBlock.h"

//void VoxelBlock::Render()
//{
//	for (int i = 0; i < BLOCKSIZE; ++i)
//	{
//		if (cells[i].type != VoxelCellType::NONE)
//		{
//		}
//	}
//}

void VoxelBlock::Initialize()
{
	fillCount = 0;
	for (int i = 0; i < BLOCKSIZE; ++i)
	{
		cells[i].type = VoxelCellType::NONE;
	}
}

bool VoxelBlock::SetCell(UINT index, const VoxelCellType& type)
{
	if (cells[index].type == type)
		return false;;

	if (type != VoxelCellType::NONE)
	{
		fillCount++;
	}
	else
	{
		fillCount--;
	}
	cells[index].type = type;
	return true;
}