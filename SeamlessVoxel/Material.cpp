#include "Material.h"
#include "Texture.h"

ID3D11ShaderResourceView* Material::GetTextureSRV()
{
	if (texture)
		return texture->GetSRV();

	return nullptr;
}
