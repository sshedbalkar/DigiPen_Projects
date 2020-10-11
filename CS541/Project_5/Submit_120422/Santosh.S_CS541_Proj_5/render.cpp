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
#include "KDTree.h"

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
KDTree kdt;
Vector3D geye;

Point3D gLight;

void DestroyScene(Scene& scene)
{

	//
}

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
	kdt.BuildKDTree(&tris);
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

		Point3D Eye = scene.viewing.BackTransform(Vector4D(0,0,0,1)).Hdiv();
		//
		// Raycasting part
		bool Intersected;
		for( int j = 0 ; j < height ; j++ )
		{
			for( int i = 0 ; i < width ; i++ )
			{
				float Sx = (2.0f/width)*(i+0.5f) - 1;
				float Sy = (2.0f/height)*(j+0.5f) - 1;

				Vector4D SxSy = Vector4D(Sx, Sy, 0.0f, 1.0f);
				Vector4D InvProjSxSy = scene.projection.BackTransform( SxSy );

				Point3D rayP = scene.viewing.BackTransform( InvProjSxSy ).Hdiv();
				Vector3D rayV = Eye - rayP;
				Ray3D ray(Eye, rayV);

				Vector4D eye = Vector4D(Eye,0.0f);
				Vector4D Ray= Vector4D(rayV,0.0f);

				float time = 1000.0f;
				Triangle3D* t = kdt.Intersect(eye, Ray*10, NULL, &time);
				if(t)
				{
					Vector3D toPoint = gLight - (*t)[0];
					toPoint.normalize();
					Vector3D color(t->k[0], t->k[1], t->k[2]);
					color *= Dot(t->N, toPoint);
					glColor3f(color[0], color[1], color[2]);
					Ray = Ray + eye;
					glVertex3f(Ray[0], Ray[1],Ray[2]);
				}
				else
				{
					//
				}
			}
		}
		glEnd();

		// @TODO: Create a useful message
		char msg[100];
		sprintf_s(msg, "Rendered by %s;  Percentage of ray/tri intersections: %f",
				"Santosh Shedbalkar", 0.0);
		DrawMessage(msg);
	}
}
