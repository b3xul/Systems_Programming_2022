//
// Created by Alessandro on 16/03/21.
//

#include "dynamicMemClass.h"

dynamicMemClass::dynamicMemClass(std::size_t size) : _size(size) {
  if (_size != 0) {
    std::cout << "Generating Memory\n";
    _ptr = new(std::nothrow) int[size];
    if (_ptr == nullptr) {
      std::cerr << "error while creating the dynamic mem in dynamicMemClass\n";
    }
  } else
    _ptr = nullptr;
}

dynamicMemClass::~dynamicMemClass() {
  if (_ptr != nullptr) {      //  Potrei anche controllare size, ma meglio controllare direttamente il puntatore!
    std::cout << "Gonna delete Memory at address: " << _ptr << " with content: " << *_ptr
              << " \n";
    delete[] _ptr;
    std::cout << "Deleted Memory at address: " << _ptr << " with content: " << *_ptr
              << " \n"; // Stampa senza errori anche se non potrebbe accedere a quella porzione di memoria!
    _ptr = nullptr;
    //std::cout << "Deleting Memory at address: " << _ptr << " with content: " << *_ptr << " \n"; SIGSEGV! Tenta
    // di stampare nullptr
  }
}