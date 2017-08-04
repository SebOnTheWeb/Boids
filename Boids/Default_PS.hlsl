struct GS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 main(GS_OUTPUT input) : SV_TARGET
{
	return input.color;
}