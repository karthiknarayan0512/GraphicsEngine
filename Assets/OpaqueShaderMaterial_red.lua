return
{
	effect = "data/Effect.lua",
	uniforms = 
	{
		{
			name = "g_color",
			values = {1, 0, 0},
			shaderType = "fragment"
		}
	},
	textures =
	{
		{
			samplerName = "textureSampler",
			path = "data/Brick.jpg"
		}
	}
}