#pragma once
#include "Defined.h"

namespace SVMath
{
	struct Rectangle
	{
	public:
		Rectangle() = default;
		Rectangle(float minX, float minY, float maxX, float maxY)
			: pmin(XMFLOAT2(minX, minY)), pmax(XMFLOAT2(maxX, maxY)) {}
		Rectangle(int minX, int minY, int maxX, int maxY)
			: pmin(XMFLOAT2(minX, minY)), pmax(XMFLOAT2(maxX, maxY)) {}
		Rectangle(const XMFLOAT2& pmin, const XMFLOAT2& pmax)
			: pmin(pmin), pmax(pmax) {}

		bool IntersectionAABB(const Rectangle& rect);
		bool IntersectionAABB(const XMFLOAT2& point);

	public:
		XMFLOAT2 pmin = XMFLOAT2(0, 0);
		XMFLOAT2 pmax = XMFLOAT2(0, 0);
	};

	inline bool Rectangle::IntersectionAABB(const Rectangle& rect)
	{
		if ((pmin.x > rect.pmax.x) || (rect.pmin.x > pmax.x))
		{
			return false;
		}

		if ((pmin.y > rect.pmax.y) || (rect.pmin.y > pmax.y))
		{
			return false;
		}

		return true;
	}

	inline bool Rectangle::IntersectionAABB(const XMFLOAT2& point)
	{
		if ((pmin.x > point.x) || (point.x > pmax.x))
		{
			return false;
		}

		if ((pmin.y > point.y) || (point.y > pmax.y))
		{
			return false;
		}

		return true;
	}

	struct RectangleInt
	{
	public:
		RectangleInt() = default;
		RectangleInt(float minX, float minY, float maxX, float maxY)
			: pmin(XMINT2(minX, minY)), pmax(XMINT2(maxX, maxY)) {}
		RectangleInt(int minX, int minY, int maxX, int maxY)
			: pmin(XMINT2(minX, minY)), pmax(XMINT2(maxX, maxY)) {}
		RectangleInt(const XMINT2& pmin, const XMINT2& pmax)
			: pmin(pmin), pmax(pmax) {}

		bool IntersectionAABB(const RectangleInt& rect);
		bool IntersectionAABB(const XMINT2& point);

	public:
		XMINT2 pmin = XMINT2(0, 0);
		XMINT2 pmax = XMINT2(0, 0);
	};

	inline bool RectangleInt::IntersectionAABB(const RectangleInt& rect)
	{
		if ((pmin.x > rect.pmax.x) || (rect.pmin.x > pmax.x))
		{
			return false;
		}

		if ((pmin.y > rect.pmax.y) || (rect.pmin.y > pmax.y))
		{
			return false;
		}

		return true;
	}

	inline bool RectangleInt::IntersectionAABB(const XMINT2& point)
	{
		if ((pmin.x > point.x) || (point.x > pmax.x))
		{
			return false;
		}

		if ((pmin.y > point.y) || (point.y > pmax.y))
		{
			return false;
		}

		return true;
	}
};