return
{
	effect = "data/TransparentEffect.lua",
	uniforms = 
	{
		{
			name = "g_color",
			values = {0, 1, 0},
			shaderType = "fragment"
		},
		{
			name = "g_alpha_modifier",
			values = {0.3},
			shaderType = "fragment"
		},		
	},
	textures =
	{
		{
			samplerName = "textureSampler",
			path = "data/Brick.jpg"
		}
	}
}