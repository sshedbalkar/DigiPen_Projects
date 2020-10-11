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
	Data structure to store the texture and lighting information (if any) of every pixel
*/
struct Shade
{
	float u;
	float v;
	float w;
	//
	Shade& operator=(const Shade& rhs)
	{
		u = rhs.u;
		v = rhs.v;
		w = rhs.w;
		//
		return *this;
	}
	Shade& operator+=(const Shade& rhs)
	{
		u += rhs.u;
		v += rhs.v;
		w += rhs.w;
		//
		return *this;
	}
	Shade& operator-=(const Shade& rhs)
	{
		u -= rhs.u;
		v -= rhs.v;
		w -= rhs.w;
		//
		return *this;
	}
	Shade& operator*=(const float& rhs)
	{
		u *= rhs;
		v *= rhs;
		w *= rhs;
		//
		return *this;
	}
	Shade& operator/=(const float& rhs)
	{
		u /= rhs;
		v /= rhs;
		w /= rhs;
		//
		return *this;
	}
};

/*
	Data structure to store a vertex information
*/
struct TransVertex
{
	Shade shade;	///< Color info
	Vector4D V;		///< Position info
};

/*
	Data structure for MIP map and its meta data
*/
struct Mipmap
{
	float *data;	///< Pointer to 1D array of the calculated mipmap values
	int maxLevel;	///< max levels in the mipmap
	int side;		///< length of a side/edge of Level 0 map
	int size;		///< total number of elements in the data array
	//
	Mipmap():
	data(NULL),
	maxLevel(-1),
	side(0),
	size(0)
	{
		//
	};
	~Mipmap()
	{
		delete[] data;
		data = NULL;
	};
};
//
/*
	PloyClip() is the function that takes a vector of 4D points of a poly and returns a vector of 4D points which represent the clipped poly
*/
vector<TransVertex*> PolyClip(vector<TransVertex*> &);

/*
	planeClip() clips a vector of 4D points with one boundary plane represnted by the co-ordinates of a point
*/
vector<TransVertex*> planeClip(vector<TransVertex*> &, const Vector4D &);

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
void ScanConvert(TransVertex *tv0, TransVertex *tv1, TransVertex *tv2, Object& obj);

/*
	Draws a pixel at a given screen coordinate if it is visible
*/
void SetPixel(int& x, int& y, float& z, Shade& s, Object& obj, float& eu, float& ev, float& pu, float& pv);

/*
	Creates MIP map for the given texture object
*/
Mipmap* createMipmap(Texture* tex);

/*
	Used to get the refernce to the color sequence in the MIPmap for the given level and coordinates
*/
float* getMipRef(Mipmap* mip, unsigned level, unsigned x, unsigned y);

////////////////////////////////////////////////////////////////////////
// This is called once after the scene is created but before it is
// ever drawn to allow for any pre-processing steps.
void PreprocessScene(Scene& scene)
{
	int sz1 = (int)scene.objects.size();
	Object* obj;
	Texture* tex;
	//
	/// Create MIP map for every Object and store its refernece in the Texture
	for (int k=sz1-1; k>-1; --k)
	{
		obj = &scene.objects[k];
		tex = obj->texture;
		tex->processed = createMipmap(tex);		///< use the available processed variable to store a refernce of the MIP map
	}
	
}

