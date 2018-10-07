#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// For managing entities
using std::vector;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	// Free meshes
	delete triangle;
	delete square;
	delete pentagon;
	delete cone;
	delete cube;
	delete sphere;
	delete cylinder;
	delete hexlis;
	delete torus;

	// Free entities
	vector<Entity*>::iterator end = entities.end();
	for (vector<Entity*>::iterator i = entities.begin(); i != end; i++)
	{
		delete(*i);
	}
	entities.clear();

	// Free camera
	delete camera;

	// Free material
	delete woodMaterial;
	delete stoneMaterial;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// Zero out sampler description
	samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);

	// Create Textures
	CreateWICTextureFromFile(device, context, L"../../DX11Starter/Assets/Textures/WoodPlanks.tif", 0, &shaderResourceView1);
	CreateWICTextureFromFile(device, context, L"../../DX11Starter/Assets/Textures/MossyBricks.jpg", 0, &shaderResourceView2);

	// Create material
	woodMaterial = new Material(pixelShader, vertexShader, shaderResourceView1, samplerState);
	stoneMaterial = new Material(pixelShader, vertexShader, shaderResourceView2, samplerState);

	// Create game entities
	entities.push_back(new Entity(cone, woodMaterial, worldMatrix, XMFLOAT3(), XMFLOAT3(), XMFLOAT3()));
	entities[0]->Move(1.0f, 1.0f, 0, 0, 0, 0);

	entities.push_back(new Entity(cube, woodMaterial, worldMatrix, XMFLOAT3(), XMFLOAT3(), XMFLOAT3()));
	entities[1]->Move(-1.0f, -1.0f, 0, 0, 2.345f, 0);

	entities.push_back(new Entity(cylinder, woodMaterial, worldMatrix, XMFLOAT3(), XMFLOAT3(), XMFLOAT3()));
	entities[2]->Move(-1.0f, -1.0f, 0, 0, 0, 0);

	entities.push_back(new Entity(torus, woodMaterial, worldMatrix, XMFLOAT3(), XMFLOAT3(), XMFLOAT3()));
	entities[3]->Move(-1.0f, 1.0f, 0, 0, 0, 0);

	entities.push_back(new Entity(sphere, stoneMaterial, worldMatrix, XMFLOAT3(), XMFLOAT3(), XMFLOAT3()));

	// Create camera
	camera = new Camera(XMFLOAT3(0, 0, -5), XMFLOAT3(0, 0, 1), viewMatrix);

	// Set up initial projection matrix
	camera->UpdateProjectionMatrix((float)width, (float)height);

	// initialize mouse movement
	prevMousePos.x = width / 2;
	prevMousePos.y = height / 2;

	// Initialize lights
	light.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light.DiffuseColor = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	light.Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);

	light2.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light2.DiffuseColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	light.Direction = XMFLOAT3(-1.0f, 1.0f, 0.0f);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - hardcoded to 3 shapes for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	/*XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);*/

	// Create mesh index and vertex values
	Vertex triVertices[] =
	{
		{ XMFLOAT3(+0.0f, +2.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(+1.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(-1.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	};

	unsigned int triIndices[] = { 0, 1, 2 };

	Vertex squareVertices[] =
	{
		{ XMFLOAT3(-2.0f, +0.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(-1.0f, +0.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(-2.0f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	};

	unsigned int squareIndices[] = {0, 1, 2, 0, 2, 3};

	Vertex pentVertices[] =
	{
		{ XMFLOAT3(+1.0f, +0.35f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(+2.0f, -0.25f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(+1.5f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(+0.5f, -1.0f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	{ XMFLOAT3(+0.0f, -0.25f, +0.0f), XMFLOAT3(+0.0f, +0.0f, -1.0f), XMFLOAT2(0, 0) },
	};

	unsigned int pentIndices[] = {0, 1, 2, 3, 0, 2, 3, 4, 0};

	// initialize meshes
	triangle = new Mesh(triVertices, 3, triIndices, 3, device);
	square = new Mesh(squareVertices, 4, squareIndices, 6, device);
	pentagon = new Mesh(pentVertices, 5, pentIndices, 9, device);
	pentagon = new Mesh(pentVertices, 5, pentIndices, 9, device);
	cone = new Mesh("../../DX11Starter/Assets/Models/cone.obj", device);
	cube = new Mesh("../../DX11Starter/Assets/Models/cube.obj", device);
	cylinder = new Mesh("../../DX11Starter/Assets/Models/cylinder.obj", device);
	hexlis = new Mesh("../../DX11Starter/Assets/Models/hexlis.obj", device);
	sphere = new Mesh("../../DX11Starter/Assets/Models/sphere.obj", device);
	torus = new Mesh("../../DX11Starter/Assets/Models/torus.obj", device);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	camera->UpdateProjectionMatrix((float)width, (float)height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Move all entities individually
	//entities[0]->Move(0.00005f, 0.00005f, 0, 0, 0, 0.5f * -totalTime);
	//entities[1]->Move(-0.00005f, 0.00005f, 0, 0, 0, 0);
	//entities[2]->Move(0.00005f, -0.00005f, 0, 0, 0, 0.25f * totalTime);
	//entities[3]->Move(-0.00005f, -0.00005f, 0, 0, 0, 0);
	//entities[4]->Move(0.00005f, 0.00005f, 0, 0, 0, 0.5f * totalTime);

	// Update camera
	camera->Update(deltaTime);

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	size_t end = entities.size();
	for (int i = 0; i < end; i++) 
	{
		pixelShader->SetData("light1", &light, sizeof(DirectionalLight));
		pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));

		entities[i]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

		entities[i]->Draw(context);
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	camera->RotateX((float)(y - prevMousePos.y));
	camera->RotateY((float)(x - prevMousePos.x));

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion