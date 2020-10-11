#include <iostream>
#include "vector.h"

int main(int argc, char **argv) {
  CS225::SparseVector v;
  for (int i=0;i<10;++i) v.Insert(i+1,i);
  const CS225::SparseVector cv(v);
  cv[3]=5;
  return 0;
}

