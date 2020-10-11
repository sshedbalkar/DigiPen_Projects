#ifndef ELEMENT2_H
#define ELEMENT2_H
#include <iostream>
#include "abstract-element.h"


namespace CS225 {
/*!
	\class Element2
	\brief A concrete class implementing AbstractElement interface. Uses dynamically allocated data.
 */
  class Element2 : public AbstractElement {
    public:
      Element2(int _val);
      Element2(const Element2& rhs);
	  virtual Element2* clone() const;
      Element2& operator=(const Element2& rhs);
      virtual int Get() const;
      virtual void Set(int new_val);
      virtual void Print() const;
      virtual ~Element2();
    private:
      int * p_val;
  };

}
#endif
