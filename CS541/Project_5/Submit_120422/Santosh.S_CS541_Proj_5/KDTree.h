#pragma once

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"
#include <list>

struct KDNode
{
	KDNode(AABB box, std::list<Triangle3D*> primitives, KDNode* left, KDNode* right);
	AABB m_box;
	std::list<Triangle3D*> m_primitives;
	KDNode* m_left;
	KDNode* m_right;
};

class KDTree
{
public:
	KDNode* BuildKDTree(std::list<Triangle3D*>* pList = NULL, KDNode* node = NULL);
	Triangle3D* Intersect(const Vector4D& eye, const Vector4D& ray, KDNode* node = NULL, float* nearestTime = NULL);
	KDNode* m_tree;
	//
private:
	void SplitBox(std::list<Triangle3D*> triList, Plane3D& plane, AABB boxL, AABB boxR, float wher);
	void ClassifyPrimitives(const Plane3D& plane, const std::list<Triangle3D*>& pList, std::list<Triangle3D*>& pLeft, std::list<Triangle3D*>& pRight);
	AABB BuildBB(const std::list<Triangle3D*>& pList);
	std::list<Triangle3D*> m_triList;

};