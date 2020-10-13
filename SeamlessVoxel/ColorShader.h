#pragma once
#include "Shader.h"

class ColorShader : public Shader
{
public:
	ColorShader() = default;
	ColorShader(const ColorShader&) = delete;
	~ColorShader() = default;

	bool Initialize(ID3D11Device * device, HWND hwnd);
	virtual bool Render(ID3D11DeviceContext* context, int indexCount, int instanceCount, Material* material) override;

private:
	bool InitializeShader(ID3D11Device * device, HWND hwnd, LPCTSTR fileName);

	bool SetShaderParameters(ID3D11DeviceContext* context);
	void RenderShader(ID3D11DeviceContext* context, int indexCount);
};

