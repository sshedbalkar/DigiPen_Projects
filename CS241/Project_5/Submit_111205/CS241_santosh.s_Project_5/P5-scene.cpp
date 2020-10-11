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
#define TO_RAD 22.0f*180.0f/7.0f

/*
	Draws Door
*/
void drawDoor(Scene& scene)
{
	scene.modeling.Push();
	//
	float size = 0.07f;
	scene.SetColor(HSVColor(0.35, 0.8, 0.2), HSVColor(0.08, 0.8, 0.4), 110);
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			scene.modeling.Translate(j * 1.1 * size, 0, 0);
			CreateRectangularPrism(scene, size,size, 0.001);
			scene.modeling.Translate(-1.1 * j * size, 0, 0);
		}
		scene.modeling.Translate(0, 1.1 * size, 0);
	}
	//
	scene.modeling.Pop();
}

/*
	Draws windows
*/
void drawWindows(Scene& scene)
{
	scene.modeling.Push();
	//
	scene.SetColor(HSVColor(0.08, 0.8, 0.4), HSVColor(0.08, 0.8, 0.1), 110);
	float size = 0.08f;
	scene.modeling.Translate(0.25, 0.4, 0);
	CreateRectangularPrism(scene, size,size, 0.001);
	scene.modeling.Translate(-2 * 0.25 + size, 0, 0);
	CreateRectangularPrism(scene, size,size, 0.0001);
	//
	scene.modeling.Pop();
}

/*
	Draws Base of the house
*/
void drawBase(Scene& scene)
{
	scene.SetColor(HSVColor(0.1, 0.8, 0.3), HSVColor(0.98, 0.8, 0.4), 110);
	scene.modeling.Push();
	//
	scene.modeling.Translate(0, -0.15, 0.2);
	scene.modeling.RotateX(-7*TO_RAD);
	CreateCylinder(scene, 0.7, 0.2, 40);
	//
	scene.modeling.Pop();
}

/*
	Draws roof of the house
*/
void drawRoof(Scene& scene)
{
	scene.SetColor(HSVColor(0.7, 0.8, 0.3), HSVColor(0.98, 0.8, 0.4), 50);
	scene.modeling.Push();
	//
	scene.modeling.RotateY(89.8*TO_RAD);
	scene.modeling.RotateX(9*TO_RAD);
	scene.modeling.Translate(-0.2, 0, -0.78);
	CreateRectangularPrism(scene, 0.8, 0.78, 0.01);
	//
	scene.modeling.Pop();
	scene.modeling.Push();
	//
	scene.modeling.RotateY(-89.8*TO_RAD);
	scene.modeling.RotateX(9*TO_RAD);
	scene.modeling.Translate(0.2, -0.02, -0.76);
	CreateRectangularPrism(scene, 0.8, 0.78, 0.01);
	//
	scene.modeling.Pop();
}

/*
	Draws the body of the house
*/
void drawBlock(Scene& scene)
{
	scene.modeling.Push();
	//
	scene.modeling.Translate(0.03, 0.3, 0.2501);
	scene.SetColor(HSVColor(0.16, 1.0, 0.5), HSVColor(0.0, 0.0, 0.2), 20);
	CreateRectangularPrism(scene, 0.8, 0.7, 0.5);
	//
	scene.modeling.Pop();
}

/*
	Create a house using its components
*/
void drawHouse(Scene& scene)
{
	scene.modeling.Push();
	//
	drawBlock(scene);
	drawDoor(scene);
	drawWindows(scene);
	drawBase(scene);
	drawRoof(scene);
	//
	scene.modeling.Pop();
}

/*
	Draws ground for the houses
*/
void drawGround(Scene& scene)
{
	scene.modeling.Push();
	//
	scene.modeling.RotateX(89.75f*TO_RAD);
	scene.modeling.Translate(1.0, -2.3, -0.22);
	scene.SetColor(HSVColor(0.36, 1.0, 0.3), HSVColor(0.0, 0.0, 0.2), 10);
	CreateRectangularPrism(scene, 5, 6, 0.1);
	//
	scene.modeling.Pop();
}

/*
	Draws 6 Houses placed in 2x3 formation
*/
void drawWorld(Scene& scene)
{
	scene.modeling.Push();
	scene.modeling.RotateX(-90*TO_RAD);
	drawGround(scene);
	float size = 1.8;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			scene.modeling.Translate(j * 1.1 * size, 0, 0);
			drawHouse(scene);
			scene.modeling.Translate(-1.1 * j * size, 0, 0);
		}
		scene.modeling.Translate(0, 0, 1.1 * size);
	}
	scene.modeling.Pop();
}

/*
	Creates a scene
*/
void CreateScene(Scene& scene)
{
	scene.Clear();
	
	scene.EnableLighting = true;
	scene.EnableFlatShading = true;

	scene.SetAmbient(Color(0.4, 0.4, 0.4));

	scene.AddLight(Point3D(100,80,50), Color(0.8, 0.8, 0.8));
	scene.AddLight(Point3D(-100,-80,-50), Color(0.8, 0.8, 0.8));
	scene.AddLight(Point3D(0,0,-50), Color(0.0, 0.0, 0.8));

	scene.modeling.Push();
	scene.modeling.Translate(-0.6, 0.0, 0);
	scene.modeling.RotateX(PI/2.0);
	scene.SetColor(HSVColor(0.083, 0.75, 0.8), HSVColor(0.0, 0.0, 0.8), 30);
	
	drawWorld(scene);
	
	scene.modeling.Pop();
}
