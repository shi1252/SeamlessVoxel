#include "VoxelBlock.h"
#include "SeamlessVoxelEngine.h"
#include "ShaderManager.h"
#include "PrimitiveGenerator.h"
#include "VoxelChunk.h"
#include "Camera.h"
#include "Cube.h"

//void VoxelBlock::Render()
//{
//	for (int i = 0; i < BLOCKSIZE; ++i)
//	{
//		if (cells[i].type != VoxelCellType::NONE)
//		{
//		}
//	}
//}

VoxelBlock::~VoxelBlock()
{
	if (mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}

void VoxelBlock::Initialize()
{
	fillCount = 0;
	for (int i = 0; i < BLOCKSIZE; ++i)
	{
		cells[i].type = VoxelCellType::NONE;
	}
}

void VoxelBlock::Render(const int& block)
{
	if (mesh != nullptr)
	{
		Camera cam;
		cam.SetPosition(0, 140, -5);
		cam.Render();

		XMFLOAT3 lbb = XMFLOAT3(parent->position.x * 16.f, block * 16.f, parent->position.y * 16.f);
		XMFLOAT3 rtf = XMFLOAT3((parent->position.x + 1.f) * 16.f, (block + 1) * 16.f, (parent->position.y + 1.f) * 16.f);
		SVMath::Cube cube(lbb, rtf);

		if (//!cam.ViewSpaceFrustumCulling(cube))
			//!Camera::mainCam->ViewSpaceFrustumCulling(cube)
			//cam.ClipSpaceFrustumCulling(cube))
			!Camera::mainCam->ClipSpaceFrustumCulling(cube))
			return;

		mesh->Render(SVEngine::svEngine->GetD3DDC());
		SVEngine::svEngine->GetShaderManager()->Render("Nothing", mesh->GetIndexCount(), mesh->GetInstCount());
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

bool VoxelBlock::CreateMesh(const int& block)
{
	if (isChanged)
	{
		if (mesh == nullptr)
		{
			mesh = new InstancingMesh<VoxelInstanceType>;
			PrimitiveGenerator::CreateBox(mesh->vertices, mesh->indices);
		}

		for (int z = 0; z < 16; ++z)
		{
			for (int y = 0; y < 16; ++y)
			{
				for (int x = 0; x < 16; ++x)
				{
					VoxelCellType type = GetCell(VoxelBlock::Index(x, y, z)).type;
					if (type != VoxelCellType::NONE)
					{
						mesh->instances.push_back(VoxelInstanceType(XMFLOAT3(parent->position.x * 16 + x, block * 16 + y, parent->position.y * 16 + z), type));
					}
				}
			}
		}

		isChanged = false;
		return mesh->CreateBuffers(SVEngine::svEngine->GetD3DDevice());
	}

	return true;
}

void VoxelBlock::ReleaseMesh()
{
	memset(cells, VoxelCellType::NONE, sizeof(VoxelCell) * BLOCKSIZE);
	if (mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}

char* VoxelBlock::GetBytesFromBlcok()
{
	char* bytes = new char[BLOCKSIZE];

	for (int i = 0; i < BLOCKSIZE; ++i)
	{
		bytes[i] = cells[i].type;
	}

	return bytes;
}
