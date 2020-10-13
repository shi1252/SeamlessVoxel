#pragma once
#include "Defined.h"

struct Vertex
{
	Vertex(const XMFLOAT3& position = XMFLOAT3(0.f, 0.f, 0.f), const XMFLOAT2& uv = XMFLOAT2(0.f, 0.f), const XMFLOAT3& normal = XMFLOAT3(0.f, 0.f, 0.f), const XMFLOAT4& color = XMFLOAT4(0.f, 0.f, 0.f, 1.f))
		: position(position), uv(uv), normal(normal), color(color) {}
	Vertex(float px, float py, float pz, float u, float v, float nx, float ny, float nz, float r = 0, float g = 0, float b = 0, float a = 1.f)
		: position(px, py, pz), uv(u, v), normal(nx, ny, nz), color(r, g, b, a) {}

	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT4 color;
};