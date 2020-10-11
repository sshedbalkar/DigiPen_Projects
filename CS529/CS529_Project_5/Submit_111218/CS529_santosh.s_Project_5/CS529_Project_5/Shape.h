////////////////////////////////////////////////////////////////
//
//	Shape.h
//	Defines Circle and AAB shapes
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef SHAPE_H
#define SHAPE_H
//
#include "Body.h"
#include "Serialization.h"
//
namespace Wye
{
	///Base Shape class
	class Shape
	{
#pragma message("Including Class: Shape")
	public:
		enum ShapeId
		{
			Sid_Circle,
			Sid_Box,
			SidNumberOfShapes
		};
		ShapeId Id;
		Body* body;
		Shape(ShapeId pid ): Id(pid) {};
		virtual void serialize(const ISerializer& stream) = 0;
		virtual void draw()=0;
		virtual bool testPoint(const Vec3&)=0;
		virtual void size(const Vec3&)=0;
	};

	///Circle shape.
	class ShapeCircle: public Shape
	{
	public:
		ShapeCircle(): Shape(Sid_Circle){};
		float Radius;
		virtual void draw();
		virtual bool testPoint(const Vec3&);
		void serialize(const ISerializer& stream);
		void size(const Vec3& sc);
	};

	///Axis Aligned Box Shape
	class ShapeAAB: public Shape
	{
	public:
		ShapeAAB(): Shape(Sid_Box){};
		Vec3 Extents;
		virtual void draw();
		virtual bool testPoint(const Vec3&);
		void serialize(const ISerializer& stream);
		void size(const Vec3& sc);
	};
}
//
#endif