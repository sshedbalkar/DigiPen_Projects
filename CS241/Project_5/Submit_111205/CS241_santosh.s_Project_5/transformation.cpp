///////////////////////////////////////////////////////////////////////
// $Id$
//
// Operations for manipulating transformations
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"

/*
	defines the minimum mouse displacement required before the tilt/spin/translation are applied
*/
#define THRESHOLD_DIST_SQUARE 16

Transformation::Transformation()
{
	Identity();
}

/*
	stores the present value of current onto the stack
*/
void Transformation::Push()
{
	// @TODO: matrixStack.push(current)
	matrixStack.push_back(current);
	//cout << "stack pushed: " << (int) matrixStack.size();
}

/*
	populates the current matrix with the most recent value on the stack. The present value of current is lost
*/
void Transformation::Pop()
{
	// @TODO: current <-- matrixStack.pop()
	current = matrixStack.back();
	matrixStack.pop_back();
	//cout << "stack poped: " << (int) matrixStack.size();
}

/*
	sets the current matrix to identity matrix
*/
void Transformation::Identity()
{
	// @TODO: current <-- identity-matrix
	for (int i=0;  i<4;  i++) {
		for (int j=0;  j<4;  j++) {
			current[i][j] = 0.0; }
		current[i][i] = 1.0; }
}

/*
	applies a rotation of theta radians along the X axis to the current matrix
*/
void Transformation::RotateX(const float theta)
{
	// @TODO: current <-- current*rotate-matrix
	Matrix4x4 m;
	double c = cos(theta), s = sin(theta);
	//
	m[0][0] = 1.0;	m[0][1] = 0.0;	m[0][2] = 0.0;	m[0][3] = 0.0;
	m[1][0] = 0.0;	m[1][1] = c;	m[1][2] = -s;	m[1][3] = 0.0;
	m[2][0] = 0.0;	m[2][1] = s;	m[2][2] = c;	m[2][3] = 0.0;
	m[3][0] = 0.0;	m[3][1] = 0.0;	m[3][2] = 0.0;	m[3][3] = 1.0;
	//
	current = MatrixMultiply(current, m);
}

/*
	applies a rotation of theta radians along the Y axis to the current matrix
*/
void Transformation::RotateY(const float theta)
{
	// @TODO: current <-- current*rotate-matrix
	Matrix4x4 m;
	double c = cos(theta), s = sin(theta);
	//
	m[0][0] = c;	m[0][1] = 0.0;	m[0][2] = s;	m[0][3] = 0.0;
	m[1][0] = 0.0;	m[1][1] = 1.0;	m[1][2] = 0.0;	m[1][3] = 0.0;
	m[2][0] = -s;	m[2][1] = 0;	m[2][2] = c;	m[2][3] = 0.0;
	m[3][0] = 0.0;	m[3][1] = 0.0;	m[3][2] = 0.0;	m[3][3] = 1.0;
	//
	current = MatrixMultiply(current, m);
}

/*
	applies a rotation of theta radians along the Z axis to the current matrix
*/
void Transformation::RotateZ(const float theta)
{
	// @TODO: current <-- current*rotate-matrix
	Matrix4x4 m;
	double c = cos(theta), s = sin(theta);
	//
	m[0][0] = c;	m[0][1] = -s;	m[0][2] = 0.0;	m[0][3] = 0.0;
	m[1][0] = s;	m[1][1] = c;	m[1][2] = 0.0;	m[1][3] = 0.0;
	m[2][0] = 0.0;	m[2][1] = 0.0;	m[2][2] = 1.0;	m[2][3] = 0.0;
	m[3][0] = 0.0;	m[3][1] = 0.0;	m[3][2] = 0.0;	m[3][3] = 1.0;
	//
	current = MatrixMultiply(current, m);
}

