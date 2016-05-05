#include "Physics.h"

#include <vector>
#include <d3d9.h>

#include <iostream>
#include <fstream>
#include <cassert>

#include "../Graphics/DebugLine.h"
#include "../UserOutput/UserOutput.h"

namespace eae6320
{
	namespace Physics
	{
		Graphics::Mesh::sVertex *vertices;
		uint32_t* indexData;

		int verticesCount;
		int indexCount;
		
		Graphics::DebugShapes::DebugLine * debuglines;

		bool IsRayIntersectingTriangle(const Math::cVector &RV0, const Math::cVector &RV1,
			const Graphics::Mesh::sVertex &TV0, const Graphics::Mesh::sVertex &TV1, const Graphics::Mesh::sVertex &TV2, Graphics::Mesh::sVertex &o_intersectionPoint)
		{
			Math::cVector tv0(TV0.x, TV0.y, TV0.z);
			Math::cVector tv1(TV1.x, TV1.y, TV1.z);
			Math::cVector tv2(TV2.x, TV2.y, TV2.z);

			Math::cVector intersectionPoint;

			Math::cVector u, v, n(TV0.nx, TV0.ny, TV0.nz);
			Math::cVector dir, w0, w;
			float r, a, b;

			u = tv1 - tv0;
			v = tv2 - tv0;

			//char buf[100];
			//sprintf(buf, "Normal point: X: %f, Y: %f, Z:%f\n", TV0.x, TV0.y, TV0.z);
			//OutputDebugString(buf);

			//sprintf(buf, "Normal: X: %f, Y: %f, Z:%f\n", n.x, n.y, n.z);
			//OutputDebugString(buf);

			dir = RV1 - RV0;
			w0 = RV0 - tv0;

			a = Math::Dot(n, w0);
			b = -Math::Dot(n, dir);

			if (b <= 0.0f)
				return false;

			r = a / b;
			if (r < 0.0f)
				return false;
			if (r > 1.0f)
				return false;

			intersectionPoint = RV0 + dir * r;

			float uu, uv, vv, wu, wv, D;
			uu = Math::Dot(u, u);
			uv = Math::Dot(u, v);
			vv = Math::Dot(v, v);
			w = w0 + dir * r;
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
			
			o_intersectionPoint.x = intersectionPoint.x;
			o_intersectionPoint.y = intersectionPoint.y;
			o_intersectionPoint.z = intersectionPoint.z;

			o_intersectionPoint.nx = n.x;
			o_intersectionPoint.ny = n.y;
			o_intersectionPoint.nz = n.z;


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

			debuglines = new Graphics::DebugShapes::DebugLine[indexCount];
			for (int j = 0; j < indexCount; j+=3)
			{
				D3DVECTOR vert1, vert2;
				
				vert1.x = vertices[indexData[j]].x;
				vert1.y = vertices[indexData[j]].y;
				vert1.z = vertices[indexData[j]].z;

				vert2.x = vertices[indexData[j+1]].x;
				vert2.y = vertices[indexData[j+1]].y;
				vert2.z = vertices[indexData[j+1]].z;

				debuglines[j].CreateLine(vert1, vert2, 0, 0, 255);

				vert1.x = vertices[indexData[j+1]].x;
				vert1.y = vertices[indexData[j+1]].y;
				vert1.z = vertices[indexData[j+1]].z;

				vert2.x = vertices[indexData[j+2]].x;
				vert2.y = vertices[indexData[j + 2]].y;
				vert2.z = vertices[indexData[j + 2]].z;

				debuglines[j+1].CreateLine(vert1, vert2, 0, 0, 255);

				vert1.x = vertices[indexData[j+2]].x;
				vert1.y = vertices[indexData[j+2]].y;
				vert1.z = vertices[indexData[j+2]].z;

				vert2.x = vertices[indexData[j]].x;
				vert2.y = vertices[indexData[j]].y;
				vert2.z = vertices[indexData[j]].z;

				debuglines[j+2].CreateLine(vert1, vert2, 0, 0, 255);
			}

			meshBinary.close();
		}

		void RenderMesh()
		{
			for (int j = 0; j < indexCount; j++)
				debuglines[j].AddLine();
		}

		bool CheckCollisions(const Math::cVector &i_StartPoint, const Math::cVector &i_EndPoint, Graphics::Mesh::sVertex &i_IntersectionPoint)
		{
			bool bRayIntersection;
			for (int j = 0; j < indexCount * 3; j += 3)
			{
				bRayIntersection = IsRayIntersectingTriangle(i_StartPoint, i_EndPoint, vertices[indexData[j]], vertices[indexData[j + 1]], vertices[indexData[j + 2]], i_IntersectionPoint);

				if (bRayIntersection)
					break;
			}

			return bRayIntersection;
		}
	}
}