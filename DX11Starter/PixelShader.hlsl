
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer lightBuffer : register(b0)
{
	DirectionalLight light1;
	DirectionalLight light2;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
};

float4 calculateLight(DirectionalLight light, VertexToPixel input)
{
	float3 normalDir = normalize(-light.Direction);
	float3 lightAmount = saturate(dot(input.normal, normalDir));

	return mul(light.DiffuseColor, lightAmount) + light.AmbientColor;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Calculate lighting direction based on values from the vertex shader and cbuffer
	return calculateLight(light1, input) + calculateLight(light2, input);
}