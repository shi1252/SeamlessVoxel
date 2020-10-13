#include "SeamlessVoxelEngine.h"
#include "Defined.h"
#include "D3DInput.h"
#include "Renderer.h"
#include "D3DClass.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "VoxelTerrain.h"

SVEngine* SVEngine::svEngine = nullptr;

SVEngine::~SVEngine()
{
	if (resourceManager)
	{
		delete resourceManager;
		resourceManager = nullptr;
	}

	if (renderer)
	{
		delete renderer;
		renderer = nullptr;
	}

	if (input)
	{
		delete input;
		input = nullptr;
	}

	CloseWindows();
}

bool SVEngine::Initialize()
{
	int width = 0;
	int height = 0;

	InitializeWindows(width, height);

	renderer = new Renderer();
	if (!renderer)
	{
		return false;
	}

	if (!renderer->Initialize(width, height, hWnd))
		return false;

	input = new D3DInput;
	if (!input)
	{
		return false;
	}

	if (!input->Initialize(hInstance, hWnd, width, height))
	{
		MessageBox(hWnd, L"Could not initialize input object.", L"Error", MB_OK);
		return false;
	}

	resourceManager = new ResourceManager();
	if (!resourceManager)
	{
		return false;
	}

	if (!resourceManager->CreateTexture("Grass", _T("Texture/Grass.jpg"), TextureType::WIC))
		return false;
	if (!resourceManager->CreateTexture("Rock_028_NORM", _T("Texture/Rock_028_NORM.jpg"), TextureType::WIC))
		return false;
	if (!resourceManager->CreateTexture("Rock_028_COLOR", _T("Texture/Rock_028_COLOR.jpg"), TextureType::WIC))
		return false;

	if (!resourceManager->CreateMaterial("Nothing", "InstanceColorShader", ""))
		return false;

	return true;
}

void SVEngine::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	timer.Reset();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();

			if (!Update())
			{
				break;
			}
		}

		if (input->IsKeyDown(DIK_ESCAPE))
			break;
	}
}

ShaderManager* SVEngine::GetShaderManager()
{
	return renderer->GetShaderManager();
}

ID3D11Device* SVEngine::GetD3DDevice()
{
	return renderer->GetD3D()->GetDevice();
}

ID3D11DeviceContext* SVEngine::GetD3DDC()
{
	return renderer->GetD3D()->GetDeviceContext();
}

LRESULT SVEngine::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			timer.Stop();
		}
		else
		{
			timer.Start();
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		timer.Stop();
		return 0;
	case WM_EXITSIZEMOVE:
		timer.Start();
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void SVEngine::InitializeWindows(int& width, int& height)
{
	svEngine = this;

	hInstance = GetModuleHandle(NULL);
	appName = _T("SeamlessVoxelEngine");

	WNDCLASSEX wc = { 0, };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	width = 800;
	height = 600;

	int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW, appName, appName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		x, y, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
}

void SVEngine::CloseWindows()
{
	DestroyWindow(hWnd);
	hWnd = NULL;

	UnregisterClass(appName, hInstance);
	hInstance = NULL;

	svEngine = nullptr;
}

bool SVEngine::Update()
{
#pragma region Input
	if (!input->Frame())
		return false;

	int mouseX = 0, mouseY = 0;
	input->GetMouseLocation(mouseX, mouseY);

	Camera* cam = renderer->GetCamera();
	cam->Render();
	float movespeed = 5.f;
	if (input->IsKeyDown(DIK_LSHIFT))
		movespeed = 50.f;
	if (input->IsKeyDown(DIK_W))
		cam->SetPosition(cam->GetPosition().x + cam->forward().x * movespeed * timer.DeltaTime(),
			cam->GetPosition().y + cam->forward().y * movespeed * timer.DeltaTime(),
			cam->GetPosition().z + cam->forward().z * movespeed * timer.DeltaTime());
	if (input->IsKeyDown(DIK_S))
		cam->SetPosition(cam->GetPosition().x - cam->forward().x * movespeed * timer.DeltaTime(),
			cam->GetPosition().y - cam->forward().y * movespeed * timer.DeltaTime(),
			cam->GetPosition().z - cam->forward().z * movespeed * timer.DeltaTime());
	if (input->IsKeyDown(DIK_A))
		cam->SetPosition(cam->GetPosition().x - cam->right().x * movespeed * timer.DeltaTime(),
			cam->GetPosition().y - cam->right().y * movespeed * timer.DeltaTime(),
			cam->GetPosition().z - cam->right().z * movespeed * timer.DeltaTime());
	if (input->IsKeyDown(DIK_D))
		cam->SetPosition(cam->GetPosition().x + cam->right().x * movespeed * timer.DeltaTime(),
			cam->GetPosition().y + cam->right().y * movespeed * timer.DeltaTime(),
			cam->GetPosition().z + cam->right().z * movespeed * timer.DeltaTime());

	if (input->GetMouseButtonState(input->RIGHT))
	{
		float mouseSensitivity = 5.f;
		int dx = 0, dy = 0;
		input->GetMouseDelta(dx, dy);

		cam->SetRotation(cam->GetRotation().x, cam->GetRotation().y + dx * mouseSensitivity * timer.DeltaTime(), cam->GetRotation().z);
		cam->SetRotation(cam->GetRotation().x + dy * mouseSensitivity * timer.DeltaTime(), cam->GetRotation().y, cam->GetRotation().z);
	}
#pragma endregion

	return renderer->Update();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
		return SVEngine::svEngine->MessageHandler(hWnd, msg, wParam, lParam);
	}
}