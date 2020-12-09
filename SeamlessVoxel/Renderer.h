#pragma once
#include "Defined.h"

class D3DClass;
class ShaderManager;
class VoxelTerrain;
class Camera;
class Light;

const bool VSYNC_ENABLED = true;
const float SCREEN_FAR = 1000.f;
const float SCREEN_NEAR = 0.1f;

class Renderer
{
public:
	Renderer() = default;
	Renderer(const Renderer&) = delete;
	~Renderer();

	bool Initialize(int width, int height, HWND hWnd);
	bool Update();

	void SetWireframeMode(bool val);

	D3DClass* GetD3D() { return d3d; }
	Camera* GetCamera() { return camera; }
	ShaderManager* GetShaderManager() { return shaderManager; }
	VoxelTerrain* GetVoxelTerrain() { return voxel; }

private:
	bool Render();

private:
	D3DClass* d3d = nullptr;
	Camera* camera = nullptr;
	ShaderManager* shaderManager = nullptr;
	VoxelTerrain* voxel = nullptr;
	Light* light = nullptr;
};