#include "vector.h"

using namespace CS225;
// -----------------------------------------------------------------------------------------------
/*!
	\fn ElementProxy::ElementProxy(SparseVector& pV, long lPos)
	\brief Default constructor

	\param pV A reference to an instance of SparseVector which wants to use ElementProxy
	\param lPos Position of the element in the list
*/
ElementProxy::ElementProxy(SparseVector& pV, long lPos):
	v(pV),
	pos(lPos)
{
	//
}

/*!
	\fn ElementProxy::~ElementProxy(void)
	\brief Dtor
*/
ElementProxy::~ElementProxy(void)
{
	//
}

/*!
	\fn ElementProxy::operator int() const
	\brief returns the int value of the ElementProxy instance
*/
ElementProxy::operator int() const
{
	return v.Get(pos);
}

/*!
	\fn const int& ElementProxy::operator= (const int& val) const
	\brief Assignment operator

	\param val Reference to the int value whose value is to be assigned to the SparseVector element

	\return The assigned value
*/
const int& ElementProxy::operator= (const int& val) const
{
	v.Insert(val, pos);
	return val;
}

/*!
	\fn ElementProxy& ElementProxy::operator= (const ElementProxy& rhs)
	\brief Assignemnt operator

	\param rhs Refernce to RHS ElementProxy instance whose value is to be copied into the SparseVector element

	\return reference to self
*/
ElementProxy& ElementProxy::operator= (const ElementProxy& rhs)
{
	v.Insert(int(rhs), pos);
	return *this;
}

/*!
	\fn int ElementProxy::operator+= (const int& val) const
	\brief Self increment operator

	\param val int value by which the current element value is to be incremented

	\return new incremented value
*/
int ElementProxy::operator+= (const int& val) const
{
	int t;
	v.Insert((t = v.Get(pos) + val), pos);
	return t;
}

/*!
	\fn int ElementProxy::operator-= (const int& val) const
	\brief Self decrement operator

	\param val int value by which the current element value is to be decremented

	\return new decremented value
*/
int ElementProxy::operator-= (const int& val) const
{
	int t;
	v.Insert((t = v.Get(pos) - val), pos);
	return t;
}

/*!
	\fn int ElementProxy::operator*= (const int& val) const
	\brief self multiplication operator

	\param val int value by which the current element value is to be multiplied

	\return new multiplied value
*/
int ElementProxy::operator*= (const int& val) const
{
	int t;
	v.Insert((t = v.Get(pos) * val), pos);
	return t;
}

/*!
	\fn int ElementProxy::operator/= (const int& val) const
	\brief Self division operator

	\param val int value by which the current element value is to be divided

	\return new divided value
*/
int ElementProxy::operator/= (const int& val) const
{
	int t;
	v.Insert((t = v.Get(pos) / val), pos);
	return t;
}

/*!
	\fn int ElementProxy::operator%= (const int& val) const
	\brief self modulus operator

	\param val int value by which the current element value is to be modulus operated

	\return new modulus value
*/
int ElementProxy::operator%= (const int& val) const
{
	int t;
	v.Insert((t = v.Get(pos) % val), pos);
	return t;
}

/*!
	\fn int ElementProxy::operator++ (int) const
	\brief Post increment operator

	\return old value
*/
int ElementProxy::operator++ (int) const
{
	int val = v.Get(pos);
	v.Insert(val + 1, pos);
	return val;
}

/*!
	\fn int ElementProxy::operator++ (void) const
	\brief Pre increment operator

	\return new incremented value
*/
int ElementProxy::operator++ (void) const
{
	int val = v.Get(pos) + 1;
	v.Insert(val, pos);
	return val;
}

/*!
	\fn int ElementProxy::operator-- (int) const
	\brief Post decrement operator

	\return old value
*/
int ElementProxy::operator-- (int) const
{
	int val = v.Get(pos);
	v.Insert(val - 1, pos);
	return val;
}

/*!
	\fn int ElementProxy::operator-- (void) const
	\brief Pre decrement operator

	\return new decremented value
*/
int ElementProxy::operator-- (void) const
{
	int val = v.Get(pos) - 1;
	v.Insert(val, pos);
	return val;
}
// ---------------------------------------------------------------------------------------
/*!
	\fn SparseVector::SparseVector(void)
	\brief Default Ctor
*/
SparseVector::SparseVector(void):
	pHead(0),
	dimension(0)
{
	//
}
	
