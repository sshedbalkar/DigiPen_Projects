#ifndef ELEMENT1_H
#define ELEMENT1_H
#include <iostream>
#include "abstract-element.h"

namespace CS225 {
/*!
	\class Element1
	\brief A concrete class implementing AbstractElement interface, Trivial.
*/
  class Element1 : public AbstractElement {
    public:
      Element1(int _val);
	  Element1(const Element1& orig);
	  virtual ~Element1();
	  virtual Element1* clone() const;
	  Element1& operator=(const Element1& rhs);
      virtual int Get() const;
      virtual void Set(int new_val);
      virtual void Print() const;
    private:
      int val;
  };
}
#endif
