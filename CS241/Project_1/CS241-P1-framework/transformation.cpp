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

Transformation::Transformation()
{
	for (int i=0;  i<4;  i++) {
		for (int j=0;  j<4;  j++) {
			current[i][j] = 0.0; }
		current[i][i] = 1.0; }
}

void Transformation::Push()
{
	// @TODO: matrixStack.push(current)
}

void Transformation::Pop()
{
	// @TODO: current <-- matrixStack.pop()
}

void Transformation::Identity()
{
	// @TODO: current <-- identity-matrix
}

void Transformation::RotateX(const float theta)
{
	// @TODO: current <-- current*rotate-matrix
}

void Transformation::RotateY(const float theta)
{
	// @TODO: current <-- current*rotate-matrix
}

void Transformation::RotateZ(const float theta)
{
	// @TODO: current <-- current*rotate-matrix
}

void Transformation::Scale(const float sx, const float sy, const float sz)
{
	// @TODO: current <-- current*scale-matrix
}

void Transformation::Translate(const float tx,
							   const float ty,
							   const float tz)
{
	// @TODO: current <-- current*translate-matrix
}

void Transformation::Perspective(const float rx,
								 const float ry,
								 const float front,
								 const float back)
{
	// @TODO: current <-- current*perspective-matrix
}

Vector4D Transformation::Transform(const Vector4D& V)
{
	// @TODO: return current*V,  not V itself!
	return V;
}

void Transformation::ArrayForOpenGL(float* array16)
{
	for (int i=0;  i<4;  i++)
		for (int j=0;  j<4;  j++)
			*(array16 + i+4*j) = current[i][j];
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
float zoom = 5;			 // Translate the object closer/farther
float rx = 0.2;			 // Half-width/distance ratio
float ry = 0.2;			 // Half-height/distance ratio
float front = .1;		 // Distance from eye to front clipping plane
float back = 10;		 // Distance from eye to baack clipping plane


// @TODO: Manipulate the viewing parameters as the mouse is
// dragged.  Suggestions:
//    button 1 down: left-right drag --> spin
//    button 1 down: up-down drag --> tilt
//    scroll up/down:  --> zoom (out/in)
//    button 2 down:  up/down, left/right drag --> xTranslate, yTranslate

void LeftButtonDown(const int x, const int y) 
{
    printf("LeftButtonDown(%3d, %3d)\n", x, y);
}

void LeftButtonDrag(const int x, const int y) 
{
    printf("LeftButtonDrag(%3d, %3d)\n", x, y);
}

void LeftButtonUp(const int x, const int y) 
{
    printf("LeftButtonUp(%3d, %3d)\n", x, y);
}


void MiddleButtonDown(const int x, const int y) 
{
    printf("MiddleButtonDown(%3d, %3d)\n", x, y);
}

void MiddleButtonDrag(const int x, const int y) 
{
    printf("MiddleButtonDrag(%3d, %3d)\n", x, y);
}

void MiddleButtonUp(const int x, const int y) 
{
    printf("MiddleButtonUp(%3d, %3d)\n", x, y);
}


void RightButtonDown(const int x, const int y) 
{
    printf("RightButtonDown(%3d, %3d)\n", x, y);
}

void RightButtonDrag(const int x, const int y) 
{
    printf("RightButtonDrag(%3d, %3d)\n", x, y);
}

void RightButtonUp(const int x, const int y) 
{
    printf("RightButtonUp(%3d, %3d)\n", x, y);
}


void MouseWheelUp() 
{
    printf("MouseWheelUp()\n");
}

void MouseWheelDown() 
{
    printf("MouseWheelDown()\n");
}

Matrix4x4 Transformation::MatrixMultiply(Matrix4x4 &m1, Matrix4x4 &m2)
{
	Matrix4x4 mresult;

	// @TODO:  Implement matrix multiplication here.  

	return mresult;
		
}

////////////////////////////////////////////////////////////////////////
// Sets the scene's viewing and projection matrices AND their inverses
// according to the parameters controlled by mouse interactions.
void BuildProjection(Scene& scene, int width, int height)
{
	// @TODO: Build the projection (perspective) transformation from
	// the width/height ratio.
	scene.projection.Identity();
	//scene.projection.Perspective(...);

	// @TODO: Build the viewing transformation from spin, tilt,
	// xTranslate, yTranslate and zoom
	scene.viewing.Identity();
	//scene.viewing.Translate(...);
	//scene.viewing.Rotate?(...);
	// ...

	// These must follow all transformation building operations.
	scene.projection.ComputeInverse();
	scene.viewing.ComputeInverse();
}
