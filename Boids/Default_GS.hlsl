struct Boid {
	float3 position: POSITION;
	float3 velocity: VELOCITY;
	float3 up: UP;
};

struct GS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

StructuredBuffer<float4x4> viewProjectionBuffer: register(t0);

[maxvertexcount(18)]
void main(point Boid input[1], 
	inout TriangleStream< GS_OUTPUT > output) {

	float4x4 viewProjectionMatrix = viewProjectionBuffer[0];

	float3 front = normalize(input[0].velocity);
	float3 up = normalize(input[0].up);
	float3 right = -cross(front, up);

	GS_OUTPUT element;
	float3 pos;

	//Triangle 1
	for (uint i = 0; i < 3; i++)
	{
		if (i == 0) {
			pos = input[0].position + front * 0.5f;
			element.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (i == 1) {
			pos = input[0].position +(-front + right + up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 2) {
			pos = input[0].position + (-front - right + up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		element.pos = mul(float4(pos, 1.0f), viewProjectionMatrix);
		output.Append(element);
	}
	output.RestartStrip();

	//Triangle 2
	for (uint i = 0; i < 3; i++)
	{
		if (i == 0) {
			pos = input[0].position + front * 0.5f;
			element.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (i == 1) {
			pos = input[0].position + (-front + right - up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 2) {
			pos = input[0].position + (-front + right + up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		element.pos = mul(float4(pos, 1.0f), viewProjectionMatrix);
		output.Append(element);
	}
	output.RestartStrip();

	//Triangle 3
	for (uint i = 0; i < 3; i++)
	{
		if (i == 0) {
			pos = input[0].position + front * 0.5f;
			element.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (i == 1) {
			pos = input[0].position + (-front - right - up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 2) {
			pos = input[0].position + (-front + right - up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		element.pos = mul(float4(pos, 1.0f), viewProjectionMatrix);
		output.Append(element);
	}
	output.RestartStrip();

	//Triangle 4
	for (uint i = 0; i < 3; i++)
	{
		if (i == 0) {
			pos = input[0].position + front * 0.5f;
			element.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
		}
		if (i == 1) {
			pos = input[0].position + (-front - right + up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 2) {
			pos = input[0].position + (-front - right - up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		element.pos = mul(float4(pos, 1.0f), viewProjectionMatrix);
		output.Append(element);
	}
	output.RestartStrip();

	//Triangle 5
	for (uint i = 0; i < 3; i++)
	{
		if (i == 0) {
			pos = input[0].position + (-front - right + up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 1) {
			pos = input[0].position + (-front + right + up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 2) {
			pos = input[0].position + (-front - right - up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		element.pos = mul(float4(pos, 1.0f), viewProjectionMatrix);
		output.Append(element);
	}
	output.RestartStrip();

	//Triangle 6
	for (uint i = 0; i < 3; i++)
	{
		if (i == 0) {
			pos = input[0].position + (-front + right - up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 1) {
			pos = input[0].position + (-front - right - up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (i == 2) {
			pos = input[0].position + (-front + right + up) * 0.5f;
			element.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		element.pos = mul(float4(pos, 1.0f), viewProjectionMatrix);
		output.Append(element);
	}
}