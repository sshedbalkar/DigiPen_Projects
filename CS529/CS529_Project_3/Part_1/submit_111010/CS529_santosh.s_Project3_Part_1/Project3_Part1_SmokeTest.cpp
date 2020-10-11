/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name		: Project3_Part1_SmokeTest.cpp
Purpose			: Tests library functions
Language		: C++/MS VS 2010 
Platform		: Microsoft Visual Studio 2010/i7 2630QM, 8GB DDR3 1600MHz, 3GB NVIDIA GT 555M/Windows 7 Home Premium
Project			: CS529_SantoshShedbalkar_Project3_1
Author			: Santosh Shedbalkar/santosh.s/60001111
Creation date	: 10/10/2011
- End Header --------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Math2D.h"
#include "Matrix2D.h"

//
void PrintVector(char *txt, Vector2D *pVec0)
{
	printf("%s:\t%f, %f\n", txt, pVec0->x, pVec0->y);
}

float CompareVector2D(Vector2D *pSrc, Vector2D *pDst)
{
	float d = 0.0f;

	d += fabs(pSrc->x - pDst->x);
	d += fabs(pSrc->y - pDst->y);

	return d;
};

float CompareMatrix2D(Matrix2D* pSrc, Matrix2D* pDst)
{
	float d = 0.0f;

	for (long j = 0; j < 3; j++)
		for (long i = 0; i < 3; i++)
			d += fabs(pSrc->m[j][i] - pDst->m[j][i]);
	
	return d;
}

