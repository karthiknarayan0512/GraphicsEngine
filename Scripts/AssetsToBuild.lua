return
{
	meshes = 
	{
		buildTool = "MeshBuilder.exe",
		assets = 
		{
			"Floor.lua",
			"Sphere.lua",
			"Box.lua",
			"Plane.lua"
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
			"TransparentShaderMaterial_almostsemi.lua",
			"TransparentShaderMaterial_semi.lua",
			"OpaqueShaderMaterial.lua",
			"OpaqueShaderMaterial_blue.lua",
			"OpaqueShaderMaterial_red.lua",
			"OpaqueShaderMaterial_eae6320Image.lua",
			"OpaqueShaderMaterial_alphaImage.lua",
		},
		source = "",
		destination = "",
	},
	textures = 
	{
		buildTool = "TextureBuilder.exe",
		assets = 
		{
			"alpha.png",
			"eae6320.png",
			"Brick.jpg"
		},
		source = "",
		destination = "",
	},
}