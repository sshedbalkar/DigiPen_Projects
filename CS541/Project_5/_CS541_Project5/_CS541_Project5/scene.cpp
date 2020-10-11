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

void CreateScene(Scene& scene)
{
	scene.Clear();

	scene.EnableLighting = true;
	scene.EnableFlatShading = true;

	scene.SetAmbient(Color(0.3f, 0.3f, 0.3f));
	scene.AddLight(Point3D(100.0f,0.0f,50.0f), Color(0.8f, 0.8f, 0.8f));
	scene.AddLight(Point3D(-100.0f,0.0f,80.0f), Color(0.5f, 0.5f, 0.5f));

	int n = 1;
	float w = n ? 2.75f/(4*n) : 0.25f;
	int count = 0;


  //int i = 0, j = 0;
  //float xpos = (i-n)/float(n) + .01f*i/10.0f + .03f*j/10.0f;
  //float ypos = (j-n)/float(n) + .02f*i/10.0f + .01f*j/10.0f;
  //float zpos =                  .03f*i/10.0f + .02f*j/10.0f;

  //float hue = (atan2f(ypos,xpos)+PI)/(2*PI);
  //float sat = max(abs(xpos), abs(ypos));
  //float rot = 10.0f*(i+j-n-n);

  //scene.modeling.Push();
  //scene.SetColor(HSVColor(hue,sat,0.9f, 0.5f),
  //  HSVColor(0.0f,0.0f,1.5f, 0.5f), 120);
  //scene.modeling.Translate(xpos, ypos, 0);
  //scene.modeling.RotateZ(rot);
  //CreateRectangularPrism(scene, w, w, 0.3f-sat/4.0f);
  //scene.modeling.Pop();
  //count++;

	for (int i=0;  i<=2*n;  i++) {
		for (int j=0;  j<=2*n;  j++) {
			float xpos = n ? (i-n)/float(n) + .01f*i/10.0f + .03f*j/10.0f : 0.0f;
			float ypos = n ? (j-n)/float(n) + .02f*i/10.0f + .01f*j/10.0f : 0.0f;
			float zpos =                      .03f*i/10.0f + .02f*j/10.0f;

			float hue = (atan2f(ypos,xpos)+PI)/(2*PI);
			float sat = max(abs(xpos), abs(ypos));
			float rot = 10.0f*(i+j-n-n);
			
			scene.modeling.Push();
			scene.SetColor(HSVColor(hue,sat,0.9f), HSVColor(0.0f,0.0f,1.5f), 120);
			scene.modeling.Translate(xpos, ypos, 0);
			scene.modeling.RotateZ(rot);
			CreateRectangularPrism(scene, w, w, 0.3f);
			scene.modeling.Pop();
			count++;
		}
	}
	printf("Count: %d\n", count*6);

}
