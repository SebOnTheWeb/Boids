struct Boid {
	float3 position: POSITION;
	float3 velocity: VELOCITY;
};

StructuredBuffer<Boid> boidBuffer: register(t0);

Boid main(uint id : SV_VERTEXID) {
	/*if (id == 0) {
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
	}*/

	return boidBuffer[id];
}