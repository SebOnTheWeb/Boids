struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

StructuredBuffer<float4x4> viewProjectionBuffer: register(t0);

VS_OUTPUT main( uint id : SV_VERTEXID ) {
	VS_OUTPUT output;

	float4x4 viewProjectionMatrix = viewProjectionBuffer[0];

	if (id == 0) {
		output.pos = mul(float4(0.0f, 0.5f, 0.0f, 1.0f), viewProjectionMatrix);
		output.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	if (id == 1) {
		output.pos = mul(float4(0.5f, -0.5f, 0.0f, 1.0f), viewProjectionMatrix);
		output.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	if (id == 2) {
		output.pos = mul(float4(-0.5f, -0.5f, 0.0f, 1.0f), viewProjectionMatrix);
		output.color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	return output;
}