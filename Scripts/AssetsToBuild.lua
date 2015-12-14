return
{
	meshes = 
	{
		buildTool = "MeshBuilder.exe",
		assets = 
		{
			"Pig.lua",
			"Cube.lua",
			"Coconut.lua",
			"Banana.lua",
			"Player.lua"
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
			"PigMaterial.lua",
			"CubeMaterial.lua",
			"CoconutMaterial.lua",
			"BananaMaterial.lua",
			"PlayerMaterial.lua"
		},
		source = "",
		destination = "",
	},
	textures = 
	{
		buildTool = "TextureBuilder.exe",
		assets = 
		{
			"pig.png",
			"Water.jpg",
			"banana.png",
			"coconuts.png",
			"PlayerTexture.png"
		},
		source = "",
		destination = "",
	},
}