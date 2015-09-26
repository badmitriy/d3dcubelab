//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
	float4 Normal: NORMAL;
	float4 color : COLOR0;
};
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( VS_OUTPUT input ) : SV_Target
{
	//float intens=mul(input.Normal,float4(0,-1,0,0));

    return input.color;
}
