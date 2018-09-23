#pragma once
#include "DXCore.h"
#include <DirectXMath.h>

class Camera
{
public:
	Camera(DirectX::XMFLOAT3 _camPos, DirectX::XMFLOAT3 _camDir, DirectX::XMFLOAT4X4 _viewMatrix);
	~Camera();

	// Getters
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	/// Will update the camera's position and rotation with
	/// a given delta time
	/// @param deltaTime: the deltaTime of the game
	void Update(float deltaTime);

	/// Will update the projection matrix of the camera, called
	/// whenever the screen size is adjusted
	/// @param width: the new width of the screen
	/// @param height: the new height of the screen
	void UpdateProjectionMatrix(float width, float height);

	/// Updates the rotation of the camera along the Y-Axis
	/// @param amount: amount to update by
	void RotateY(float amount);

	/// Updates the rotation of the camera along the X-Axis
	/// @param amount: amount to update by
	void RotateX(float amount);
private:
	// Basics for setting up the camera
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Required for a look-to camera
	DirectX::XMFLOAT3 camPos;
	DirectX::XMFLOAT3 camDir;
	float xRot;
	float yRot;

	float rotScale;
};

