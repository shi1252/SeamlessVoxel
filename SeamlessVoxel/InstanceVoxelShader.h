#pragma once
#include "Shader.h"

class InstanceVoxelShader : public Shader
{
public:
	InstanceVoxelShader() = default;
	InstanceVoxelShader(const InstanceVoxelShader&) = delete;
	~InstanceVoxelShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	virtual bool Render(ID3D11DeviceContext* context, int indexCount, int instanceCount, Material* material) override;

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCTSTR fileName);

	bool SetShaderParameters(ID3D11DeviceContext* context, Material* material);
	void RenderShader(ID3D11DeviceContext* context, int indexCount, int instanceCount);

private:
	ID3D11Buffer* lightBuffer = nullptr;
	ID3D11Buffer* camBuffer = nullptr;

	ID3D11SamplerState* sampleState = nullptr;
};