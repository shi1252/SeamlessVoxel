#pragma once
#include "Mesh.h"

template <typename T>
class InstancingMesh : public Mesh
{
public:
	InstancingMesh() = default;
	InstancingMesh(const InstancingMesh&) = delete;
	~InstancingMesh();

	void Render(ID3D11DeviceContext * context);

	bool CreateBuffers(ID3D11Device * device);
	void ReleaseBuffers();

	int GetInstCount() { return instCount; }

protected:
	void RenderBuffers(ID3D11DeviceContext * context);

public:
	std::vector<T> instances;

protected:
	ID3D11Buffer* instBuffer = nullptr;
	int instCount = 0;
};

template<typename T>
inline InstancingMesh<T>::~InstancingMesh()
{
	ReleaseBuffers();
}

template<typename T>
inline void InstancingMesh<T>::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

template<typename T>
bool InstancingMesh<T>::CreateBuffers(ID3D11Device* device)
{
	if (Mesh::CreateBuffers(device))
	{
		if (instances.empty())
			return true;

		// Initialize index buffer desc
		D3D11_BUFFER_DESC instbd;
		instbd.Usage = D3D11_USAGE_DEFAULT;
		instbd.ByteWidth = sizeof(T) * instances.size();
		instbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instbd.CPUAccessFlags = 0;
		instbd.MiscFlags = 0;
		instbd.StructureByteStride = 0;

		// Set ptr to subresource about index data
		D3D11_SUBRESOURCE_DATA instanceData;
		instanceData.pSysMem = &instances[0];
		instanceData.SysMemPitch = 0;
		instanceData.SysMemSlicePitch = 0;

		// Create index buffer
		if (FAILED(device->CreateBuffer(&instbd, &instanceData, &instBuffer)))
			return false;

		instCount = instances.size();
		instances.clear();

		return true;
	}

	return false;
}

template<typename T>
void InstancingMesh<T>::ReleaseBuffers()
{
	if (instBuffer)
	{
		instBuffer->Release();
		instBuffer = nullptr;
	}
}

template<typename T>
void InstancingMesh<T>::RenderBuffers(ID3D11DeviceContext* context)
{
	UINT stride[2] = { sizeof(Vertex), sizeof(T) };
	UINT offset[2] = { 0,0 };

	ID3D11Buffer* vInstBuffers[2] = { vb, instBuffer };

	context->IASetVertexBuffers(0, 2, vInstBuffers, stride, offset);
	context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}