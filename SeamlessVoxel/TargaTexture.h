#pragma once
#include "Defined.h"
#include "Texture.h"

class TargaTexture : public Texture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TargaTexture() = default;
	TargaTexture(const TargaTexture&) = delete;
	~TargaTexture();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, LPCTSTR fileName);

	bool LoadTarga(LPCTSTR fileName, int& height, int& width);

	unsigned char* targaData = nullptr;
	ID3D11Texture2D* texture = nullptr;
};