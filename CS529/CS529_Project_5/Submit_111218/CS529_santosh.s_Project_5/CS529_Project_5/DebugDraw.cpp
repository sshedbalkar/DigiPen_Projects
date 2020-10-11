///////////////////////////////////////////////////////////////////////////////////////
//
//	DebugDraw.cpp
//
//	Authors: Chris Peters 
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "DebugDraw.h"

namespace Wye
{
	Drawer Drawer::Instance;

	void Drawer::moveTo(Vec3 v)
	{
		_writePosition = v;
	}

	void Drawer::setColor(Vec4 color)
	{
		flush();
		_color = color;
	}

	void Drawer::lineTo(Vec3 newPosition)
	{
		LineSegment seg = {_writePosition, newPosition};
		_lineSegments.push_back(seg);
		++_segmentsDrawn;
		_writePosition = newPosition;
	}

	void Drawer::flush()
	{
		if(_segmentsDrawn > 0)
		{
			LineSet set = {_color, _segmentsDrawn};
			_sets.push_back(set);
			_segmentsDrawn = 0;
		}
	}

	void Drawer::clear()
	{
		_segmentsDrawn = 0;
		_sets.clear();
		_lineSegments.clear();
	}

	void Drawer::drawSegment(Vec3 start, Vec3 end)
	{
		moveTo(start);
		lineTo(end);
	}

	void Drawer::drawCircle2D(Vec3 center, float radius, unsigned numberOfSegments)
	{
		//const unsigned numberOfSegments = 16;
		const float increment = 2.0f * (D3DX_PI) / float(numberOfSegments);

		float theta = 0.0f;
		moveTo(center + radius * Vec3(cosf(theta), sinf(theta), center.z));
		for (unsigned i = 1; i <= numberOfSegments; ++i)
		{
			lineTo(center + radius * Vec3(cosf(increment*i), sinf(increment*i), center.z));
		}
	}

	void Drawer::drawSquare2D(Vec3 center, float size)
	{
		//float halfSize = size / 2.0f;
		//moveTo(center + Vec3(halfSize, halfSize, center.z));
		//lineTo(center + Vec3(halfSize, -halfSize, center.z));
		//lineTo(center + Vec3(-halfSize, -halfSize, center.z));
		//lineTo(center + Vec3(-halfSize, halfSize, center.z));
		//lineTo(center + Vec3(halfSize, halfSize, center.z));
		drawRect2D(center, Vec2(size, size));
	}

	void Drawer::drawRect2D(Vec3 center, Vec2 size)
	{
		size /= 2;
		moveTo(Vec3(center.x - size.x, center.y - size.y, center.z));
		lineTo(Vec3(center.x + size.x, center.y - size.y, center.z));
		lineTo(Vec3(center.x + size.x, center.y + size.y, center.z));
		lineTo(Vec3(center.x - size.x, center.y + size.y, center.z));
		lineTo(Vec3(center.x - size.x, center.y - size.y, center.z));
	}
	void Drawer::drawRect3D(Vec3 center, Vec3 size)
	{
		size /= 2;
		moveTo(Vec3(center.x - size.x, center.y - size.y, center.z - size.z));
		lineTo(Vec3(center.x + size.x, center.y - size.y, center.z - size.z));
		lineTo(Vec3(center.x + size.x, center.y + size.y, center.z - size.z));
		lineTo(Vec3(center.x - size.x, center.y + size.y, center.z - size.z));
		lineTo(Vec3(center.x - size.x, center.y - size.y, center.z - size.z));
		lineTo(Vec3(center.x - size.x, center.y - size.y, center.z + size.z));
		lineTo(Vec3(center.x + size.x, center.y - size.y, center.z + size.z));
		lineTo(Vec3(center.x + size.x, center.y + size.y, center.z + size.z));
		lineTo(Vec3(center.x - size.x, center.y + size.y, center.z + size.z));
		lineTo(Vec3(center.x - size.x, center.y - size.y, center.z + size.z));
		moveTo(Vec3(center.x - size.x, center.y + size.y, center.z - size.z));
		lineTo(Vec3(center.x - size.x, center.y + size.y, center.z + size.z));
		moveTo(Vec3(center.x + size.x, center.y + size.y, center.z - size.z));
		lineTo(Vec3(center.x + size.x, center.y + size.y, center.z + size.z));
		moveTo(Vec3(center.x + size.x, center.y - size.y, center.z - size.z));
		lineTo(Vec3(center.x + size.x, center.y - size.y, center.z + size.z));
	}
	void Drawer::drawCube(Vec3 center, float size)
	{
		drawRect3D(center, Vec3(size, size, size));
	}
}