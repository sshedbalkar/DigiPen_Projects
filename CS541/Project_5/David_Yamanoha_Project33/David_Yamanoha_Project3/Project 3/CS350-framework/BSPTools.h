#pragma once

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"
#include <list>

struct BSPNode
{
	BSPNode(APolygon* data, BSPNode* left, BSPNode* right, int lvl);
	int			level;
	APolygon*	pData;
	BSPNode*	pLeft;
	BSPNode*	pRight;
};

class BSPTree
{
public:
				BSPTree();
	BSPNode*	BuildBSPTree(std::list<APolygon*>& pList, int level = 0);

	void		GetNumNodes(int* numLeft, int* numRight) const;
	void		GetNumSplits(int* numSplits) const;
	void		GetDepth(int* depth) const;

private:
	int			m_depth;
	int			m_numNodesLeft;
	int			m_numNodesRight;
	int			m_numSplits;

	APolygon*	SelectAndRemove(std::list<APolygon*>& pList);
	bool		Front(APolygon* split, APolygon* front);
	bool		Back(APolygon* split, APolygon* back);
	bool		Split(APolygon* split, APolygon* poly, std::list<APolygon*>& left, std::list<APolygon*>& right);
};