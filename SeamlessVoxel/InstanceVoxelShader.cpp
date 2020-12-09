#include "InstanceVoxelShader.h"
#include "SeamlessVoxelEngine.h"
#include "ShaderManager.h"
#include "Light.h"
#include "Material.h"

InstanceVoxelShader::~InstanceVoxelShader()
{
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = nullptr;
	}

	if (camBuffer)
	{
		camBuffer->Release();
		camBuffer = nullptr;
	}

	if (sampleState)
	{
		sampleState->Release();
		sampleState = nullptr;
	}
}

bool InstanceVoxelShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	return InitializeShader(device, hwnd, L"hlsl/InstanceVoxelShader.hlsl");
}

bool InstanceVoxelShader::Render(ID3D11DeviceContext* context, int indexCount, int instanceCount, Material* material)
{
	if (!SetShaderParameters(context, material))
		return false;

	RenderShader(context, indexCount, instanceCount);
}

bool InstanceVoxelShader::InitializeShader(ID3D11Device* device, HWND hwnd, LPCTSTR fileName)
{
	ID3D10Blob* errorMsg = nullptr;

	// Compile vs code
	ID3D10Blob* vsBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(fileName, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &vsBuffer, &errorMsg)))
	{
		// Print error message if the shader compile faied
		if (errorMsg)
			OutputShaderErrorMessage(errorMsg, hwnd, fileName);
		// Print error if the shader file couldn't find
		else
			MessageBox(hwnd, fileName, L"Missing Shader file", MB_OK);

		return false;
	}

	// Compile ps code
	ID3D10Blob* psBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(fileName, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &psBuffer, &errorMsg)))
	{
		// Print error message if the shader compile faied
		if (errorMsg)
			OutputShaderErrorMessage(errorMsg, hwnd, fileName);
		// Print error if the shader file couldn't find
		else
			MessageBox(hwnd, fileName, L"Missing Shader file", MB_OK);

		return false;
	}

	// Create vs shader using buffer
	if (FAILED(device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), NULL, &vs)))
		return false;

	// Create ps shader using buffer
	if (FAILED(device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), NULL, &ps)))
		return false;

	// Initialize input layout desc
	// It has to equal with VertexIn
	D3D11_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT ,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

		{"INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETYPE", 0, DXGI_FORMAT_R8_UINT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1}
	};

	// Get layout count
	UINT numElements = sizeof(inputLayout) / sizeof(inputLayout[0]);

	// Create input layout
	if (FAILED(device->CreateInputLayout(inputLayout, numElements,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &layout)))
		return false;

	// Release vs buffer, ps buffer
	vsBuffer->Release();
	vsBuffer = nullptr;

	psBuffer->Release();
	psBuffer = nullptr;

	// Intialize constant buffer desc
	D3D11_BUFFER_DESC matBufferDesc;
	matBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matBufferDesc.MiscFlags = 0;
	matBufferDesc.StructureByteStride = 0;

	// Create constant buffer ptr to access vs constant buffer
	if (FAILED(device->CreateBuffer(&matBufferDesc, NULL, &matBuffer)))
		return false;

	matBufferDesc.ByteWidth = sizeof(LightBuffer);
	if (FAILED(device->CreateBuffer(&matBufferDesc, NULL, &lightBuffer)))
		return false;

	matBufferDesc.ByteWidth = sizeof(CameraBuffer);
	if (FAILED(device->CreateBuffer(&matBufferDesc, NULL, &camBuffer)))
		return false;

	// Initialize texture sampler desc
	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MipLODBias = 0.f;
	sd.MaxAnisotropy = 1;
	sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sd.BorderColor[0] = 0;
	sd.BorderColor[1] = 0;
	sd.BorderColor[2] = 0;
	sd.BorderColor[3] = 0;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	// Create texture sampler state
	if (FAILED(device->CreateSamplerState(&sd, &sampleState)))
		return false;

	return true;
}

bool InstanceVoxelShader::SetShaderParameters(ID3D11DeviceContext* context, Material* material)
{
	// Lock to write to constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;

#pragma region MatrixBuffer
	if (FAILED(context->Map(matBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	ShaderParameter* params = SVEngine::svEngine->GetShaderManager()->GetShaderParameters();

	// Transpose matrix to shader can use it
	XMMATRIX worldMat = XMMatrixTranspose(params->GetParam<XMMATRIX>("world"));
	XMMATRIX viewMat = XMMatrixTranspose(params->GetParam<XMMATRIX>("view"));
	XMMATRIX projMat = XMMatrixTranspose(params->GetParam<XMMATRIX>("proj"));

	// Get ptr of constant buffer data
	MatrixBuffer* data = (MatrixBuffer*)mappedResource.pData;

	// Set constant buffer data
	data->world = worldMat;
	data->view = viewMat;
	data->proj = projMat;

	// Unlock constant buffer
	context->Unmap(matBuffer, 0);
#pragma endregion

#pragma region LightBuffer
	if (FAILED(context->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	LightBuffer* data2 = (LightBuffer*)mappedResource.pData;

	data2->ambient = params->GetParam<Light*>("light")->GetAmbientColor();
	data2->diffuse = params->GetParam<Light*>("light")->GetDiffuseColor();
	data2->lightDir = params->GetParam<Light*>("light")->GetLightDirection();
	data2->padding = 0.f;

	context->Unmap(lightBuffer, 0);
#pragma endregion

#pragma region CameraBuffer
	if (FAILED(context->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	CameraBuffer* data3 = (CameraBuffer*)mappedResource.pData;

	data3->camPos = params->GetParam<XMFLOAT4>("camPos");

	context->Unmap(camBuffer, 0);
#pragma endregion

	// Set constant buffer position value in vs
	UINT bufferNumber = 0;

	// Set constant buffer of vs
	context->VSSetConstantBuffers(bufferNumber++, 1, &matBuffer);
	context->VSSetConstantBuffers(bufferNumber++, 1, &camBuffer);

	// Set constant buffer of ps
	context->PSSetConstantBuffers(0, 1, &lightBuffer);

	ID3D11ShaderResourceView* texture[4] = { nullptr, nullptr, nullptr, nullptr };
	for (int i = 0; i < 4; ++i)
	{
		texture[i] = material->GetTextureSRV(i);
	}

	// Set texture resource of ps
	context->PSSetShaderResources(0, 4, texture);

	return true;
}

void InstanceVoxelShader::RenderShader(ID3D11DeviceContext* context, int indexCount, int instanceCount)
{
	// Set input layout
	context->IASetInputLayout(layout);

	// Set vs, ps
	context->VSSetShader(vs, NULL, 0);
	context->PSSetShader(ps, NULL, 0);

	// Set ps sampler state
	context->PSSetSamplers(0, 1, &sampleState);

	// Draw
	context->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}
