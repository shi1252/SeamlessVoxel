#pragma once
#include "Defined.h"
#include <vector>

namespace SVMath
{
	struct Cube
	{
	public:
		Cube() = default;
		Cube(const XMFLOAT3& lbb, const XMFLOAT3& rtf) : lbb(lbb), rtf(rtf) {}

		std::vector<XMFLOAT3> GetAllCubePoints() const;

	public:
		// Left Bottom Backward
		XMFLOAT3 lbb = XMFLOAT3(0.f, 0.f, 0.f);
		// Right Top Forward
		XMFLOAT3 rtf = XMFLOAT3(0.f, 0.f, 0.f);
	};
};