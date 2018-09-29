struct GridVertex {
	float3 position: POSITION;
	float3 color: COLOR;
};

struct PS_INPUT {
	float4 pos: SV_POSITION;
	float4 color: COLOR;
};

StructuredBuffer<float4x4> viewProjectionBuffer: register(t0);
StructuredBuffer<GridVertex> gridBuffer: register(t1);

PS_INPUT main(uint id : SV_VERTEXID) {
	PS_INPUT output;

	float4x4 viewProjectionMatrix = viewProjectionBuffer[0];

	float4 pos = mul(float4(gridBuffer[id].position, 1.0f), viewProjectionMatrix);
	float4 color = float4(gridBuffer[id].color, 1.0f);

	output.pos = pos;
	output.color = color;

	return output;
}