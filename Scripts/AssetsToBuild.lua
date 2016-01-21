return
{
	meshes = 
	{
		buildTool = "MeshBuilder.exe",
		assets = 
		{
			"railing_m.material"
		},
		source = "",
		destination = "",
	},
	dependencies = 
	{
		buildTool = "GenericBuilder.exe",
		assets = 
		{
			"shaders.inc",
		},
		source = "",
		destination = "",	
	},
	shaderinfo =
	{
		buildTool = "EffectBuilder.exe",
		assets = 
		{
			"Effect.lua",
			"TransparentEffect.lua"
		},
		source = "",
		destination = "",
	},
	shaders = 
	{
		buildTool = "ShaderBuilder.exe",
		assets = 
		{
			"vertex.shader",
			"fragment.shader",
			"transparent.shader",
		},
		source = "",
		destination = "",
		optionalArguments = 
		{
			"vertex",
			"fragment",
			"fragment"
		},
		dependencies = 
		{
			"shaders.inc",
		}
	},
	materials =
	{
		buildTool = "MaterialBuilder.exe",
		assets = 
		{
			"railing.material"
		},
		source = "",
		destination = "",
	},
	textures = 
	{
		buildTool = "TextureBuilder.exe",
		assets = 
		{
			"railing.png"
		},
		source = "",
		destination = "",
	},
}