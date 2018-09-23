#include "Camera.h"

// For the DirectX Math library
using namespace DirectX;

Camera::Camera(XMFLOAT3 _camPos, XMFLOAT3 _camDir, DirectX::XMFLOAT4X4 _viewMatrix)
{
	viewMatrix = _viewMatrix;

	xRot = _camDir.x;
	yRot = _camDir.y;

	camPos = _camPos;
	camDir = _camDir;
}

Camera::~Camera()
{
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::Update(float deltaTime)
{
	// Camera rotation
	XMFLOAT3 defaultForward = XMFLOAT3(0, 0, 1);
	XMFLOAT3 defaultUp = XMFLOAT3(0, 1, 0);

	XMVECTOR rotQuaternion = XMQuaternionRotationRollPitchYaw(xRot, yRot, 0);
	//XMVECTOR upDirection = XMVector3Rotate(XMLoadFloat3(&defaultUp), rotQuaternion);

	XMStoreFloat3(&camDir, XMVector3Rotate(XMLoadFloat3(&defaultForward), rotQuaternion));
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(XMMatrixLookToLH(XMLoadFloat3(&camPos), XMLoadFloat3(&camDir), XMLoadFloat3(&defaultUp))));

	// Camera controls
	if (GetAsyncKeyState('W') & 0x8000) // Move forward
	{
		XMStoreFloat3(&camPos, (XMLoadFloat3(&camPos) + XMLoadFloat3(&camDir) * deltaTime));
	}
	if (GetAsyncKeyState('S') & 0x8000) // Move backwards
	{
		XMStoreFloat3(&camPos, (XMLoadFloat3(&camPos) - XMLoadFloat3(&camDir) * deltaTime));
	}
	if (GetAsyncKeyState('A') & 0x8000) // Move left
	{
		XMStoreFloat3(&camPos, (XMLoadFloat3(&camPos) + XMVector3Cross(XMLoadFloat3(&camDir), XMLoadFloat3(&defaultUp)) * deltaTime));
	}
	if (GetAsyncKeyState('D') & 0x8000) // Move right
	{
		XMStoreFloat3(&camPos, (XMLoadFloat3(&camPos) - XMVector3Cross(XMLoadFloat3(&camDir), XMLoadFloat3(&defaultUp)) * deltaTime));
	}
}
