#include "Shader.h"

Shader::~Shader()
{
	if (matBuffer)
	{
		matBuffer->Release();
		matBuffer = nullptr;
	}

	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}

	if (ps)
	{
		ps->Release();
		ps = nullptr;
	}

	if (vs)
	{
		vs->Release();
		vs = nullptr;
	}
}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMsg, HWND hwnd, LPCTSTR fileName)
{
	// Print debug message
	OutputDebugStringA(reinterpret_cast<const char*>(errorMsg->GetBufferPointer()));

	// Release errorMsg
	errorMsg->Release();
	errorMsg = nullptr;

	MessageBox(hwnd, L"Error compiling shader.", fileName, MB_OK);
}