/*
	scales the current matrix along X, Y and Z axis

	sx - amount to be scaled along X axis
	sy - amount to be scaled along Y axis
	sz - amount to be scaled along Z axis
*/
void Transformation::Scale(const float sx, const float sy, const float sz)
{
	// @TODO: current <-- current*scale-matrix
	Matrix4x4 m;
	//
	m[0][0] = sx;	m[0][1] = 0.0;	m[0][2] = 0.0;	m[0][3] = 0.0;
	m[1][0] = 0.0;	m[1][1] = sy;	m[1][2] = 0.0;	m[1][3] = 0.0;
	m[2][0] = 0.0;	m[2][1] = 0.0;	m[2][2] = sz;	m[2][3] = 0.0;
	m[3][0] = 0.0;	m[3][1] = 0.0;	m[3][2] = 0.0;	m[3][3] = 1.0;
	//
	current = MatrixMultiply(current, m);
}

/*
	translates the current matrix along X, Y and Z axis

	tx - amount to be translated along X axis
	ty - amount to be translated along Y axis
	tz - amount to be translated along Z axis
*/
void Transformation::Translate(const float tx,
							   const float ty,
							   const float tz)
{
	// @TODO: current <-- current*translate-matrix
	Matrix4x4 m;
	//
	m[0][0] = 1.0;	m[0][1] = 0.0;	m[0][2] = 0.0;	m[0][3] = tx;
	m[1][0] = 0.0;	m[1][1] = 1.0;	m[1][2] = 0.0;	m[1][3] = ty;
	m[2][0] = 0.0;	m[2][1] = 0.0;	m[2][2] = 1.0;	m[2][3] = tz;
	m[3][0] = 0.0;	m[3][1] = 0.0;	m[3][2] = 0.0;	m[3][3] = 1.0;
	//
	current = MatrixMultiply(current, m);
}

/*
	calculates the perspective matrix and applies to the current matrix

	rx - half screen width to eye distance ratio
	ry - half screen height to eye distance ratio
	front - distance of the front clipping plane from the screen
	back - distance of the back clipping plane from the screen
*/
void Transformation::Perspective(const float rx,
								 const float ry,
								 const float front,
								 const float back)
{
	// @TODO: current <-- current*perspective-matrix
	Matrix4x4 m;
	m[0][0] = 1/rx;			m[0][1] = 0.0;			m[0][2] = 0.0;					m[0][3] = 0;
	m[1][0] = 0.0;			m[1][1] = 1/ry;			m[1][2] = 0.0;					m[1][3] = 0;
	m[2][0] = 0.0;			m[2][1] = 0.0;			m[2][2] = back/(back - front);	m[2][3] = -(front*back)/(back - front);
	m[3][0] = 0.0;			m[3][1] = 0.0;			m[3][2] = 1.0;					m[3][3] = 0;

	current = MatrixMultiply(current, m);
}

/*
	transforms a given 4D point using the current transformation matrix
*/
Vector4D Transformation::Transform(const Vector4D& V)
{
	// @TODO: return current*V,  not V itself!

	Vector4D v1, v2;
	v1 = current[0];
	v2[0] = v1[0] * V[0] + v1[1] * V[1] + v1[2] * V[2] + v1[3] * V[3];
	v1 = current[1];
	v2[1] = v1[0] * V[0] + v1[1] * V[1] + v1[2] * V[2] + v1[3] * V[3];
	v1 = current[2];
	v2[2] = v1[0] * V[0] + v1[1] * V[1] + v1[2] * V[2] + v1[3] * V[3];
	v1 = current[3];
	v2[3] = v1[0] * V[0] + v1[1] * V[1] + v1[2] * V[2] + v1[3] * V[3];

	return v2;
}

void Transformation::ArrayForOpenGL(float* array16)
{
	for (int i=0;  i<4;  i++)
		for (int j=0;  j<4;  j++)
			*(array16 + i+4*j) = current[i][j];
}

Vector4D Transformation::BackTransform(const Vector4D& V)
{
return Vector4D(
		inverse[0][0]*V[0] + inverse[0][1]*V[1] + inverse[0][2]*V[2] + inverse[0][3]*V[3],
		inverse[1][0]*V[0] + inverse[1][1]*V[1] + inverse[1][2]*V[2] + inverse[1][3]*V[3],
		inverse[2][0]*V[0] + inverse[2][1]*V[1] + inverse[2][2]*V[2] + inverse[2][3]*V[3],
		inverse[3][0]*V[0] + inverse[3][1]*V[1] + inverse[3][2]*V[2] + inverse[3][3]*V[3]
	);
}

