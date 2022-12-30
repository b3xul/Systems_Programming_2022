//
// Created by Alessandro on 16/03/21.
//

#include <iostream>
#include "dynamicMemClass.h"

int main() {
  dynamicMemClass obj(10);

  // Gestione del vettore di TipoDato: Allocazione di un vettore di 10 interi->in questo caso SO inizializza heap a 0
  int *ptrInt = new(std::nothrow) int[10];
  if (ptrInt == nullptr) {
    std::cerr << "Error in mem allocation\n";   // std::cerr e' l'equivalente di stderr in C
    return -1;
  }

  // Uso del vettore di TipoDato
  for (int i = 0; i < 10; i++)
    std::cout << ptrInt[i] << " ";
  std::cout << "\n";

  // Libero vettore di TipoDato
  if (ptrInt != nullptr) {
    std::cout << "deleting memory...\n";
    delete[] ptrInt;
    ptrInt = nullptr;   // Questa assegnazione permette di evitare una liberazione multipla della memoria, a patto
    // che ogni liberazione sia sempre racchiusa dal blocco if(prtInt != nullptr)
  }

  return 0;
}
