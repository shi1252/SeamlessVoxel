#pragma once
#include "Defined.h"

struct MatrixBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
};

struct LightBuffer
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT3 lightDir;
	//float specularPower;
	//XMFLOAT4 specular;
	float padding;
};

struct CameraBuffer
{
	XMFLOAT4 camPos;
};