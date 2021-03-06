///////////////////////////////////////////////////////////////////////
// $Id: scenelib.h 1016 2007-06-12 17:07:55Z gherron $
//
// Operations for manipulating transformations
//
// Gary Herron
//
// Copyright � 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#if !defined(_TRANSFORMATION_H)
#define _TRANSFORMATION_H

int invert(Matrix4x4* mat, Matrix4x4* inv);

////////////////////////////////////////////////////////////////////////
// Class Transformation implements a Matrix4x4 along with the usual
// methods to build a transformation from primitives, plus the ability
// to push and pop the matrix onto a stack.
class Transformation
{
public:
	Transformation();			// Constructor -- yields an identity

	// Standard graphics pipeline matrix stack operations
	void Push();
	void Pop();

	// Standard graphics pipeline operations to build the transformation.
	void Identity();
	void RotateX(float theta);
	void RotateY(float theta);
	void RotateZ(float theta);
	void Scale(float sx, float sy, float sz);
	void Translate(float tx, float ty, float tz);
	void Perspective(float sx, float sy, float front, float back);

	// Called to compute the inverse transformation when the forward
	// transformation is finsihed being built.
	void ComputeInverse() { invert(&curr, &inv); }

	// Methods to (forward and inverse) transform points and planes.
	Vector4D Transform(const Vector4D& V);
	Vector4D InverseTransform(const Vector4D& V);

	Plane3D Transform(const Plane3D& V);
	Plane3D InverseTransform(const Plane3D& V);

	// Get matrix as a float* for passing to OpenGL
	void ArrayForOpenGL(float* array16);

private:
	Matrix4x4 curr;				// The current transformation
	Matrix4x4 inv;				// Its inverse
	std::vector<Matrix4x4> matrixStack;	// A stack for Push/Pop methods.
	
};

#endif // _TRANSFORMATION_H
