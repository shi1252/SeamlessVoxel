#pragma once
#include "Defined.h"
#include <string>

class Texture;

class Material
{
public:
	Material(std::string name, std::string shaderName, Texture* texture) 
		: name(name), shaderName(shaderName), texture(texture) {}
	Material(const Material&) = delete;
	~Material() = default;

	std::string GetName() { return name; }
	std::string GetShaderName() { return shaderName; }
	Texture* GetTexture() { return texture; }
	ID3D11ShaderResourceView* GetTextureSRV();

private:
	std::string name;
	std::string shaderName;
	Texture* texture = nullptr;
};