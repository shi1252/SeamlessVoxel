#pragma once
#include "Defined.h"
#include <string>
#include <vector>

class Texture;

class Material
{
public:
	Material(std::string name, std::string shaderName, Texture* texture);
	Material(std::string name, std::string shaderName, std::vector<Texture*> textures)
		: name(name), shaderName(shaderName), textures(textures) {}
	Material(const Material&) = delete;
	~Material() = default;

	std::string GetName() { return name; }
	std::string GetShaderName() { return shaderName; }

	void AddTexture(Texture* texture) { textures.push_back(texture); }
	Texture* GetTexture(int index) { return textures[index]; }
	ID3D11ShaderResourceView* GetTextureSRV(int index);

private:
	std::string name;
	std::string shaderName;
	std::vector<Texture*> textures;
};