/*
	Function:
	Mipmap* createMipmap(Texture* tex)
	Creates MIP map object out of a given texture object (Works only with textures whose dimension is 2^n)

	Parameters:
	tex			Pointer to a Texture object whoise MIp map is to be created

	Return:
	Pointer to a newly created Mipmap object
*/
Mipmap* createMipmap(Texture* tex)
{
	Mipmap *map;
	int w1, w2, h1, h2;
	float *mip;
	int level;
	float *prevFirst, *prevLast;
	float *first;
	float *hookl, *hookr;
	float *c1, *c2, *c3, *c4;
	//
	w2 = tex->width;
	h2 = tex->height;
	//
	map = new Mipmap();
	map->size = w2 * w2 * 4 - 1;
	mip = new float[map->size];
	//
	/// Create level 0 MIP map
	for(int y = h2 - 1; y > -1; --y)
	{
		for(int x = w2 - 1; x > -1; --x)
		{
			hookr = tex->texel(x, y, 0);
			hookl = mip + (3 * ((y * w2) + x));
			//
			*(hookl+0) = *(hookr+0);
			*(hookl+1) = *(hookr+1);
			*(hookl+2) = *(hookr+2);
		}
	}
	//
	level = -1;
	first = mip;
	//
	/// Create rest of the levels of the MIP map
	while(w2 > 0)
	{
		prevFirst = first;
		prevLast = prevFirst + (3 * ((w2 * h2) - 1));
		//
		h1 = h2;
		w1 = w2;
		h2 >>= 1;
		w2 >>= 1;
		//
		++level;
		first = prevLast + 3;
		for(int y = h2 - 1; y > -1; --y)
		{
			for(int x = w2 - 1; x > -1; --x)
			{
				c1 = prevFirst + (3 * (((y + 0) * w1) + x + 0));
				c2 = prevFirst + (3 * (((y + 0) * w1) + x + 1));
				c3 = prevFirst + (3 * (((y + 1) * w1) + x + 1));
				c4 = prevFirst + (3 * (((y + 1) * w1) + x + 0));
				//
				hookl = first + (3 * ((y * w2) + x));
				//
				*(hookl+0) = (*(c1 + 0) + *(c2 + 0) + *(c3 + 0) + *(c4 + 0)) / 4;
				*(hookl+1) = (*(c1 + 1) + *(c2 + 1) + *(c3 + 1) + *(c4 + 1)) / 4;
				*(hookl+2) = (*(c1 + 2) + *(c2 + 2) + *(c3 + 2) + *(c4 + 2)) / 4;
			}
		}
	}
	//
	map->data = mip;
	map->maxLevel = level;
	map->side = tex->width;
	//
	return map;
}

/*
	Function:
	float* getMipRef(Mipmap* mip, unsigned level, unsigned x, unsigned y)
	Returns a refernce to the color sequence in the mipmap

	Parameters:
	mip			Pointer to the MIP map
	level		level of the map
	x			x coordinate of the level map
	y			y coordinate of the level map

	Return:
	Reference to the RGB color sequence
*/
float* getMipRef(Mipmap* mip, unsigned level, unsigned x, unsigned y)
{
	unsigned side = mip->side;
	unsigned side2 = side;
	side2 /= (1 << level);
	float mul = pow(0.25f, static_cast<int>(level));
	int offset = static_cast<int>((side * side) * (1 - mul) / 0.75f);
	offset += (y * side2) + x;
	return (mip->data) + (3 * offset);
}
/*
	Z buffer array
*/
float *pZBuff = NULL;

