#include "Renderer.h"
#include "D3DClass.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "VoxelTerrain.h"

Renderer::~Renderer()
{
	if (voxel)
	{
		delete voxel;
		voxel = nullptr;
	}

	if (camera)
	{
		delete camera;
		camera = nullptr;
	}

	if (shaderManager)
	{
		delete shaderManager;
		shaderManager = nullptr;
	}

	if (d3d)
	{
		_aligned_free(d3d);
		d3d = nullptr;
	}
}

bool Renderer::Initialize(int width, int height, HWND hWnd)
{
	d3d = (D3DClass*)_aligned_malloc(sizeof(D3DClass), 16);
	if (!d3d)
	{
		return false;
	}

	if (!d3d->Initialize(width, height, VSYNC_ENABLED, hWnd, SCREEN_FAR, SCREEN_NEAR))
	{
		MessageBox(hWnd, _T("Could not initialize Direct3D."), _T("Error"), MB_OK);
		return false;
	}

	shaderManager = new ShaderManager(d3d->GetDevice(), hWnd);
	if (!shaderManager)
	{
		return false;
	}

	camera = new Camera;
	if (!camera)
	{
		return false;
	}

	camera->SetPosition(0.f, 0.f, -5.0f);

	voxel = new VoxelTerrain();
	if (!voxel)
	{
		return false;
	}

	return true;
}

bool Renderer::Update()
{
	return Render();
}

void Renderer::SetWireframeMode(bool val)
{
	d3d->SetWireframeMode(val);
}

bool Renderer::Render()
{
	d3d->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	camera->Render();
	// Get world, view, proj matrices
	XMMATRIX world, view, proj, ortho;
	d3d->GetWorldMatrix(world);
	camera->GetViewMatrix(view);
	d3d->GetProejctionMatrix(proj);
	d3d->GetOrthoMatrix(ortho);

#pragma region Set Parameters
	ShaderParameter* params = shaderManager->GetShaderParameters();
	params->SetParam("world", world);
	params->SetParam("view", view);
	params->SetParam("proj", proj);
	params->SetParam("ortho", ortho);

	//XMFLOAT3 camPos3 = camera->GetPosition();
	//XMFLOAT4 camPos4 = XMFLOAT4(camPos3.x, camPos3.y, camPos3.z, 1.f);
	//params.SetParam("camPos", camPos4);

	//params.SetParam("light", light);
#pragma endregion

	voxel->Render();

	d3d->EndScene();

	return true;
}
