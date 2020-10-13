#pragma once

enum VoxelCellType : unsigned char
{
	NONE,
	DIRT
};

struct VoxelCell
{
public:
	VoxelCell() = default;

	VoxelCell& operator=(const VoxelCellType& rhs) { type = rhs; return *this; }

public:
	VoxelCellType type = VoxelCellType::NONE;
};