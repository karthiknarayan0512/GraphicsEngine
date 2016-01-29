#pragma once

#include "../Windows/Includes.h"

#include <stdint.h>

struct sDebugVertex
{
	// POSITION
	// 3 floats == 12 bytes
	// Offset = 0
	float x, y, z;
	// COLOR0
	// 4 uint8_ts == 4 bytes
	// Offset = 12
	uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
};

struct sDebugDiffuseVertex
{
	float x, y, z;
	DWORD diffuse;
};