/*
	Z buffer height and width
*/
static int iZWidth = 0, iZHeight = 0;
//
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
            glEnable(GL_LIGHT0+li); 
		}

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
				vector<TransVertex*> vertices;
				int sz = poly.size();
				Vertex* vtx;
				TransVertex* tv;
                // Loop through the polygon's  vertices.
                for (int i=0;  i< sz; i++)  {
					vtx = &poly[i];
					Vector4D P = vtx->V;
					tv = new TransVertex;
					Shade* sh = &(tv->shade);
					//
                    // Send the normal to OpenGL (for lighting calculations).
                    glNormal3fv(vtx -> N.begin());
                    // Transform the vertex and send to OpenGL
                    Vector4D T = scene.viewing.Transform(P);
                    tv -> V = scene.projection.Transform(T);
					//
					float u = vtx -> u;
					float v = vtx -> v;
					float w = tv -> V[3];
					//
					sh -> w = 1 / w;
					sh -> u = u / w;
					sh -> v = v / w;
					//
					vertices.push_back(tv);
                    //@@ Store vertex T in polygon in polygon P.
                   // glVertex2f(T[0]/T[3]*width/2+width/2, T[1]/T[3]*height/2+height/2); //@@ REMOVE ME!!
                }

				// Call the clipper function to clip the transformed 4D points representing the poly
				vector<TransVertex*> clipped = PolyClip(vertices);
				vector<TransVertex*> ppoints;
				sz = clipped.size();
				float w2 = (float)(width / 2);
				float h2 = (float)(height / 2);
				Vector4D* v;
				// do H-division on the clipped poly and render it
				for(int i = 0; i < sz; ++i)
				{
					v =  &((tv = clipped[i])->V);
					Vector3D S = v->Hdiv();
                    
					// Scale the coordiantes from the H-Coordinates to the screen coordinates
					S[0] *= w2;
					S[0] += w2;
					S[1] *= h2;
					S[1] += h2;
					//
					(*v)[0] = S[0];
					(*v)[1] = S[1];
					(*v)[2] = S[2];
					ppoints.push_back(tv);
				}
				int i2point = 1;
				sz = ppoints.size() - 1;
				while(i2point < sz)
				{
					glDisable(GL_LIGHTING);
					glBegin(GL_POINTS);
					//
					// Scan convert a triangle polygon
					ScanConvert(ppoints[0], ppoints[i2point], ppoints[i2point + 1], obj);
					//glBegin(GL_TRIANGLES);
					//glVertex2f((ppoints[0])[0], (ppoints[0])[1]);
					//glVertex2f((ppoints[i2point])[0], (ppoints[i2point])[1]);
					//glVertex2f((ppoints[i2point + 1])[0], (ppoints[i2point + 1])[1]);
					//glVertex2f((ppoints[0])[0], (ppoints[0])[1]);
					glEnd(); 
					//
					++i2point;
				}
				//
				sz = clipped.size();
				for(int l = 0; l < sz; ++l)
				{
					delete clipped[l];
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
	void ScanConvert(TransVertex *tv0, TransVertex *tv1, TransVertex *tv2, Object& obj)
	Function to scan convert a given triangle polygon represented by its vertices in the screen coordinate

	Parameters:
	p0, p1, p2: Pointers to the vertices of the triangle poly in the screen coordinate
	tv0			pointer to the TransVertex object at vertex 0
	tv1			pointer to the TransVertex object at vertex 1
	tv2			pointer to the TransVertex object at vertex 2
	obj			Refernce to the Object object

	Return:
	void
*/
void ScanConvert(TransVertex *tv0, TransVertex *tv1, TransVertex *tv2, Object& obj)
{
	Vector4D *p0 = &(*tv0).V, *p1 = &(*tv1).V, *p2 = &(*tv2).V;
	//
	/// Rearrange the vertices so that y0 <= y1 <= y2
	if((*p0)[1] > (*p1)[1])
	{
		TransVertex *temp = tv0;
		tv0 = tv1;
		tv1 = temp;
		p0 = &tv0->V;
		p1 = &tv1->V;
	}
	if((*p0)[1] > (*p2)[1])
	{
		TransVertex *temp = tv0;
		tv0 = tv2;
		tv2 = temp;
		p0 = &tv0->V;
		p2 = &tv2->V;
	}
	if((*p1)[1] > (*p2)[1])
	{
		TransVertex *temp = tv1;
		tv1 = tv2;
		tv2 = temp;
		p1 = &tv1->V;
		p2 = &tv2->V;
	}
	//
	/// Color info of the vertices
	Shade *s0 = &(*tv0).shade, *s1 = &(*tv1).shade, *s2 = &(*tv2).shade;
	//
	float x0 = (*p0)[0], y0 = (*p0)[1], z0 = (*p0)[2], x1 = (*p1)[0], y1 = (*p1)[1], z1 = (*p1)[2], x2 = (*p2)[0], y2 = (*p2)[1], z2 = (*p2)[2];
	float u0 = (*s0).u, v0 = (*s0).v, w0 = (*s0).w, u1 = (*s1).u, v1 = (*s1).v, w1 = (*s1).w, u2 = (*s2).u, v2 = (*s2).v, w2 = (*s2).w;
	float dy1 = y1 - y0, dy2 = y2 - y0, dy3 = y2 - y1;
	//
	// Draw the lower half of the poly
	if(y0 != y1)
	{
		/// Calculation of slopes and constants of the X and Z line equations
		float mx01 = (x1 - x0)/dy1;
		float bx01 = (x0 * y1 - x1 * y0)/dy1;
		float mz01 = (z1 - z0)/dy1;
		float bz01 = (z0 * y1 - z1 * y0)/dy1;
		float mx02 = (x2 - x0)/dy2;
		float bx02 = (x0 * y2 - x2 * y0)/dy2;
		float mz02 = (z2 - z0)/dy2;
		float bz02 = (z0 * y2 - z2 * y0)/dy2;
		//
		/// Calculation of slopes and constants of the u, v and w line equations
		float mu01 = (u1 - u0)/dy1;
		float bu01 = (u0 * y1 - u1 * y0)/dy1;
		float mv01 = (v1 - v0)/dy1;
		float bv01 = (v0 * y1 - v1 * y0)/dy1;
		float mw01 = (w1 - w0)/dy1;
		float bw01 = (w0 * y1 - w1 * y0)/dy1;
		float mu02 = (u2 - u0)/dy2;
		float bu02 = (u0 * y2 - u2 * y0)/dy2;
		float mv02 = (v2 - v0)/dy2;
		float bv02 = (v0 * y2 - v2 * y0)/dy2;
		float mw02 = (w2 - w0)/dy2;
		float bw02 = (w0 * y2 - w2 * y0)/dy2;
		//
		int cy0 = (int)ceilf(y0);
		int cy1 = (int)ceilf(y1);
		//
		float xs = (mx01 * cy0) + bx01;
		float xl = (mx02 * cy0) + bx02;
		float zs = (mz01 * cy0) + bz01;
		float zl = (mz02 * cy0) + bz02;
		//
		float us = (mu01 * cy0) + bu01;
		float ul = (mu02 * cy0) + bu02;
		float vs = (mv01 * cy0) + bv01;
		float vl = (mv02 * cy0) + bv02;
		float ws = (mw01 * cy0) + bw01;
		float wl = (mw02 * cy0) + bw02;
		//
		float x0t, z0t, x1t, z1t, z, x1td, mpz, bpz;
		float u0t, v0t, w0t, u1t, v1t, w1t, mpu, bpu, mpv, bpv, mpw, bpw;
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
				//
				u0t = us;
				v0t = vs;
				w0t = ws;
				u1t = ul;
				v1t = vl;
				w1t = wl;
			}
			else
			{
				x0t = xl;
				z0t = zl;
				x1t = xs;
				z1t = zs;
				//
				u0t = ul;
				v0t = vl;
				w0t = wl;
				u1t = us;
				v1t = vs;
				w1t = ws;
			}
			//
			x1td = x1t - x0t;
			//
			mpz = (z1t - z0t)/x1td;
			bpz = (z0t * x1t - z1t * x0t)/x1td;
			//
			mpu = (u1t - u0t)/x1td;
			bpu = (u0t * x1t - u1t * x0t)/x1td;
			mpv = (v1t - v0t)/x1td;
			bpv = (v0t * x1t - v1t * x0t)/x1td;
			mpw = (w1t - w0t)/x1td;
			bpw = (w0t * x1t - w1t * x0t)/x1td;
			//
			cx0 = (int)ceilf(x0t);
			cx1 = (int)ceilf(x1t);
			//
			z = (mpz * cx0) + bpz;
			//
			Shade sh;
			sh.u = (mpu * cx0) + bpu;
			sh.v = (mpv * cx0) + bpv;
			sh.w = (mpw * cx0) + bpw;
			//
			// Iterate over all the X values in the scan line
			for(int x = cx0; x < cx1; ++x)
			{
				SetPixel(x, y, z, sh, obj, mu02, mv02, mpu, mpv);
				//SetPixel(x, y, z, sh, obj, (mu01+mu02)/2, (mv01+mv02)/2, mpu, mpv);
				//
				z += mpz;
				//
				sh.u += mpu;
				sh.v += mpv;
				sh.w += mpw;
			}
			//
			xs += mx01;
			xl += mx02;
			zs += mz01;
			zl += mz02;
			//
			us += mu01;
			ul += mu02;
			vs += mv01;
			vl += mv02;
			ws += mw01;
			wl += mw02;
		}
	}
	//
	// Draw the upper half of the poly
	if(y1 != y2)
	{
		/// Calculation of slopes and constants of the X and Z line equations
		float mx12 = (x2 - x1)/dy3;
		float bx12 = (x1 * y2 - x2 * y1)/dy3;
		float mz12 = (z2 - z1)/dy3;
		float bz12 = (z1 * y2 - z2 * y1)/dy3;
		float mx02 = (x2 - x0)/dy2;
		float bx02 = (x0 * y2 - x2 * y0)/dy2;
		float mz02 = (z2 - z0)/dy2;
		float bz02 = (z0 * y2 - z2 * y0)/dy2;
		//
		/// Calculation of slopes and constants of the u, v and w line equations
		float mu12 = (u2 - u1)/dy3;
		float bu12 = (u1 * y2 - u2 * y1)/dy3;
		float mv12 = (v2 - v1)/dy3;
		float bv12 = (v1 * y2 - v2 * y1)/dy3;
		float mw12 = (w2 - w1)/dy3;
		float bw12 = (w1 * y2 - w2 * y1)/dy3;
		float mu02 = (u2 - u0)/dy2;
		float bu02 = (u0 * y2 - u2 * y0)/dy2;
		float mv02 = (v2 - v0)/dy2;
		float bv02 = (v0 * y2 - v2 * y0)/dy2;
		float mw02 = (w2 - w0)/dy2;
		float bw02 = (w0 * y2 - w2 * y0)/dy2;
		//
		int cy1 = (int)ceilf(y1);
		int cy2 = (int)ceilf(y2);
		//
		float xs = (mx12 * cy1) + bx12;
		float xl = (mx02 * cy1) + bx02;
		float zs = (mz12 * cy1) + bz12;
		float zl = (mz02 * cy1) + bz02;
		//
		float us = (mu12 * cy1) + bu12;
		float ul = (mu02 * cy1) + bu02;
		float vs = (mv12 * cy1) + bv12;
		float vl = (mv02 * cy1) + bv02;
		float ws = (mw12 * cy1) + bw12;
		float wl = (mw02 * cy1) + bw02;
		//
		float x0t, z0t, x1t, z1t, z, x1td, mpz, bpz;
		float u0t, v0t, w0t, u1t, v1t, w1t, mpu, bpu, mpv, bpv, mpw, bpw;
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
				//
				u0t = us;
				v0t = vs;
				w0t = ws;
				u1t = ul;
				v1t = vl;
				w1t = wl;
			}
			else
			{
				x0t = xl;
				z0t = zl;
				x1t = xs;
				z1t = zs;
				//
				u0t = ul;
				v0t = vl;
				w0t = wl;
				u1t = us;
				v1t = vs;
				w1t = ws;
			}
			//
			x1td = x1t - x0t;
			//
			mpz = (z1t - z0t)/x1td;
			bpz = (z0t * x1t - z1t * x0t)/x1td;
			//
			mpu = (u1t - u0t)/x1td;
			bpu = (u0t * x1t - u1t * x0t)/x1td;
			mpv = (v1t - v0t)/x1td;
			bpv = (v0t * x1t - v1t * x0t)/x1td;
			mpw = (w1t - w0t)/x1td;
			bpw = (w0t * x1t - w1t * x0t)/x1td;
			//
			cx0 = (int)ceilf(x0t);
			cx1 = (int)ceilf(x1t);
			//
			z = (mpz * cx0) + bpz;
			//
			Shade sh;
			sh.u = (mpu * cx0) + bpu;
			sh.v = (mpv * cx0) + bpv;
			sh.w = (mpw * cx0) + bpw;
			//
			// Iterate over all the X values in the scan line
			for(int x = cx0; x < cx1; ++x)
			{
				//SetPixel(x, y, z, sh, obj, (mu12+mu02)/2, (mv12+mv02)/2, mpu, mpv);
				SetPixel(x, y, z, sh, obj, mu02, mv02, mpu, mpv);
				//
				z += mpz;
				//
				sh.u += mpu;
				sh.v += mpv;
				sh.w += mpw;
			}
			//
			xs += mx12;
			xl += mx02;
			zs += mz12;
			zl += mz02;
			//
			us += mu12;
			ul += mu02;
			vs += mv12;
			vl += mv02;
			ws += mw12;
			wl += mw02;
		}
	}
}

