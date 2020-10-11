#include <stdlib.h>
#include <stdio.h>
#include "spvector.h"

/*!
	\struct ElementNode
	\brief Data structure for a basic node of storing data based on its position

	\param data Integer data to be stored
	\param pos Position of the node in the sparse list
	\param next Pointer to the next node
*/
struct ElementNode {
  int    data;
  int    pos;
  struct ElementNode* next;
};

/*!
	\struct RowNode
	\brief Data structure for storing a list of ElementNode

	\param pos position of the row node in the RowNode list
	\param elements An ElementNode pointer to the first node of a list of ElementNode
	\param next pointer to the next row node
*/
struct RowNode {
  int pos;
  struct ElementNode* elements;
  struct RowNode* next;
};

typedef struct ElementNode ElementNode;
typedef struct RowNode RowNode;

/*!
	\fn int lowOrderDeterminant(ConstRowNode_handle, int);
	\brief Prototype of a function that is used to calculate the determinant of 1 \
	order or 2 order matrix
*/
int lowOrderDeterminant(ConstRowNode_handle, int);

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
int insert_element(ElementNode_handle *p_e,int pos,int val)
{
	int ret = 0, found = 0;
	struct ElementNode *pNode = NULL, *curr = *p_e, *prev = 0;

	/*! Find out if there is already a node present at the specified position*/
	if(curr)
	{
		while(!found && curr && curr -> pos <= pos)
		{
			if(curr -> pos == pos)
			{
				pNode = curr;
				found = 1;
			}
			prev = curr;
			curr = curr -> next;
		}
	}

	/*! If there is no node present at the position then insert one */
	if(!found)
	{
		if(val)
		{
			pNode = (struct ElementNode *) malloc(sizeof(struct ElementNode));
			if(pNode)
			{
				pNode -> data = val;
				pNode -> next = curr;
				pNode -> pos = pos;
				if(prev)
				{
					prev -> next = pNode;
				}
				else
				{
					*p_e = pNode;
				}
			}
			else
			{
				printf("[ERROR]: spvector::insert_element: error allocating dynamic memory\n");
				ret = 1;
			}
		}
	}
	/*! If there is a node present then delete it if valuse is 0 or change it if \
	the value is not zero */
	else 
	{
		if(!val)
		{
			delete_element(p_e, pos);
			pNode = NULL;
		}
		else
		{
			pNode -> data = val;
		}
	}

	/*! If the Head pointer is pointing to 0 memory location then change it so \
	that it points to the new node */
	if(!(*p_e))
	{
		*p_e = pNode;
	}
	return ret;
}

/*!
	\fn void free_elements(ConstElementNode_handle p_e)
	\brief Frees all the nodes of a linked list

	\param p_e Pointer to the head node of the list
*/
void free_elements(ConstElementNode_handle p_e)
{
	struct ElementNode *temp;
	
	while(p_e)
	{
		temp = p_e -> next;
		free((ElementNode *) p_e);
		p_e = temp;
	}
}

/*!
	\fn void delete_element(ElementNode_handle *p_e,int pos)
	\brief Deletes a node present at a specifed position

	\param p_e Pointer to the head node of the list
	\param pos Position of the node to be deleted
*/
void delete_element(ElementNode_handle *p_e,int pos)
{
	struct ElementNode *curr = *p_e, *prev = NULL;
	
	while (curr && curr -> pos != pos)
	{
		prev = curr;
		curr = curr -> next;
	}
	if(curr != NULL)
	{
		if (prev == NULL)
		{
			(*p_e) = (*p_e) -> next;
		}
		else 
		{
			prev -> next = curr -> next;
		}
		free(curr);
	}
}

