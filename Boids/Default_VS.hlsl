struct Boid {
	float3 position: POSITION;
	float3 velocity: VELOCITY;
};

StructuredBuffer<Boid> boidBuffer: register(t0);

Boid main(uint id : SV_VERTEXID) {



	return boidBuffer[id];
}