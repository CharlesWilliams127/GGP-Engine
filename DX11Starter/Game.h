#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Lights.h"
#include "WICTextureLoader.h"
#include <DirectXMath.h>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// Sample mesh objects
	Mesh * triangle;
	Mesh * square;
	Mesh * pentagon;
	Mesh * cone;
	Mesh * cube;
	Mesh * cylinder;
	Mesh * hexlis;
	Mesh * sphere;
	Mesh * torus;

	// Vector to store various game entities
	std::vector<Entity *> entities;

	// Simple camera
	Camera * camera;

	// Material
	Material * woodMaterial;
	Material * stoneMaterial;

	// Lighting
	DirectionalLight light;
	DirectionalLight light2;

	// Shader resource view for texturing
	ID3D11ShaderResourceView * shaderResourceView1;
	ID3D11ShaderResourceView * shaderResourceView2;

	// Samplers for texturing
	ID3D11SamplerState * samplerState;
	D3D11_SAMPLER_DESC samplerDesc;
};

