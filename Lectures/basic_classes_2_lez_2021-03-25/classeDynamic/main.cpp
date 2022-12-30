//
// Created by Alessandro on 16/03/21.
//
#include <iostream>
#include "dynamicMemClass.h"

int main() {
  dynamicMemClass obj(10);
  dynamicMemClass obj2{obj};
  dynamicMemClass obj3{std::move(obj)};

  std::cout << "Size of obj: " << obj.getActualSize() << '\n';

  dynamicMemClass *ptrO = new dynamicMemClass{1};
  std::memcpy(ptrO, &obj2, sizeof(dynamicMemClass));

  std::cout << ptrO->getActualSize() << '\n';

  const int *stolenPtr = obj.getPtr();
  //1. const int * or int const * are pointer to an integer constant:the integer value that it is pointing at cannot be
  // changed USING THAT POINTER! (can be changed in other ways if originally it was not a constant integer.
  //2. int * const defines a constant pointer to an integer. You cannot make the pointer point to some
  // other memory once you have initialized.
  //3. const int * const ptr :ptr _cannot_ point to anywhere else, and what it points to _cannot_ be modified by it.

  stolenPtr = obj2.getPtr(); //allowed
  // *stolenPtr = 4; assignment of read-only location ‘* stolenPtr’
  delete[] stolenPtr;
  stolenPtr = nullptr; //allowed


  /*int * ptrInt = new (std::nothrow) int[10];
  if (ptrInt == nullptr) {
      std::cerr << "Error in mem allocation\n";
      return -1;
  }

  for (int i = 0; i < 10; i++)
      std::cout << ptrInt[i] << " ";
  std::cout << "\n";

  if (ptrInt != nullptr) {
      std::cout << "deleting memory...\n";
      delete[] ptrInt;
      ptrInt = nullptr;
  }*/

  return 0;
}

