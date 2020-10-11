#include "Shape.h"
#include "DebugDraw.h"
#include "DebugTools.h"
#include "Utility.h"
//
namespace Wye
{
	void ShapeCircle::draw()
	{
		//std::cout<<"Circle: Drawing\n";
		Vec3 pos = body->Position;
		pos.z = 0.0f;
		Drawer::Instance.drawCircle2D(pos, Radius, 30);
	}

	bool ShapeCircle::testPoint(const Vec3& point)
	{
		Vec3 delta = body->Position - point;
		float dis = normalize(delta);
		if(dis < Radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ShapeCircle::serialize(const ISerializer& stream)
	{
		stream.readProperty("radius", Radius);
	}

	void ShapeCircle::size(const Vec3& sc)
	{
		Radius = sc.x;
	}

	void ShapeAAB::draw()
	{
		Vec3 pos = body->Position;
		pos.z = 0.0f;
		Drawer::Instance.drawRect2D(pos, Vec2(Extents.x*2, Extents.y*2));
	}

	bool ShapeAAB::testPoint(const Vec3& point)
	{
		Vec3 delta = body->Position - point;
		if(fabs(delta.x) < Extents.x)
		{
			if(fabs(delta.y) < Extents.y)
			{
				//return true;
				if(fabs(delta.z) < Extents.z)
				{
					return true;
				}
			}
		}
		return false;
	}

	void ShapeAAB::serialize(const ISerializer& stream)
	{
		stream.readProperty("extents", Extents);
	}

	void ShapeAAB::size(const Vec3& sc)
	{
		Extents = sc;
		std::cout<<"Extents: "<<Extents<<"\n";
	}
}