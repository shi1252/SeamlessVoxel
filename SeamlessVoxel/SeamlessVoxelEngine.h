#pragma once
#include "Defined.h"
#include "Timer.h"

class Renderer;
class ResourceManager;
class ShaderManager;
class D3DInput;

class SVEngine
{
public:
	SVEngine(HINSTANCE h) : hInstance(h) {}
	SVEngine(const SVEngine&) = delete;
	~SVEngine();

	bool Initialize();
	void Run();

	Renderer* GetRenderer() { return renderer; }
	ResourceManager* GetResourceManager() { return resourceManager; }
	ShaderManager* GetShaderManager();

	ID3D11Device* GetD3DDevice();
	ID3D11DeviceContext* GetD3DDC();

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool Update();

	bool InitializeWindows(int& width, int& height);
	void CloseWindows();

public:
	static SVEngine* svEngine;

private:
	LPCTSTR appName = _T("Seamless Voxel");
	HINSTANCE hInstance;
	HWND hWnd;

	Renderer* renderer = nullptr;
	D3DInput* input = nullptr;
	ResourceManager* resourceManager = nullptr;

	Timer timer;
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);