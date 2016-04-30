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
			"lambert3.mesh",
			"BB8Head.mesh",
			"BB8Body.mesh",
			"HeadRing.mesh",
			"HeadTop.mesh",
			"phong5.mesh",
			"phong6.mesh",
			"phong3.mesh",
			"phong2.mesh",
			"phong4.mesh",
			"RightAntenna.mesh",
			"LeftAntenna.mesh",
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
			"SpriteEffect.lua"
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
			"SpriteVertex.shader",
			"SpriteFragment.shader",
		},
		source = "",
		destination = "",
		optionalArguments = 
		{
			"vertex",
			"fragment",
			"fragment",
			"vertex",
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
			"railing.material",
			"cement.material",
			"floor.material",
			"wall.material",
			"metal.material",
			"texture.material",
			"numbers.material",
			"BB8Head.material",
			"BB8Body.material",
			"HeadRing.material",
			"HeadTop.material",
			"phong5.material",
			"phong6.material",
			"phong3.material",
			"phong2.material",
			"phong4.material",
			"RightAntenna.material",
			"LeftAntenna.material",
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
			"logo.png",
			"numbers.png",
			"HeadTop.jpg",
			"HeadRing.jpg",
			"BB8Head.jpg",
			"BB8Body.jpg",
		},
		source = "",
		destination = "",
	},
}