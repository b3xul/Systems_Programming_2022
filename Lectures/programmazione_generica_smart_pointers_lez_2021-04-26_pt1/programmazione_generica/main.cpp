#include <iostream>

#include "Stack.hpp"

int main() {
  /* template<typename T, typename R>
   * std::vector<T> data;
   * R n_elem;
  */
  Stack<int, int> obj1, obj2;
  Stack<char, unsigned long> obj3;
  Stack<uint64_t, uint64_t> obj4;

  Stack<Stack<int, int>, int> obj5;
  Stack<Stack<uint64_t, uint64_t>, int> obj6;

  /* Se metto un metodo statico dentro la Template, sono poi costretto a richiamarlo usando la template
   * Stack<int, int>::print_counter() -> 1, non 3!!
   * Questo perchè Stack<int,int> e Stack<char, unsigned long> sono 2 CLASSI DIVERSE, NON LEGATE TRA LORO:
   * polimorfismo statico in fase di compilazione
   * Stack<int, int> è una classe A
   * Stack<char, unsigned long> è una classe B
   * Stack<Stack<int, int>, int> è una classe C
   * Tutte completamente diverse tra loro, perchè il codice creato a compile-time per quella classe è completamente
   * diverso!
   * */
  Stack<int, int>::print_counter();
  Stack<char, unsigned long>::print_counter();
  Stack<uint64_t, uint64_t>::print_counter();
  Stack<Stack<int, int>, int>::print_counter();
  Stack<Stack<uint64_t, uint64_t>, int>::print_counter();

  swap(obj1, obj2);
  //swap(obj1, obj3); //no matching function for call to ‘swap(Stack<int, int>&, Stack<char, long unsigned int>&)’

  return 0;
}