/// Debug color array
static float cols[][3] = 
{
	{0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f},
	{1.0f, 1.0f, 1.0f}
};

/*
	Function:
	void SetPixel(int& x, int& y, float& z, Shade& s, Object& obj, float& eu, float& ev, float& pu, float& pv)
	Performs depth test on the given (x, y, z) screen point and draws it if the test succeeds

	Params:
	x		- Reference to the x screen coordinate
	y		- Reference to the y screen coordinate
	z		- Reference to the z screen coordinate
	s		- Reference to the color info structure of that pixel
	obj		- Reference to the Obj which is trying to write to this pixel
	eu		- Reference to the scanline-to-scanline change in u value (=du/dy)
	ev		- Reference to the scanline-to-scanline change in v value (=dv/dy)
	pu		- Reference to the pixel-to-pixel change in u value (=du/dx)
	pv		- Reference to the pixel-to-pixel change in v value (=dv/dx)


	Return:
	void
*/
void SetPixel(int& x, int& y, float& z, Shade& s, Object& obj, float& eu, float& ev, float& pu, float& pv)
{
	int t1 = y * iZWidth;
	t1 += x;
	float *pz = pZBuff + t1;
	if(*pz > z)
	{
		float u = s.u / s.w;
		float v = s.v / s.w;
		//
		/// wrap u and v between (0, 1)
		u = (u < 0.0f) ? (ceilf(-u) + u) : ((u > 1.0f) ? u - floorf(u) : u);
		v = (v < 0.0f) ? (ceilf(-v) + v) : ((v > 1.0f) ? v - floorf(v) : v);
		//
		float a = (eu * pv - ev * pu) / s.w;			///< calculate area of the quad
		Texture* tex = obj.texture;
		Mipmap *mip = reinterpret_cast<Mipmap*>(tex->processed);		///< get the MIP map from the texture
		//
		/// Calculation of the level of the MIP map
		float d = log(a * tex->width * tex->height);
		d /= (2.0f * (log(2.0f)));
		d = (d < 0.0f) ? 0.0f : ((d > mip->maxLevel) ? mip->maxLevel : d);
		//
		int id = int(d);
		int wid = (mip->side) >> id;
		//
		/// Scale u and v to the Mip map dimension
		u *= wid;
		v *= wid;
		//
		/// calculate u, v values for the interpolation
		int u0 = (int)floor(u);
		int v0 = (int)floor(v);
		int u1 = u0 + 1;
		int v1 = v0 + 1;
		float uf = u - u0;
		float vf = v - v0;
		float uf2 = 1.0f - uf;
		float vf2 = 1.0f - vf;
		//
		/// Get 4 surrounding color values
		float *t00 = getMipRef(mip, id, u0, v0);
		float *t01 = getMipRef(mip, id, u0, v1);
		float *t10 = getMipRef(mip, id, u1, v0);
		float *t11 = getMipRef(mip, id, u1, v1);
		//
		/// Perform bilinear interpolation of the color and store the RGB values in an array
		float pix[] = 
		{
			(uf2 * vf2 * *(t00+0)) + (uf * vf2 * *(t10+0)) + (uf2 * vf * *(t01+0)) + (uf * vf * *(t11+0)), 
			(uf2 * vf2 * *(t00+1)) + (uf * vf2 * *(t10+1)) + (uf2 * vf * *(t01+1)) + (uf * vf * *(t11+1)), 
			(uf2 * vf2 * *(t00+2)) + (uf * vf2 * *(t10+2)) + (uf2 * vf * *(t01+2)) + (uf * vf * *(t11+2))
		};
		//
		/// Set the Color
		//glColor3fv(*(cols + id));			//debug color
		glColor3fv(pix);					// Set color
		glVertex2f((GLfloat)x, (GLfloat)y);	// draw pixel
		//
		*pz = z;	///< update the depth value of the pixel
	}
}

