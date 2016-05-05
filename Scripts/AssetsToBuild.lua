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
			"lambert2.mesh",
			"phong1.mesh",
			"phong2.mesh",
			"phong3.mesh",
			"phong4.mesh",
			"phong5.mesh",
			"phong6.mesh",
			"phong7.mesh",
			"phong8.mesh",
			"phong9.mesh",
			"phong10.mesh",
			"BrownFlag.mesh",
			"RailingFlag.mesh",
			"FlagPole.mesh"
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
			"SpriteEffect.lua",
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
			"SpriteVertex.shader",
			"SpriteFragment.shader",
			"debugshapeVertex.shader",
			"debugshapeFragment.shader"
		},
		source = "",
		destination = "",
		optionalArguments = 
		{
			"vertex",
			"fragment",
			"fragment",
			"vertex",
			"fragment",
			"vertex",
			"fragment",
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
			"railing_m.material",
			"ceiling_m.material",
			"cement_m.material",
			"floor_m.material",
			"walls_m.material",
			"metal_m.material",
			"lambert2.material",
			"texture.material",
			"numbers.material",
			"Stamina.material",
			"phong1.material",
			"phong2.material",
			"phong3.material",
			"phong4.material",
			"phong5.material",
			"phong6.material",
			"phong7.material",
			"phong8.material",
			"phong9.material",
			"phong10.material",
			"BrownFlag.material",
			"RailingFlag.material",
			"FlagPole.material"
		},
		source = "",
		destination = "",
	},
	textures = 
	{
		buildTool = "TextureBuilder.exe",
		assets = 
		{
			"railing_m.png",
			"floor_m.png",
			"cement_m.png",
			"ceiling_m.png",
			"walls_m.png",
			"metal_m.png",
			"logo.png",
			"numbers.png",
			"BB8Head.png",
			"BB8Body.png",
			"lambert2.png",
			"BlackUtes.png",
			"StaminaMeter.png"
		},
		source = "",
		destination = "",
	},
}