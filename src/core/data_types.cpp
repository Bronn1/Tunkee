#include "data_types.h"

Angle VertexToAngle(HexVertexNumber vertex)
{
	{
		using enum HexVertex;
		HexVertex vertexNum = static_cast<HexVertex>(vertex.vertexNum);
		switch (vertexNum)
		{
		case Vertex0:
			return Angle{ 90 };
		case Vertex1:
			return Angle{ 135 };
		case Vertex2:
			return Angle{ 225 };
		case Vertex3:
			return Angle{ 270 };
		case Vertex4:
			return Angle{ 315 };
		case Vertex5:
			return Angle{ 45 };
		}
	}
}
