#include "Entity.h"

// For the DirectX Math library
using namespace DirectX;

Entity::Entity(Mesh * _mesh, Material * _material, XMFLOAT4X4 _matrix, XMFLOAT3 _pos, XMFLOAT3 _rot, XMFLOAT3 _scale)
{
	// Assign member variables
	mesh = _mesh;
	material = _material;
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

void Entity::Move(float translateX, float translateY, float translateZ, float rotX, float rotY, float rotZ)
{
	position.x += translateX;
	position.y += translateY;
	position.z += translateZ;

	XMMATRIX translate = XMMatrixTranslation(position.x, position.y, position.z);

	XMMATRIX rotationX = XMMatrixRotationX(rotX);
	XMMATRIX rotationY = XMMatrixRotationY(rotY);
	XMMATRIX rotationZ = XMMatrixRotationZ(rotZ);

	XMMATRIX rotation = rotationX * rotationY * rotationZ;

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(translate * rotation));
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

void Entity::PrepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	material->GetVertexShader()->SetMatrix4x4("world", worldMatrix);
	material->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	material->GetVertexShader()->SetMatrix4x4("projection", projectionMatrix);
	material->GetPixelShader()->SetShaderResourceView("diffuseTexture", material->getShaderResourceView());
	material->GetPixelShader()->SetSamplerState("basicSampler", material->getSamplerState());

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	material->GetVertexShader()->CopyAllBufferData();
	material->GetPixelShader()->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();
}
