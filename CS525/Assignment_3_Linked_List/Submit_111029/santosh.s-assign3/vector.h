#include <iostream>

/*!
	\namespace CS225
	\brief The namespace which is the container for the sparse vector implementation
*/
namespace CS225
{
	/*!
		\struct ElementNode
		\brief structure of a a node whose list is to be built
	*/
	struct ElementNode
	{
		int data;
		int pos;
		struct ElementNode *next;
	};

	//! forward declaration
	class ElementProxy;

	/*!
		\class SparseVector
		\brief The linked list class which implements the sparse vector features
	*/
	class SparseVector
	{
		//! Public methods and properties
		public:
			//! Ctors/Dtors
			SparseVector(void);						//! Default Ctor
			SparseVector(const SparseVector& rhs);	//! Copy Ctor
			~SparseVector(void);					//! Dtor
			//
			//! Overloaded Operators
			SparseVector& operator= (const SparseVector& rhs);
			ElementProxy operator[] (long pos);
			int operator[] (long pos) const;
			const SparseVector operator+ (const SparseVector& rhs) const;
			const SparseVector operator- (const SparseVector& rhs) const;
			const SparseVector operator* (const int& rhs) const;
			int operator* (const SparseVector& rhs) const;
			//
			//! Other methods
			int Get(long pos) const;
			void Insert(int val, long pos);
			void Delete(long pos);
			void PrintRaw () const
			{
				ElementNode* curr=pHead;
				std::cout << "Raw vector: ";
				while (curr)
				{
					std::cout << "(" << curr->data << ", " << curr->pos << ")";
					curr=curr->	next;
				}
				std::cout << std::endl;
			}
			//
			//! Public Properties (none)
			//
			//! Friend functions
			friend std::ostream& operator<<(std::ostream &out, const SparseVector &v);//implemented
			friend const SparseVector operator* (const int& lhs, const SparseVector& rhs);
		//! Private methods and properties
		private:
			//
			//! Private Properties
			ElementNode* pHead;
			long dimension;
			//
			//! Private methods
			void deleteAllElements(void);
	};
	//
	/*!
		\class ElementProxy
		\brief A proxy class which will be used for implementing the '[]' behavior of the SparseVector
	*/
	class ElementProxy
	{
		//! Public methods and properties
		public:
			//! Ctors, Dtors
			ElementProxy(SparseVector& pV, long lPos);		//! Ctor
			~ElementProxy(void);							//! Dtor
			//
			//! Overloaded Operators
			operator int() const;
			const int& operator= (const int& val) const;
			ElementProxy& operator= (const ElementProxy& rhs);
			int operator+= (const int& val) const;
			int operator-= (const int& val) const;
			int operator*= (const int& val) const;
			int operator/= (const int& val) const;
			int operator%= (const int& val) const;
			int operator++ (void) const;
			int operator++ (int) const;
			int operator-- (void) const;
			int operator-- (int) const;
		//
		//! Private methods and properties
		private:
			SparseVector &v;
			long pos;
	};
	//
	//! Global overloaded operator functions
	extern std::ostream& operator<<(std::ostream &out, const SparseVector &v);
	extern const SparseVector operator* (const int& lhs, const SparseVector& rhs);
}

