/*
	This is an example of a fragment shader
*/

#include "shaders.inc"

// Entry Point
//============
// Color
uniform vec3 g_color;
uniform float g_alpha_modifier;
uniform sampler2D textureSampler;

#if defined( EAE6320_PLATFORM_GL )
// Input
//======

// Whatever arbitrary data (i.e. everything excluding position) was output from the vertex shader
// will be interpolated across the triangle and given as input to the fragment shader

layout( location = 0 ) in vec4 i_color;
layout( location = 1 ) in float2 i_texcoords;

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;
#endif

void main(
#if defined( EAE6320_PLATFORM_D3D )

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
#endif
	)
{
	// Set the fragment to the interpolated color that originated as per-vertex data
	// (where color is represented by 4 floats representing "RGBA" == "Red/Green/Blue/Alpha")
	{
		o_color = tex2D(textureSampler, i_texcoords);
		o_color.rgb *= i_color.rgb * g_color;
		o_color.a *= i_color.a * g_alpha_modifier;
	}
}