////////////////////////////////////////////////////////////////////////
// Calculates the inverse matrix by performing the gaussian matrix
// reduction with partial pivoting followed by back/substitution with
// the loops manually unrolled.
//
// Taken from Mesa implementation of OpenGL:  http://mesa3d.sourceforge.net/
////////////////////////////////////////////////////////////////////////
#define MAT(m,r,c) ((*m)[r][c])
#define SWAP_ROWS(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }

int invert(Matrix4x4* mat, Matrix4x4* inv)
{
   double wtmp[4][8];
   double m0, m1, m2, m3, s;
   double *r0, *r1, *r2, *r3;

   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

   r0[0] = MAT(mat,0,0);
   r0[1] = MAT(mat,0,1);
   r0[2] = MAT(mat,0,2);
   r0[3] = MAT(mat,0,3);
   r0[4] = 1.0;
   r0[5] = r0[6] = r0[7] = 0.0;
   
   r1[0] = MAT(mat,1,0);
   r1[1] = MAT(mat,1,1);
   r1[2] = MAT(mat,1,2);
   r1[3] = MAT(mat,1,3);
   r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0;

   r2[0] = MAT(mat,2,0);
   r2[1] = MAT(mat,2,1);
   r2[2] = MAT(mat,2,2);
   r2[3] = MAT(mat,2,3);
   r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0;

   r3[0] = MAT(mat,3,0);
   r3[1] = MAT(mat,3,1);
   r3[2] = MAT(mat,3,2);
   r3[3] = MAT(mat,3,3);
   r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

   /* choose pivot - or die */
   if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
   if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
   if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
   if (0.0 == r0[0])  return 0;

   /* eliminate first variable     */
   m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
   s = r0[1]; r1[1] -= m1*s; r2[1] -= m2*s; r3[1] -= m3*s;
   s = r0[2]; r1[2] -= m1*s; r2[2] -= m2*s; r3[2] -= m3*s;
   s = r0[3]; r1[3] -= m1*s; r2[3] -= m2*s; r3[3] -= m3*s;
   s = r0[4];
   if (s != 0.0) { r1[4] -= m1*s; r2[4] -= m2*s; r3[4] -= m3*s; }
   s = r0[5];
   if (s != 0.0) { r1[5] -= m1*s; r2[5] -= m2*s; r3[5] -= m3*s; }
   s = r0[6];
   if (s != 0.0) { r1[6] -= m1*s; r2[6] -= m2*s; r3[6] -= m3*s; }
   s = r0[7];
   if (s != 0.0) { r1[7] -= m1*s; r2[7] -= m2*s; r3[7] -= m3*s; }

   /* choose pivot - or die */
   if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
   if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
   if (0.0 == r1[1])  return 0;

   /* eliminate second variable */
   m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
   r2[2] -= m2*r1[2]; r3[2] -= m3*r1[2];
   r2[3] -= m2*r1[3]; r3[3] -= m3*r1[3];
   s = r1[4]; if (0.0 != s) { r2[4] -= m2*s; r3[4] -= m3*s; }
   s = r1[5]; if (0.0 != s) { r2[5] -= m2*s; r3[5] -= m3*s; }
   s = r1[6]; if (0.0 != s) { r2[6] -= m2*s; r3[6] -= m3*s; }
   s = r1[7]; if (0.0 != s) { r2[7] -= m2*s; r3[7] -= m3*s; }

   /* choose pivot - or die */
   if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
   if (0.0 == r2[2])  return 0;

   /* eliminate third variable */
   m3 = r3[2]/r2[2];
   r3[3] -= m3*r2[3];
   r3[4] -= m3*r2[4];
   r3[5] -= m3*r2[5];
   r3[6] -= m3*r2[6];
   r3[7] -= m3*r2[7];

   /* last check */
   if (0.0 == r3[3]) return 0;

   s = 1.0F/r3[3];             /* now back substitute row 3 */
   r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

   m2 = r2[3];                 /* now back substitute row 2 */
   s  = 1.0F/r2[2];
   r2[4] = s*(r2[4] - r3[4]*m2);
   r2[5] = s*(r2[5] - r3[5]*m2);
   r2[6] = s*(r2[6] - r3[6]*m2);
   r2[7] = s*(r2[7] - r3[7]*m2);
   m1 = r1[3];
   r1[4] -= r3[4]*m1;
   r1[5] -= r3[5]*m1;
   r1[6] -= r3[6]*m1;
   r1[7] -= r3[7]*m1;
   m0 = r0[3];
   r0[4] -= r3[4]*m0;
   r0[5] -= r3[5]*m0;
   r0[6] -= r3[6]*m0;
   r0[7] -= r3[7]*m0;

   m1 = r1[2];                 /* now back substitute row 1 */
   s  = 1.0F/r1[1];
   r1[4] = s*(r1[4] - r2[4]*m1);
   r1[5] = s*(r1[5] - r2[5]*m1);
   r1[6] = s*(r1[6] - r2[6]*m1);
   r1[7] = s*(r1[7] - r2[7]*m1);
   m0 = r0[2];
   r0[4] -= r2[4]*m0;
   r0[5] -= r2[5]*m0;
   r0[6] -= r2[6]*m0;
   r0[7] -= r2[7]*m0;

   m0 = r0[1];                 /* now back substitute row 0 */
   s  = 1.0F/r0[0];
   r0[4] = s*(r0[4] - r1[4]*m0);
   r0[5] = s*(r0[5] - r1[5]*m0);
   r0[6] = s*(r0[6] - r1[6]*m0);
   r0[7] = s*(r0[7] - r1[7]*m0);

   MAT(inv,0,0) = r0[4];
   MAT(inv,0,1) = r0[5],
   MAT(inv,0,2) = r0[6];
   MAT(inv,0,3) = r0[7],
   MAT(inv,1,0) = r1[4];
   MAT(inv,1,1) = r1[5],
   MAT(inv,1,2) = r1[6];
   MAT(inv,1,3) = r1[7],
   MAT(inv,2,0) = r2[4];
   MAT(inv,2,1) = r2[5],
   MAT(inv,2,2) = r2[6];
   MAT(inv,2,3) = r2[7],
   MAT(inv,3,0) = r3[4];
   MAT(inv,3,1) = r3[5],
   MAT(inv,3,2) = r3[6];
   MAT(inv,3,3) = r3[7];

   return 1;
}


