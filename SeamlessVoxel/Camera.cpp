#include "Camera.h"

Camera* Camera::mainCam = nullptr;

Camera::Camera()
{
	position = XMFLOAT3(0.f, 0.f, 0.f);
	rotation = XMFLOAT3(0.f, 0.f, 0.f);
	if (mainCam == nullptr)
		mainCam = this;
}

void Camera::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return position;
}

XMFLOAT3 Camera::GetRotation()
{
	return rotation;
}

XMFLOAT3 Camera::forward()
{
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, XMVector4Transform(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z))));
	return forward;
}

XMFLOAT3 Camera::right()
{
	XMFLOAT3 right;
	XMStoreFloat3(&right, XMVector4Transform(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z))));
	return right;
}

void Camera::Render()
{
	XMVECTOR up, pos, target;
	XMMATRIX rotMat;

	up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	pos = XMLoadFloat3(&position);
	target = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	rotMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));

	target = XMVector3TransformCoord(target, rotMat);
	up = XMVector3TransformCoord(up, rotMat);

	target = XMVectorAdd(pos, target);

	viewMat = XMMatrixLookAtLH(pos, target, up);
}

void Camera::GetViewMatrix(XMMATRIX& view)
{
	view = viewMat;
}