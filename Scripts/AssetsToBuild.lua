return
{
	meshes = 
	{
		buildTool = "MeshBuilder.exe",
		assets = 
		{
			"Football.lua",
			"Floor.lua",
			"Pipe.lua",
			"Dodec.lua"
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
		},
		source = "",
		destination = "",
	}
}