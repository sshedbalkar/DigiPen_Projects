#include "KDTools.h"

Box3D AABBtoBox3D(const AABB& aabb)
{
	Vector4 displacement(1000.0f, 1000.0f, 1000.0f, 0.0f);
	Box3D bb;
	Vector4 extent = ((aabb.max + displacement) - (aabb.min + displacement)) / 2.0f;
	Vector4 origin = aabb.min + extent;
	bb.origin[0] = origin.x;
	bb.origin[1] = origin.y;
	bb.origin[2] = origin.z;
	bb.extent[0] = extent.x;
	bb.extent[1] = extent.y;
	bb.extent[2] = extent.z;
	return bb;
}

KDNode::KDNode(Box3D box, std::list<Triangle3D*> primitives, KDNode* left, KDNode* right) : m_box(box), m_primitives(primitives), m_left(left), m_right(right)
{
}

KDNode*	KDTree::BuildKDTree(std::list<Triangle3D*>* pList, KDNode* node)
{
	if(node == NULL)
	{
		Box3D bb = AABBtoBox3D(BuildBB(*pList));
		m_tree = new KDNode(bb, *pList, NULL, NULL);
		node = m_tree;
	}

	if(node->m_primitives.size() < 15)
		return NULL;
	
	Box3D boxL, boxR;
	Plane3D splitPlane;
	std::list<Triangle3D*> leftPrims;
	std::list<Triangle3D*> rightPrims;

	float wher = 0.0f;
	float minWher = wher;
	int minPrims = 100000000;
	while(wher < 1.0f)
	{
		wher += 0.1f;
		SplitBox(node->m_primitives, splitPlane, boxL, boxR, wher);
		ClassifyPrimitives(splitPlane, node->m_primitives, leftPrims, rightPrims);
		int nPrims = abs((int)leftPrims.size() - (int)rightPrims.size());
		if(minPrims > nPrims)
		{
			minPrims = nPrims;
			minWher = wher;
		}

		leftPrims.clear();
		rightPrims.clear();
	}
	
	SplitBox(node->m_primitives, splitPlane, boxL, boxR, minWher);
	ClassifyPrimitives(splitPlane, node->m_primitives, leftPrims, rightPrims);
	

	node->m_right = new KDNode(AABBtoBox3D(BuildBB(rightPrims)), rightPrims, NULL, NULL);
	node->m_left = new KDNode(AABBtoBox3D(BuildBB(leftPrims)), leftPrims, NULL, NULL);

	BuildKDTree(NULL, node->m_right);
	BuildKDTree(NULL, node->m_left);

 	return NULL;
}

Triangle3D* KDTree::Intersect(const Vector4& eye, const Vector4& ray, KDNode* node, float* nearestTime)
{	
	if(!node)
		node = m_tree;

	if(!node->m_left || !node->m_right)
	{
		std::list<Triangle3D*>::iterator b = node->m_primitives.begin();
		std::list<Triangle3D*>::iterator e = node->m_primitives.end();

		Vector4 cast = eye + ray * 10.0f;
		Segment3D s(Point3D(eye.x, eye.y, eye.z), Point3D(cast.x, cast.y, cast.z));
		Vector3D rPoint;
		while(b != e)
		{
			float time;
			if( Intersects(s, *(*b), &rPoint, &time) )
			{
				if( *nearestTime > time )
				{
					*nearestTime = time;
					return *b;
				}
			}
			b++;
		}
		return NULL;
	}
	
	Triangle3D* nearestTri = NULL;
	Triangle3D* tri = NULL;
	if(Intersects(Ray3D(Vector3D(eye.x, eye.y, eye.z), Vector3D(ray.x, ray.y, ray.z)), node->m_box))
	{
		if(node->m_left)
			tri = Intersect(eye, ray, node->m_left, nearestTime);
		if(tri)
			nearestTri = tri;
		if(node->m_right)
			tri = Intersect(eye, ray, node->m_right, nearestTime);
		if(tri)
			nearestTri = tri;

		return nearestTri;
	}
	else
		return NULL;
	
}

