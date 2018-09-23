#include "Material.h"



Material::Material(SimplePixelShader * _pixelShader, SimpleVertexShader * _vertexShader)
{
	pixelShader = _pixelShader;
	vertexShader = _vertexShader;
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
