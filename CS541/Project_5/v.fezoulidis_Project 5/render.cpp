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


struct Node
{
	Node() : left(NULL),right(NULL), dim(-1) {}
	Node* left;
	Node* right;
	float div;
	int dim;
	std::vector<APolygon*> polygons;
};
static Vector3D minGlobalV(999.9f,999.9f,999.9f);
static Vector3D maxGlobalV(-999.9f,-999.9f,-999.9f);
Node* root;
void SubDivide( Node* node, Vector3D min, Vector3D max, int depth );
void InsertPolygon( APolygon* poly, Node* node ,IColor color);
bool CreateKDTree( Scene& scene, Point3D& eye, Point3D& direction, Point3D& result, Node* node, IColor *color );
void drawTestText();

void PreprocessScene(Scene& scene)
{
	for (unsigned int k=0;  k<scene.objects.size();  k++) 
	{
		Object& obj = scene.objects[k];

		for (int j=0;  j< (int)obj.polygons.size();  j++) 
		{
			APolygon& poly = obj.polygons[j];
			
			for (int i=0;  i< (int)poly.size(); i++)  
			{
				if ( minGlobalV[0] > poly[i].V[0] ) minGlobalV[0] = poly[i].V[0];
				if ( minGlobalV[1] > poly[i].V[1] ) minGlobalV[1] = poly[i].V[1];
				if ( minGlobalV[2] > poly[i].V[2] ) minGlobalV[2] = poly[i].V[2];
				
				if ( maxGlobalV[0] < poly[i].V[0] ) maxGlobalV[0] = poly[i].V[0];
				if ( maxGlobalV[1] < poly[i].V[1] ) maxGlobalV[1] = poly[i].V[1];
				if ( maxGlobalV[2] < poly[i].V[2] ) maxGlobalV[2] = poly[i].V[2];
			}
		}
	}
	root = new Node;
	SubDivide( root, minGlobalV, maxGlobalV, 0 );

	for (unsigned int k=0;  k<scene.objects.size();  k++) 
	{
		Object& obj = scene.objects[k];

		for (int j=0;  j< (int)obj.polygons.size();  j++) 
		{
			IColor c;
			c.d=obj.Kd;
			c.s=obj.Ks;
			c.normal=obj.polygons[j][0].N;
			InsertPolygon( &(obj.polygons[j]), root,c );
		}
	}
}


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
		//glEnable(GL_LIGHTING);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene.ambient.rgba);
		//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
		//for (unsigned int li=0;  li<scene.lights.size();  li++) {
		//	Point3D& P3 = scene.lights[li].position;
		//	Vector4D P4(P3[0], P3[1], P3[2], 0); // OpenGL needs a 4 vector
		//	glLightfv(GL_LIGHT0+li, GL_POSITION, P4.begin());
		//	glLightfv(GL_LIGHT0+li, GL_DIFFUSE,  scene.lights[li].color.rgba);
		//	glLightfv(GL_LIGHT0+li, GL_SPECULAR, scene.lights[li].color.rgba);
		//	glEnable(GL_LIGHT0+li); }
		int total = 0;

		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

		Point3D Eye = scene.viewing.BackTransform(Vector4D(0,0,0,1)).Hdiv();

		glBegin( GL_POINTS );
		
		for ( int i = 1; i <= width; ++i )
		{
			for ( int j = 1; j <= height; ++j )
			{
				Vector4D p( (float)(i)/(float)width, (float)(j)/(float)height, 0.999f, 1.0f );
				p[0] *= 2.0f; p[0] -= 1.0f;
				p[1] *= 2.0f; p[1] -= 1.0f;
				
				p = scene.projection.BackTransform( p );
				p[0] /= p[3]; p[1] /= p[3]; p[2] /= p[3];
				p[3] = 1.0f;
				p = scene.viewing.BackTransform( p );

				Vector3D orig = Eye;
				Vector3D dest = p.ToVec3(); 
				Vector3D intersect;
				IColor c;
				drawTestText();

				if ( CreateKDTree ( scene, orig, dest, intersect, root,&c ) )
				{
					glNormal3fv( c.normal.begin() );
					glColor3f(c.d[0],c.d[1],c.d[2]);
					//glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,    c.d.rgba);
						
					glVertex3fv( intersect.begin() );
					++total;
				}

			}
		}
		glEnd();

		char msg[200];
		sprintf_s(msg, "Percentage of ray / triangle intersections: %f", (float)total / (float)(width * height ) );
		DrawMessage(msg);
	}
}