////////////////////////////////////////////////////////////////////////
// Interactive transformation manipulation procedures and attributes
////////////////////////////////////////////////////////////////////////

// @TODO: A set of variables to
//   to storeaccumulate user mouse interactions,
//   and used to build the viewing and perspective transformations.
//   NOTE: These are *my* favorites -- change as you will.

float spin = 0.0f;	     // Rotate around objects z axis
float tilt = 0.0f;	     // Rotate around the screen x (horizontal) axis
float xTranslate = 0.0;	 // Translate the object along screen x
float yTranslate = 0.0;	 // Translate the object along screen y
float zoom = 2;			 // Translate the object closer/farther
float rx = 0.2;			 // Half-width/distance ratio
float ry = 0.2;			 // Half-height/distance ratio
float front = 0.1;		 // Distance from eye to front clipping plane
float back = 10;		 // Distance from eye to baack clipping plane

int clickX;				// marks the X value where the left/right mouse is clicked
int clickY;				// marks the Y value where the left/right mouse is clicked
int startX;				// saves the current xTranslate value before any change is made due to the right mouse click
int startY;				// saves the current yTranslate value before any change is made due to the right mouse click
int startSpin;			// saves the current spin value before any change is made due to the left mouse click
int startTilt;			// saves the current tilt value before any change is made due to the left mouse click
int downLeft = 0;		// indicates whether left mouse button is down
int downRight = 0;		// indicates whether right mouse button is down
int downMid = 0;		// indicates whether mid mouse button is down

// @TODO: Manipulate the viewing parameters as the mouse is
// dragged.  Suggestions:
//    button 1 down: left-right drag --> spin
//    button 1 down: up-down drag --> tilt
//    scroll up/down:  --> zoom (out/in)
//    button 2 down:  up/down, left/right drag --> xTranslate, yTranslate

