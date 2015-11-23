return
{
	effect = "data/TransparentEffect.lua",
	uniforms = 
	{
		{
			name = "g_color",
			values = {1, 1, 1},
			shaderType = "fragment"
		},
		{
			name = "g_alpha_modifier",
			values = {0.3},
			shaderType = "fragment"
		},		
	},
}