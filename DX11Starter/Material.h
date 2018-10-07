#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>

class Material
{
public:
	Material(SimplePixelShader * _pixelShader, SimpleVertexShader * _vertexShader, ID3D11ShaderResourceView * _shaderResourceView, ID3D11SamplerState * _samplerState);
	~Material();

	// Getters
	SimplePixelShader * GetPixelShader();
	SimpleVertexShader * GetVertexShader();
	ID3D11ShaderResourceView * getShaderResourceView();
	ID3D11SamplerState * getSamplerState();
private:
	SimplePixelShader * pixelShader;
	SimpleVertexShader * vertexShader;

	// Shader samplers
	ID3D11ShaderResourceView * shaderResourceView;
	ID3D11SamplerState * samplerState;
};

