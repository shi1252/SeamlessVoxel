#pragma once
#include "Defined.h"
#include "Vertex.h"
#include <vector>

namespace PrimitiveGenerator
{
	void CreateBox(std::vector<Vertex>& outVertices, std::vector<UINT>& outIndices, XMFLOAT3 position = XMFLOAT3(0, 0, 0), XMFLOAT3 scale = XMFLOAT3(1, 1, 1));
};