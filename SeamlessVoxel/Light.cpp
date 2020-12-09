#include "Light.h"

void Light::SetAmbientColor(XMFLOAT4 color)
{
	ambient = color;
}

void Light::SetDiffuseColor(XMFLOAT4 color)
{
	diffuse = color;
}

void Light::SetPosition(XMFLOAT3 pos)
{
	position = pos;
}

void Light::SetRotation(XMFLOAT3 rot)
{
	rotation = rot;
}

XMFLOAT3 Light::GetLightDirection()
{
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, XMVector4Transform(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z))));
	return forward;
}