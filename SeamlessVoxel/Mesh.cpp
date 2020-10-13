#include "Mesh.h"

Mesh::~Mesh()
{
	ReleaseBuffers();
}

void Mesh::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

bool Mesh::CreateBuffers(ID3D11Device* device)
{
	if (vertices.empty() || indices.empty())
		return true;

	// Initialize vertex buffer desc
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Set ptr to subresource about vertext data
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	if (FAILED(device->CreateBuffer(&vbd, &vertexData, &vb)))
		return false;

	// Initialize index buffer desc
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(ULONG) * indices.size();
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Set ptr to subresource about index data
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	if (FAILED(device->CreateBuffer(&ibd, &indexData, &ib)))
		return false;

	vCount = vertices.size();
	iCount = indices.size();

	// Release vertices, indices
	vertices.clear();
	indices.clear();

	return true;
}

void Mesh::ReleaseBuffers()
{
	if (ib)
	{
		ib->Release();
		ib = nullptr;
	}

	if (vb)
	{
		vb->Release();
		vb = nullptr;
	}
}

void Mesh::RenderBuffers(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}