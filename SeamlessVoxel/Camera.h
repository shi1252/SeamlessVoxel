#pragma once
#include "Defined.h"
#include "Cube.h"

class Camera : public AlignedAllocationPolicy<16>
{
public:
	Camera();
	Camera(const Camera&) = delete;
	~Camera() = default;

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	XMFLOAT3 forward();
	XMFLOAT3 right();

	void Render();
	void GetViewMatrix(XMMATRIX & view);

	bool ViewSpaceFrustumCulling(const SVMath::Cube& cube);
	bool ClipSpaceFrustumCulling(const SVMath::Cube& cube);

	static Camera* mainCam;

private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMMATRIX viewMat;
};

