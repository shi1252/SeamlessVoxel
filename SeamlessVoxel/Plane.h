#pragma once
#include "Defined.h"

namespace SVMath
{
	struct Plane
	{
	public:
		Plane() = default;
		Plane(const XMFLOAT3& normal, float invDistance) : normal(normal), invDistance(invDistance) {}
		Plane(const XMFLOAT3& p1, const XMFLOAT3& p2, const XMFLOAT3& p3);

		Plane Normalize();

		static float PlaneDotPoint(const Plane& plane, const XMFLOAT3& point)
		{
			XMVECTOR plv;
			plv.m128_f32[0] = plane.normal.x;
			plv.m128_f32[1] = plane.normal.y;
			plv.m128_f32[2] = plane.normal.z;
			plv.m128_f32[3] = plane.invDistance;
			XMVECTOR pov = XMLoadFloat3(&point);
			return XMPlaneDotCoord(plv, pov).m128_f32[0];
		}

	public:
		XMFLOAT3 normal = XMFLOAT3(0.f, 1.f, 0.f);
		float invDistance = 0.f;
	};
};