#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

int main() {
  /* STANDARD LIBRARY E FOR RANGE LOOP */
  std::vector<int> test(10, 0);

  // 1. for range loop per copia
  for (auto elem:test) {  // elem creato per copia!
    elem = 10;
    std::cout << elem << std::endl; // 10 10 10 10... ma appena usciti dal loop non ho modificato test!
  }
  // 2. for range loop per riferimento
  for (auto &elem : test) {
    elem = 20;
    std::cout << elem << std::endl; // 20 20 20 20... uscito dal loop ho modificato test!
  }
  //3. for range loop per riferimento costante
  for (const auto &elem : test) { // cbegin() and cend(); Iteratori costanti, loop molto più efficiente
    std::cout << elem << std::endl;
  }

  int start_to = 10;
  std::cout << "Start_to = " << start_to << '\n';

  for (auto &elem: test)
    elem = start_to++; // 10 11 12 13..

  for (auto it = test.crbegin(); it != test.crend(); it++)  //sfruttando iteratori constant reverse (disponibili
    // perchè il contenitore vector supporta iteratori con accesso random (supportano anche operatore --)
    std::cout << (*it) << std::endl; // 19 18 17 16...
  // molto meglio di riordinare il contenitore!

  /* FUNZIONI LAMBDA */

  //g-generator function object that will be called.
  //The signature of the function should be equivalent to the following: Ret fun();
  //The type Ret must be such that an object of type ForwardIt can be dereferenced and assigned a value of type Ret.
  std::cout << "Start_to = " << start_to << '\n'; // Start_to è stato modificato e ora vale 20
  auto incrementCopy = [start_to]() { return start_to + 1; }; // start_to che viene catturato = 20
  auto incrementCopyMutable = [start_to]() mutable { return start_to++; }; // start_to che viene catturato = 20
  auto incrementRef = [&start_to]() { return start_to++; }; // start_to che viene catturato = 20
  auto stampa = [](const auto &elem) { std::cout << elem << std::endl; }; // lambda function "template" sfruttando auto
  // const reference: fast
  start_to = 30; // start to is modified
  // generate (iteratorI, iteratorF, function WITH NO PARAMETERS that generate something)
  //std::generate(test.rbegin(), test.rend(), [](int val) { return val++; }); error: no match for call to
  // ‘(main()::<lambda(int)>) ()’
  std::generate(test.rbegin(), test.rend(), incrementRef);
  std::for_each(test.crbegin(), test.crend(), stampa);  // 30 31 32 33... ANCHE SE LO START_TO INIZIALMENTE CATTURATO
  // ERA 20, IL RIFERIMENTO VIENE SPOSTATO A 30 PRIMA CHE LA GENERATE VENGA ESEGUITA, E ALLA FINE DELL'ESECUZIONE
  // PUNTA A 40.
  std::cout << "Start_to = " << start_to << '\n'; // Start_to è stato modificato e ora vale 40

  std::generate(test.rbegin(), test.rend(), incrementCopy);
  std::for_each(test.crbegin(), test.crend(), stampa);  // 21 21 21 21... ANCHE SE LO START_TO ATTUALMENTE VALE 40,
  // AVENDONE FATTO UNA COPIA QUANDO VALEVA 20, ORA LO STATO DELL'OGGETTO FUNZIONE INCREMENTCOPY VALE 20

  std::generate(test.rbegin(), test.rend(), incrementCopy);
  std::for_each(test.crbegin(), test.crend(), stampa);  // 21 21 21 21... ANCHE SE LO START_TO ATTUALMENTE VALE 40,
  // AVENDONE FATTO UNA COPIA QUANDO VALEVA 20, ORA LO STATO DELL'OGGETTO FUNZIONE INCREMENTCOPY VALE 20

  std::generate(test.rbegin(), test.rend(), incrementCopyMutable);
  std::for_each(test.crbegin(), test.crend(), stampa);  // 21 22 23 24... ANCHE SE LO START_TO ATTUALMENTE VALE 40,
  // AVENDONE FATTO UNA COPIA QUANDO VALEVA 20, ORA LO STATO DELL'OGGETTO FUNZIONE INCREMENTCOPYMUTABLE VALE 20
  std::cout << "Start_to = " << start_to << '\n'; // Start_to non viene modificato da incrementCopyMutable perchè
  // essa va ad agire sulla copia interna dello stato!

  // start_to nel programma principale vale 40, in incrementRef vale 40, in incrementCopy vale 20, in
  // incrementCopyMutable vale 30

  std::vector<float> test2(10, 1.2);
  std::for_each(test2.begin(), test2.end(), stampa); // si può usare funzione stampa come fosse una template function
  // perchè abbiamo usato auto!

  return 0;
}
