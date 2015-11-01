// Header Files
//=============

#include "cMeshBuilder.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include "../../Engine/Windows/Functions.h"
#include "../../External/Lua/Includes.h"
#include "../../Engine/Graphics/Mesh.h"

// Interface
//==========

// Build
//------

void GetVerticesFromMeshFile(lua_State* luaState, eae6320::Graphics::Mesh::sVertex *& vertices, int& verticesCount)
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
		vertices = new eae6320::Graphics::Mesh::sVertex[verticesCount];

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
				lua_pushinteger(luaState, 1);
				lua_gettable(luaState, -2);
				float f = (float)lua_tonumber(luaState, -1);
				vertices[i-1].x = f;
				lua_pop(luaState, 1);

				lua_pushinteger(luaState, 2);
				lua_gettable(luaState, -2);
				f = (float)lua_tonumber(luaState, -1);
				vertices[i - 1].y = f;
				lua_pop(luaState, 1);
			}
			lua_pop(luaState, 1);

			const char* const colorkey = "color";
			lua_pushstring(luaState, colorkey);
			lua_gettable(luaState, -2);
			if (lua_istable(luaState, -1))
			{
				lua_pushinteger(luaState, 1);
				lua_gettable(luaState, -2);
				uint8_t f = (uint8_t)lua_tointeger(luaState, -1);
				vertices[i - 1].r = f;
				lua_pop(luaState, 1);

				lua_pushinteger(luaState, 2);
				lua_gettable(luaState, -2);
				f = (uint8_t)lua_tointeger(luaState, -1);
				vertices[i - 1].g = f;
				lua_pop(luaState, 1);

				lua_pushinteger(luaState, 3);
				lua_gettable(luaState, -2);
				f = (uint8_t)lua_tointeger(luaState, -1);
				vertices[i - 1].b = f;
				lua_pop(luaState, 1);

				lua_pushinteger(luaState, 4);
				lua_gettable(luaState, -2);
				f = (uint8_t)lua_tointeger(luaState, -1);
				vertices[i - 1].a = f;
				lua_pop(luaState, 1);
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

bool eae6320::cMeshBuilder::Build( const std::vector<std::string>& )
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

	Graphics::Mesh::sVertex *vertices;
	int verticesCount = 0;

	//::MessageBox(NULL, "", "", MB_OK);
	GetVerticesFromMeshFile(luaState, vertices, verticesCount);

	// Get indices data
	uint32_t* indexData = NULL;
	int indicesCount = 0;

	GetIndicesFromMeshFile(luaState, indexData, indicesCount);

	lua_pop(luaState, 1);
	std::ofstream meshBinary(m_path_target, std::ofstream::binary);

	// Write vertexCount
	char *vertexCount = NULL;
	vertexCount = reinterpret_cast<char *>(&verticesCount);
	meshBinary.write(vertexCount, sizeof(verticesCount));

	// Write vertices
	char *verticesBuffer = NULL;
	verticesBuffer = reinterpret_cast<char *>(vertices);
	meshBinary.write(verticesBuffer, verticesCount * sizeof(Graphics::Mesh::sVertex));

	//Write indices count
	char *indexCount = NULL;
	indexCount = reinterpret_cast<char *>(&indicesCount);
	meshBinary.write(indexCount, sizeof(indicesCount));

	// Write index array
	char *indexBuffer = NULL;
	indexBuffer = reinterpret_cast<char *>(indexData);
	meshBinary.write(indexBuffer, indicesCount * 3 * sizeof(uint32_t));

	meshBinary.close();
	
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
