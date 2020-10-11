///////////////////////////////////////////////////////////////////////
// $Id$
//
// Scene definition for polygon clipping project.
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

	scene.SetAmbient(Color(0.25, 0.25, 0.25));
	scene.AddLight(Point3D(1,2,3), Color(0.7, 0.7, 0.7));

 	scene.modeling.Push();
 	scene.SetColor(HSVColor(0.3, 1.0, 0.8), HSVColor(0.0, 1.0, 0.0), 0);
 	scene.modeling.Translate(-0.5,0,0);
 	CreateRectangularPrism(scene, 0.5,0.5,1);
 	scene.modeling.Pop();

 	scene.modeling.Push();
 	scene.SetColor(HSVColor(0.0, 1.0, 0.9), HSVColor(0.0, 1.0, 0.0), 0);
 	scene.modeling.Translate(0.5, 0, 0);
 	CreateCylinder(scene, 0.25, 1, 12);
 	scene.modeling.Pop();

 	//scene.modeling.Push();
 	//scene.modeling.Translate(0,0.5,-0.5);
 	//scene.SetColor(HSVColor(0.5, 1.0, 0.8), HSVColor(0.0, 1.0, 0.0), 0);
 	//CreateSphere(scene, 0.75, 12, PI, PI/2.0);
 	//scene.modeling.Pop();

	scene.modeling.Push();
	scene.modeling.Translate(0,0,-0.51);
	scene.SetColor(HSVColor(0.0, 0.0, 0.5), HSVColor(0.0, 1.0, 0.0), 0);
	CreateRect(scene, 2, 10);
	scene.modeling.Pop();
}
