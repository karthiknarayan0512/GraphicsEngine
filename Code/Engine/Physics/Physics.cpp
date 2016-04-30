#include "Physics.h"

#include <vector>
#include <d3d9.h>

#include <iostream>
#include <fstream>
#include <cassert>


#include "../UserOutput/UserOutput.h"

namespace eae6320
{
	namespace Physics
	{
		Graphics::Mesh::sVertex *vertices;
		uint32_t* indexData;

		int verticesCount;
		int indexCount;
		

		bool IsRayIntersectingTriangle(const Math::cVector &RV0, const Math::cVector &RV1,
			const Math::cVector &TV0, const Math::cVector &TV1, const Math::cVector &TV2, Math::cVector &o_intersectionPoint)
		{
			Math::cVector u, v, n;
			Math::cVector dir, w0, w;
			float r, a, b;

			u = TV1 - TV0;
			v = TV2 - TV0;
			n = Math::Cross(u, v);
			Math::cVector normalized = n.CreateNormalized();

			dir = RV1 - RV0;
			w0 = RV0 - TV0;

			a = -Math::Dot(n, w0);
			b = Math::Dot(n, dir);

			if (b <= 0.0f)
				return false;

			r = a / b;
			if (r < 0.0f)
				return false;
			if (r > 1.0f)
				return false;

			o_intersectionPoint = RV0 + dir * r;

			float uu, uv, vv, wu, wv, D;
			uu = Math::Dot(u, u);
			uv = Math::Dot(u, v);
			vv = Math::Dot(v, v);
			w = o_intersectionPoint - TV0;
			wu = Math::Dot(w, u);
			wv = Math::Dot(w, v);
			D = uv * uv - uu * vv;

			float s, t;
			s = (uv * wv - vv * wu) / D;
			if (s < 0.0f || s > 1.0f)
				return false;
			t = (uv * wu - uu * wv) / D;
			if (t < 0.0f || (s + t) > 1.0f)
				return false;
			 
			return true;
		}

		void Initialize(const char * i_CollisionMesh)
		{
			bool wereThereErrors = false;

			std::ifstream meshBinary(i_CollisionMesh, std::ofstream::binary);

			meshBinary.seekg(0, meshBinary.end);
			int length = (int)meshBinary.tellg();
			meshBinary.seekg(0, meshBinary.beg);

			char * buffer = new char[length];
			meshBinary.read(buffer, length);

			// Get vertexCount
			verticesCount = *reinterpret_cast<int *>(buffer);

			// Get vertices
			vertices = reinterpret_cast<Graphics::Mesh::sVertex *>(buffer + sizeof(verticesCount));

			//Write indices count
			indexCount = *reinterpret_cast<int *>(buffer + sizeof(verticesCount) +
				verticesCount * sizeof(Graphics::Mesh::sVertex));

			// Write index array
			indexData = reinterpret_cast<uint32_t*>(buffer + sizeof(verticesCount) +
				verticesCount * sizeof(Graphics::Mesh::sVertex) +
				sizeof(indexCount));

			meshBinary.close();
		}

		bool CheckCollisions(const Math::cVector &i_StartPoint, const Math::cVector &i_EndPoint, Math::cVector &i_IntersectionPoint)
		{
			bool bRayIntersection;
			for (int j = 0; j < indexCount; j += 3)
			{
				Math::cVector TV0(vertices[indexData[j]].x, vertices[indexData[j]].y, vertices[indexData[j]].z);
				Math::cVector TV1(vertices[indexData[j + 1]].x, vertices[indexData[j + 1]].y, vertices[indexData[j + 1]].z);
				Math::cVector TV2(vertices[indexData[j + 2]].x, vertices[indexData[j + 2]].y, vertices[indexData[j + 2]].z);

				bRayIntersection = IsRayIntersectingTriangle(i_StartPoint, i_EndPoint, TV0, TV1, TV2, i_IntersectionPoint);

				if (bRayIntersection)
					break;
			}

			return bRayIntersection;
		}


	}
}