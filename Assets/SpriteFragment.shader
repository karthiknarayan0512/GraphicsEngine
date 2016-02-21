/*
	This is an example of a fragment shader
*/

#include "shaders.inc"

// Entry Point
//============
// Color
uniform sampler2D textureSampler;

void main(

	// Input
	//======

	// Whatever arbitrary data (i.e. everything excluding position) was output from the vertex shader
	// will be interpolated across the triangle and given as input to the fragment shader

	in float4 i_color : COLOR0,
	in float2 i_texcoords : TEXCOORD0,

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : COLOR0
	)
{
	// Set the fragment to the interpolated color that originated as per-vertex data
	// (where color is represented by 4 floats representing "RGBA" == "Red/Green/Blue/Alpha")
	{
		o_color = tex2D(textureSampler, i_texcoords);
		o_color.rgb *= i_color.rgb;
	}
}