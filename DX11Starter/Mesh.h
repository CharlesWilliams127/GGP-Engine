#pragma once
#include "DXCore.h"
#include "Vertex.h"
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fstream>

class Mesh
{
public:
	Mesh(Vertex * vertices, int vertCount, unsigned int * indices, int _indexCount, ID3D11Device * device);
	Mesh(char * fileName, ID3D11Device * device);
	~Mesh();

	// Getters
	ID3D11Buffer * GetVertexBuffer();
	ID3D11Buffer * GetIndexBuffer();
	int GetIndexCount();
private:
	// Vertex and Index buffers
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;

	void CreateBuffers(Vertex * vertices, int vertCount, unsigned int * indices, int indCount, ID3D11Device * device);

	// index count for the index buffer
	int indexCount;
};