/*!
	\fn int scalar_product(ConstElementNode_handle p_e1,ConstElementNode_handle p_e2)
	\brief Calculates the scalar product of 2 ElementNode lists

	\param p_e1 First list
	\param p_e2 Second list

	\return Scalar product value of the 2 lists
*/
int scalar_product(ConstElementNode_handle p_e1,ConstElementNode_handle p_e2)
{
	int sum = 0;
	signed int val2;
	
	while(p_e1)
	{
		if((val2 = get(p_e2, p_e1 -> pos)) != -1)
		{
			sum += (p_e1 -> data) * val2;
		}
		p_e1 = p_e1 -> next;
	}
	return sum;
}

/*!
	\fn int get(ConstElementNode_handle p_e, int pos)
	\brief Returns the value at the specific position

	\param p_e Pointer to the head node of the list
	\param pos Position of the node in the list whose value is to be read

	\return Value present at the node. If there is no node present at the position \
	then return 0
*/
int get(ConstElementNode_handle p_e, int pos)
{
	int ret = 0;

	while (p_e && p_e -> pos <= pos)
	{
		if(p_e -> pos == pos)
		{
			ret = p_e -> data;
		}
		p_e = p_e -> next;
	}
	return ret;
}

/*!
	\fn ElementNode_handle add(ConstElementNode_handle p_e1,ConstElementNode_handle p_e2)
	\brief Creates a new list whose elements denote the sum of the elements of 2 lists

	\param p_e1 First list
	\param p_e2 Second list

	\return A new list of the sum
*/
ElementNode_handle add(ConstElementNode_handle p_e1,ConstElementNode_handle p_e2)
{
	ElementNode_handle result = 0;
	int sum, val2;
	
	while(p_e1)
	{
		insert_element(&result, p_e1 -> pos, p_e1 -> data);
		p_e1 = p_e1 -> next;
	}
	while(p_e2)
	{
		val2 = get(result, p_e2 -> pos);
		sum = val2 + (p_e2 -> data);
		insert_element(&result, p_e2 -> pos, sum);
		p_e2 = p_e2 -> next;
	}
	return result;
}

/*!
	\fn int insert_row(RowNode_handle *p_r,int pos,ElementNode_handle p_e)
	\brief Inserts a list of elements at position pos.\
	If a row at position pos already exists, do not insert, return 2 \
	If the list p_e is empty, do nothing

	\param p_r Pointer to the head node of the row list
	\param pos The position at which the row is to be inserted
	\param p_e New elements list to be inserted in the row list
	
	\return 1 If malloc fails \
			2 If position po already exists \
			0 Otherwise
*/
int insert_row(RowNode_handle *p_r,int pos,ElementNode_handle p_e)
{
	int ret = 0, found = 0;
	struct RowNode *pNode = NULL, *curr = *p_r, *prev = 0;

	if(curr)
	{
		while(!found && curr && curr -> pos <= pos)
		{
			if(curr -> pos == pos)
			{
				pNode = curr;
				found = 1;
			}
			prev = curr;
			curr = curr -> next;
		}
	}
	if(!found)
	{
		if(p_e)
		{
			pNode = (struct RowNode *) malloc(sizeof(struct RowNode));
			if(pNode)
			{
				pNode -> elements = p_e;
				pNode -> next = curr;
				pNode -> pos = pos;
				if(prev)
				{
					prev -> next = pNode;
				}
				else
				{
					*p_r = pNode;
				}
			}
			else
			{
				printf("[ERROR]: spvector::insert_row: error allocating dynamic memory\n");
				ret = 1;
			}
		}
	}
	else 
	{
		if(!p_e)
		{
			pNode = NULL;
		}
		else
		{
			pNode -> elements = p_e;
		}
		ret = 2;
	}
	if(!(*p_r))
	{
		*p_r = pNode;
	}
	return ret;
}

