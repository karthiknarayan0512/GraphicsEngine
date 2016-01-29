return
{
	meshes = 
	{
		buildTool = "MeshBuilder.exe",
		assets = 
		{
			"railing_m.mesh",
			"ceiling_m.mesh",
			"cement_m.mesh",
			"floor_m.mesh",
			"metal_m.mesh",
			"walls_m.mesh",
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
			"TransparentEffect.lua",
			"debugEffect.lua"
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
			"debugshapeFragment.shader",
			"debugshapeVertex.shader"
		},
		source = "",
		destination = "",
		optionalArguments = 
		{
			"vertex",
			"fragment",
			"fragment",
			"fragment",
			"vertex"
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
			"railing.material",
			"cement.material",
			"floor.material",
			"wall.material",
			"metal.material",
		},
		source = "",
		destination = "",
	},
	textures = 
	{
		buildTool = "TextureBuilder.exe",
		assets = 
		{
			"railing.png",
			"floor.png",
			"cement_wall.png",
			"wall.png",
			"metal_brace.png",
		},
		source = "",
		destination = "",
	},
}