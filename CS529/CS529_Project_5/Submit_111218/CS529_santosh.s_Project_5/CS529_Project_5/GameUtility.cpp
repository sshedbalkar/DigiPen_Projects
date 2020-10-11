#include "GameUtility.h"
#include "GameData.h"
//
static unsigned _depth = INT_MAX-1;
namespace Wye
{
	Vec3 coordToPixel(float x, float y)
	{
		int bs = GAMEDATA->blockSize();
		Vec2 gs = GAMEDATA->gridHalfSize();
		return Vec3((x - gs.x + 0.5f) * bs, (gs.y - 0.5f - y) * bs, 25.0f);
	}
	Vec2 pixelToCoord(const Vec3& pixelPos)
	{
		int bs = GAMEDATA->blockSize();
		Vec2 gs = GAMEDATA->gridHalfSize();
		return Vec2(floorf((pixelPos.x/bs)+gs.x-0.0f), floorf(gs.y-0.0f-(pixelPos.y/bs)));
	}
	unsigned getNextHighestDepth()
	{
		return --_depth;
	}
}