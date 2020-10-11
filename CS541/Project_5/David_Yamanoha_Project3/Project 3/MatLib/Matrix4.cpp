/**
 *	\file	Matrix4.cpp
 *  \author David Yamanoha
 *  \date   09.08.08
 */
#include "Matrix4.h"
#include <Memory.h>
#include <math.h>

/**
*	Constructor.
*	\returns	n/a.
*	\callergraph
*	\callgraph
*/
Matrix4::Matrix4()
{
	LoadIdentity();	
}

/**
*	Sets the matrix to the identity.  (Matrix diagnol from upper left to bottom right set to 1.0, all other elements are
*	set to zero.
*	\returns void.
*	\callergraph
*	\callgraph
*/
void Matrix4::LoadIdentity()
{
	memset(&m_matrix, 0, sizeof(Matrix_t));
	m_matrix._11 = 1.0f;
	m_matrix._22 = 1.0f;
	m_matrix._33 = 1.0f;
	m_matrix._44 = 1.0f;
}

/**
*	Sets a particular row of the matrix to a vector.
*	\param row	The row to to be set.
*	\param v	The values to set the matrix row to.
*	\returns	void.
*	\callergraph
*	\callgraph
*/
void Matrix4::SetRow(int row, const Vector4& v)
{	
	if(!CheckMatrixBounds(row))
		return;

	(*this)(row, 1) = v.x;
	(*this)(row, 2) = v.y;
	(*this)(row, 3) = v.z;
	(*this)(row, 4) = v.w;
}

/**
*	Sets all rows of the matrix to vector values.
*	\param r1	The vector to set row 1 to.
*	\param r2	The vector to set row 2 to.
*	\param r3	The vector to set row 3 to.
*	\param r4	The vector to set row 4 to.
*	\returns	void.
*	\callergraph
*	\callgraph
*/
void Matrix4::SetRows(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4)
{
	SetRow(1, r1);
	SetRow(2, r2);
	SetRow(3, r3);
	SetRow(4, r4);
}

/**
*	Sets a particular column of the matrix to a vector.
*	\param col	The row to to be set.
*	\param v	The values to set the matrix row to.
*	\returns	void.
*	\callergraph
*	\callgraph
*/
void Matrix4::SetColumn(int col, const Vector4& v)
{
	if(!CheckMatrixBounds(col))
		return;

	int colOfst = --col;

	float* matrix = (float*)&m_matrix;
	matrix[colOfst + 0 * M_DIMENSION] = v.x;
	matrix[colOfst + 1 * M_DIMENSION] = v.y;
	matrix[colOfst + 2 * M_DIMENSION] = v.z;
	matrix[colOfst + 3 * M_DIMENSION] = v.w;
}

/**
*	Sets all columns of the matrix to vector values.
*	\param c1	The vector to set column 1 to.
*	\param c2	The vector to set column 2 to.
*	\param c3	The vector to set column 3 to.
*	\param c4	The vector to set column 4 to.
*	\returns	void.
*	\callergraph
*	\callgraph
*/
void Matrix4::SetColumns(const Vector4& c1, const Vector4& c2, const Vector4& c3, const Vector4& c4)
{
	SetColumn(1, c1);
	SetColumn(2, c2);
	SetColumn(3, c3);
	SetColumn(4, c4);
}

/**
*	Retrieves a row from the matrix.  (One based.)
*	\param	row The row to retrieve.
*	\returns The requested row from the matrix.
*	\callergraph
*	\callgraph
*/
Vector4	Matrix4::GetRow(int row) const
{
	Vector4 v;
	if(!CheckMatrixBounds(row))
		return v;

	int rowOfst = --row * M_DIMENSION;
	
	float* matrix = (float*)&m_matrix;
	v.x = matrix[rowOfst + 0];
	v.y = matrix[rowOfst + 1];
	v.z = matrix[rowOfst + 2];
	v.w = matrix[rowOfst + 3];

	return v;
}

/**
*	Retrieves a column from the matrix.  (One based.)
*	\param	col The column to retrieve.
*	\returns The requested column from the matrix.
*	\callergraph
*	\callgraph
*/
Vector4	Matrix4::GetColumn(int col) const
{	
	Vector4 v;
	if(!CheckMatrixBounds(col))
		return v;

	v.x = (*this)(1, col);
	v.y = (*this)(2, col);
	v.z = (*this)(3, col);
	v.w = (*this)(4, col);	

	return v;
}


/** 
*	Initializes the coordinate system to represent the rotation by a quaternion.
*	\param	q Quaternion to base the matrix on.
*	\callergraph 
*	\callgraph
*/
void Matrix4::Build(const Quaternion& q)
{
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float w = q.w;
	SetRows(	Vector4(1 - 2*y*y - 2*z*z, 2*x*y + 2*w*z, 2*x*z - 2*w*y, 0.0f),
				Vector4(2*x*y - 2*w*z, 1 - 2*x*x - 2*z*z, 2*y*z + 2*w*x, 0.0f),
				Vector4(2*x*z + 2*w*y, 2*y*z - 2*w*x, 1 - 2*x*x - 2*y*y, 0.0f),
				Vector4(0, 0, 0, 1)												);

	//SetColumns(	Vector4(1 - 2*y*y - 2*z*z,	2*x*y + 2*w*z,		2*x*z - 2*w*y,		0.0f),
	//			Vector4(2*x*y - 2*w*z,		1 - 2*x*x - 2*z*z,	2*y*z + 2*w*x,		0.0f),
	//			Vector4(2*x*z + 2*w*y,		2*y*z - 2*w*x,		1 - 2*x*x - 2*y*y,	0.0f),
	//			Vector4(0,					0,					0,					1)	);
}

