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

/*
	Triangle scan converter function
*/
void ScanConvert(Vector3D *p0, Vector3D *p1, Vector3D *p2);

/*
	Draws a pixel at a given screen coordinate if it is visible
*/
void SetPixel(int& x, int& y, float& z);

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene(Scene& scene)
{
}

/*
	Z buffer array
*/
float *pZBuff = NULL;

/*
	Z buffer height and width
*/
static int iZWidth = 0, iZHeight = 0;
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
        //!! Turn off Depth testing (since you'll do your own).
        glDisable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

        //@@ Allocate, or if necessary, resize, an existing Z-Buffer
        //@@ Clear Z-buffer to all 1.0's
		//
		if(iZWidth != width || iZHeight != height)
		{
			delete [] pZBuff;
			iZWidth = width;
			iZHeight = height;
			pZBuff = new float[iZWidth * iZHeight];
		}
		for(int i = (iZWidth * iZHeight) - 1; i > -1; --i)
		{
			*(pZBuff + i) = 1.0f;
		}
        // Turn off OpenGL's transformation machinery since we'll be
        // using the student's implementation of transformations.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        //!! Setup screen coordinates for pixel addressing.
        gluOrtho2D(0,width,0,height);
        glPointSize(1.0);  //@@ Change this to 1 point size of 1.0!!

        // Set some lighting parameters for OpenGL
        glEnable(GL_LIGHTING);
        //glEnable(GL_DEPTH_TEST);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene.ambient.rgba);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
        for (unsigned int li=0;  li<scene.lights.size();  li++) {
            Point3D& P3 = scene.lights[li].position;
            Vector4D P4(P3[0], P3[1], P3[2], 0); // OpenGL needs a 4 vector
            glLightfv(GL_LIGHT0+li, GL_POSITION, P4.begin());
            glLightfv(GL_LIGHT0+li, GL_DIFFUSE,  scene.lights[li].color.rgba);
            glLightfv(GL_LIGHT0+li, GL_SPECULAR, scene.lights[li].color.rgba);
            glEnable(GL_LIGHT0+li); }

        // Loop through the scene's objects
        for (unsigned int k=0;  k<scene.objects.size();  k++) {
            Object& obj = scene.objects[k];

            // Send this object's material properties to OpenGL (for lighting)
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, obj.Kd.rgba);
            glMateriali(GL_FRONT, GL_SHININESS, obj.n);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj.Ks.rgba);
        
            // Loop through the object's polygons
            for (int j=0;  j< (int)obj.polygons.size();  j++) {
                APolygon& poly = obj.polygons[j];
            
                // Set OpenGL's drawing mode
                //glBegin(GL_POINTS); //!! Pixel drawing mode; was GL_POLYGON
				
            
                // Loop through the polygon's vertices.
                //@@ Create an empty Polygon P
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
                    //@@ Store vertex T in polygon in polygon P.
                   // glVertex2f(T[0]/T[3]*width/2+width/2, T[1]/T[3]*height/2+height/2); //@@ REMOVE ME!!
                }

				// Call the clipper function to clip the transformed 4D points representing the poly
				vector<Vector4D> clipped = PolyClip(vertices);
				vector<Vector3D> ppoints;
				// do H-division on the clipped poly and render it
				for(int i = 0; i < clipped.size(); ++i)
				{
					Vector3D S = clipped[i].Hdiv();
                    
					// Scale the coordiantes from the H-Coordinates to the screen coordinates
					S[0] *= (float)(width / 2);
					S[0] += (float)(width / 2);
					S[1] *= (float)(height / 2);
					S[1] += (float)(height / 2);
					ppoints.push_back(S);
				}
				int i2point = 1, vlen = ppoints.size() - 1;
				while(i2point < vlen)
				{
					glBegin(GL_POINTS);

					// Scan convert a triangle polygon
					ScanConvert(&ppoints[0], &ppoints[i2point], &ppoints[i2point + 1]);
					//glBegin(GL_TRIANGLES);
					//glVertex2f((ppoints[0])[0], (ppoints[0])[1]);
					//glVertex2f((ppoints[i2point])[0], (ppoints[i2point])[1]);
					//glVertex2f((ppoints[i2point + 1])[0], (ppoints[i2point + 1])[1]);
					//glVertex2f((ppoints[0])[0], (ppoints[0])[1]);
					glEnd(); 
					//
					++i2point;
				}
                //@@ Clip the polygon P

                //@@ Convert clipped vertices to pixel coords (include the Hdiv)
                //@@ Split the polygon into triangles
                //@@ Scan convert each triangle, resulting in individual pixels
                //@@ Depth-test and depth-update each pixel
                //@@ Output individual pixel with glVertex2f(px,py)
            
                // End openGL's polygon mode
                //glEnd(); 

            }
        }

        // @TODO: Create a useful message
        DrawMessage("Rendered by Santosh Shedbalkar", width, height);
    }
}

