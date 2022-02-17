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
			return Angle{ 150 };
		case Vertex2:
			return Angle{ 210 };
		case Vertex3:
			return Angle{ 270 };
		case Vertex4:
			return Angle{ 330 };
		case Vertex5:
			return Angle{ 30 };
		}
	}
}

HexVertexNumber AngleToClosestVertex(Angle angle)
{
	if (angle >= Angle{ 0 } && angle < Angle{ 60 })
		return HexVertexNumber{ 5 };
	else if (angle >= Angle{ 60 } && angle < Angle{ 120 })
		return HexVertexNumber{ 0 };
	else if (angle >= Angle{ 120 } && angle < Angle{ 180 })
		return HexVertexNumber{ 1 };
	else if (angle >= Angle{ 180 } && angle < Angle{ 240 })
		return HexVertexNumber{ 2 };
	else if (angle >= Angle{ 240 } && angle < Angle{ 300 })
		return HexVertexNumber{ 3 };
	else 
		return HexVertexNumber{ 4 }; // last vertex left
}
