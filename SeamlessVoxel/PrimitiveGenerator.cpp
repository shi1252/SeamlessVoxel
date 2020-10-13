#include "PrimitiveGenerator.h"

namespace PrimitiveGenerator
{
	void CreateBox(std::vector<Vertex>& outVertices, std::vector<UINT>& outIndices, XMFLOAT3 position, XMFLOAT3 scale)
	{
		float w = 0.5f * scale.x + position.x;
		float h = 0.5f * scale.y + position.y;
		float d = 0.5f * scale.z + position.z;

		Vertex v[24] =
		{
			// front face
			Vertex(-w, -h, -d, 0.f, 1.f, 0.f, 0.f, -1.f),
			Vertex(-w, +h, -d, 0.f, 0.f, 0.f, 0.f, -1.f),
			Vertex(+w, +h, -d, 1.f, 0.f, 0.f, 0.f, -1.f),
			Vertex(+w, -h, -d, 1.f, 1.f, 0.f, 0.f, -1.f),

			// back face
			Vertex(-w, -h, +d, 1.f, 1.f, 0.f, 0.f, 1.f),
			Vertex(+w, -h, +d, 0.f, 1.f, 0.f, 0.f, 1.f),
			Vertex(+w, +h, +d, 0.f, 0.f, 0.f, 0.f, 1.f),
			Vertex(-w, +h, +d, 1.f, 0.f, 0.f, 0.f, 1.f),

			// top face
			Vertex(-w, +h, -d, 0.f, 1.f, 0.f, 1.f, 0.f),
			Vertex(-w, +h, +d, 0.f, 0.f, 0.f, 1.f, 0.f),
			Vertex(+w, +h, +d, 1.f, 0.f, 0.f, 1.f, 0.f),
			Vertex(+w, +h, -d, 1.f, 1.f, 0.f, 1.f, 0.f),

			// bottom face
			Vertex(-w, -h, -d, 1.f, 1.f, 0.f, -1.f, 0.f),
			Vertex(+w, -h, -d, 0.f, 1.f, 0.f, -1.f, 0.f),
			Vertex(+w, -h, +d, 0.f, 0.f, 0.f, -1.f, 0.f),
			Vertex(-w, -h, +d, 1.f, 0.f, 0.f, -1.f, 0.f),

			// left face
			Vertex(-w, -h, +d, 0.f, 1.f, -1.f, 0.f, 0.f),
			Vertex(-w, +h, +d, 0.f, 0.f, -1.f, 0.f, 0.f),
			Vertex(-w, +h, -d, 1.f, 0.f, -1.f, 0.f, 0.f),
			Vertex(-w, -h, -d, 1.f, 1.f, -1.f, 0.f, 0.f),

			// right face
			Vertex(+w, -h, -d, 0.f, 1.f, 1.f, 0.f, 0.f),
			Vertex(+w, +h, -d, 0.f, 0.f, 1.f, 0.f, 0.f),
			Vertex(+w, +h, +d, 1.f, 0.f, 1.f, 0.f, 0.f),
			Vertex(+w, -h, +d, 1.f, 1.f, 1.f, 0.f, 0.f)
		};
		outVertices.assign(&v[0], &v[24]);
		//vertices.insert(std::end(vertices), std::begin(v), std::end(v));

		UINT i[36] =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 5, 6,
			4, 6, 7,

			// top face
			8, 9, 10,
			8, 10, 11,

			// bottom face
			12, 13, 14,
			12, 14, 15,

			// left face
			16, 17, 18,
			16, 18, 19,

			// right face
			20, 21, 22,
			20, 22, 23
		};
		outIndices.assign(&i[0], &i[36]);
	}
}