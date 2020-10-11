///////////////////////////////////////////////////////////////////////
// 
//
// All student written rendering code can go in this file.
//
// Gary Herron
//
// Copyright 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"
//
#define MAX_TREE_DEPTH 6
struct KDNode
{
	KDNode()
		:left(NULL),
		right(NULL),
		dim(-1)
	{
		//
	}
	//
	KDNode* left;
	KDNode* right;
	//
	float div;
	int dim;
	std::vector<APolygon*> polygons;
};
//
KDNode* treeRoot;
static Vector3D minVol(99999.9f,99999.9f,99999.9f);
static Vector3D maxVol(-99999.9f,-99999.9f,-99999.9f);
//
void calculateVolumes(Scene& scene);
bool buildKDTree(Scene& scene, Point3D& eye, Point3D& direction, Point3D& intersection, KDNode* node, VertexColor *color);
void splitVolume(KDNode* node, Vector3D min, Vector3D max, int depth);
void addPoly(APolygon* poly, KDNode* node, VertexColor color);
//
void PreprocessScene(Scene& scene)
{
	calculateVolumes(scene);
	treeRoot = new KDNode;
	splitVolume(treeRoot, minVol, maxVol, 0);
	//
	for (unsigned int k = 0; k < scene.objects.size(); k++)
	{
		Object& obj = scene.objects[k];
		for (int j = 0; j < (int)obj.polygons.size(); j++)
		{
			VertexColor c;
			c.diff = obj.Kd;
			c.spec = obj.Ks;
			c.normal = obj.polygons[j][0].N;
			addPoly(&(obj.polygons[j]), treeRoot, c);
		}
	}
}
//
void DestroyScene(Scene& scene) 
{
	//
}
//
/*
	calculateVolumes: Calculates minimum and maximum bounding volumes of the scene
*/
void calculateVolumes(Scene& scene)
{
	for (unsigned int k = 0; k < scene.objects.size(); k++)
	{
		Object& obj = scene.objects[k];
		for (int j = 0; j < (int)obj.polygons.size(); j++)
		{
			APolygon& poly = obj.polygons[j];
			for (int i = 0; i < (int)poly.size(); i++)
			{
				if (minVol[0] > poly[i].V[0])
				{
					minVol[0] = poly[i].V[0];
				}
				if (minVol[1] > poly[i].V[1])
				{
					minVol[1] = poly[i].V[1];
				}
				if (minVol[2] > poly[i].V[2])
				{
					minVol[2] = poly[i].V[2];
				}
				//
				if (maxVol[0] < poly[i].V[0])
				{
					maxVol[0] = poly[i].V[0];
				}
				if (maxVol[1] < poly[i].V[1])
				{
					maxVol[1] = poly[i].V[1];
				}
				if (maxVol[2] < poly[i].V[2])
				{
					maxVol[2] = poly[i].V[2];
				}
			}
		}
	}
}
//
/*
	splitVolume: Recursively splits a given min and max bound volume and assigns them to nodes until the max tree depth is reached
*/
void splitVolume(KDNode* node, Vector3D min, Vector3D max, int depth)
{
	if (depth > MAX_TREE_DEPTH)
	{
		return;
	}
	//
	node->dim = depth % 2;
	int dim = node->dim;
	//
	node->div = max[dim] - min[dim];
	if (depth == 0)
	{
		node->div *= 0.36f;
	}
	else if (depth == 1)
	{
		node->div *= 0.36f;
	}
	else
	{
		node->div *= 0.5f;
	}
	node->div += min[dim];
	//
	KDNode* left = new KDNode;
	KDNode* right = new KDNode;
	node->left = left;
	node->right = right;
	//
	float div = node->div;
	float temp = max[dim];
	//
	max[dim] = div;
	splitVolume(left, min, max, depth + 1);
	max[dim] = temp;
	//
	temp = min[dim];
	//
	min[dim] = div;
	splitVolume(right, min, max, depth + 1);
	min[dim] = temp;
}
//
/*
	addPoly: Adds a given polygon in the appropriate node position
*/
void addPoly(APolygon* poly, KDNode* node, VertexColor color)
{
	if (node->dim == -1)
	{
		node->polygons.push_back(poly);
		return;
	}
	bool fit = true;
	bool smaller = true;
	int dim = node->dim;
	(*poly)[0].c = color;
	//
	if ((*poly)[0].V[dim] > node->div)
	{
		smaller = false;
	}
	for (unsigned int i = 1; i < poly->size(); ++i)
	{
		if (((*poly)[i].V[dim] < node->div) && !smaller)
		{
			fit = false;
		}
		if (((*poly)[i].V[dim] > node->div) && smaller)
		{
			fit = false;
		}
	}
	//
	if (fit)
	{
		addPoly(poly, (smaller ? node->left : node->right), color);
	}
	else
	{
		node->polygons.push_back(poly);
	}
}
//
/*
	buildKDTree: Recursively builds a new KD tree from the given scene data
*/
bool buildKDTree(Scene& scene, Point3D& eye, Point3D& direction, Point3D& intersection, KDNode* node, VertexColor *color)
{
	Ray3D ray(eye, direction);
	bool found = false;
	Point3D p;
	//
	for (unsigned int j = 0; j < node->polygons.size(); ++j)
	{
		APolygon& poly = *(node->polygons[j]);
		for (int i = 0; i < (int)poly.size() - 2; i++)
		{
			Triangle3D tri(poly[0].V.ToVec3(), poly[i+1].V.ToVec3(), poly[i+2].V.ToVec3());
			if (Intersects(ray, tri, &p))
			{
				intersection = p;
				*color = poly[0].c;
				found = true;
			}
		}
	}
	//
	if (node->dim == -1)
	{
		return found;
	}
	//
	int dim = node->dim;
	bool recurse = false;
	bool smaller = eye[dim] < node->div;
	//
	if (smaller)
	{
		recurse = buildKDTree(scene, eye, direction, p, node->left, color);
	}
	else
	{
		recurse = buildKDTree( scene, eye, direction, p, node->right, color);
	}
	//
	if (!recurse)
	{
		if (smaller && (direction[dim] >= node->div))
		{
			recurse = buildKDTree(scene, eye, direction, p, node->right, color);
		}
		else if (!smaller && (direction[dim] <= node->div))
		{
			recurse = buildKDTree(scene, eye, direction, p, node->left,color);
		}
	}
	if (recurse)
	{
		intersection = p;
		found = true;
	}
	return found;
}
//
void DrawScene(Scene& scene, int width, int height)
{
	if (scene.UseOpenGLRendering)  {
		glEnable(GL_DEPTH_TEST);
		DrawSceneWithOpenGL(scene, width, height);
		glDisable(GL_DEPTH_TEST);
		DrawMessage("Rendered by OpenGL (toggle with 'g')");
	}

	else {
		glDisable(GL_DEPTH_TEST);
		//glEnable (GL_DEPTH_TEST);
		
		float v[16], m[16];
		scene.viewing.ArrayForOpenGL(v);
		scene.projection.ArrayForOpenGL(m);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(v);
		
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(m);

		glColor3f( 0.5f, 0.5f, 0.5f );
		glPointSize(2.0);
		/*
		glEnable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene.ambient.rgba);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
		for (unsigned int li=0;  li<scene.lights.size();  li++) {
			Point3D& P3 = scene.lights[li].position;
			Vector4D P4(P3[0], P3[1], P3[2], 0); // OpenGL needs a 4 vector
			glLightfv(GL_LIGHT0+li, GL_POSITION, P4.begin());
			glLightfv(GL_LIGHT0+li, GL_DIFFUSE,  scene.lights[li].color.rgba);
			glLightfv(GL_LIGHT0+li, GL_SPECULAR, scene.lights[li].color.rgba);
			glEnable(GL_LIGHT0+li); }
		*/
		int total = 0;
		float w;
		//
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
		Point3D Eye = scene.viewing.BackTransform(Vector4D(0,0,0,1)).Hdiv();
		glBegin(GL_POINTS);
		//
		for (int i = 1; i <= width; ++i)
		{
			for (int j = 1; j <= height; ++j)
			{
				Vector4D vec((float)(i)/(float)width, (float)(j)/(float)height, 0.999f, 1.0f);
				//
				vec[0] *= 2.0f;
				vec[0] -= 1.0f;
				//
				vec[1] *= 2.0f;
				vec[1] -= 1.0f;
				//
				vec = scene.projection.BackTransform(vec);
				w = vec[3];
				vec[0] /= w;
				vec[1] /= w;
				vec[2] /= w;
				vec[3] = 1.0f;
				vec = scene.viewing.BackTransform(vec);
				//
				Vector3D orig = Eye;
				Vector3D direction = vec.ToVec3(); 
				Vector3D intersect;
				VertexColor col;
				//
				if (buildKDTree(scene, orig, direction, intersect, treeRoot, &col))
				{
					glNormal3fv(col.normal.begin());
					glColor3f(col.diff[0], col.diff[1], col.diff[2]);
					glVertex3fv(intersect.begin());
					++total;
				}
			}
		}
		glEnd();
		//
		char msg[200];
		sprintf_s(msg, "Rendered by: Santosh Shedbalkar, Percentage of ray/triangle intersections: %f", (float)total / (float)(width * height));
		DrawMessage(msg);
	}
}