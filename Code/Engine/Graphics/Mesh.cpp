#include <iostream>
#include <cassert>

#include "../../External/Lua/Includes.h"
#include "Mesh.h"

void GetVerticesFromMeshFile(lua_State* luaState, float **& positions, uint8_t ** &colors, int& verticesCount)
{
	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "vertices" table will be at -1:
	verticesCount = 0;
	const char* const verticesKey = "vertices";
	lua_pushstring(luaState, verticesKey);
	lua_gettable(luaState, -2);

	if (lua_istable(luaState, -1))
	{
		verticesCount = luaL_len(luaState, -1);
		positions = new float*[verticesCount];
		colors = new uint8_t*[verticesCount];

		// Get Vertex Data
		for (int i = 1; i <= verticesCount; ++i)
		{
			lua_pushinteger(luaState, i);
			lua_gettable(luaState, -2);

			const char* const positionkey = "position";
			lua_pushstring(luaState, positionkey);
			lua_gettable(luaState, -2);
			if (lua_istable(luaState, -1))
			{
				const int positionCount = luaL_len(luaState, -1);
				positions[i - 1] = new float[positionCount];
				for (int j = 1; j <= positionCount; ++j)
				{
					lua_pushinteger(luaState, j);
					lua_gettable(luaState, -2);
					float f = (float)lua_tonumber(luaState, -1);
					positions[i - 1][j - 1] = f;
					lua_pop(luaState, 1);
				}
			}
			lua_pop(luaState, 1);

			const char* const colorkey = "color";
			lua_pushstring(luaState, colorkey);
			lua_gettable(luaState, -2);
			if (lua_istable(luaState, -1))
			{
				const int colorCount = luaL_len(luaState, -1);
				colors[i - 1] = new uint8_t[colorCount];
				for (int j = 1; j <= colorCount; ++j)
				{
					lua_pushinteger(luaState, j);
					lua_gettable(luaState, -2);
					uint8_t f = (uint8_t)lua_tointeger(luaState, -1);
					colors[i - 1][j - 1] = f;
					lua_pop(luaState, 1);
				}
			}

			lua_pop(luaState, 1);
			lua_pop(luaState, 1);
		}
	}

	// Pop the vertices table
	lua_pop(luaState, 1);
}

void GetIndicesFromMeshFile(lua_State* luaState, uint32_t*& indexData, int &indicesCount)
{
	const char* const indicesKey = "indices";
	lua_pushstring(luaState, indicesKey);
	lua_gettable(luaState, -2);

	if (lua_istable(luaState, -1))
	{
		indicesCount = luaL_len(luaState, -1);
		int index = 0;
		for (int i = 1; i <= indicesCount; i++)
		{
			lua_pushinteger(luaState, i);
			lua_gettable(luaState, -2);
			if (lua_istable(luaState, -1))
			{
				const int indexCount = luaL_len(luaState, -1);
				if (indexData == NULL)
					indexData = new uint32_t[indicesCount  * indexCount];
				for (int j = 1; j <= indexCount; j++)
				{
					lua_pushinteger(luaState, j);
					lua_gettable(luaState, -2);
					uint32_t f = (uint32_t)lua_tointeger(luaState, -1);
					indexData[index++] = f;
					lua_pop(luaState, 1);
				}
			}
			lua_pop(luaState, 1);
		}
	}

	lua_pop(luaState, 1);
}

bool eae6320::Mesh::LoadMeshFromFile(const char* i_FilePath)
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
			goto OnExit;
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	{
		const int luaResult = luaL_loadfile(luaState, i_FilePath);
		if (luaResult != LUA_OK)
		{
			wereThereErrors = true;
			::MessageBox(NULL, lua_tostring(luaState, -1), "Error", MB_OK);
			std::cerr << lua_tostring(luaState, -1);
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
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
					goto OnExit;
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
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::cerr << lua_tostring(luaState, -1);
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}

	float ** positions = NULL;
	uint8_t ** colors = NULL;
	int verticesCount = 0;

	GetVerticesFromMeshFile(luaState, positions, colors, verticesCount);

	// Get indices data
	uint32_t* indexData = NULL;
	int indicesCount = 0;

	GetIndicesFromMeshFile(luaState, indexData, indicesCount);

	lua_pop(luaState, 1);

	LoadVertexAndIndicesData(positions, colors, indexData, verticesCount, indicesCount);
OnExit:

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