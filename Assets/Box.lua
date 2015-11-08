return
{
	vertices = 
	{
		{
			position = { -1.0, -1.0, 1.0},
			color = { 1, 0, 0, 1} 
		},

		{
			position = { 1.0, -1.0, 1.0},
			color = { 0, 0, 1, 1} 
		},

		{
			position = { 1.0, 1.0, 1.0},
			color = { 0, 1, 0, 1} 
		},

		{
			position = { -1.0, 1.0, 1.0},
			color = { 1, 1, 0, 1} 
		},

		{
			position = { -1.0, -1.0, -1.0},
			color = { 1, 0, 0, 1} 
		},

		{
			position = { 1.0, -1.0, -1.0},
			color = { 0, 0, 1, 1} 
		},

		{
			position = { 1.0, 1.0, -1.0},
			color = { 0, 1, 0, 1} 
		},

		{
			position = { -1.0, 1.0, -1.0},
			color = { 1, 1, 0, 1} 
		}
	},
	
	indices = 
	{
	--Front face
		{ 0, 1, 2 },
		{ 0, 2, 3 },
	-- Back face
		{ 4, 5, 6 },
		{ 4, 6, 7 },
	-- Top face
		{ 3, 2, 6 },
		{ 3, 6, 7 },
	-- Bottom face
		{ 0, 5, 1 },
		{ 0, 4, 5 },
	-- Left face
		{ 0, 7, 4 },
		{ 0, 3, 7 },
	-- Right face
		{ 1, 5, 6 },
		{ 1, 6, 2 },
		-- { 0, 3, 7 },
	}
}