/*
	PloyClip	- Function to clip a vector of points representing a poly
	poly		- a vector of TransVertex pointers representing the poly

	return		- a vector of TransVertex pointers representing clipped polygon
*/
vector<TransVertex*> PolyClip(vector<TransVertex*> &poly)
{
	vector<TransVertex*> R;
	R = poly;
	//
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
	poly			- A vector of TransVertex pointers representing a poly
	eqn				- A 4D point whose coords represent the coords of the boundary plane eqn

	return			- A vector of TransVertex pointers representing the clipped poly
*/
vector<TransVertex*> planeClip(vector<TransVertex*> &poly, const Vector4D &eqn)
{
	vector<TransVertex*> R;
	vector<TransVertex*> del;
	TransVertex *T, *S;
	//
	int len = poly.size();
	bool t0, t1;
	float d0, d1, t;
	//
	if(len > 0)
	{
		S = poly[len - 1];
		//
		for (int i = 0;  i < len; ++i)
		{
			T = poly[i];
			//
			d0 = evaluate(eqn, S -> V);
			d1 = evaluate(eqn, T -> V);
			//
			t0 = d0 > -EPSILON;
			t1 = d1 > -EPSILON;
			if(t0 && t1)
			{
				R.push_back(T);
			}
			else 
			{
				if(t0 || t1)
				{
					TransVertex* I = new TransVertex;
					Vector4D* v = &I->V;
					Shade* s1 = &I->shade;
					//
					t = d0 / (d0 - d1);
					//
					*v = T->V;
					*v -= S->V;
					*v *= t;
					*v += S->V;
					//
					*s1 = (*T).shade;
					*s1 -= (*S).shade;
					*s1 *= t;
					*s1 += (*S).shade;
					//
					R.push_back(I);
					if(t1)
					{
						R.push_back(T);
					}
					else
					{
						del.push_back(T);
					}
				}
				else
				{
					del.push_back(T);
				}
			}
			S = T;
		}
	}
	len = del.size();
	for(int i = len - 1; i > -1; --i)
	{
		delete del[i];
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
	for(unsigned int i = 0; i < v.size(); ++i)
	{
		cout << i << ": " << v[i] << endl;
	}
}