/*!
	\fn SparseVector::SparseVector(const SparseVector& rhs)
	\brief Copy Constructor
*/
SparseVector::SparseVector(const SparseVector& rhs):
	pHead(0),
	dimension(0)
{
	ElementNode *curr = rhs.pHead;
	while(curr)
	{
		Insert(curr->data, curr->pos);
		curr = curr->next;
	}
}
	
/*!
	\fn SparseVector::~SparseVector(void)
	\brief Dtor
*/
SparseVector::~SparseVector(void)
{
	deleteAllElements();
}

/*!
	\fn void SparseVector::Insert(int val, long pos)
	\brief Inserts a new int value at a specified position. If an element already exists at the specified pos then its value is changed. If the value to be inserted is 0 then the node is deleted, if it exists

	\param val New int value to be inserted
	\param pos The position where the value is to be inserted or changed

	\return none
*/
void SparseVector::Insert(int val, long pos)
{
	int found = 0;
	ElementNode *pNode = NULL, *curr = pHead, *prev = 0;

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
			pNode = new ElementNode();
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
					pHead = pNode;
				}
				++dimension;
			}
			else
			{
				std::cout<<"[ERROR]: spvector::insert_element: error allocating dynamic memory\n";
			}
		}
	}
	/*! If there is a node present then delete it if valuse is 0 or change it if \
	the value is not zero */
	else 
	{
		if(!val)
		{
			Delete(pos);
			pNode = NULL;
		}
		else
		{
			pNode -> data = val;
		}
	}

	/*! If the Head pointer is pointing to 0 memory location then change it so \
	that it points to the new node */
	if(!pHead)
	{
		pHead = pNode;
	}
}

/*!
	\fn void SparseVector::Delete(long pos)
	\brief Deletes a spcific node at a given position

	\param pos the position of the node to be deleted

	\return none
*/
void SparseVector::Delete(long pos)
{
	ElementNode *curr = pHead, *prev = NULL;
	
	while (curr && curr -> pos < pos)
	{
		prev = curr;
		curr = curr -> next;
	}
	if(curr && curr -> pos == pos)
	{
		if (prev == NULL)
		{
			pHead = pHead -> next;
		}
		else 
		{
			prev -> next = curr -> next;
		}
		delete(curr);
		--dimension;
	}
}

/*!
	\fn int SparseVector::Get(long pos) const
	\brief returns the int value at a specified node

	\param pos the node positiin whose value is to be read

	\return the int value at the given position
*/
int SparseVector::Get(long pos) const
{
	ElementNode *curr = pHead;
	
	while (curr && curr -> pos < pos)
	{
		curr = curr -> next;
	}
	if(curr && curr -> pos == pos)
	{
		return curr->data;
	}
	return 0;
}

/*!
	\fn void SparseVector::deleteAllElements(void)
	\brief deletes all nodes in the list

	\return none
*/
void SparseVector::deleteAllElements(void)
{
	ElementNode *pTemp;
	while(pHead != NULL)
	{
		pTemp = pHead -> next;
		Delete(pHead -> pos);
		pHead = pTemp;
	}
}

/*!
	\fn ElementProxy SparseVector::operator[] (long pos)
	\brief creates and returns a new instance of ElementProxy which can be used later in an expression to change the value of the node it represents

	\param pos The position of the node which could be changed

	\return a new instance of ElementProxy
*/
ElementProxy SparseVector::operator[] (long pos)
{
	return ElementProxy(*this, pos);
}

/*!
	\fn int SparseVector::operator[](long pos) const
	\brief returns the value at a given position

	\param pos The position of the node whose value is to be read

	\return value at the given position
*/
int SparseVector::operator[](long pos) const
{
	return Get(pos);
}

/*!
	\fn SparseVector& SparseVector::operator= (const SparseVector& rhs)
	\brief Assignment operator

	\param rhs An instance of the SparseVector whose node will be copied to self

	\return refernce to self
*/
SparseVector& SparseVector::operator= (const SparseVector& rhs)
{
	if(this != &rhs)
	{
		ElementNode *curr = rhs.pHead;
		deleteAllElements();
		while(curr)
		{
			Insert(curr->data, curr->pos);
			curr = curr->next;
		}
		}
	//
	return *this;
}