int main()
{
	Vector2D v1, v2, v3, v4, v5, result;
  float scale;

	v1.x = v1.y = 7.0f;
	Vector2DZero(v1);
	result.x = result.y = 0.0f;
	printf("Vector2DZero: %s\n", (CompareVector2D(&result, &v1) < EPSILON) ? "Pass" : "Fail");

	
	Vector2DSet(v1, 1.0f, 2.0f);
	result.x = 1.0f;	result.y = 2.0f;
	printf("Vector2DSet: %s\n", (CompareVector2D(&result, &v1) < EPSILON) ? "Pass" : "Fail");


	v1.x = 2.0f; v1.y = -4.0f;
	Vector2DNeg(v2, v1);
	result.x = -2.0f;	result.y = 4.0f;
	printf("Vector2DNeg: %s\n", (CompareVector2D(&result, &v2) < EPSILON) ? "Pass" : "Fail");

	v1.x = 2.0f; v1.y = -4.0f;
	v2.x = 1.0f; v2.y = 7.0f;
	Vector2DAdd(v3, v1, v2);
	result.x = result.y = 3.0f;
	printf("Vector2DAdd: %s\n", (CompareVector2D(&result, &v3) < EPSILON) ? "Pass" : "Fail");


	v1.x = 2.0f; v1.y = -4.0f;
	v2.x = 1.0f; v2.y = 7.0f;
	Vector2DSub(v3, v1, v2);
	result.x = 1.0f;	result.y = -11.0f;
	printf("Vector2DSub: %s\n", (CompareVector2D(&result, &v3) < EPSILON) ? "Pass" : "Fail");


	v1.x = 3.0f; v1.y = 4.0f;
	Vector2DNormalize(v2, v1);
	result.x = 0.6f;	result.y = 0.8f;
	printf("Vector2DNormalize: %s\n", (CompareVector2D(&result, &v2) < EPSILON) ? "Pass" : "Fail");


	v1.x = 2.0f; v1.y = -5.0f;
	Vector2DScale(v2, v1, 3.0f);
	result.x = 6.0f;	result.y = -15.0f;
	printf("Vector2DScale: %s\n", (CompareVector2D(&result, &v2) < EPSILON) ? "Pass" : "Fail");

  v1.x = 2.0f; v1.y = -5.0f;
  v2.x = 6.0f; v2.y =  2.0f;
  scale = 3.0f;
  Vector2DScaleAdd(v3, v1, v2, scale);
  result.x = 12.0f; result.y = -13.0f;
  printf("Vector2DScaleAdd: %s\n", (CompareVector2D(&result, &v3) < EPSILON) ? "Pass" : "Fail");

  Vector2DScaleSub(v3, v1, v2, scale);
  result.x = 0.f; result.y = -17.f;
  printf("Vector2DScaleSub: %s\n", (CompareVector2D(&result, &v3) < EPSILON) ? "Pass" : "Fail");

	v1.x = 3.0f; v1.y = -4.0f;
	printf("Vector2DLength: %s\n", (fabs(Vector2DLength(v1) - 5.0f)  < EPSILON) ? "Pass" : "Fail");


	v1.x = 3.0f; v1.y = -4.0f;
	printf("Vector2DSquareLength: %s\n", (fabs(Vector2DSquareLength(v1) - 25.0f)  < EPSILON) ? "Pass" : "Fail");


	v1.x = 2.0f;	v1.y = 3.0f;
	v2.x = 4.0f;	v2.y = -1.0f;
	printf("Vector2DDistance: %s\n", (fabs(Vector2DDistance(v1, v2) - 4.472136)  < EPSILON) ? "Pass" : "Fail");


	v1.x = 2.0f;	v1.y = 3.0f;
	v2.x = 4.0f;	v2.y = -1.0f;
	printf("Vector2DSquareDistance: %s\n", (fabs(Vector2DSquareDistance(v1, v2) - 20.0f)  < EPSILON) ? "Pass" : "Fail");


	v1.x = 3.0f;	v1.y = 2.0f;
	v2.x = 4.0f;	v2.y = -6.0f;
	printf("Vector2DDotProduct: %s\n", (fabs(Vector2DDotProduct(v1, v2)) < EPSILON) ? "Pass" : "Fail");

  printf("\n------Testing Line Segments------\n\n");
  LineSegment2D ls;
  v1.x = 2.f; v2.y = 3.f;
  v2.x = 7.f; v2.y = -1.f;
  BuildLineSegment2D(ls, v1, v2);
  
  /*result.x = .514496f; result.y = .857493f;

  printf("Line Segment Point 0: %s\n", (CompareVector2D(&ls.mP0, &v1) < EPSILON) ? "Pass" : "Fail");
  printf("Line Segment Point 1: %s\n", (CompareVector2D(&ls.mP1, &v2) < EPSILON) ? "Pass" : "Fail");
  printf("Line Segment Normal: %s\n", (CompareVector2D(&ls.mN, &result) < EPSILON) ? "Pass" : "Fail");*/

  printf("\n------Testing StaticPointToStaticLineSegment Collision------\n\n");
  v1.x = 0.f; v1.y = 0.f;
  v2.x = 10.f; v2.y = 10.f;
  v3.x = 5.f; v3.y = 5.f;
  BuildLineSegment2D(ls, v1, v2);
  printf("StaticPointToStaticLineSegment - Colliding: %s\n", (fabs(StaticPointToStaticLineSegment(v3, ls)) < EPSILON) ? "Pass" : "Fail");

  v3.x = 2.f; v3.y = 7.f;
  printf("StaticPointToStaticLineSegment - Non Colliding: %s\n", (fabs(StaticPointToStaticLineSegment(v3, ls)) > EPSILON) ? "Pass" : "Fail");

  //These two tests are to see if students did any extra work to determine whether
  // or not the point is on the segment or the infinite extension.
  v3.x = -1.f; v3.y = -1.f;
  printf("StaticPointToStaticLineSegment - Non Colliding Behind Segment: %s\n", (fabs(StaticPointToStaticLineSegment(v3, ls)) < EPSILON) ? "Pass" : "Check Code");

  v3.x = 11.f; v3.y = 11.f;
  printf("StaticPointToStaticLineSegment - Non Colliding In Front Of Segment: %s\n", (fabs(StaticPointToStaticLineSegment(v3, ls)) < EPSILON) ? "Pass" : "Check Code");


  printf("\n------Testing AnimatedPointToStaticLineSegment Collision------\n\n");
  v1.x = 0.f; v1.y = 5.f;
  v2.x = 10.f; v2.y = 5.f;
  Vector2DZero(v3);
  result.x = 5.f; result.y = 5.f;
  float t = AnimatedPointToStaticLineSegment(v1, v2, ls, v3);
  printf("AnimatedPointToStaticLineSegment - Time: %s\n", (fabs(t - 0.5f) < EPSILON) ? "Pass" : "Fail");
  printf("AnimatedPointToStaticLineSegment - Collision: %s\n", (CompareVector2D(&result, &v3) < EPSILON) ? "Pass" : "Fail");

  printf("\n------Testing AnimatedPointToStaticLineSegment Non Collision------\n\n");
  v1.x = 2.f; v1.y = 5.f;
  v2.x = 5.f; v2.y = 6.f;
  Vector2DZero(v3);
  t = AnimatedPointToStaticLineSegment(v1, v2, ls, v3);
  printf("AnimatedPointToStaticLineSegment - Non Collision: %s\n", (fabs(t + 1.f) < EPSILON) ? "Pass" : "Fail");

  printf("\n-----Testing AnimatedPointToStaticLineSegment Non Collision Behind Line Segment-----\n\n");
  v1.x = -1.f; v1.y = 0.f;
  v2.x = 0.f; v2.y = -1.f;
  Vector2DZero(v3);
  t = AnimatedPointToStaticLineSegment(v1, v2, ls, v3);
  printf("AnimatedPointToStaticLineSegment - Non Collision: %s\n", (fabs(t + 1.f) < EPSILON) ? "Pass" : "Fail");

  printf("\n--Testing AnimatedPointToStaticLineSegment Non Collision In Front Of Line Segment--\n\n");
  v1.x = 10.f; v1.y = 11.f;
  v2.x = 11.f; v2.y = 10.f;
  Vector2DZero(v3);
  t = AnimatedPointToStaticLineSegment(v1, v2, ls, v3);
  printf("AnimatedPointToStaticLineSegment - Non Collision: %s\n", (fabs(t + 1.f) < EPSILON) ? "Pass" : "Fail");

  printf("\n------Testing AnimatedCircleToStaticLineSegment Collision------\n\n");
  v1.x = 0.f; v1.y = 5.f;
  v2.x = 10.f; v2.y = 5.f;
  Vector2DZero(v3);
  result.x = 3.585787f; result.y = 5.f;
  float radius = 1.f;
  t = AnimatedCircleToStaticLineSegment(v1, v2, radius, ls, v3);
  printf("AnimatedCircleToStaticLineSegment - Time: %s\n", (fabs(t - .358579f) < EPSILON) ? "Pass" : "Fail");
  printf("AnimatedCircleToStaticLineSegment - Collision: %s\n", (CompareVector2D(&result, &v3) < EPSILON) ? "Pass" : "Fail");

  printf("\n------Testing AnimatedCircleToStaticLineSegment Non Collision------\n\n");
  v1.x = 2.f; v1.y = 5.f;
  v2.x = 4.f; v2.y = 6.f;
  Vector2DZero(v3);
  t = AnimatedCircleToStaticLineSegment(v1, v2, radius, ls, v3);
  printf("AnimatedCircleToStaticLineSegment - Non Collision: %s\n", (fabs(t + 1.f) < EPSILON) ? "Pass" : "Fail");

  printf("\n-----Testing AnimatedCircleToStaticLineSegment Non Collision Behind Line Segment-----\n\n");
  v1.x = -2.f; v1.y = 1.f;
  v2.x = 1.f; v2.y = -2.f;
  Vector2DZero(v3);
  t = AnimatedCircleToStaticLineSegment(v1, v2, radius, ls, v3);
  printf("AnimatedCircleToStaticLineSegment - Non Collision: %s\n", (fabs(t + 1.f) < EPSILON) ? "Pass" : "Fail");

  printf("\n--Testing AnimatedCircleToStaticLineSegment Non Collision In Front Of Line Segment--\n\n");
  v1.x = 9.f; v1.y = 12.f;
  v2.x = 12.f; v2.y = 9.f;
  Vector2DZero(v3);
  t = AnimatedCircleToStaticLineSegment(v1, v2, radius, ls, v3);
  printf("AnimatedCircleToStaticLineSegment - Non Collision: %s\n", (fabs(t + 1.f) < EPSILON) ? "Pass" : "Fail");

  //These two are another extra credit check.  If students implemented non-line intersection
  //  but collision with wall (slide 21 - lecture 6), then I will be notified here to 
  //  check their code.
  printf("\n----Testing AnimatedCircleToStaticLineSegment Collision with LS End Point----\n\n");
  v1.x = 8.f; v1.y = 13.f;
  v2.x = 13.f; v2.y = 8.f;
  Vector2DZero(v3);
  t = AnimatedCircleToStaticLineSegment(v1, v2, radius, ls, v3);
  printf("AnimatedCircleToStaticLineSegment - Collision: %s\n", (fabs(t - 1.f) <= 1.f) ? "Check Code" : "Pass");

  printf("\n----Testing AnimatedCircleToStaticLineSegment Collision with LS Start Point----\n\n");
  v1.x = -3.f; v1.y = 2.f;
  v2.x = 2.f; v2.y = -3.f;
  Vector2DZero(v3);
  t = AnimatedCircleToStaticLineSegment(v1, v2, radius, ls, v3);
  printf("AnimatedCircleToStaticLineSegment - Collision: %s\n", (fabs(t - 1.f) <= 1.f) ? "Check Code" : "Pass");

  printf("\n------Testing ReflectAnimatedPointOnStaticLineSegment------\n\n");
  v1.x = 5.f; v1.y = 0.f;
  v2.x = 5.f; v2.y = 10.f;
  BuildLineSegment2D(ls, v1, v2);

  v1.x = 0.f; v1.y = 10.f;
  v2.x = 10.f; v2.y = 0.f;
  Vector2DZero(v3); Vector2DZero(v4);
  t = ReflectAnimatedPointOnStaticLineSegment(v1, v2, ls, v3, v4);
  result.x = 5.f; result.y = 5.f;
  printf("ReflectAnimatedPointOnStaticLineSegment Pint: %s\n", (CompareVector2D(&result, &v3) < EPSILON) ? "Pass" : "Fail");

  result.x = -5.f; result.y = -5.f;
  printf("ReflectAnimatedPointOnStaticLineSegment R: %s\n", (CompareVector2D(&result, &v4) < EPSILON) ? "Pass" : "Fail");
	
  printf("\n------Testing ReflectAnimatedCircleOnStaticLineSegment------\n\n");
  Vector2DZero(v3); Vector2DZero(v4);
  t = ReflectAnimatedCircleOnStaticLineSegment(v1, v2, 1.f, ls, v3, v4);
  result.x = 4.f; result.y = 6.f;
  printf("ReflectAnimatedCircleOnStaticLineSegment Pint: %s\n", (CompareVector2D(&result, &v3) < EPSILON) ? "Pass" : "Fail");

  result.x = -6.f; result.y = -6.f;
  printf("ReflectAnimatedCircleOnStaticLineSegment R: %s\n", (CompareVector2D(&result, &v4) < EPSILON) ? "Pass" : "Fail");

  printf("\n------Testing StaticPointToStaticCircle------\n\n");
  v1.x = 10.f; v1.y = 10.f;
  v2.x = 11.4f; v2.y = 11.4f;
  radius = 2.f;
  printf("StaticPointToStaticCircle Collision: %s\n", (StaticPointToStaticCircle(v2, v1, radius)) ? "Pass" : "Fail" );

  v2.x = 12.f; v2.y = 12.f;
  printf("StaticPointToStaticCircle Non Collision: %s\n", (!StaticPointToStaticCircle(v2, v1, radius)) ? "Pass" : "Fail" );

  printf("\n------Testing AnimatedPointToStaticCircle------\n\n");
  float sqrt2 = sqrt(2.f);
  v1.x = 10.f; v1.y = sqrt2;
  v2.x = -10.f; v2.y = sqrt2;
  v3.x = 0.f; v3.y = 0.f;
  Vector2DZero(v4);
  t = AnimatedPointToStaticCircle(v1, v2, v3, 2.f, v4);
  result.x = sqrt2; result.y = sqrt2;
  printf("AnimatedPointToStaticCircle Collision Location: %s\n", (CompareVector2D(&result, &v4) < EPSILON) ? "Pass" : "Fail");

  v1.y = 2.1f;
  v2.y = 2.1f;
  Vector2DZero(v4);
  t = AnimatedPointToStaticCircle(v1, v2, v3, 2.f, v4);
  printf("AnimatedPointToStaticCircle Non Collision: %s\n", (abs(t + 1.f) < EPSILON) ? "Pass" : "Fail");

  printf("\n------Testing ReflectAnimatedPointOnStaticCircle------\n\n");
  v1.x = 2.f*sqrt2; v1.y = sqrt2;
  v2.x = 0.f; v2.y = sqrt2;
  result.x = sqrt2; result.y = sqrt2;
  Vector2DZero(v4); Vector2DZero(v5);
  t = ReflectAnimatedPointOnStaticCircle(v1, v2, v3, 2.f, v4, v5);
  printf("ReflectAnimatedPointOnStaticCircle Pi check: %s\n", (CompareVector2D(&result, &v4) < EPSILON) ? "Pass" : "Fail");

  result.x = 0.f; result.y = sqrt2;
  printf("ReflectAnimatedPointOnStaticCircle R check: %s\n", (CompareVector2D(&result, &v5) < EPSILON) ? "Pass" : "Fail");


  printf("\n------Testing AnimatedCircleToStaticCircle------\n\n");
  v1.x = 4.f; v1.y = 2.f*sqrt2;
  v2.x = 0.f; v2.y = 2.f*sqrt2;
  v3.x = 0.f; v3.y = 0.f;
  Vector2DZero(v4);
  radius = 2.f;
  t = AnimatedCircleToStaticCircle(v1, v2, radius, v3, radius, v4);
  result.x = 2.f*sqrt2; result.y = 2.f*sqrt2;
  printf("AnimatedCircleToStaticCircle Collision Location: %s\n", (CompareVector2D(&result, &v4) < EPSILON) ? "Pass" : "Fail");

  v1.y = 4.1f;
  v2.y = 4.1f;
  Vector2DZero(v4);
  t = AnimatedCircleToStaticCircle(v1, v2, radius, v3, radius, v4);
  printf("AnimatedCircleToStaticCircle Non Collision: %s\n", (abs(t + 1.f) < EPSILON) ? "Pass" : "Fail");

  printf("\n------Testing ReflectAnimatedCircleOnStaticCircle------\n\n");
  v1.x = 4.f*sqrt2; v1.y = 2.f*sqrt2;
  v2.x = 0.f; v2.y = 2.f*sqrt2;
  v3.x = 0.f; v3.y = 0.f;
  Vector2DZero(v4); Vector2DZero(v5);
  t = ReflectAnimatedCircleOnStaticCircle(v1, v2, radius, v3, radius, v4, v5);

  result.x = 2*sqrt2; result.y = 2*sqrt2;
  printf("ReflectAnimatedCircleOnStaticCircle Pi: %s\n", (CompareVector2D(&result, &v4) < EPSILON) ? "Pass" : "Fail");

  result.x = 0.f; result.y = 2*sqrt2;
  printf("ReflectAnimatedCircleOnStaticCircle R: %s\n", (CompareVector2D(&result, &v5) < EPSILON) ? "Pass" : "Fail");



  printf("\n------Running Matrix Tests------\n\n");

  Matrix2D id, m0, m1;
  Vector2D  u;
	float d, x, y;
	long  n;

	// create an id matrix for reference
	for (long i = 0; i < 3; i++)
		for (long j = 0; j < 3; j++)
			id.m[j][i] = (i == j) ? 1.0f : 0.0f;

  // ====================
	// test Matrix2DIdentity
	// ====================

	Matrix2DIdentity(m0);
	d = CompareMatrix2D(&id, &m0);
	printf("Matrix2DIdentity : %s\n", (CompareMatrix2D(&id, &m0) < EPSILON) ? "Pass" : "Fail");

	// ====================
	// test Matrix2DTrans
	// ====================

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;

	Matrix2DTranslate(m0, x, y);
	m0.m[0][2] -= x;
	m0.m[1][2] -= y;
	printf("Matrix2DTranslate: %s\n", (CompareMatrix2D(&id, &m0) < EPSILON) ? "Pass" : "Fail");

	// ====================
	// test Matrix2DScale
	// ====================

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;

	Matrix2DScale(m0, x, y);
	m0.m[0][0] /= x;
	m0.m[1][1] /= y;

	printf("Matrix2DScale    : %s\n", (CompareMatrix2D(&id, &m0) < EPSILON) ? "Pass" : "Fail");

	// ====================
	// test Matrix2DConcat
	// ====================

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;

	Matrix2DTranslate (m0, x, y);
	Matrix2DScale (m1, x, y);
	Matrix2DConcat(m0, m0, m1);
	m0.m[0][2] -= x;
	m0.m[1][2] -= y;
	m0.m[0][0] /= x;
	m0.m[1][1] /= y;

	printf("Matrix2DConcat 1 : %s\n", (CompareMatrix2D(&id, &m0) < EPSILON) ? "Pass" : "Fail");

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;

	Matrix2DTranslate (m0, x, y);
	Matrix2DScale (m1, x, y);
	Matrix2DConcat(m0, m1, m0);
	m0.m[0][2] -= x * x;
	m0.m[1][2] -= y * y;
	m0.m[0][0] /= x;
	m0.m[1][1] /= y;

	printf("Matrix2DConcat 2 : %s\n", (CompareMatrix2D(&id, &m0) < EPSILON) ? "Pass" : "Fail");

	// ====================
	// test Matrix2DRotRad
	// ====================

	n = (rand() % 16) + 15;
	Matrix2DIdentity(m0);
	Matrix2DRotRad  (m1, 2.0f * PI / n);

	for (long i = 0; i < n; i++)
		Matrix2DConcat(m0, m1, m0);

	printf("Matrix2DRotRad   : %s (%d)\n", (CompareMatrix2D(&id, &m0) < EPSILON) ? "Pass" : "Fail", n);

	// ====================
	// test Matrix2DRotDeg
	// ====================

	n = (rand() % 16) + 15;
	Matrix2DIdentity(m0);
	Matrix2DRotDeg  (m1, 360.0f / n);

	for (long i = 0; i < n; i++)
		Matrix2DConcat(m0, m1, m0);

	printf("Matrix2DRotDeg   : %s (%d)\n", (CompareMatrix2D(&id, &m0) < EPSILON) ? "Pass" : "Fail", n);

	// ====================
	// test Matrix2DTranspose
	// ====================

	Matrix2DRotRad   (m0, rand() / (float)(RAND_MAX) * 2.0f * PI);
	Matrix2DTranspose(m1, m0);
	Matrix2DConcat   (m0, m1, m0);

	printf("Matrix2DTranspose: %s\n", (CompareMatrix2D(&id, &m0) < EPSILON) ? "Pass" : "Fail");

	// ====================
	// test Matrix2DMultVec
	// ====================

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;

	n = (rand() % 16) + 15;
  Vector2DSet		(u, x, y);
	Matrix2DRotRad	(m0, 2.0f * PI / n);

	for (long i = 0; i < n; i++)
		Matrix2DMultVec(u, m0, u);

	printf("Matrix2DMultVec  : %s\n", ((fabs(u.x - x) + fabs(u.y - y)) < EPSILON) ? "Pass" : "Fail");

	// generate 2 random numbers
	x = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;
	y = 2.0f * rand() / (float)(RAND_MAX) - 1.0f;

	n = (rand() % 16) + 15;
	Vector2DSet		(u, x, y);
	Matrix2DTranslate	(m0, x, y);

	for (long i = 1; i < n; i++)
		Matrix2DMultVec(u, m0, u);

	printf("Matrix2DMultVec  : %s\n", ((fabs(u.x - x * n) + fabs(u.y - y * n)) < EPSILON) ? "Pass" : "Fail");

  printf("\n------Testing New Collision Functions------\n\n");

  //StaticPointToStaticRect
  Vector2DSet(v1, 1.f, 1.f); //point
  Vector2DSet(v2, 0.f, 0.f); //rect
  printf("StaticPointToStaticRect Collision: %s\n", (StaticPointToStaticRect(v1, v2, 2.f, 2.f) ? "Pass" : "Fail"));
  printf("StaticPointToStaticRect Non Collision: %s\n\n", (!StaticPointToStaticRect(v1, v2, 1.f, 1.f) ? "Pass" : "Fail"));

  //StaticCircleToStaticCircle
  Vector2DSet(v1, 2.f, 0.f);
  printf("StaticCircleToStaticCircle Collision Touch: %s\n", (StaticCircleToStaticCircle(v1, 1.f, v2, 1.f) ? "Pass" : "Fail"));
  printf("StaticCircleToStaticCircle Collision: %s\n", (StaticCircleToStaticCircle(v1, 2.f, v2, 1.f) ? "Pass" : "Fail"));
  printf("StaticCircleToStaticCircle Non Collision: %s\n\n", (!StaticCircleToStaticCircle(v1, 0.5f, v2, 1.f) ? "Pass" : "Fail"));

  //StaticRectToStaticRect
  Vector2DSet(v1, 2.f, 2.f);
  printf("StaticRectToStaticRect Non Collision: %s\n", (!StaticRectToStaticRect(v1, 1.f, 1.f, v2, 1.f, 1.f) ? "Pass" : "Fail"));
  printf("StaticRectToStaticRect Collision Touch: %s\n", (StaticRectToStaticRect(v1, 2.f, 2.f, v2, 2.f, 2.f) ? "Pass" : "Fail"));
  printf("StaticRectToStaticRect Collision Intersect: %s\n", (StaticRectToStaticRect(v1, 3.f, 3.f, v2, 3.f, 3.f) ? "Pass" : "Fail"));

  return 1;
}