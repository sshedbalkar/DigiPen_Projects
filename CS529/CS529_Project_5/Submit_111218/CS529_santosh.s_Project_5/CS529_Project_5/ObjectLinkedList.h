///////////////////////////////////////////////////////////////////////////////
///
///	\file ObjectLinkedList.h
///	
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#ifndef OBJECT_LINK_LIST_H
#define OBJECT_LINK_LIST_H
//#pragma message("Compiling: "__FILE__)
//
namespace Wye
{
	///	Intrusively linked list for objects. This enables the ability to store 
	/// objects in an efficient linked list structure without duplicating code. 
	/// Object to be used in this container must have Prev and Next pointers as
	/// members.
	template<typename type>
	class ObjectLinkList
	{
#pragma message("Including Class: ObjectLinkList")
	public:
		typedef type* pointer;
		typedef type& reference;

		ObjectLinkList():
			_first(NULL),
			_last(NULL),
			_objectCount(0)
		{
			//
		}

		//Pop the front element. If the list is empty return NULL.
		pointer pop_front()
		{
			if(_first==NULL)
			{
				//List is empty
				return NULL;
			}
			else
			{
				//get the first object
				type* first = _first;
				_first = first->Next;
			
				//Update linked list pointers
				if(_first)
				{
					//more elements in list
					_first->Prev = NULL;
				}
				else
				{
					//popped last object in list
					_last = NULL;
				}
				//
				--_objectCount;
				return first;
			}
		}
		//
		void push_back(pointer object)
		{
			++_objectCount;
			//
			if( _last == NULL )
			{
				//list was empty
				_first = object;
				_last = object;
				object->Next = NULL;
				object->Prev = NULL;
			}
			else
			{
				object->Prev = _last;
				object->Next = NULL;
				_last->Next = object;
				_last = object;
			}
		}
		//
		void erase(pointer object)
		{
			--_objectCount;
			//
			if( object->Next == NULL && object->Prev == NULL)
			{
				//Only object in list make list empty
				_last = NULL;
				_first = NULL;
			}
			else if( object->Next == NULL )
			{
				//Object is last update Last
				_last = object->Prev;
				if( _last ) _last->Next = NULL;

			}
			else if( object->Prev == NULL )
			{
				//Object is first update first
				_first = object->Next;
				if( _first ) _first->Prev = NULL;
			}
			else
			{
				//Object is in middle just update pointers
				object->Prev->Next = object->Next;
				object->Next->Prev = object->Prev;
			}
		}
		//
		///Intrusive linked list iterator
		class iterator
		{
		public:
			friend class ObjectLinkList;
			iterator(){};
			iterator(pointer p): _item(p) {}
			void operator--(){_item=_item->Prev;}
			void operator++(){_item=_item->Next;}
			reference operator*(){return *_item;}
			pointer operator->(){return _item;}
			bool operator==(const iterator& i){return _item==i._item;}
			bool operator!=(const iterator& i){return _item!=i._item;}
			operator bool(){return _item!=NULL;}
			operator pointer(){return _item;}
			pointer GetPtr(){return _item;}
		private:
			pointer _item;
		};
		//
		iterator begin(){ return _first; }
		iterator end(){ return NULL;}
		pointer last(){ return _last; }
		unsigned size(){ return _objectCount;}
	private:
		pointer _first;
		pointer _last;
		unsigned _objectCount;
	};
}
#endif
