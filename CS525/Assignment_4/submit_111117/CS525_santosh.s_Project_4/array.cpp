#include "array.h"

/*!
	\fn CS225::Array::Array(int * array, unsigned int _size, const ElementFactory* _pElementFactory)
	\brief Conversion constructor

	\param array pointer to an array of ints
	\param _size size of the array
	\param _pElementFactory pointer to the factory object which constructs the elements
*/
CS225::Array::Array(int * array, 
	unsigned int _size, 
	const ElementFactory* _pElementFactory
	): 
data(new AbstractElement*[_size]),
	size(_size),
	pElementFactory(_pElementFactory),
	refs(new int(1))
{
	for (unsigned int i=0;i<size;++i) 
	{
		//create Element with id = 1 (that is Element1 )
		data[i] = pElementFactory->MakeElement( 1, array[i] );
	}
}

/*!
	\fn CS225::Array::Array(const Array& orig)
	\brief Copy Constructor

	\param orig original instance
*/
CS225::Array::Array(const Array& orig):
data(orig.data),
	size(orig.size),
	pElementFactory(orig.pElementFactory),
	refs(orig.refs)
{
	++*refs;
}

/*!
	\fn CS225::Array::~Array()
	\brief Destructor
*/
CS225::Array::~Array()
{
	if(--*refs < 1)
	{
		deleteData();
		delete refs;
	}
}

/*!
	\fn CS225::Array& CS225::Array::operator=(const Array& rhs)
	\brief Assignment operator

	\param rhs reference of the RHS instance

	\return reference to self
*/
CS225::Array& CS225::Array::operator=(const Array& rhs)
{
	if(this != &rhs)
	{
		if(--*refs < 1)
		{
			deleteData();
			delete refs;
		}
		//
		data = rhs.data;
		size = rhs.size;
		pElementFactory = rhs.pElementFactory;
		refs = rhs.refs;
		++*refs;
	}
	return *this;
}

/*!
	\fn void CS225::Array::deleteData()
	\brief Internal helper function used to delete all the data
*/
void CS225::Array::deleteData()
{
	for(unsigned int i = 0; i < size; ++i)
	{
		delete data[i];
	}
	delete[] data;
}

/*!
	\fn int CS225::Array::Get(unsigned int pos) const
	\brief Returns the value at the specified position

	\param pos Position of the element whose value is to be read

	\return Value at the specified position
*/
int CS225::Array::Get(unsigned int pos) const { return data[pos]->Get(); }

/*!
	\fn void CS225::Array::Set(int id, int pos, int value) 
	\brief Changes the value of the element or the element type itself

	\param id Desired element type
	\param pos Position of the element to be modified
	\param value New value

	\return nothing
*/
void CS225::Array::Set(int id, int pos, int value) 
{ 
	if(*refs > 1)
	{
		deepCopy();
	}
	//setter will delete the old and create new 
	//slightly inefficient if new and old have the same type
	delete data[pos];
	data[pos] = pElementFactory->MakeElement(id,value); 
}

/*!
	\fn void CS225::Array::deepCopy()
	\brief Internal helper function used to create a copy of the data object
*/
void CS225::Array::deepCopy()
{
	AbstractElement** arr = new AbstractElement*[size];
	for(unsigned int i = 0; i < size; ++i)
	{
		arr[i] = (data[i] -> clone());
	}
	data = arr;
	--*refs;
	refs = new int(1);
}

/*!
	\fn void CS225::Array::Print() const 
	\brief Prints the Array object
*/
void CS225::Array::Print() const {
	for (unsigned int i=0;i<size;++i) data[i]->Print(); 
	std::cout << std::endl;
}
