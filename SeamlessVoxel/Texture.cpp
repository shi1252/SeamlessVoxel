#include "Texture.h"

Texture::~Texture()
{
	if (srv)
	{
		srv->Release();
		srv = nullptr;
	}
}

bool Texture::LoadTexture(ID3D11Device* device, LPCTSTR fileName, TextureType textureType)
{
	switch (textureType)
	{
	case TextureType::WIC:
		if (FAILED(CreateWICTextureFromFile(device, fileName, nullptr, &srv)))
			return false;
		break;

	case TextureType::DDS:
		if (FAILED(CreateDDSTextureFromFile(device, fileName, nullptr, &srv)))
			return false;
		break;
	}

	return true;
}