//
// Created by Alessandro on 12/03/21.
//

#include <iostream>
#include "classeprova.h"

int main() {
  mynamespace::classeprova obj1, obj2(5, 4);

  cout << "Hello, World! " << '\n'; //std::endl; // '\n' + flush();
  obj1.stampa_dati();
  obj2.stampa_dati();
  return 0;
}
