#include "BSPTools.h"

BSPNode::BSPNode(APolygon* data, BSPNode* left, BSPNode* right, int lvl) : pData(data), pLeft(left), pRight(right), level(lvl){}

BSPTree::BSPTree() : m_numNodesRight(0), m_numNodesLeft(0), m_numSplits(0), m_depth(0){}

BSPNode* BSPTree::BuildBSPTree(std::list<APolygon*>& polyList, int level)
{
	unsigned numPolys = polyList.size();
	if(!numPolys)
		return NULL;

	// Select a root polygon.
	APolygon* root = SelectAndRemove(polyList);
	std::list<APolygon*> front;
	std::list<APolygon*> back;
	// Categorize each polygon wrt root.
	std::list<APolygon*>::iterator iter = polyList.begin();
	while(iter != polyList.end())
	{
		if(Front(root, *iter))
			front.push_back(*iter);
		else if(Back(root, *iter))
			back.push_back(*iter);
		else
			Split(root, *iter, front, back);

		iter++;
	}

	if(level > m_depth)
		m_depth = level;
	
	return new BSPNode(root, BuildBSPTree(back, level + 1), BuildBSPTree(front, level + 1), level + 1);
}

void BSPTree::GetNumNodes(int* numLeft, int* numRight) const
{
	*numLeft = m_numNodesLeft;
	*numRight = m_numNodesRight;
}

void BSPTree::GetNumSplits(int* numSplits) const
{
	*numSplits = m_numSplits;
}

void BSPTree::GetDepth(int* depth) const
{
	*depth = m_depth;
}

APolygon* BSPTree::SelectAndRemove(std::list<APolygon*>& polyList)
{
	// front : 25741 splits
	// back : 144658 splits
	//APolygon* poly = polyList.front();
	//polyList.pop_front();
	//return poly;

	// Compute the average vertex.
	unsigned totalNumVerts = 0;
	unsigned nPolys = polyList.size();
	std::list<APolygon*>::iterator iter = polyList.begin();
	Vector4D average;
	average = (polyList.front())[0][0].V;
	while(iter != polyList.end())
	{
		unsigned nVerts = (*iter)->size();
		for(unsigned v = 0; v < nVerts; v++)
			average += (*(*iter))[v].V;
		totalNumVerts += nVerts;

		iter++;
	}
	average *= 1.0f/totalNumVerts;

	// Find the polygon the vertex closest to the average.
	float nearest = (float)(unsigned)-1;
	std::list<APolygon*>::iterator itNearest = polyList.begin();
	iter = polyList.begin();
	while(iter != polyList.end())
	{
		unsigned nVerts = (*iter)->size();
		for(unsigned v = 0; v < nVerts; v++)
		{
			Vector4D delta = (*(*iter))[v].V - average;
			float distance = delta.length();
			if(distance < nearest)
			{
				itNearest = iter;
				nearest = distance;
			}
		}
		iter++;
	}

	APolygon* nearestToAverage = *itNearest;
	polyList.erase(itNearest);
	return nearestToAverage;
}

bool BSPTree::Front(APolygon* split, APolygon* front)
{
	Vector3D p0((*split)[0].V[0], (*split)[0].V[1], (*split)[0].V[2]);
	Vector3D p1((*split)[1].V[0], (*split)[1].V[1], (*split)[1].V[2]);
	Vector3D p2((*split)[2].V[0], (*split)[2].V[1], (*split)[2].V[2]);

	Vector3D u = p0 - p1;
	Vector3D v = p2 - p1;
	Vector3D n = Cross(u, v);
	n.normalize();

	unsigned nVerts = front->size();
	for(unsigned vert = 0; vert < nVerts; vert++)
	{
		Vector3D cv((*front)[vert].V[0], (*front)[vert].V[1], (*front)[vert].V[2]);
		Vector3D fromPlane =  cv - p1;
		if(Dot(fromPlane, n) <= -EPSILON)
			return false;
	}
	m_numNodesRight++;
	return true;
}

bool BSPTree::Back(APolygon* split, APolygon* back)
{ 
	Vector3D p0((*split)[0].V[0], (*split)[0].V[1], (*split)[0].V[2]);
	Vector3D p1((*split)[1].V[0], (*split)[1].V[1], (*split)[1].V[2]);
	Vector3D p2((*split)[2].V[0], (*split)[2].V[1], (*split)[2].V[2]);

	Vector3D u = p0 - p1;
	Vector3D v = p2 - p1;
	Vector3D n = Cross(u, v);
	n.normalize();

	unsigned nVerts = back->size();
	for(unsigned vert = 0; vert < nVerts; vert++)
	{
		Vector3D cv((*back)[vert].V[0], (*back)[vert].V[1], (*back)[vert].V[2]);
		Vector3D fromPlane =  cv - p1;
		if(Dot(fromPlane, n) >= EPSILON)
			return false;
	}
	m_numNodesLeft++;
	return true;
}

bool BSPTree::Split(APolygon* split, APolygon* poly, std::list<APolygon*>& left, std::list<APolygon*>& right)
{
	std::vector<Vector3D> front;
	std::vector<Vector3D> back;

	Vector3D p0((*split)[0].V[0], (*split)[0].V[1], (*split)[0].V[2]);
	Vector3D p1((*split)[1].V[0], (*split)[1].V[1], (*split)[1].V[2]);
	Vector3D p2((*split)[2].V[0], (*split)[2].V[1], (*split)[2].V[2]);

	Vector3D u = p0 - p1;
	Vector3D v = p2 - p1;
	Vector3D n = Cross(u, v);
	if(n.length() < EPSILON)
	{
		left.push_back(poly);
		return false;
	}
	n.normalize();

	m_numSplits++;
	m_numNodesLeft++;
	m_numNodesRight++;
	
	Plane3D plane(n[0], n[1], n[2], -Dot(p1, n));

	unsigned nVerts = poly->size();
	for(unsigned vert = 0; vert < nVerts; vert++)
	{	
		int nextVert = (vert + 1) % nVerts;
		Vector3D cv((*poly)[vert].V[0], (*poly)[vert].V[1], (*poly)[vert].V[2]);
		Vector3D cvNext((*poly)[nextVert].V[0], (*poly)[nextVert].V[1], (*poly)[nextVert].V[2]);

		Vector3D fromSplit =  cv - p1;
		Vector3D fromSplitNext = cvNext - p1;
		
		float planeDist = Dot(fromSplit, n);
		float planeDistNext = Dot(fromSplitNext, n);

		if(planeDist >= EPSILON)
			front.push_back(cv);
		else
			back.push_back(cv);
		// Split the polygon at this point.
		Segment3D seg;
		seg.point1 = Point3D(cv[0], cv[1], cv[2]);
		seg.point2 = Point3D(cvNext[0], cvNext[1], cvNext[2]);
		Vector3D intersect;
		if(Intersects(seg, plane, &intersect))
		{	
 			front.push_back(intersect);
			back.push_back(intersect);
		}

	}

	APolygon* frontPoly = new APolygon;
	for(unsigned f = 0; f < front.size(); f++)
	{
		Vertex v;
		v.k = (*poly)[0].k;
		v.N = n;
		v.V = Vector4D(front[f], 1.0f);
		frontPoly->push_back(v);
	}
	APolygon* backPoly = new APolygon;
	for(unsigned b = 0; b < back.size(); b++)
	{
		Vertex v;
		v.k = (*poly)[0].k;
		v.N = n;
		v.V = Vector4D(back[b], 1.0f);
		backPoly->push_back(v);
	}
	
	left.push_back(frontPoly);
	right.push_back(backPoly);

	return true;
}