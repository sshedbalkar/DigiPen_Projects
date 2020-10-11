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

#define EPSILON 0.001

using namespace std;

// define plane equations as an array of points
Vector4D planeEqn[6] = {
						Vector4D(	1.0f,	0.0f,	0.0f,	0.9f),
						Vector4D(	-1.0f,	0.0f,	0.0f,	0.9f),
						Vector4D(	0.0f,	1.0f,	0.0f,	0.9f),
						Vector4D(	0.0f,	-1.0f,	0.0f,	0.9f),
						Vector4D(	0.0f,	0.0f,	1.0f,	0.0f),
						Vector4D(	0.0f,	0.0f,	-1.0f,	1.0f)
						};
/*
	PloyClip() is the function that takes a vector of 4D points of a poly and returns a vector of 4D points which represent the clipped poly
*/
vector<Vector4D> PolyClip(vector<Vector4D> &);

/*
	planeClip() clips a vector of 4D points with one boundary plane represnted by the co-ordinates of a point
*/
vector<Vector4D> planeClip(vector<Vector4D> &, const Vector4D &);

/*
	evaluate() evaluates a 4D point against a plane equation and returns the signed distance of the point from the plane
*/
float evaluate(const Vector4D &, const Vector4D &);

/*
	utility function to print a vector of Vector4D points
*/
void printVect(vector<Vector4D> &);
////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene(Scene& scene)
{
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
        glEnable(GL_DEPTH_TEST);

        // Turn off OpenGL's transformation machinery since we'll be
        // using the student's implementation of transformations.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Set some lighting parameters for OpenGL
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
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

        // Loop through the scene's objects
        for (unsigned int k=0;  k<scene.objects.size();  k++) {
            Object& obj = scene.objects[k];

            // Set this object's material properties
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, obj.Kd.rgba);
            glMateriali(GL_FRONT, GL_SHININESS, obj.n);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj.Ks.rgba);
        
            // Loop through the object's polygons
            for (int j=0;  j< (int)obj.polygons.size();  j++) {
                APolygon& poly = obj.polygons[j];
            
                // Set OpenGL's polygon mode
                glBegin(GL_POLYGON);
            
				// After the transformation store the 4d points of the poly in a vector to be clipepd by the clipper
				vector<Vector4D> vertices;
                // Loop through the polygon's  vertices.
                for (int i=0;  i< (int)poly.size(); i++)  {
                    // Send the normal to OpenGL (for lighting calculations).
                    glNormal3fv(poly[i].N.begin());

                    // Transform the vertex and send to OpenGL
                    Vector4D P = poly[i].V;
                    Vector4D T = scene.viewing.Transform(P);
                    T = scene.projection.Transform(T);
					vertices.push_back(T);
                    
                }

				// Call the clipper function to clip the transformed 4D points representing the poly
				vector<Vector4D> clipped = PolyClip(vertices);

				// do H-division on the clipped poly and render it
				for(int i = 0; i < clipped.size(); ++i)
				{
					Vector3D S = clipped[i].Hdiv();
                    glVertex3fv(S.begin());
				}
                // End openGL's polygon mode
                glEnd(); 

            }
        }

        // @TODO: Create a useful message
        DrawMessage("Rendered by <student>", width, height);
    }
}

/*
	PloyClip	- Function to clip a vector of points representing a poly
	poly		- a vector of 4D points representing the poly

	return		- a vector of 4D points representing clipped polygon
*/
vector<Vector4D> PolyClip(vector<Vector4D> &poly)
{
	vector<Vector4D> R;
	R = poly;
	
	// call the planeClip() function with 6 plane equations on the poly
	for(int i = 0; i < 6; ++i)
	{
		R = planeClip(R, planeEqn[i]);
	}
	//
	return R;
}

/*
	planeClip		- Function to clip an array of 4D points representing a poly with a boundary plane, implements the Sutherland-Hodgeman Algorithm
	poly			- A vector of 4D points representing a poly
	eqn				- A 4D point whose coords represent the coords of the boundary plane eqn

	return			- A vector of 4D points representing the clipped poly
*/
vector<Vector4D> planeClip(vector<Vector4D> &poly, const Vector4D &eqn)
{
	vector<Vector4D> R;
	Vector4D *P, *T, *S;
	//
	int len = poly.size();
	bool t0, t1;
	float d0, d1, t;
	//
	if(len > 0)
	{
		S = &(poly[len - 1]);
		//
		for (int i = 0;  i < len; ++i)
		{
			Vector4D I;
			T = &(poly[i]);
			//
			d0 = evaluate(eqn, *S);
			d1 = evaluate(eqn, *T);
			//
			t0 = d0 > -EPSILON;
			t1 = d1 > -EPSILON;
			if(t0 && t1)
			{
				R.push_back(*T);
			}
			else 
			{
				if(t0 || t1)
				{
					t = d0 / (d0 - d1);
					I = *T;
					I -= *S;
					I *= t;
					I += *S;
					//
					R.push_back(I);
					if(t1)
					{
						R.push_back(*T);
					}
				}
			}
			S = T;
		}
	}
	return R;
}

/*
	evaluate		- Function to evaluate a point against a plane equation
	eqn				- A 4D point representing the coords of a plane eqn
	p				- A point which needs evaluation

	return			- Signed distance of the point from the plane
*/
float evaluate(const Vector4D &eqn, const Vector4D &p)
{
	return p[0] * eqn[0] + p[1] * eqn[1] + p[2] * eqn[2] + p[3] * eqn[3];
}

/*
	printVect		- Utility function to print a vector of 4D points
	v				- vector that needs printing
*/
void printVect(vector<Vector4D> &v)
{
	for(int i = 0; i < v.size(); ++i)
	{
		cout << i << ": " << v[i] << endl;
	}
}
