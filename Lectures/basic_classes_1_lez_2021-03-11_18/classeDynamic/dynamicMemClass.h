//
// Created by Alessandro on 16/03/21.
//

#ifndef INC_04_C___DYNAMICMEMCLASS_H
#define INC_04_C___DYNAMICMEMCLASS_H

#include <cstddef>
#include <memory>   // necessario per avere std::nothrow
#include <iostream> // necessario per avere std::cerr

class dynamicMemClass {
 public:
  dynamicMemClass(std::size_t size = 0);
  ~dynamicMemClass(); // DISTRUTTORE

  //int * getPtr(); // Causerebbe problemi? Come implementarlo?

 private:
  int *_ptr; // comodo anche nominarli con un significato specifico, per esempio con m_ptr
  // dove muore? Nel DISTRUTTORE
  std::size_t _size;  // ogni oggetto che gestisce memoria dinamica dovra' sempre avere associato al puntatore la
  // dimensione della memoria dinamica allocata
};

#endif //INC_04_C___DYNAMICMEMCLASS_H
