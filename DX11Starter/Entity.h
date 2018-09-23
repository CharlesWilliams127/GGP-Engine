#pragma once
#include "DXCore.h"
#include "Mesh.h"
#include "Material.h"
#include <DirectXMath.h>

class Entity
{
public:
	Entity(Mesh * _mesh, Material * _material, DirectX::XMFLOAT4X4 _matrix, DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _rot, DirectX::XMFLOAT3 _scale);
	~Entity();

	// Getters
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();

	// Setters 
	void SetWorldMatrix(DirectX::XMFLOAT4X4 value);
	void SetPosition(DirectX::XMFLOAT3 value);
	void SetRotation(DirectX::XMFLOAT3 value);
	void SetScale(DirectX::XMFLOAT3 value);

	// Movement

	/// Will move the entity relative to the values given
	/// @param translateX: Amount to translate in the X direction
	/// @param translateY: Amount to translate in the Y direction
	/// @param translateZ: Amount to translate in the Z direction
	/// @param rotX: Amount to rotate in the X direction
	/// @param rotY: Amount to rotate in the Y direction
	/// @param rotZ: Amount to rotate in the Z direction
	void Move(float translateX, float translateY, float translateZ, float rotX, float rotY, float rotZ);

	// Drawing

	/// Will draw the entity to the screen, potentially will
	/// be moved to a Renderer class at a later date
	/// @param device: Pointer to the DirectX device context
	/// used for drawing
	void Draw(ID3D11DeviceContext * context);

	/// Responsible for setting up the shaders prior to drawing
	/// @param viewMatrix: the camera's view matrix
	/// @param projectionMatrix: the camera's projection matrix
	void PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

private:
	// Transform data
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	// Mesh data
	Mesh * mesh;
	Material * material;
};

