#pragma once
#include "DXCore.h"
#include "Mesh.h"
#include <DirectXMath.h>

class Entity
{
public:
	Entity(Mesh * _mesh, DirectX::XMFLOAT4X4 _matrix, DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _rot, DirectX::XMFLOAT3 _scale);
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
	/// @param x: Amount to translate in the X direction
	/// @param y: Amount to translate in the Y direction
	/// @param z: Amount to translate in the Z direction
	void Move(float x, float y, float z);

	// Drawing

	/// Will draw the entity to the screen, potentially will
	/// be moved to a Renderer class at a later date
	/// @param device: Pointer to the DirectX device context
	/// used for drawing
	void Draw(ID3D11DeviceContext * context);

private:
	// Transform data
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	// Mesh data
	Mesh * mesh;
};

