#pragma once
#include "Defined.h"

enum class TextureType
{
	DDS,
	WIC
};

class Texture
{
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	virtual ~Texture();

	bool LoadTexture(ID3D11Device* device, LPCTSTR fileName, TextureType textureType);

	ID3D11ShaderResourceView* GetSRV() { return srv; }

protected:
	ID3D11ShaderResourceView* srv = nullptr;
};