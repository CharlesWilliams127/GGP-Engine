#include "Entity.h"

// For the DirectX Math library
using namespace DirectX;

Entity::Entity(Mesh * _mesh, XMFLOAT4X4 _matrix, XMFLOAT3 _pos, XMFLOAT3 _rot, XMFLOAT3 _scale)
{
	// Assign member variables
	mesh = _mesh;
	worldMatrix = _matrix;
	position = _pos;
	rotation = _rot;
	scale = _scale;
}

Entity::~Entity()
{
}

#pragma region Getters

XMFLOAT4X4 Entity::GetWorldMatrix()
{
	return worldMatrix;
}

XMFLOAT3 Entity::GetPosition()
{
	return position;
}

XMFLOAT3 Entity::GetRotation()
{
	return rotation;
}

XMFLOAT3 Entity::GetScale()
{
	return scale;
}
#pragma endregion

#pragma region Setters
void Entity::SetWorldMatrix(XMFLOAT4X4 value)
{
	worldMatrix = value;
}

void Entity::SetPosition(XMFLOAT3 value)
{
	position = value;
}

void Entity::SetRotation(XMFLOAT3 value)
{
	rotation = value;
}

void Entity::SetScale(XMFLOAT3 value)
{
	scale = value;
}
#pragma endregion

// Movement

void Entity::Move(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;

	XMMATRIX translate = XMMatrixTranslation(position.x, position.y, position.z);

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(translate));
}

// Drawing

void Entity::Draw(ID3D11DeviceContext * context)
{
	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ID3D11Buffer * vertBuffer = mesh->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}
