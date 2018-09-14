#pragma once
#include "DXCore.h"
#include "Vertex.h"
#include <DirectXMath.h>

class Mesh
{
public:
	Mesh(Vertex * vertices, int vertCount, unsigned int * indices, int _indexCount, ID3D11Device * device);
	~Mesh();

	// Getters
	ID3D11Buffer * GetVertexBuffer();
	ID3D11Buffer * GetIndexBuffer();
	int GetIndexCount();
private:
	// Vertex and Index buffers
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;

	// index count for the index buffer
	int indexCount;
};

