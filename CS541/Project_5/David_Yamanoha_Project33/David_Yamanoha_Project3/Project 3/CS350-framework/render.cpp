///////////////////////////////////////////////////////////////////////
// $Id$
//
// All student written rendering code can go in this file.
//
// Gary Herron
//
// Copyright Å© 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"
#include "BSPTools.h"
#include "KDTools.h"
#include "Camera.h"

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
BSPTree bspt;
BSPNode* bspn = NULL;
KDTree kdt;
Vector3D geye;

Point3D gLight;

void PreprocessScene(Scene& scene)
{
	gLight = scene.lights[0].position;
	// @TODO: Create your BSPNTree here;
	// Put all polygons into a list.
	std::list<APolygon*> polys;

	unsigned numObjs = (unsigned)scene.objects.size();
	for(unsigned obj = 0; obj < numObjs; obj++)
	{
		unsigned numPolys = (unsigned)scene.objects[obj].polygons.size();
		for(unsigned poly = 0; poly < numPolys; poly++)
		{
			APolygon* p = &scene.objects[obj].polygons[poly];
			for(unsigned v = 0; v < p->size(); v++)
				(*p)[v].k = scene.objects[obj].Kd;

			polys.push_back(p);
		}
	}
	//bspn = bspt.BuildBSPTree(polys);

	std::list<Triangle3D*> tris;
	std::list<APolygon*>::iterator b = polys.begin();
	std::list<APolygon*>::iterator e = polys.end();

	Triangle3D* ta;
	Triangle3D* tb;
	while(b != e)
	{	
		ta = new Triangle3D;
		tb = new Triangle3D;
		(*ta)[0] = Point3D((*(*b))[0].V[0] , (*(*b))[0].V[1] , (*(*b))[0].V[2] );
		(*ta)[1] = Point3D((*(*b))[1].V[0] , (*(*b))[1].V[1] , (*(*b))[1].V[2] );
		(*ta)[2] = Point3D((*(*b))[2].V[0] , (*(*b))[2].V[1] , (*(*b))[2].V[2] );
		(*ta).k = (*(*b))[0].k;
		(*ta).N = (*(*b))[0].N;
		(*tb)[0] = Point3D((*(*b))[2].V[0] , (*(*b))[2].V[1] , (*(*b))[2].V[2] );
		(*tb)[1] = Point3D((*(*b))[3].V[0] , (*(*b))[3].V[1] , (*(*b))[3].V[2] );
		(*tb)[2] = Point3D((*(*b))[0].V[0] , (*(*b))[0].V[1] , (*(*b))[0].V[2] );
		(*tb).k = (*(*b))[3].k;
		(*tb).N = (*(*b))[3].N;
		tris.push_back(ta);
		tris.push_back(tb);
		b++;
	}

	//float sz = 1.0f;
	//ta[0] = Point3D(sz + 0.8f, -1.0f, 0.0f);
	//ta[1] = Point3D(0.0f, sz - 1.0f, 0.0f);
	//ta[2] = Point3D(-sz, 0.0f, 0.0f);

	//tb[0] = Point3D(-sz, 0.0f, 0.0f);
	//tb[1] = Point3D(0.0f, -sz - 1.0f, 0.0f);
	//tb[2] = Point3D(sz, 0.0f, 0.0f);

	//tris.push_back(ta);
	//tris.push_back(tb);

	//ta[0] = Point3D(sz, 0.0f, -sz);
	//ta[1] = Point3D(0.0f, sz, -sz);
	//ta[2] = Point3D(-1.0f, 0.0f, -sz);

	//tb[0] = Point3D(-sz, 0.0f, -sz);
	//tb[1] = Point3D(0.0f, -sz, -sz);
	//tb[2] = Point3D(sz, 0.0f, -sz);

	//tris.push_back(ta);
	//tris.push_back(tb);
	

	kdt.BuildKDTree(&tris);
}


// @TODO: Uncomment (if you wish) and use to draw each polygon
void DrawPoly(APolygon* poly) // poly has a color, normal, and list of vertices
{	

	Vector3D p0((*poly)[0].V[0], (*poly)[0].V[1], (*poly)[0].V[2]);
	Vector3D p1((*poly)[1].V[0], (*poly)[1].V[1], (*poly)[1].V[2]);
	Vector3D p2((*poly)[2].V[0], (*poly)[2].V[1], (*poly)[2].V[2]);

	Vector3D u = p0 - p1;
	Vector3D v = p2 - p1;
	Vector3D n = Cross(v, u);
	n.normalize();

	// For the edges, set color, disable lighting, choose line mode:
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	glColor3f(0,0,0);			// Black
	glLineWidth(4);
	glBegin(GL_POLYGON);
	unsigned numVerts = poly->size();
	for(unsigned vert = 0; vert < numVerts; vert++) // Poly.N = number of vertices
	{
		glVertex3fv((GLfloat*)&(*poly)[vert].V);	// vertex of type float[3]
	}
	glEnd();

	// For poly filling: enable lighting, set normal and color
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glNormal3fv((GLfloat*)&n);	// Set the normal lighting calculations
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,(GLfloat*)&(*poly)[0].k); // float[4]
	glBegin(GL_POLYGON);
	for(unsigned vert = 0; vert < numVerts; vert++) // Poly.N = number of vertices
		glVertex3fv((GLfloat*)&(*poly)[vert].V);	// vertex of type float[3]
	glEnd();
}

