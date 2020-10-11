
///////////////////////////////////////////////////////////////////////
// $Id: scenelib.h 1016 2007-06-12 17:07:55Z gherron $
//
// Operations for manipulating transformations
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#if !defined(_TRANSFORMATION_H)
#define _TRANSFORMATION_H


 class Matrix4x4
 {
	public:
	Vector4D M[4];
	Vector4D& operator[](const unsigned int i) { return M[i]; }
 };

int invert(Matrix4x4* mat, Matrix4x4* inv);

////////////////////////////////////////////////////////////////////////
// Class Transformation represents a transformation with a 4x4 matrix.
// It provides the usual methods to build a transformation from
// primitives, plus the ability to push and pop the matrix onto a
// stack.
class Transformation
{
public:
	Transformation();			// Constructor -- yields an identity

	// Standard graphics pipeline matrix stack operations
	void Push();
	void Pop();

	// Standard graphics pipeline operations to build the transformation.
	void Identity();
	void RotateX(const float theta);
	void RotateY(const float theta);
	void RotateZ(const float theta);
	void Scale(const float sx, const float sy, const float sz);
	void Translate(const float tx, const float ty, const float tz);
	void Perspective(const float rx, const float ry,
					 const float front, const float back);

	// Called to compute the inverse transformation when the forward
	// transformation is finsihed being built.
	void ComputeInverse() { invert(&current, &inverse); }


	// Methods to (forward and inverse) transform points and planes.
	Vector4D Transform(const Vector4D& V);
	//Vector4D InverseTransform(const Vector4D& V);
	//Plane3D Transform(const Plane3D& V);
	//Plane3D InverseTransform(const Plane3D& V);

	// Get matrix as a float* for passing to OpenGL
	void ArrayForOpenGL(float* array16);

private:
	
	Matrix4x4 MatrixMultiply(Matrix4x4 &m1, Matrix4x4 &m2);
	Matrix4x4 current;					// The current transformation
	Matrix4x4 inverse;					// Its inverse
	std::vector<Matrix4x4> matrixStack;	// A stack for Push/Pop methods.	
};

// Procedures called by the framework to implement interactive viewing.
void LeftButtonDown(const int x, const int y);
void LeftButtonDrag(const int x, const int y);
void LeftButtonUp(const int x, const int y);

void MiddleButtonDown(const int x, const int y);
void MiddleButtonDrag(const int x, const int y);
void MiddleButtonUp(const int x, const int y);

void RightButtonDown(const int x, const int y);
void RightButtonDrag(const int x, const int y);
void RightButtonUp(const int x, const int y);

void MouseWheelUp();
void MouseWheelDown();

class Scene;
void BuildProjection(Scene& scene, int width, int height);

#endif // _TRANSFORMATION_H
