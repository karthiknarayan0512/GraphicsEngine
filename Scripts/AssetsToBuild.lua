return
{
	meshes = 
	{
		buildTool = "MeshBuilder.exe",
		assets = 
		{
			"Mesh.lua",
			"Triangle.lua",
		},
		source = "",
		destination = "",
		optionalArguments = "",
	},
	shaders =
	{
		buildTool = "EffectBuilder.exe",
		assets = 
		{
			"Effect.lua",
		},
		source = "",
		destination = "",
		optionalArguments = "",
	},
	vertexshader = 
	{
		buildTool = "ShaderBuilder.exe",
		assets = 
		{
			"vertex.shader",
		},
		source = "",
		destination = "",
		optionalArguments = "vertex",
	},
	fragmentshader = 
	{
		buildTool = "ShaderBuilder.exe",
		assets = 
		{
			"fragment.shader",
		},
		source = "",
		destination = "",
		optionalArguments = "fragment",
	}
}