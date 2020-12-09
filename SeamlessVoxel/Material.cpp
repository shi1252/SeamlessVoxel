#include "Material.h"
#include "Texture.h"

Material::Material(std::string name, std::string shaderName, Texture* texture)
	: name(name), shaderName(shaderName)
{
	textures.push_back(texture);
}

ID3D11ShaderResourceView* Material::GetTextureSRV(int index)
{
	if (textures.size() > 0)
		return textures[index]->GetSRV();

	return nullptr;
}
