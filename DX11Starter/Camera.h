#pragma once
#include "DXCore.h"
#include "Mesh.h"
#include <DirectXMath.h>

class Camera
{
public:
	Camera(DirectX::XMFLOAT3 _camPos, DirectX::XMFLOAT3 _camDir, DirectX::XMFLOAT4X4 _viewMatrix);
	~Camera();

	// Getters
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	void Update(float deltaTime);
private:
	// Basics for setting up the camera
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Required for a look-to camera
	DirectX::XMFLOAT3 camPos;
	DirectX::XMFLOAT3 camDir;
	float xRot;
	float yRot;
};

