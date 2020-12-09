#include "ResourceManager.h"
#include "SeamlessVoxelEngine.h"

ResourceManager::~ResourceManager()
{
	materials.clear();
	textures.clear();
}

bool ResourceManager::CreateTexture(std::string name, LPCTSTR fileName, TextureType textureType)
{
	if (textures.find(name) != textures.end())
		return false;

	textures[name] = std::make_unique<Texture>();
	return textures[name].get()->LoadTexture(SVEngine::svEngine->GetD3DDevice(), fileName, textureType);
}

bool ResourceManager::CreateMaterial(std::string name, std::string shaderName, Texture* texture)
{
	if (materials.find(name) != materials.end())
		return false;

	materials[name] = std::make_unique<Material>(name, shaderName, texture);
	return true;
}

bool ResourceManager::CreateMaterial(std::string name, std::string shaderName, std::string textureName)
{
	if (materials.find(name) != materials.end())
		return false;

	materials[name] = std::make_unique<Material>(name, shaderName, GetTexture(textureName));
	return true;
}

Texture* ResourceManager::GetTexture(std::string name)
{
	if (textures.find(name) == textures.end())
		return nullptr;

	return textures[name].get();
}

Material* ResourceManager::GetMaterial(std::string name)
{
	if (materials.find(name) == materials.end())
		return nullptr;

	return materials[name].get();
}
