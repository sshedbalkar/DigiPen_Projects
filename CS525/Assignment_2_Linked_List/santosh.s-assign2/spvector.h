#ifndef MATRIX_H
#define MATRIX_H
#include <stdio.h> /* printf */

/* opaque pointer since definition of Node is hidden from the client we need to
 * provide some way to use Node struct, and here it is -- typedef.  At this
 * point Node is NOT KNOWN to the compiler (which makes Node an INCOMPLETE
 * TYPE, but declaring a pointer to an incomplete type is allowed by C (and
 * C++).
 *
 * see http://en.wikipedia.org/wiki/Opaque_pointer
 *
 * we also need a special type ConstElementNode_handle:
 * notice that:
 * even though "ElementNode*" is same as "ElementNode_handle"
 * "const ElementNode*" and "const ElementNode_handle"
 * are DIFFERENT:
 * const ElementNode* p; is modifiable pointer to constant data
 * while 
 * const ElementNode_handle p; is constant pointer to modifiable data
 */
typedef struct ElementNode* ElementNode_handle;
typedef const struct ElementNode* ConstElementNode_handle;
typedef struct RowNode* RowNode_handle;
typedef const struct RowNode* ConstRowNode_handle;

/*===================*
 * element functions *
 *===================*/

/* print elements as a vector, adding 0's for non-existing positions
 * assume the length of the vector = dim
 * doesn't check if the list contains positions beyond dim */
void printf_elements(ConstElementNode_handle p_el, char *fmt, int dim); 

/* 
 * print all elements in the list with positions */
void print_elements(ConstElementNode_handle p_el); /* no zeros */

/*!
	\fn int insert_element(ElementNode_handle *p_e,int pos,int val)
	\brief Inserts an element into the list. If there is an element already present\
	at the position then the value is replaced by the new value. If the new value \
	is 0 then the node is deleted

	\param p_e Pointer to the head node of the list
	\param pos Position of the node which needs to be changed
	\param val The value to be inserted/updated

	\return 0 If the operation was successful, 1 otherwise
*/
int insert_element(ElementNode_handle * p_e,int pos,int val);

/*!
	\fn void delete_element(ElementNode_handle *p_e,int pos)
	\brief Deletes a node present at a specifed position

	\param p_e Pointer to the head node of the list
	\param pos Position of the node to be deleted
*/
void delete_element(ElementNode_handle *p_e,int pos);

/*!
	\fn int get(ConstElementNode_handle p_e, int pos)
	\brief Returns the value at the specific position

	\param p_e Pointer to the head node of the list
	\param pos Position of the node in the list whose value is to be read

	\return Value present at the node. If there is no node present at the position \
	then return 0
*/
int get(ConstElementNode_handle p_e, int pos);

/*!
	\fn int scalar_product(ConstElementNode_handle p_e1,ConstElementNode_handle p_e2)
	\brief Calculates the scalar product of 2 ElementNode lists

	\param p_e1 First list
	\param p_e2 Second list

	\return Scalar product value of the 2 lists
*/
int scalar_product(ConstElementNode_handle p_e1,ConstElementNode_handle p_e2);

/*!
	\fn ElementNode_handle add(ConstElementNode_handle p_e1,ConstElementNode_handle p_e2)
	\brief Creates a new list whose elements denote the sum of the elements of 2 lists

	\param p_e1 First list
	\param p_e2 Second list

	\return A new list of the sum
*/
ElementNode_handle add(ConstElementNode_handle p_e1,ConstElementNode_handle p_e2);

/*!
	\fn void free_elements(ConstElementNode_handle p_e)
	\brief Frees all the nodes of a linked list

	\param p_e Pointer to the head node of the list
*/
void free_elements(ConstElementNode_handle p_e);

/*===============* 
 * row functions * 
 *===============*/

/* prints rows as a matrix, adding 0's for non-existing positions
 * (both for non-existing rows and elements)
 * assume the number of rows = dim
 * assume the length of the vector = dim
 * doesn't check if the list contains positions beyond dim */
void printf_rows(ConstRowNode_handle p_r, char *fmt, int dim);

/* 
 * prints all non-zero elements of the list of rows */
void print_rows(ConstRowNode_handle p_r);

/*!
	\fn int insert_row(RowNode_handle *p_r,int pos,ElementNode_handle p_e)
	\brief Inserts a list of elements at position pos.\
	If a row at position pos already exists, do not insert, return 2 \
	If the list p_e is empty, do nothing

	\param p_r Pointer to the head node of the row list
	\param p_e New elements list to be inserted in the row list
	
	\return 1 If malloc fails \
			2 If position po already exists \
			0 Otherwise
*/
int insert_row(RowNode_handle *p_r,int pos,ElementNode_handle p_e);

/*!
	\fn int insert_element2(RowNode_handle *pp_r,int pos_row,int pos_col,int val)
	\brief Inserts an element at the specified (row, col) position in a row list \
		If there is already a node present then it is updated/deleted if the value \
		is non-zero/zero

	\param pp_r Pointer to the head node of the row list
	\param pos_row Position of the row in the list
	\param pos_col Position of the node in the elemets list

	\return 0 If the insertion was successful, 1 otherwise
*/
int insert_element2(RowNode_handle *pp_r,int pos_row,int pos_col,int val);

/*!
	\fn RowNode_handle find_row(RowNode_handle *pp_r,int pos)
	\brief returns a pointer to the row present at a specific position in a row list

	\param pp_r Pointer to the head node of a row list
	\param pos Position of the row in the list

	\return Pointer to the row if the row is present, NULL otherwise
*/
RowNode_handle find_row(RowNode_handle *pp_r,int pos);

/*!
	\fn RowNode_handle transpose(ConstRowNode_handle p_r)
	\brief Creates a new row list of the transpose of the matrix represented by \
	the given list

	\param p_r Pointer to the head node of the row list

	\return A new list which is the transpose of the given row list
*/
RowNode_handle transpose(ConstRowNode_handle p_r);

/*!
	\fn RowNode_handle mult(ConstRowNode_handle p_r1,ConstRowNode_handle p_r2)
	\brief Multiples 2 matrices represented by 2 row lists and returns a new \
	product list

	\param p_r1 Pointer to the head node of the first list
	\param p_r2 Pointer to the head node of the second list

	\return a new row list of the product
*/
RowNode_handle mult(ConstRowNode_handle p_r1,ConstRowNode_handle p_r2);

/*!
	\fn void free_rows(RowNode_handle p_r)
	\brief Deletes all rows of a row list and frees the memory

	\param p_r Pointer to the head node of the row list
*/
void free_rows(RowNode_handle p_r);

/*!
	\fn int determinant(ConstRowNode_handle p_r, int dim)
	\brief Calculates the determinant of the matrix represented by the row list

	\param p_r Pointer to the head node of the row list
	\param dim Dimension of the matrix represented by the row list

	\return determinant of the matrix
*/
int determinant(ConstRowNode_handle p_r,int dim); 

#endif