void KDTree::SplitBox(std::list<Triangle3D*> triList, Plane3D& plane, Box3D boxL, Box3D boxR, float wher)
{

	AABB aabb = BuildBB(triList);
	Vector3D mins, maxes;
	mins[0] = aabb.min.x;
	mins[1] = aabb.min.y;
	mins[2] = aabb.min.z;
	maxes[0] = aabb.max.x;
	maxes[1] = aabb.max.y;
	maxes[2] = aabb.max.z;

	int maxAxis = 0;
	Vector3D axisLens = maxes - mins;
	for(unsigned ma = 1; ma < 3; ma++)
	{
		if(axisLens[ma] > axisLens[maxAxis])
			maxAxis = ma;
	}

	Vector3D normal;
	normal[maxAxis] = 1.0f;

	Vector3D pointOnPlane = mins + normal  * (axisLens[maxAxis] * wher);
	
	float d = -Dot(pointOnPlane, normal);

	plane[0] = normal[0];
	plane[1] = normal[1];
	plane[2] = normal[2];
	plane[3] = d;

	Vector3D boxLmin = mins;
	Vector3D boxLmax = maxes;
	boxLmax[maxAxis] = pointOnPlane[maxAxis];
	Vector3D boxRmin = mins;
	boxRmin[maxAxis] = pointOnPlane[maxAxis];
	Vector3D boxRmax = maxes;

	AABB aabbBoxL;
	aabbBoxL.min.Set(boxLmin[0], boxLmin[1], boxLmin[2], 1.0f);
	aabbBoxL.max.Set(boxLmax[0], boxLmax[1], boxLmax[2], 1.0f);
	AABB aabbBoxR;
	aabbBoxR.min.Set(boxRmin[0], boxRmin[1], boxRmin[2], 1.0f);
	aabbBoxR.max.Set(boxRmax[0], boxRmax[1], boxRmax[2], 1.0f);
	boxL = AABBtoBox3D(aabbBoxL);
	boxR = AABBtoBox3D(aabbBoxR);
}

void KDTree::ClassifyPrimitives(const Plane3D& plane, const std::list<Triangle3D*>& pList, std::list<Triangle3D*>& pLeft, std::list<Triangle3D*>& pRight)
{
	std::list<Triangle3D*>::const_iterator b = pList.begin();
	std::list<Triangle3D*>::const_iterator e = pList.end();

	while(b != e)
	{
		Triangle3D t = **b;

		if(Distance(t[0], plane) > 0.0f && Distance(t[1], plane) > 0.0f && Distance(t[2], plane) > 0.0f )
			pRight.push_back(*b);
		else if(Distance(t[0], plane) < 0.0f && Distance(t[1], plane) < 0.0f && Distance(t[2], plane) < 0.0f)
			pLeft.push_back(*b);
		else
		{
			pRight.push_back(*b);
			pLeft.push_back(*b);
		}

		b++;
	};
}

AABB KDTree::BuildBB(const std::list<Triangle3D*>& triList)
{
	std::list<Triangle3D*>::const_iterator b = triList.begin();
	std::list<Triangle3D*>::const_iterator e = triList.end();

	AABB bb;
	bb.min.Set((unsigned)-1, (unsigned)-1, (unsigned)-1, 1.0f);
	bb.max.Set(-(float)(unsigned)-1, -(float)(unsigned)-1, -(float)(unsigned)-1, 1.0f);

	while(b != e)
	{	
		for(unsigned v = 0; v < 3; v++)
		{
			if(bb.min.x > (*(*b))[v][0])
				bb.min.x = (*(*b))[v][0];
			if(bb.min.y > (*(*b))[v][1])
				bb.min.y = (*(*b))[v][1];
			if(bb.min.z > (*(*b))[v][2])
				bb.min.z = (*(*b))[v][2];
			
			if(bb.max.x < (*(*b))[v][0])
				bb.max.x = (*(*b))[v][0];
			if(bb.max.y < (*(*b))[v][1])
				bb.max.y = (*(*b))[v][1];
			if(bb.max.z < (*(*b))[v][2])
				bb.max.z = (*(*b))[v][2];
			
		}
		b++;
	}	

	return bb;
}