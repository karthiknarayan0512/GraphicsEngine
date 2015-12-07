return
{
	effect = "data/Effect.lua",
	uniforms = 
	{
		{
			name = "g_color",
			values = {0, 0, 1},
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