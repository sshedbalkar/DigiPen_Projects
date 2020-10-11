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
void PreprocessScene(Scene& scene)
{
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
		// @TODO:   Traverse your KDTree here, findinngt front-most intersection
		// @TODO:       Draw intersection point to screen

		// @TODO: Create a useful message
		char msg[100];
		sprintf_s(msg, "Rendered by %s;  Percentage of ray/tri intersections: %f",
				"??", 0.0);
		DrawMessage(msg);
	}
}
