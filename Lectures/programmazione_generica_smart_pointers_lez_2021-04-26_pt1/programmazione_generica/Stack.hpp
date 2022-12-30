//
// Created by Alessandro on 15/04/21.
//

// new->header file->.hpp
#ifndef INC_07_PROGRAMMAZIONE_STACK_HPP
#define INC_07_PROGRAMMAZIONE_STACK_HPP

#include <vector>
#include <iostream>

template<typename Tipo1>
void swap(Tipo1 &val1, Tipo1 &val2);

template<typename T, typename R>  /* Template che lavora su 2 tipi generici da usare all'interno della classe */
class Stack {
 public:
  Stack();
  ~Stack();
  void push(T val);
  T pop();
  R getNElem() const;
  static void print_counter();
 private:
  std::vector<T> data;
  R n_elem;
  static std::size_t counter; // numero di stack creati
};

/* Implemento le funzioni generiche (metodi) direttamente nel .hpp */
template<typename T, typename R>
Stack<T, R>::Stack() {
  n_elem = 0;
  counter++;
}

template<typename T, typename R>
Stack<T, R>::~Stack() { // do per scontato che T abbia implementati i concetti di RAII: T deve disallocare tutte le
  // sue cose in modo corretto. Resource Acquisition Is Initialization: is a programming idiom[2] used in several
  // object-oriented, statically-typed programming languages to describe a particular language behavior. In RAII,
  // holding a resource is a class invariant, and is tied to object lifetime: resource allocation (or acquisition) is
  // done during object creation (specifically initialization), by the constructor, while resource deallocation
  // (release) is done during object destruction (specifically finalization), by the destructor. In other words,
  // resource acquisition must succeed for initialization to succeed. Thus the resource is guaranteed to be held
  // between when initialization finishes and finalization starts (holding the resources is a class invariant), and
  // to be held only when the object is alive. Thus if there are no object leaks, there are no resource leaks.
  // Se T genera memory leak, è sua responsabilità! Dato che qua nel template non alloco memoria dinamica sicuramente
  // non genero memory leak.
}

template<typename T, typename R>
void Stack<T, R>::push(T val) {
  data.push_back(val);
  n_elem++;
}

template<typename T, typename R>
T Stack<T, R>::pop() {
  T val = data[0];
  data.erase(data.begin());
  n_elem--;
  return val;
}
template<typename T, typename R>
R Stack<T, R>::getNElem() const {
  return n_elem;
}
template<typename T, typename R>
void Stack<T, R>::print_counter() {
  std::cout << "Counter: " << counter << '\n';
}

template<typename T, typename R> std::size_t Stack<T, R>::counter = 0;

template<typename Tipo1>
void swap(Tipo1 &val1, Tipo1 &val2) {
  Tipo1 tmp;
  tmp = val1;
  val1 = val2;
  val2 = tmp;
}

#endif //INC_07_PROGRAMMAZIONE_STACK_HPP
