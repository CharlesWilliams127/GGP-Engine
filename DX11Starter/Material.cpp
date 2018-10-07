#include "Material.h"



Material::Material(SimplePixelShader * _pixelShader, SimpleVertexShader * _vertexShader, ID3D11ShaderResourceView * _shaderResourceView, ID3D11SamplerState * _samplerState)
{
	pixelShader = _pixelShader;
	vertexShader = _vertexShader;

	samplerState = _samplerState;
	shaderResourceView = _shaderResourceView;
}


Material::~Material()
{
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

ID3D11ShaderResourceView * Material::getShaderResourceView()
{
	return shaderResourceView;
}

ID3D11SamplerState * Material::getSamplerState()
{
	return samplerState;
}
