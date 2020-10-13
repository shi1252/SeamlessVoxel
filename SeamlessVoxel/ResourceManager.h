#pragma once
#include "Defined.h"
#include "Texture.h"
#include "Material.h"
#include <unordered_map>

class Material;

class ResourceManager
{
public:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	~ResourceManager();

	bool CreateTexture(std::string name, LPCTSTR fileName, TextureType textureType);
	bool CreateMaterial(std::string name, std::string shaderName, Texture* texture);
	bool CreateMaterial(std::string name, std::string shaderName, std::string textureName);

	Texture* GetTexture(std::string name);
	Material* GetMaterial(std::string name);

private:

public:

private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
	std::unordered_map<std::string, std::unique_ptr<Material>> materials;
};