/** 
*	Retrieves the entire matrix transform.
*	\returns 4x4 float struct representing the matrix.
*	\callergraph
*	\callgraph
*/
Matrix_t Matrix4::GetTransform() const
{
	return m_matrix;
}

/**
*	Matrix-vector multiplication.
*	\param v	Vector operand.
*	\returns	Result of the matrix multiplication.
*	\callergraph
*	\callgraph
*/
Vector4	Matrix4::operator*(const Vector4 v) const
{
	Vector4 result;
#ifdef GL
		result.x = v * GetRow(1);
		result.y = v * GetRow(2);
		result.z = v * GetRow(3);
		result.w = v * GetRow(4);
#else
		result.x = v * GetColumn(1);
		result.y = v * GetColumn(2);
		result.z = v * GetColumn(3);
		result.w = v * GetColumn(4);
#endif

	return result;
}

/**
*	Matrix - matrix multiplication.
*	\param m	Matrix to concatenate with.
*	\return	Concatenated matrices <i>(*this)</i> and <i>m</i>.
*	\callergraph
*	\callgraph
*/
Matrix4 Matrix4::operator*(const Matrix4 m) const
{
	Matrix4 result;
	for(unsigned i = 1; i <= M_DIMENSION; i++)
	{
		for(unsigned j = 1; j <= M_DIMENSION; j++)
		{
			result(i,j) = GetRow(i) * m.GetColumn(j);
		}
	}
	return result;
}

/** 
*	Retrieves the inverse of the matrix.
*	\note	IMPORTANT:  The inverse matrix here is <i>not</i> the inverse of the actual matrix.  It is, however, the 
*			the coordinate system un-translated, and un-rotated.  This function should be used as a helper for 
*			coordinate system conversions, and does not reflect actual matrix inversion such as Guass-Jordan.
*	\returns	Matrix un-translated and un-rotated matrix representation.
*	\callergraph
*	\callgraph
*/
Matrix4 Matrix4::operator~() const
{
	Vector4 translation(GetRow(4));
	translation.w = 0.0f;
	Matrix4 mf;
#ifdef GL
		mf.SetRows(GetColumn(1), GetColumn(2), GetColumn(3), Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		mf.SetColumn(4, -translation);
#else
		mf.SetColumns(GetRow(1), GetRow(2), GetRow(3), Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		mf.SetRow(4, Vector4( -(translation * mf.GetColumn(1)), -(translation * mf.GetColumn(2)), -(translation * mf.GetColumn(3)), 1.0f));
#endif
	return mf;
}

/**
*	Matrix equality test.
*	\param m Operand matrix to test eqaulity against.
*	\returns Equality test result.
*/
bool Matrix4::operator==(const Matrix4 m)const
{
	for(unsigned i = 1; i <= M_DIMENSION; i++)
	{
		for(unsigned j = 1; j <= M_DIMENSION; j++)
		{
			if((*this)(i, j) != m(i,j))
				return false;
		}
	}
	return true;
}

/**
*	Matrix in-equality test.
*	\param m Operand matrix to test in-eqaulity against.
*	\returns In-quality test result.
*/
bool Matrix4::operator!=(const Matrix4 m)const
{
	return !((*this) == m);
}

/**
*	Matrix Assignment.
*	\param m Operand matrix to assign value against.
*	\returns void.
*/
void Matrix4::operator=(const Matrix4 m)
{
	for(unsigned i = 1; i <= M_DIMENSION; i++)
	{
		for(unsigned j = 1; j <= M_DIMENSION; j++)
		{
			(*this)(i, j) = m(i,j);
		}
	}
}

/** 
*	Matrix accessor.  
*	\param i	Matrix Row.
*	\param j	Matrix Column.
*	\returns	The element of the matrix associated with the requested column and row.
*	\callergraph
*	\callgraph
*/
float& Matrix4::operator()(int i, int j) const
{
	j--;
	i--;
	float* matrix = (float*)&m_matrix;
	return matrix[i * M_DIMENSION + j];
}

/** 
*	Checks to see if a requested index lies within the bounds of the matrix.
*	\returns	Bounds test result.
*	\callergraph
*	\callgraph
*/
bool Matrix4::CheckMatrixBounds(int dimension) const
{
	if(dimension < 1 || dimension > M_DIMENSION)
		return false;
	return true;
}

/** 
*	Checks element-by-element variance between <i>(*this)</i> and <i>m</i>.  
*	\note		Since matricies are filled with float values, it is rarely common for float a to be equal to float b,
				even though it is possible that they should be.  Using this function will create a matrix of 
				difference values.  When used with ElementVariance(), you can see the actual difference between the two 
				matrices as a single 'absolute difference' float value.  This can be used to determine when to 
				renormalize a matrix.
*	\param	m	Matrix to test for variance against.
*	\returns	Variance matrix.
*	\callergraph
*	\callgraph
*/
Matrix4	Matrix4::ElementVariance(const Matrix4& m) const
{
	Matrix4 result;
	for(unsigned i = 1; i <= M_DIMENSION; i++)
	{
		for(unsigned j = 1; j <= M_DIMENSION; j++)
		{
			result(i,j) = (*this)(i,j) - m(i,j);
		}
	}
	return result;
}

/** 
*	Computes the amount of variance in a single matrix.  This is typically used in conjunction with 
*	<i>ElementVariance().  See documentation of ElementVariance() for more details.</i>.
*	\returns	Ammount of variance in the matrix.
*	\callergraph
*	\callgraph
*/
float Matrix4::Variance() const
{
	float sum = 0.0f;
	for(unsigned i = 1; i <= M_DIMENSION; i++)
	{
		for(unsigned j = 1; j < M_DIMENSION; j++)
		{
			sum += fabs((*this)(i, j));
		}
	}
	return sum;
}