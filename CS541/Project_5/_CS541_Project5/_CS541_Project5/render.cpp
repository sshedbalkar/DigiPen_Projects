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

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.

std::vector<Triangle3D> triangleList;
Point3D intersectionPoint;
Triangle3D *temp;
bool Intersected = false;
Point3D closestIntersectPoint;
float minimumDistance;
Triangle3D* closestTriangle;

void PreprocessScene(Scene& scene)
{
	for( int i = 0 ; i < scene.objects.size() ; i++ )
	{
		for( int j = 0 ; j < scene.objects[i].polygons.size() ; j++ )
		{
			Point3D p0 = scene.objects[i].polygons[j][0].V.Hdiv();
			Point3D p1 = scene.objects[i].polygons[j][1].V.Hdiv();
			Point3D p2 = scene.objects[i].polygons[j][2].V.Hdiv();
			Point3D p3 = scene.objects[i].polygons[j][3].V.Hdiv();

			Triangle3D t1(p0,p1,p2);
			Triangle3D t2(p0,p2,p3);

			t1.m_diffuse = scene.objects[i].Kd;
			t1.m_specular = scene.objects[i].Ks;
			t1.m_normal = scene.objects[i].polygons[j][0].N;
			t2.m_diffuse = scene.objects[i].Kd;
			t2.m_specular = scene.objects[i].Ks;
			t2.m_normal = scene.objects[i].polygons[j][0].N;

			triangleList.push_back(t1);
			triangleList.push_back(t2);
		}
	}
	// @TODO: Create your KDTree here;
}


void DestroyScene(Scene& scene) 
{
	// @TODO: Destroy your KDTree here;
}


////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be drawn.
void DrawScene(Scene& scene, int width, int height)
{
	// Choose OpenGL or student rendering here.

	if (scene.UseOpenGLRendering)  {
		glEnable(GL_DEPTH_TEST);
		DrawSceneWithOpenGL(scene, width, height);
		glDisable(GL_DEPTH_TEST);
		DrawMessage("Rendered by OpenGL (toggle with 'g')");
	}

	// Student rendering code goes here
	else {
		// BSPTree/KDTree projects must disable depth test, other projects
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

		// @TODO: You may find the worldspace position of the eye useful
		Point3D Eye = scene.viewing.BackTransform(Vector4D(0,0,0,1)).Hdiv();

		// @TODO: Generate rays here.  For each ray:
		glBegin(GL_POINTS);

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

				float dist = 0.0f;
				Intersected = false;
				for( int k = 0 ; k < triangleList.size() ; k++ )
				{
					if(Intersects(ray, triangleList[k], &intersectionPoint) )
					{
						float Distance = (intersectionPoint - Eye).lengthSquared();
						if(!Intersected )
						{
							Intersected = true;
							minimumDistance = Distance;
							closestIntersectPoint = intersectionPoint;
							closestTriangle = &(triangleList[k]);
						}
						else if( Distance < minimumDistance )
						{
							minimumDistance = Distance;
							closestIntersectPoint = intersectionPoint;
							closestTriangle = &(triangleList[k]);
						}
					}
				}

				if(Intersected)
				{
					glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,    closestTriangle->m_diffuse.rgba);
					glNormal3fv( closestTriangle->m_normal.begin() );
					glVertex3fv( closestIntersectPoint.begin()  );
				}
			}
		}

		glEnd();


		// @TODO:   Traverse your KDTree here, findinngt front-most intersection
		// @TODO:       Draw intersection point to screen

		// @TODO: Create a useful message
		char msg[100];
		sprintf_s(msg, "Rendered by %s;  Percentage of ray/tri intersections: %f",
				"student", 0.0);
		DrawMessage(msg);
	}
}
