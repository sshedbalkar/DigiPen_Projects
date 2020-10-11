///////////////////////////////////////////////////////////////////////
// $Id$
//
// Scene definition for the z-buffer scanline project.
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
	
	scene.EnableLighting = false;
	scene.EnableFlatShading = false;
	
	TileTexture* tile = new TileTexture(256,256);
	WoodTexture* wood = new WoodTexture(256,256);
	
	scene.SetAmbient(Color(0.4, 0.4, 0.4));
	scene.AddLight(Point3D(100,80,50), Color(0.8, 0.8, 0.8));
	scene.AddLight(Point3D(-100,-80,-50), Color(0.8, 0.8, 0.8));
	
	scene.SetTexture(tile, NULL);
	scene.modeling.Push();
	scene.modeling.Translate(-0.6, 0.0, 0);
	scene.modeling.RotateX(PI/2.0);
	scene.SetColor(HSVColor(0.083, 0.75, 0.8), HSVColor(0.0, 0.0, 0.0), 0);
	//CreateRect(scene, 1.0, 1.0);
	CreateSphere(scene, 0.5, 48, PI, -PI);
	scene.modeling.Pop();
	
	scene.SetTexture(wood, NULL);
	scene.modeling.Push();
	scene.modeling.Translate(0.6, 0.0, 0);
	scene.SetColor(HSVColor(0.125, 0.7, 0.8), HSVColor(0.0, 0.0, 0.0), 0);
	CreateRect(scene, 1.0, 1.0);
	scene.modeling.Pop();

}
