#include "Camera.h"
#include "Plane.h"
#include "SeamlessVoxelEngine.h"
#include "Renderer.h"
#include "D3DClass.h"

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

bool Camera::ViewSpaceFrustumCulling(const SVMath::Cube& cube)
{
	std::vector<XMFLOAT3> v = cube.GetAllCubePoints();

	for (int i = 0; i < v.size(); ++i)
	{
		XMStoreFloat3(&v[i], XMVector3Transform(XMLoadFloat3(&v[i]), viewMat));
	}

	const float SCREEN_FAR = 1000.f;
	const float SCREEN_NEAR = 0.1f;

	float tanHalfFov = tanf(3.141592654f / 4.0f * 0.5f);
	SVMath::Plane frustum[6] =
	{
		SVMath::Plane(XMFLOAT3(0.f, 0.f, 1.f), SCREEN_NEAR),
		SVMath::Plane(XMFLOAT3(0.f, 0.f, -1.f), SCREEN_FAR),
		SVMath::Plane(XMFLOAT3(-1.f, 0.f, tanHalfFov), 0.f).Normalize(),
		SVMath::Plane(XMFLOAT3(1.f, 0.f, tanHalfFov), 0.f).Normalize(),
		SVMath::Plane(XMFLOAT3(0.f, -1.f, tanHalfFov), 0.f).Normalize(),
		SVMath::Plane(XMFLOAT3(0.f, 1.f, tanHalfFov), 0.f).Normalize()
	};

	for (int i = 0; i < 6; ++i)
	{
		if (SVMath::Plane::PlaneDotPoint(frustum[i], v[0]) + frustum[i].invDistance >= 0.f)
		{
			continue;
		}

		if (SVMath::Plane::PlaneDotPoint(frustum[i], v[1]) + frustum[i].invDistance >= 0.f)
		{
			continue;
		}

		if (SVMath::Plane::PlaneDotPoint(frustum[i], v[2]) + frustum[i].invDistance >= 0.f)
		{
			continue;
		}

		if (SVMath::Plane::PlaneDotPoint(frustum[i], v[3]) + frustum[i].invDistance >= 0.f)
		{
			continue;
		}

		if (SVMath::Plane::PlaneDotPoint(frustum[i], v[4]) + frustum[i].invDistance >= 0.f)
		{
			continue;
		}

		if (SVMath::Plane::PlaneDotPoint(frustum[i], v[5]) + frustum[i].invDistance >= 0.f)
		{
			continue;
		}

		if (SVMath::Plane::PlaneDotPoint(frustum[i], v[6]) + frustum[i].invDistance >= 0.f)
		{
			continue;
		}

		if (SVMath::Plane::PlaneDotPoint(frustum[i], v[7]) + frustum[i].invDistance >= 0.f)
		{
			continue;
		}

		//for (const auto& p : v)
		//{
		//	//XMMATRIX newView = viewMat;
		//	//newView.r[3].m128_f32[2] = -newView.r[3].m128_f32[2];
		//	XMVECTOR vp = XMVector3Transform(XMLoadFloat3(&p), viewMat);
		//	//float dist = XMVector3Dot(XMLoadFloat3(&frustum[i].normal), vp).m128_f32[0];

		//	if (XMVector3Dot(XMLoadFloat3(&frustum[i].normal), vp).m128_f32[0] + frustum[i].invDistance > 0.f)
		//	{
		//		break;
		//	}
		//}
		
		return false;
	}

	return true;
}

bool Camera::ClipSpaceFrustumCulling(const SVMath::Cube& cube)
{
	XMMATRIX proj;
	SVEngine::svEngine->GetRenderer()->GetD3D()->GetProejctionMatrix(proj);

	return false;
}
