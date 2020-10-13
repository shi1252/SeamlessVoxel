cbuffer MatrixBuffer
{
	matrix worldMat;
	matrix viewMat;
	matrix projMat;
};

struct VertexIn
{
	float4 position : POSITION;
	float4 color : COLOR;
	float3 instancePos : INSTANCEPOS;
	uint type : INSTANCETYPE;
	uint instanceID : SV_InstanceID;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VertexOut VS(VertexIn input)
{
	VertexOut output;

	input.position += float4(input.instancePos, 0.0f);
	input.position.w = 1.0f;

	output.position = mul(input.position, worldMat);
	output.position = mul(output.position, viewMat);
	output.position = mul(output.position, projMat);

	output.color = input.color;

	return output;
}

float4 PS(VertexOut input) : SV_TARGET
{
	return input.color;
}