/*!
	\fn const SparseVector SparseVector::operator+ (const SparseVector& rhs) const
	\brief Adds 2 sparseVector instances and returns a new instance

	\param rhs RHS SparseVector instance which needs to be added to self

	\return A new instance of SparseVector whose nodes represent the addition of corresponding nodes of self and rhs
*/
const SparseVector SparseVector::operator+ (const SparseVector& rhs) const
{
	SparseVector sv;
	ElementNode *curr = pHead;
	int sum;
	//
	while(curr)
	{
		sv.Insert(curr->data, curr->pos);
		curr = curr -> next;
	}
	curr = rhs.pHead;
	while(curr)
	{
		sum =  Get(curr -> pos);
		sum += (curr -> data);
		sv.Insert(sum, curr -> pos);
		curr = curr -> next;
	}
	//
	return sv;
}

/*!
	\fn const SparseVector SparseVector::operator- (const SparseVector& rhs) const
	\brief Subtracts a sparseVector instance from self and returns a new instance

	\param rhs RHS SparseVector instance which needs to be subtracted from self

	\return A new instance of SparseVector whose nodes represent the subtractiosn of corresponding nodes of self and rhs
*/
const SparseVector SparseVector::operator- (const SparseVector& rhs) const
{
	SparseVector sv;
	ElementNode *curr = pHead;
	int sum;
	//
	while(curr)
	{
		sv.Insert(curr->data, curr->pos);
		curr = curr -> next;
	}
	curr = rhs.pHead;
	while(curr)
	{
		sum =  Get(curr -> pos);
		sum -= (curr -> data);
		sv.Insert(sum, curr -> pos);
		curr = curr -> next;
	}
	//
	return sv;
}

/*!
	\fn const SparseVector SparseVector::operator* (const int& rhs) const
	\brief multiplies an RHS SparseVector and self returns the resultant SparseVector instance

	\param rhs RHS SparseVector instance which is to be multiplied with self

	\return a new instance of SparseVector whose nodes represent the multiplication of corresponding nodes of self and RHS
*/
const SparseVector SparseVector::operator* (const int& rhs) const
{
	SparseVector sv;
	ElementNode *curr = pHead;
	//
	while(curr)
	{
		sv.Insert(rhs * (curr->data), curr->pos);
		curr = curr -> next;
	}
	//
	return sv;
}

/*!
	\fn int SparseVector::operator* (const SparseVector& rhs) const
	\brief Calculates dot product of a given RHS SparseVector and self

	\param rhs RHS SparseVector instance which is to be used to calculate the Dot product

	\return Dot product value
*/
int SparseVector::operator* (const SparseVector& rhs) const
{
	int sum = 0;
	ElementNode *curr = pHead;
	//
	while(curr)
	{
		sum += (curr -> data) * rhs.Get(curr -> pos);
		curr = curr -> next;
	}
	return sum;
}

// -----------------------------------------------------------------------------------------------
/*!
	\fn std::ostream& CS225::operator<<(std::ostream &out, const CS225::SparseVector &v)
	\brief Overloaded << operator which is used to print a SparseVector on an output stream

	\param out The output steam where the value of the SparseVector is to printed
	\param v Instance of SparseVector which is to be printed

	\return  refernce to the modified out stream
*/
std::ostream& CS225::operator<<(std::ostream &out, const CS225::SparseVector &v)
{
	int i,last_pos=-1; 
	CS225::ElementNode* p_e=v.pHead; 
	while (p_e)
	{ 
		for (i=last_pos+1;i<p_e->pos;++i) out << " " << "0"; 
		out << " " << p_e->data; 
		last_pos=p_e->pos; 
		p_e = p_e->next; 
	} 
	for (i=last_pos+1;i<v.dimension;++i) out << " " << "0"; 

	return out; 
}

/*!
	\fn const SparseVector CS225::operator* (const int& lhs, const SparseVector& rhs)
	\brief This overloaded * operator function is used to implement expressions of the sort: (int value) * (SparseVector instance)

	\param lhs LHS int value which is to multiplied with a given instance of SparseVector
	\param rhs RHS SarseVector instance whose nodes are to be multiplied with an int value

	\return A new instance of SparseVector whose nodes represent the scalar multiplication of the corresponding nodes of RHS and LHS int value
*/
const SparseVector CS225::operator* (const int& lhs, const SparseVector& rhs)
{
	SparseVector sv;
	ElementNode *curr = rhs.pHead;
	//
	while(curr)
	{
		sv.Insert(lhs * (curr->data), curr->pos);
		curr = curr -> next;
	}
	//
	return sv;
}
