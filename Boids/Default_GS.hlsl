struct Boid {
	float3 position: POSITION;
	float3 velocity: VELOCITY;
};

struct GS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

StructuredBuffer<float4x4> viewProjectionBuffer: register(t0);

[maxvertexcount(3)]
void main(point Boid input[1], 
	inout TriangleStream< GS_OUTPUT > output) {

	float4x4 viewProjectionMatrix = viewProjectionBuffer[0];

	float3 front = normalize(input[0].velocity);
	float3 up = float3(0.0f, 1.0f, 0.0f); //TODO: Fix world matrix
	float3 right = -cross(front, up);

	for (uint i = 0; i < 3; i++)
	{
		GS_OUTPUT element;

		float3 pos;

		if (i == 0) {
			pos = input[0].position + front * 0.5f;
			element.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (i == 1) {
			pos = input[0].position +(-front + right) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 2) {
			pos = input[0].position + (-front - right) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		element.pos = mul(float4(pos, 1.0f), viewProjectionMatrix);

		output.Append(element);
	}
}