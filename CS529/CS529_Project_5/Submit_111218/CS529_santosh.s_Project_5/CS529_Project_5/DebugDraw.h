/////////////////////////////////////////////////////////////////////////////////////////
///
///	\file DebugDraw.h
///	Defines the debug drawer.
///
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H
//#pragma once //Makes sure this header is only included once
//
#include "VMath.h"
//
namespace Wye
{
	///Global object that provides debug drawing functionality. This enables any system
	///to draw to the screen in a immediate mode method without a dependency on
	///graphics or having to be called in the render loop.
	class Drawer
	{
	public:
		///Set the color of the lines being drawn.
		void setColor(Vec4 color);
		///Move the draw position.
		void moveTo(Vec3 newPos);
		///Draw a line between the current position and the provided position.
		void lineTo(Vec3 newPos);
		///Draw a circle using line segments.
		void drawCircle2D(Vec3 center, float radius, unsigned numberOfSegments = 16);
		///Draw an axis aligned box.
		void drawSquare2D(Vec3 center, float size);
		void drawCube(Vec3 center, float size);
		///Draw an axis aligned box.
		void drawRect2D(Vec3 center, Vec2 size);
		void drawRect3D(Vec3 center, Vec3 size);
		///Draw a line between two points.
		void drawSegment(Vec3 start, Vec3 end);
		static Drawer Instance;
	private:
		friend class Graphics;
		void flush();
		struct LineSet
		{
			Vec4 Color;
			unsigned Segments;
		};

		struct LineSegment
		{
			Vec3 A;
			Vec3 B;
		};

		Vec3 _writePosition;
		Vec4 _color;
		unsigned _segmentsDrawn;
		std::vector<LineSet> _sets;
		std::vector<LineSegment> _lineSegments;
		void clear();
	};
}
#endif