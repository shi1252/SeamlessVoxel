#pragma once
#include "Defined.h"
#include "Vertex.h"
#include <vector>

class Material;

class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh&) = delete;
	virtual ~Mesh();

	virtual void Render(ID3D11DeviceContext* context);

	virtual bool CreateBuffers(ID3D11Device* device);
	virtual void ReleaseBuffers();

	int GetVertexCount() { if (vertices.empty()) return vCount; else return vertices.size(); }
	int GetIndexCount() { if (indices.empty()) return iCount; else return indices.size(); }

protected:
	virtual void RenderBuffers(ID3D11DeviceContext* context);

public:
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
	Material* material = nullptr;

protected:
	ID3D11Buffer* vb = nullptr;
	ID3D11Buffer* ib = nullptr;

	int vCount = 0;
	int iCount = 0;
};