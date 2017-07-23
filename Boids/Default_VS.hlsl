float4 main( uint id : SV_VERTEXID ) : SV_POSITION {
	float4 pos = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (id == 0) {
		pos = float4(0.0f, 0.5f, 0.0f, 1.0f);
	}
	if (id == 1) {
		pos = float4(0.5f, -0.5f, 0.0f, 1.0f);
	}
	if (id == 2) {
		pos = float4(-0.5f, -0.5f, 0.0f, 1.0f);
	}

	return pos;
}