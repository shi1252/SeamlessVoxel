#include "ShaderManager.h"
#include "Shader.h"
#include "SeamlessVoxelEngine.h"
#include "Renderer.h"
#include "D3DClass.h"
#include "ResourceManager.h"
#include "ColorShader.h"
#include "InstanceColorShader.h"
#include "InstanceVoxelShader.h"

ShaderManager::ShaderManager(ID3D11Device* device, HWND hwnd)
{
	shaders["ColorShader"] = new ColorShader();
	shaders["ColorShader"]->Initialize(device, hwnd);

	shaders["InstanceColorShader"] = new InstanceColorShader();
	shaders["InstanceColorShader"]->Initialize(device, hwnd);

	shaders["InstanceVoxelShader"] = new InstanceVoxelShader();
	shaders["InstanceVoxelShader"]->Initialize(device, hwnd);
}

ShaderManager::~ShaderManager()
{
	shaders.clear();
}

bool ShaderManager::Render(const std::string& materialName, int indexCount, int instanceCount)
{
	Material* material = SVEngine::svEngine->GetResourceManager()->GetMaterial(materialName);
	if (material == nullptr || shaders.find(material->GetShaderName()) == shaders.end())
		return false;

	return shaders[material->GetShaderName()]->Render(SVEngine::svEngine->GetD3DDC(), indexCount, instanceCount, material);
}