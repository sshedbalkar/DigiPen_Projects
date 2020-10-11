///////////////////////////////////////////////////////////////////////
// $Id$
//
// Scene definition for the Phong lighting project.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"

int sceneID = 1;

void CreateScene(Scene& scene)
{
	scene.Clear();

	scene.EnableLighting = true;
	scene.EnableFlatShading = true;

	scene.SetAmbient(Color(0.2, 0.2, 0.2));
	scene.AddLight(Point3D(100,0,50), Color(0.8, 0.8, 0.8));
	scene.AddLight(Point3D(0,100,80), Color(0.5, 0.5, 0.5));

	scene.SetColor(HSVColor(0.33, 1.0, 0.7), HSVColor(0.0, 0.0, 0.8), 30);
	CreateCone(scene, 0.5, 0.75, 8);
}
