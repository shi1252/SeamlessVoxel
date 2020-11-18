#include "Plane.h"

namespace SVMath
{
	Plane::Plane(const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3)
	{
		XMVECTOR v1 = XMVectorSubtract(XMLoadFloat3(&p2), XMLoadFloat3(&p1));
		XMVECTOR v2 = XMVectorSubtract(XMLoadFloat3(&p3), XMLoadFloat3(&p1));

		XMStoreFloat3(&normal, XMVector3Cross(v1, v2));
		invDistance = SVMath::Dot(-normal, p1);
	}

	Plane Plane::Normalize()
	{
		float normalSize = SVMath::Size(normal);
		return Plane(normal /= normalSize, invDistance /= normalSize);
	}
};