/*
	mark the point to begin tilt and spin value changes
*/
void LeftButtonDown(const int x, const int y) 
{
    //printf("LeftButtonDown(%3d, %3d)\n", x, y);
	clickX = x;
	clickY = y;
	startSpin = spin;
	startTilt = tilt;
	downLeft = 1;
}

/*
	apply spin/tilt changes based on current mouse values
*/
void LeftButtonDrag(const int x, const int y) 
{
    //printf("LeftButtonDrag(%3d, %3d)\n", x, y);
	if(downLeft)
	{
		int dx, dy;
		dx = x - clickX;
		dy = y - clickY;

		if((dx * dx + dy * dy) > THRESHOLD_DIST_SQUARE)
		{
			spin = startSpin + (dx * 0.01);
			tilt = startTilt - (dy * 0.01);
		}
	}
}

/*
	stops the spin/tilt value changes
*/
void LeftButtonUp(const int x, const int y) 
{
    //printf("LeftButtonUp(%3d, %3d)\n", x, y);
	downLeft = 0;
}


void MiddleButtonDown(const int x, const int y) 
{
    //printf("MiddleButtonDown(%3d, %3d)\n", x, y);
}

void MiddleButtonDrag(const int x, const int y) 
{
    //printf("MiddleButtonDrag(%3d, %3d)\n", x, y);
}

void MiddleButtonUp(const int x, const int y) 
{
    //printf("MiddleButtonUp(%3d, %3d)\n", x, y);
}

/*
	marks the point to begin translation value changes
*/
void RightButtonDown(const int x, const int y) 
{
    //printf("RightButtonDown(%3d, %3d)\n", x, y);
	clickX = x;
	clickY = y;
	startX = xTranslate;
	startY = yTranslate;
	downRight = 1;
}

/*
	calculates the X, Y translation to be applied using the mouse values
*/
void RightButtonDrag(const int x, const int y) 
{
    //printf("RightButtonDrag(%3d, %3d)\n", x, y);
	if(downRight)
	{
		int dx, dy;
		dx = x - clickX;
		dy = y - clickY;

		if((dx * dx + dy * dy) > THRESHOLD_DIST_SQUARE)
		{
			xTranslate = startX + (dx * 0.005);
			yTranslate = startY - (dy * 0.005);
		}
	}
}

/*
	stops the X, Y translation value changes
*/
void RightButtonUp(const int x, const int y) 
{
    //printf("RightButtonUp(%3d, %3d)\n", x, y);
	downRight = 0;
}

/*
	increases the Z (depth) value
*/
void MouseWheelUp() 
{
    //printf("MouseWheelUp()\n");
	zoom += 0.5;
}

/*
	decreases the Z (depth) value
*/
void MouseWheelDown() 
{
    //printf("MouseWheelDown()\n");
	zoom -= 0.5;
}

/*
	implements 4 X 4 matrix multiplication and returns the resultatnt matrix
*/
Matrix4x4 Transformation::MatrixMultiply(Matrix4x4 &m1, Matrix4x4 &m2)
{
	Matrix4x4 mresult;

	// @TODO:  Implement matrix multiplication here.  

	int i, j, k;

	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 4; ++j)
		{
			mresult[i][j] = 0;
			for(k = 0; k < 4; ++k)
			{
				mresult[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	return mresult;
}

////////////////////////////////////////////////////////////////////////
// Sets the scene's viewing and projection matrices AND their inverses
// according to the parameters controlled by mouse interactions.
void BuildProjection(Scene& scene, int width, int height)
{
	//cout << "Height: " << height << ", Width: " << width;
	// @TODO: Build the projection (perspective) transformation from

	float asp = (float) height/width;
	ry = rx * asp;

	scene.projection.Identity();
	scene.projection.Perspective(rx, ry, front, back);

	// @TODO: Build the viewing transformation from spin, tilt,
	// xTranslate, yTranslate and zoom

	scene.viewing.Identity();
	scene.viewing.Translate(xTranslate, yTranslate, 5);
	scene.viewing.RotateX(tilt);
	scene.viewing.RotateZ(spin);
	scene.viewing.Scale(zoom, zoom, zoom);
	// These must follow all transformation building operations.
	scene.projection.ComputeInverse();
	scene.viewing.ComputeInverse();
}