void DestroyScene(Scene& scene) 
{

}


void SubDivide( Node* node, Vector3D min, Vector3D max, int treedepth )
{
	if ( treedepth > 6 ) return;

	node->dim = treedepth % 2;
	int dim = node->dim;

	node->div = max[dim] - min[dim];
	if ( treedepth == 0 ) node->div *= 0.36f;
	else if ( treedepth == 1 ) node->div *= 0.36f;
	else node->div *= 0.5f;
	node->div += min[dim];

	Node* left = new Node;
	Node* right = new Node;

	float div = node->div;

	node->left = left;
	node->right = right;

	float temp = max[dim];
	max[dim] = div;
	SubDivide( left, min, max, treedepth + 1 );
	max[dim] = temp;
	temp = min[dim];
	min[dim] = div;
	SubDivide( right, min, max, treedepth + 1 );
	min[dim] =temp;
}

void InsertPolygon( APolygon* poly, Node* node,IColor color )
{
	if ( node->dim == -1 ) { node->polygons.push_back( poly ); return; }
	bool fit = true;
	bool lesser = true;
	int dim = node->dim;
	(*poly)[0].c=color;
	if ( (*poly)[0].V[dim] > node->div ) lesser = false;
	for ( unsigned int i = 1; i < poly->size(); ++i )
	{
		if ( ((*poly)[i].V[dim] < node->div ) && !lesser ) fit = false;
		if ( ((*poly)[i].V[dim] > node->div ) && lesser ) fit = false;
	}
	if ( fit )
	{
		InsertPolygon( poly, lesser ? node->left : node->right ,color);
	}
	else
	{
		node->polygons.push_back( poly );
	}
}



bool CreateKDTree( Scene& scene, Point3D& eye, Point3D& direction, Point3D& result, Node* node,IColor *color )
{
	Ray3D ray( eye, direction );
	bool found = false;
	Point3D p;
	for ( unsigned int j = 0;  j < node->polygons.size();  ++j)
	{
		APolygon& poly = *(node->polygons[j]);
			
		for (int i=0;  i< (int)poly.size() - 2; i++)  
		{
			Triangle3D tri( poly[0].V.ToVec3(),poly[i+1].V.ToVec3(),poly[i+2].V.ToVec3() );
			if ( Intersects( ray, tri, &p ) )
			{
				result = p;
				*color=poly[0].c;
				found = true;
			}
		}
	}
	if ( node->dim == -1 ) return found;

	int dim = node->dim;
	bool recurse = false;
	bool lesser = eye[dim] < node->div;
	
	if ( lesser ) 
	{
		recurse = CreateKDTree( scene, eye, direction, p, node->left,color );
	}
	else
	{
		recurse = CreateKDTree( scene, eye, direction, p, node->right,color );
	}
	if ( !recurse )
	{
		if ( lesser && ( direction[dim] >= node->div ))
		{
			recurse = CreateKDTree( scene, eye, direction, p, node->right,color );
		}
		else if ( !lesser && ( direction[dim] <= node->div ))
		{
			recurse = CreateKDTree( scene, eye, direction, p, node->left,color );
		}
	}
	if ( recurse )
	{
		result = p;
		found = true;
	}
	return found;
}

void drawTestText()
{
	for (int i=0;i<1000;++i)
	{
		//cout << i <<endl;
	}
}

