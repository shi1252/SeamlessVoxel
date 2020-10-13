#pragma once
#include "Defined.h"
#include "ShaderBuffers.h"
#include "ShaderParameter.h"

class Material;

class Shader : public AlignedAllocationPolicy<16>
{
public:
	Shader() = default;
	Shader(const Shader&) = delete;
	virtual ~Shader();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd) = 0;
	virtual bool Render(ID3D11DeviceContext* context, int indexCount, int instanceCount, Material* material) = 0;

protected:
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCTSTR fileName) = 0;
	void OutputShaderErrorMessage(ID3D10Blob* errorMsg, HWND hwnd, LPCTSTR fileName);

protected:
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* layout = nullptr;

	ID3D11Buffer* matBuffer = nullptr;
};