/*!
	\fn int insert_element2(RowNode_handle *pp_r,int pos_row,int pos_col,int val)
	\brief Inserts an element at the specified (row, col) position in a row list \
		If there is already a node present then it is updated/deleted if the value \
		is non-zero/zero

	\param pp_r Pointer to the head node of the row list
	\param pos_row Position of the row in the list
	\param pos_col Position of the node in the elemets list
	\param val The value to be inserted

	\return 0 If the insertion was successful, 1 otherwise
*/
int insert_element2(RowNode_handle *pp_r,int pos_row,int pos_col,int val)
{
	int ret;
	struct RowNode *curr = *pp_r, *prev = 0;
	struct ElementNode *p_e = 0;

	while(curr && curr -> pos < pos_row)
	{
		prev = curr;
		curr = curr -> next;
	}
	if(curr && curr -> pos != pos_row)
	{
		curr = NULL;
	}
	if(curr == NULL)
	{
		ret = insert_element(&p_e, pos_col, val);
		insert_row(pp_r, pos_row, p_e);
		curr = find_row(pp_r, pos_row);
	}
	else
	{
		ret = insert_element(&(curr -> elements), pos_col, val);
		p_e = curr -> elements;
	}
	if(!p_e)
	{
		if(curr)
		{
			if(prev)
			{
				prev -> next = curr -> next;
			}
			else
			{
				*pp_r = curr -> next;
			}
			free(curr);
		}
	}
	return ret;
}

/*!
	\fn RowNode_handle find_row(RowNode_handle *pp_r,int pos)
	\brief returns a pointer to the row present at a specific position in a row list

	\param pp_r Pointer to the head node of a row list
	\param pos Position of the row in the list

	\return Pointer to the row if the row is present, NULL otherwise
*/
RowNode_handle find_row(RowNode_handle *pp_r,int pos)
{
	struct RowNode *temp = *pp_r;
	RowNode_handle ptr = 0;
	
	while(temp && temp -> pos != pos)
	{
		temp = temp -> next;
	}
	if(temp != NULL)
	{
		ptr = temp;
	}
	return ptr;
}

/*!
	\fn RowNode_handle transpose(ConstRowNode_handle p_r)
	\brief Creates a new row list of the transpose of the matrix represented by \
	the given list

	\param p_r Pointer to the head node of the row list

	\return A new list which is the transpose of the given row list
*/
RowNode_handle transpose(ConstRowNode_handle p_r)
{
	RowNode_handle pList = 0;
	ElementNode *p_e;
	
	while(p_r)
	{
		p_e = p_r -> elements;
		while(p_e)
		{
			insert_element2(&pList, p_e -> pos, p_r -> pos, p_e -> data);
			p_e = p_e -> next;
		}
		p_r = p_r -> next;
	}
	return pList;
}

/*!
	\fn RowNode_handle mult(ConstRowNode_handle p_r1,ConstRowNode_handle p_r2)
	\brief Multiples 2 matrices represented by 2 row lists and returns a new \
	product list

	\param p_r1 Pointer to the head node of the first list
	\param p_r2 Pointer to the head node of the second list

	\return a new row list of the product
*/
RowNode_handle mult(ConstRowNode_handle p_r1,ConstRowNode_handle p_r2)
{
	RowNode_handle p_r = 0, p_r2T, second;
	int prod;
	
	p_r2T = transpose(p_r2);
	while(p_r1)
	{
		second = p_r2T;
		while(second)
		{
			prod = scalar_product(p_r1 -> elements, second -> elements);
			insert_element2(&p_r, p_r1 -> pos, second -> pos, prod);
			second = second -> next;
		}
		p_r1 = p_r1 -> next;
	}
	free_rows(p_r2T);
	return p_r;
}

/*!
	\fn void free_rows(RowNode_handle p_r)
	\brief Deletes all rows of a row list and frees the memory

	\param p_r Pointer to the head node of the row list
*/
void free_rows(RowNode_handle p_r)
{
	struct RowNode *temp;
	
	while(p_r)
	{
		temp = p_r -> next;
		free_elements(p_r -> elements);
		free((void *) p_r);
		p_r = temp;
	}
}

/*print functions*/
void printf_elements(ConstElementNode_handle p_e, char *fmt, int dim) {
	int i,last_pos=-1;
	/*printf("printf p_e:%p\n", p_e);*/
	while (p_e) {
		for (i=last_pos+1;i<p_e->pos;++i) { printf(fmt,0); }
		printf(fmt,p_e->data);
		last_pos=p_e->pos;
		p_e = p_e->next;
	}
	for (i=last_pos+1;i<dim;++i) { printf(fmt,0); }
}

