#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>

class Material
{
public:
	Material(SimplePixelShader * _pixelShader, SimpleVertexShader * _vertexShader);
	~Material();

	// Getters
	SimplePixelShader * GetPixelShader();
	SimpleVertexShader * GetVertexShader();
private:
	SimplePixelShader * pixelShader;
	SimpleVertexShader * vertexShader;
};