/*
	Function:
	ScanConvert(Vector3D *p0, Vector3D *p1, Vector3D *p2)
	Function to scan convert a given triangle polygon represented by its vertices in the screen coordinate

	Parameters:
	p0, p1, p2: Pointers to the vertices of the triangle poly in the screen coordinate

	Return:
	void
*/
void ScanConvert(Vector3D *p0, Vector3D *p1, Vector3D *p2)
{
	// Rearrange the points so that y0 <= y1 <= y2
	if((*p0)[1] > (*p1)[1])
	{
		Vector3D *temp = p0;
		p0 = p1;
		p1 = temp;
	}
	if((*p0)[1] > (*p2)[1])
	{
		Vector3D *temp = p0;
		p0 = p2;
		p2 = temp;
	}
	if((*p1)[1] > (*p2)[1])
	{
		Vector3D *temp = p1;
		p1 = p2;
		p2 = temp;
	}
	//
	float x0 = (*p0)[0], y0 = (*p0)[1], z0 = (*p0)[2], x1 = (*p1)[0], y1 = (*p1)[1], z1 = (*p1)[2], x2 = (*p2)[0], y2 = (*p2)[1], z2 = (*p2)[2];
	float dy1 = y1 - y0, dy2 = y2 - y0, dy3 = y2 - y1;
	//
	// Draw the lower half of the poly
	if(y0 != y1)
	{
		// Calculation of slopes and constants of the X and Z line equations
		float mx01 = (x1 - x0)/dy1;
		float bx01 = (x0 * y1 - x1 * y0)/dy1;
		float mz01 = (z1 - z0)/dy1;
		float bz01 = (z0 * y1 - z1 * y0)/dy1;
		float mx02 = (x2 - x0)/dy2;
		float bx02 = (x0 * y2 - x2 * y0)/dy2;
		float mz02 = (z2 - z0)/dy2;
		float bz02 = (z0 * y2 - z2 * y0)/dy2;
		//
		int cy0 = ceilf(y0);
		int cy1 = ceilf(y1);
		float xs = (mx01 * cy0) + bx01;
		float zs = (mz01 * cy0) + bz01;
		float xl = (mx02 * cy0) + bx02;
		float zl = (mz02 * cy0) + bz02;
		//
		float mp, bp, x0t, z0t, x1t, z1t, z;
		int cx0, cx1;
		//
		// Draw Scan lines
		for(int y = cy0; y < cy1; ++y)
		{
			if(xs < xl)
			{
				x0t = xs;
				z0t = zs;
				x1t = xl;
				z1t = zl;
			}
			else
			{
				x0t = xl;
				z0t = zl;
				x1t = xs;
				z1t = zs;
			}
			//
			mp = (z1t - z0t)/(x1t - x0t);
			bp = (z0t * x1t - z1t * x0t)/(x1t - x0t);
			//
			cx0 = ceilf(x0t);
			cx1 = ceilf(x1t);
			z = (mp * cx0) + bp;
			//
			// Iterate over all the X values in the scan line
			for(int x = cx0; x < cx1; ++x)
			{
				SetPixel(x, y, z);
				z += mp;
			}
			//
			xs += mx01;
			zs += mz01;
			xl += mx02;
			zl += mz02;
		}
	}
	//
	// Draw the upper half of the poly
	if(y1 != y2)
	{
		// Calculation of slopes and constants of the X and Z line equations
		float mx12 = (x2 - x1)/dy3;
		float bx12 = (x1 * y2 - x2 * y1)/dy3;
		float mz12 = (z2 - z1)/dy3;
		float bz12 = (z1 * y2 - z2 * y1)/dy3;
		float mx02 = (x2 - x0)/dy2;
		float bx02 = (x0 * y2 - x2 * y0)/dy2;
		float mz02 = (z2 - z0)/dy2;
		float bz02 = (z0 * y2 - z2 * y0)/dy2;
		//
		int cy1 = ceilf(y1);
		int cy2 = ceilf(y2);
		float xs = (mx12 * cy1) + bx12;
		float zs = (mz12 * cy1) + bz12;
		float xl = (mx02 * cy1) + bx02;
		float zl = (mz02 * cy1) + bz02;
		//
		float mp, bp, x0t, z0t, x1t, z1t, z;
		int cx0, cx1;
		//
		// Draw Scan lines
		for(int y = cy1; y < cy2; ++y)
		{
			if(xs < xl)
			{
				x0t = xs;
				z0t = zs;
				x1t = xl;
				z1t = zl;
			}
			else
			{
				x0t = xl;
				z0t = zl;
				x1t = xs;
				z1t = zs;
			}
			//
			mp = (z1t - z0t)/(x1t - x0t);
			bp = (z0t * x1t - z1t * x0t)/(x1t - x0t);
			//
			cx0 = ceilf(x0t);
			cx1 = ceilf(x1t);
			z = (mp * cx0) + bp;
			//
			// Iterate over all the X values in the scan line
			for(int x = cx0; x < cx1; ++x)
			{
				SetPixel(x, y, z);
				z += mp;
			}
			//
			xs += mx12;
			zs += mz12;
			xl += mx02;
			zl += mz02;
		}
	}
}

/*
	Function:
	SetPixel(int& x, int& y, float& z)
	Performs depth test on the given (x, y, z) screen point and draws it if the test succeeds

	Params:
	x		- Reference to the x screen coordinate
	y		- Reference to the y screen coordinate
	z		- Reference to the z screen coordinate

	Return:
	void
*/
void SetPixel(int& x, int& y, float& z)
{
	int t1 = y * iZWidth;
	t1 += x;
	float *pz = pZBuff + t1;
	if(*pz > z)
	{
		glVertex2f(x, y);
		*pz = z;
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
