struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

struct GS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

[maxvertexcount(3)]
void main(triangle VS_OUTPUT input[3], 
	inout TriangleStream< GS_OUTPUT > output) {
	for (uint i = 0; i < 3; i++)
	{
		GS_OUTPUT element;
		element.pos = input[i].pos;
		element.color = input[i].color;
		output.Append(element);
	}
}