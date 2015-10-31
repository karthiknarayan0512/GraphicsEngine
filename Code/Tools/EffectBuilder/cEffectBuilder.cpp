// Header Files
//=============

#include "cEffectBuilder.h"

#include <iostream>
#include <cassert>
#include <fstream>

#include "../../Engine/Windows/Functions.h"
#include "../../External/Lua/Includes.h"

bool eae6320::cEffectBuilder::Build(const std::vector<std::string>&)
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

	// Get the vertex shader path
	const char* const vertexPathKey = "vertex";
	lua_pushstring(luaState, vertexPathKey);
	lua_gettable(luaState, -2);

	const char *vertexPath = lua_tostring(luaState, -1);
	lua_pop(luaState, 1);

	const char* const shaderPathKey = "fragment";
	lua_pushstring(luaState, shaderPathKey);
	lua_gettable(luaState, -2);

	const char *shaderPath = lua_tostring(luaState, -1);
	lua_pop(luaState, 1);

	std::ofstream effectBinary(m_path_target, std::ofstream::binary);
	effectBinary.write(vertexPath, strlen(vertexPath));
	effectBinary.write("\0", 1);
	effectBinary.write(shaderPath, strlen(shaderPath));
	effectBinary.write("\0", 1);

	effectBinary.close();

	lua_pop(luaState, 1);

	if (luaState)
	{
		// If I haven't made any mistakes
		// there shouldn't be anything on the stack,
		// regardless of any errors encountered while loading the file:
		assert(lua_gettop(luaState) == 0);

		lua_close(luaState);
		luaState = NULL;
	}

	return !wereThereErrors;
}
