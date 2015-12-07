// Header Files
//=============

#include "cMaterialBuilder.h"
#include "../../External/Lua/Includes.h"
#include "../../Engine/Graphics/Material.h"
#include "../../Engine/Windows/Includes.h"

#include <iostream>
#include <cassert>
#include <fstream>

// Build
//------

bool eae6320::cMaterialBuilder::Build(const std::vector<std::string>& i_arguments)
{
	bool wereThereErrors = false;

	// Create a new Lua state
	lua_State* luaState = NULL;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			wereThereErrors = true;
			std::cerr << "Failed to create a new Lua state"
				"\n";
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	{
		const int luaResult = luaL_loadfile(luaState, m_path_source);
		if (luaResult != LUA_OK)
		{
			wereThereErrors = true;
			std::cerr << lua_tostring(luaState, -1);
			// Pop the error message
			lua_pop(luaState, 1);
		}
	}

	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		const int argumentCount = 0;
		const int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		const int noMessageHandler = 0;
		const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const int returnedValueCount = lua_gettop(luaState);
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					wereThereErrors = true;
					std::cerr << "Asset files must return a table (instead of a " <<
						luaL_typename(luaState, -1) << ")\n";
					// Pop the returned non-table value
					lua_pop(luaState, 1);
				}
			}
			else
			{
				wereThereErrors = true;
				std::cerr << "Asset files must return a single table (instead of " <<
					returnedValueCount << " values)"
					"\n";
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
			}
		}
		else
		{
			wereThereErrors = true;
			std::cerr << lua_tostring(luaState, -1);
			// Pop the error message
			lua_pop(luaState, 1);
		}
	}

	// Open the binary file for writing
	std::ofstream materialBinary(m_path_target, std::ofstream::binary);

	// Get the vertex shader path
	const char* const effectPathKey = "effect";
	lua_pushstring(luaState, effectPathKey);
	lua_gettable(luaState, -2);

	const char *effectPath = lua_tostring(luaState, -1);
	lua_pop(luaState, 1);

	// Write the effect path
	materialBinary.write(effectPath, strlen(effectPath) + 1);

	// Get uniform table
	const char* const uniformskey = "uniforms";
	lua_pushstring(luaState, uniformskey);
	lua_gettable(luaState, -2);

	Graphics::Material::sParameterToSet *uniforms;
	
	if (lua_istable(luaState, -1))
	{
		uint8_t uniformsCount = luaL_len(luaState, -1);
		uniforms = new Graphics::Material::sParameterToSet[uniformsCount];

		// Write the uniform count
		char *uniformCount = NULL;
		uniformCount = reinterpret_cast<char *>(&uniformsCount);
		materialBinary.write(uniformCount, sizeof(uniformsCount));

		for (uint8_t i = 1; i <= uniformsCount; i++)
		{
			lua_pushinteger(luaState, i);
			lua_gettable(luaState, -2);

			const char* const namekey = "name";
			lua_pushstring(luaState, namekey);
			lua_gettable(luaState, -2);

			const char *uniformName = lua_tostring(luaState, -1);
			lua_pop(luaState, 1);

			materialBinary.write(uniformName, strlen(uniformName) + 1);

			uniforms[i-1].uniformHandle = NULL;

			const char* const valueskey = "values";
			lua_pushstring(luaState, valueskey);
			lua_gettable(luaState, -2);

			if (lua_istable(luaState, -1))
			{
				uniforms[i-1].valueCountToSet = luaL_len(luaState, -1);

				for (uint8_t j = 1; j <= uniforms[i-1].valueCountToSet; j++)
				{
					lua_pushinteger(luaState, j);
					lua_gettable(luaState, -2);

					uniforms[i-1].values[j - 1] = static_cast<float>(lua_tonumber(luaState, -1));
					lua_pop(luaState, 1);
				}
			}

			lua_pop(luaState, 1);

			const char* const shaderTypekey = "shaderType";
			lua_pushstring(luaState, shaderTypekey);
			lua_gettable(luaState, -2);

			if (strcmp(lua_tostring(luaState, -1), "fragment") == 0)
				uniforms[i-1].shaderType = eae6320::Graphics::ShaderTypes::Fragment;
			else if (strcmp(lua_tostring(luaState, -1), "vertex") == 0)
				uniforms[i-1].shaderType = eae6320::Graphics::ShaderTypes::Vertex;

			lua_pop(luaState, 1);

			char *uniformsBuffer = NULL;
			uniformsBuffer = reinterpret_cast<char *>(&uniforms[i-1]);
			materialBinary.write(uniformsBuffer, sizeof(Graphics::Material::sParameterToSet));

			lua_pop(luaState, 1);
		}
	}

	lua_pop(luaState, 1);

	// Get textures table
	const char* const texturesKey = "textures";
	lua_pushstring(luaState, texturesKey);
	lua_gettable(luaState, -2);

	if (lua_istable(luaState, -1))
	{
		uint8_t texturesCount = luaL_len(luaState, -1);

		// Write the uniform count
		char *textureCount = NULL;
		textureCount = reinterpret_cast<char *>(&texturesCount);
		materialBinary.write(textureCount, sizeof(texturesCount));

		for (uint8_t i = 0; i < texturesCount; i++)
		{
			lua_pushinteger(luaState, i + 1);
			lua_gettable(luaState, -2);

			const char* const samplerNameKey = "samplerName";
			lua_pushstring(luaState, samplerNameKey);
			lua_gettable(luaState, -2);

			const char *samplerPath = lua_tostring(luaState, -1);
			materialBinary.write(samplerPath, strlen(samplerPath) + 1);

			lua_pop(luaState, 1);

			const char* const texturePathKey = "path";
			lua_pushstring(luaState, texturePathKey);
			lua_gettable(luaState, -2);

			const char *texturePath = lua_tostring(luaState, -1);
			materialBinary.write(texturePath, strlen(texturePath) + 1);

			lua_pop(luaState, 1);

			lua_pop(luaState, 1);
		}
	}

	lua_pop(luaState, 1);

	lua_pop(luaState, 1);

	if (luaState)
	{
		// If I haven't made any mistakes
		// there shouldn't be anything on the stack,
		// regardless of any errors encountered while loading the file:
		assert(lua_gettop(luaState) == 0);

		luaState = NULL;
	}

	return !wereThereErrors;
}