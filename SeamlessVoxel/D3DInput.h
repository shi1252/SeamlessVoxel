#pragma once
#include "Defined.h"
#include <dinput.h>

class D3DInput
{
public:
	enum MState
	{
		LEFT = 0,
		RIGHT,
		MID
	};

	D3DInput() = default;
	D3DInput(const D3DInput&) = delete;
	~D3DInput();

	bool IsKeyDown(unsigned char keyCode);

	bool Initialize(HINSTANCE hInstance, HWND hWnd, int width, int height);
	bool Frame();

	void GetMouseLocation(int& x, int& y);
	void GetMouseDelta(int& x, int& y);
	bool GetMouseButtonState(MState button);
	bool GetMouseButtonDown(MState button);
	bool GetMouseButtonUp(MState button);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	unsigned char keyboardState[256] = { 0, };
	DIMOUSESTATE mouseState;

	int width;
	int height;
	int mouseX;
	int mouseY;
	DIMOUSESTATE prevMouseState;
	bool init = true;

	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;
	IDirectInputDevice8* mouse = nullptr;
};