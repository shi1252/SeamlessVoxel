#pragma once
#include "Defined.h"
#include "ShaderParameter.h"
#include <string>
#include <unordered_map>

class Shader;
class Material;

class ShaderManager
{
public:
	ShaderManager(ID3D11Device* device, HWND hwnd);
	ShaderManager(const ShaderManager&) = delete;
	~ShaderManager();

	ShaderParameter* GetShaderParameters() { return &parameters; }

	bool Render(const std::string& materialName, int indexCount, int instanceCount);

private:
	std::unordered_map<std::string, Shader*> shaders;
	ShaderParameter parameters;
};