void print_elements(ConstElementNode_handle p_e) {
	while (p_e) {
		printf("%i at pos %i, ",p_e->data,p_e->pos);
		p_e = p_e->next;
	}
}

void print_rows(ConstRowNode_handle p_r) {
	while (p_r) {
		printf("row %i: ", p_r->pos);
		print_elements(p_r->elements);
		printf("\n");
		p_r = p_r->next;
	}
}

void printf_rows(ConstRowNode_handle p_r, char *fmt, int dim) {
	int i,j,last_pos=-1;
	while (p_r) {
		for (i=last_pos+1;i<p_r->pos;++i) { /* output row of 0's */
			for (j=0;j<dim;++j) printf(fmt,0); 
			printf("\n");
		}
		printf_elements(p_r->elements,fmt,dim);
		printf("\n");
		last_pos=p_r->pos;
		p_r = p_r->next;
	}
	for (i=last_pos+1;i<dim;++i) { /* output row of 0's */
		for (j=0;j<dim;++j) printf(fmt,0); 
		printf("\n");
	}
}

/*!
	\fn int determinant(ConstRowNode_handle p_r, int dim)
	\brief Calculates the determinant of the matrix represented by the row list

	\param p_r Pointer to the head node of the row list
	\param dim Dimension of the matrix represented by the row list

	\return determinant of the matrix
*/
int determinant(ConstRowNode_handle p_r, int dim)
{
	int i, j, i_count, j_count, count, det = 0, val, sign;
	RowNode_handle arr = 0, pTempRow;
	
	if(dim < 1)
	{
		printf("[ERROR]: spvector::determinant: Invalid Dimension");
	}
	else if(p_r)
	{
		/*! 
			\brief If the diemnsion of the matrix is 1 or 2 then return the determinant
		*/
		if(dim < 3) 
		{
			det = lowOrderDeterminant(p_r, dim);
		}
		else
		{
			/*!
				\brief Code to create a new row list of a minor of the matrix to \
				call the function recursively to find out the determinant
			*/
			count = 0;
			while(count < dim)
			{
				i_count = 0;
				i = 1;
				while(i < dim)
				{
					j_count = j = 0;
					while(j < dim)
					{
						if(j == count)
						{
							++j;
							continue;
						}
						pTempRow = find_row((RowNode_handle *) &p_r, i);
						val = (pTempRow) ? get(pTempRow -> elements, j) : 0;
						insert_element2(&arr, i_count, j_count, val);
						++j_count;
						++j;
					}
					++i_count;
					++i;
				}
				val = get(p_r -> elements, count);
				sign = (count % 2 == 0) ? 1 : -1;
				det += sign * val * determinant(arr, dim-1);

				free_rows(arr);
				arr = 0;

				++count;
			}
		}
	}
	return det;
}

/*!
	\fn int lowOrderDeterminant(ConstRowNode_handle, int);
	\brief Calculates the determinant of 1 order or 2 order matrix

	\param p_r Pointer to the head node of the row list
	\param dim Dimension of the matrix represented by the row list

	\return determinant value
*/
int lowOrderDeterminant(ConstRowNode_handle p_r,int dim)
{
	int det = 0, e00, e01, e10, e11;
	ConstRowNode_handle r0 = 0, r1 = 0;
	
	if(dim == 1)
	{
		det = (p_r -> elements) -> data;
	}
	else if(dim == 2)
	{
		if(p_r)
		{
			r0 = p_r;
			r1 = p_r -> next;
		}
		
		if(r0 && r1)
		{
			e00 = get(r0 -> elements, 0);
			e01 = get(r0 -> elements, 1);
			e10 = get(r1 -> elements, 0);
			e11 = get(r1 -> elements, 1);
			det = e00 * e11 - e01 * e10;
		}
	}

	return det;
}