void DrawBSP(BSPNode* node, Point3D* eye)
{
	if(node == NULL)
		return;

	float epsilon = 0.0001f;
	
	Vector3D p0 = Vector3D(node->pData[0][0].V[0], node->pData[0][0].V[1], node->pData[0][0].V[2]);
	Vector3D p1 = Vector3D(node->pData[0][1].V[0], node->pData[0][1].V[1], node->pData[0][1].V[2]);
	Vector3D p2 = Vector3D(node->pData[0][2].V[0], node->pData[0][2].V[1], node->pData[0][2].V[2]);
	Vector3D u(p0 - p1);
	Vector3D v(p2 - p1);
	Vector3D n = Cross(u, v);
	n.normalize();

	Vector3D vEye = Vector3D((*eye)[0], (*eye)[1], (*eye)[2]);
	Vector3D planeToEye = vEye - p0;
	if(Dot(planeToEye, n) >= epsilon)
	{
		DrawBSP(node->pLeft, eye);
		DrawPoly(node->pData);	
		DrawBSP(node->pRight, eye);	
	}
	else
	{
		DrawBSP(node->pRight, eye);
		DrawPoly(node->pData);	
		DrawBSP(node->pLeft, eye);
	}

	
}

void DrawKD(Vector3D* eye)
{
	float d = 480.0f;

	Vector4 veye((*eye)[0], (*eye)[1], (*eye)[2]);

	Camera c;
	c.SetNearPlaneDistance(2.51f);
	c.GetNearPlane()->SetWidth(d);
	c.GetNearPlane()->SetHeight(d);
	c.SetPosition(veye);
	c.SetOrientation(-veye);
	
	/***** GL DRAW *****/
	//std::list<Triangle3D>::iterator b = kdt.m_polys.begin();
	//std::list<Triangle3D>::iterator e = kdt.m_polys.end();
	//while(b != e)
	//{

	//	glBegin(GL_TRIANGLES);
	//		glVertex3f((*b)[0][0], (*b)[0][1], (*b)[0][2] );
	//		glVertex3f((*b)[1][0], (*b)[1][1], (*b)[1][2] );
	//		glVertex3f((*b)[2][0], (*b)[2][1], (*b)[2][2] );
	//	glEnd();

	//	b++;
	//}
	/***** GL DRAW *****/

	//DrawBoxes(kdt.m_tree);
	//return;

	Vector4 ur = c.GetRayCast(d, 0);
	Vector4 ul = c.GetRayCast(0, 0);
	Vector4 bl = c.GetRayCast(0, -d);
	Vector4 br = c.GetRayCast(d, -d);


	Vector4 W = (ul - bl)/d;
	Vector4 E = (ur - br)/d;

	Vector4 l = ul;
	Vector4 r = ur;
	Vector4 delta, ray;
	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	for(unsigned j = 0; j < d; j++)
	{
		printf("%f\%\n", (j * d) / (d*d) * 100.0f);

		l += W;
		r += E;
		delta = (r - l) / d;
		for(unsigned i = 0; i < d; i++)
		{
			ray = (l + delta * i) - veye;
			float time = 1000.0f;
			Triangle3D* t = kdt.Intersect(veye, ray * 10.0f, NULL, &time);
			if(t)
			{
				Vector3D toPoint = gLight - (*t)[0];
				toPoint.normalize();
				Vector3D color(t->k[0], t->k[1], t->k[2]);
				color *= Dot(t->N, toPoint);
				glColor3f(color[0], color[1], color[2]);
				ray = ray + veye;
			glVertex3f(ray.x, ray.y, ray.z);
			}
			//else
			//	glColor3f(1.0f, 1.0f, 1.0f);
			
			
		}

	}
	glEnd();
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene(Scene& scene, int width, int height)
{
	// Choose OpenGL or student rendering here.  The decision can be
	// based on useOpenGLRendering, useFastRendering, or both:

	// if (useOpenGLRendering || useFastRendering)  {
	if (scene.UseOpenGLRendering)  {
		glEnable(GL_DEPTH_TEST);
		DrawSceneWithOpenGL(scene, width, height);
		glDisable(GL_DEPTH_TEST);
		DrawMessage("Rendered by OpenGL (toggle with 'g')", width, height);
	}

	// Student rendering code goes here
	else {
		// BSPTree project must disable depth test, other projects
		// may need to enable it.
		glDisable(GL_DEPTH_TEST);

		// Communicate matrices to OpenGL
		float v[16], m[16];
		scene.viewing.ArrayForOpenGL(v);
		scene.projection.ArrayForOpenGL(m);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(v);
		
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(m);

		// Set some lighting parameters for OpenGL
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

		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

		// @TODO: Render your BSPTree here
		//DrawSceneWithOpenGL(scene, width, height);
		Point3D Eye = scene.viewing.InverseTransform(Vector4D(0,0,0,1)).Hdiv();
		//TraverseTree(...);  // @TODO: Your code calls DrawPoly for each poly
		//DrawBSP(bspn, &Eye);
		int ts = timeGetTime();
		DrawKD(&Eye);


		//unsigned numObjs = (unsigned)scene.objects.size();
		//for(unsigned obj = 0; obj < numObjs; obj++)
		//{
		//	unsigned numPolys = (unsigned)scene.objects[obj].polygons.size();
		//	for(unsigned cpoly = 0; cpoly < numPolys; cpoly++)
		//	{
		//		APolygon* poly = &(scene.objects[obj].polygons[cpoly]);
		//		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (GLfloat*)&scene.objects[obj].Kd); // float[4]
		//		DrawPoly(poly);
		//	}
		//}


		// @TODO: Create a useful message
		char msg[100];
		
		
		int dt = timeGetTime() - ts;
		sprintf(msg, "Rendered by David Yamanoha;  Rendertime: %f seconds" , (float)dt/1000.0f);
		DrawMessage(msg, width, height);
	}
}
