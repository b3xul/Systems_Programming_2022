// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 27/12/21.
//

#ifndef MYVECTOR_H_
#define MYVECTOR_H_

#include <iostream>
#include <cstring>

#define DEBUG 1

class Vertex {
 private:
  float x, y, z;
  int *memoryBlock = nullptr;
 public:
  Vertex() : x(1), y(1), z(1) {
    this->memoryBlock = new int[5];
#ifdef DEBUG
    //std::cout << "Vertex default constructor activated!\n";
#endif
  }
  explicit Vertex(float scalar) : x(scalar), y(scalar), z(scalar) {
    this->memoryBlock = new int[5];
#ifdef DEBUG
    std::cout << "Vertex scalar constructor activated!\n";
#endif

  }
  Vertex(float x, float y, float z) : x(x), y(y), z(z) {
    this->memoryBlock = new int[5];
#ifdef DEBUG
    std::cout << "Vertex parametric constructor activated!\n";
#endif

  }
  ~Vertex() {
#ifdef DEBUG
    std::cout << "Vertex destructor activated!\n";
#endif
    delete[] this->memoryBlock; // It is the object responsibility to delete itself!
    this->memoryBlock = nullptr;
  }

  Vertex(const Vertex &vertex) : x(vertex.x), y(vertex.y), z(vertex.z) {
#ifdef DEBUG
    std::cout << "Vertex COPY constructor activated!\n";
#endif
    this->memoryBlock = new int[5];
    for (std::size_t i = 0; i < 5; i++) {
      this->memoryBlock[i] = vertex.memoryBlock[i];
    }
  }
  Vertex(Vertex &&vertex) noexcept: x(vertex.x), y(vertex.y), z(vertex.z) {
    std::cout << "Vertex MOVE constructor activated!\n";
    this->memoryBlock = vertex.memoryBlock;
    vertex.memoryBlock = nullptr;
  }
  // If the class definition does not explicitly declare a copy constructor, a non-explicit one is declared implicitly.
  // If the class definition declares a move constructor or move assignment operator, the implicitly declared copy
  // constructor is defined as deleted; otherwise, it is defined as defaulted.
  Vertex &operator=(const Vertex &source) {
#ifdef DEBUG
    std::cout << "Vertex assignment operator by COPY activated(" << this << ")\n";
#endif
    if (this != &source) {
      Vertex temp = Vertex(source);
      swapVertex(*this, temp);
    } else
      std::cerr << "Can't assign one object to itself!\n";
    return *this;
  }
  void swapVertex(Vertex &a, Vertex &b) {
    std::swap(a.x, b.x);
    std::swap(a.y, b.y);
    std::swap(a.z, b.z);
    std::swap(a.memoryBlock, b.memoryBlock);
  }

  Vertex &operator=(Vertex &&source) noexcept {
#ifdef DEBUG
    std::cout << "Vertex assignment operator by MOVEment activated!\n";
#endif

    if (this != &source) {
      /*      if (this->memoryBlock != nullptr) {
      #ifdef DEBUG
              std::cout << "Deleting destination memory (" << this->memoryBlock << ") before copying by movement\n";
      #endif
              delete[] this->memoryBlock; //evitare leak se oggetto destinazione contenteva già memoria dinamica
            }*/
      this->x = 0;
      this->y = 0;
      this->z = 0;
      this->memoryBlock = nullptr;
      swapVertex(*this, source);
      //Non liberiamo delete[] source.data_!

    } else
      std::cerr << "Can't assign by movement one object inside itself!\n";

    return *this;
  }

  friend std::ostream &operator<<(std::ostream &stream, const Vertex &vertex) {
    stream << vertex.x << ", " << vertex.y << ", " << vertex.z;
    return stream;
  }
};

//typedef Vertex T;

template<typename T>
class MyVector {
 private:
  const std::size_t INITIAL_SIZE = 2;
  T *v; //-V122
  std::size_t size; // /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h -> #define __SIZE_TYPE__ long unsigned int
  // -V122
  std::size_t capacity; //-V122
  void reallocate(std::size_t new_capacity);

 public:
  T *getV() const;
  std::size_t getSize() const;
  std::size_t getCapacity() const;
  MyVector();
  explicit MyVector(std::size_t capacity);

  ~MyVector();

  MyVector(const MyVector<T> &source); // copy constructor
  static void swapVector(MyVector<T> &a, MyVector<T> &b);
  MyVector<T> &operator=(const MyVector<T> &source); // copy assignment
  MyVector(MyVector<T> &&source) noexcept; // movement constructor
  MyVector<T> &operator=(MyVector<T> &&source) noexcept; // movement assignment

  void reserve(std::size_t capacity);
  void shrink_to_fit();

  void push_back(const T &new_element);
  void push_back(T &&new_element); // Must not be marked const!!!

  template<typename... Args>
  T &emplace_back(Args &&... args);
  //void emplace_back(const T &&new_element);

  void pop_back();
  void assign(std::size_t count, const T &element);

  T &at(std::size_t pos);
  T &front();
  T &back();
  T *data();

  bool empty();

  void clear();

  const T &operator[](std::size_t pos) const;
  T &operator[](std::size_t pos);
  //friend std::ostream &operator<<(std::ostream &out, const MyVector<T>&m);
  size_t Size() const;
  size_t Capacity() const;
};

template<typename T>
MyVector<T>::MyVector() : v(nullptr), size(0), capacity(0) { //default constructor
#ifdef DEBUG
  std::cout << "Default constructor activated (" << this << ")\n";
#endif
  this->reallocate(INITIAL_SIZE);
}

template<typename T>
MyVector<T>::MyVector(const std::size_t new_capacity) : v(nullptr), size(0), capacity(0) { //parametric constructor
#ifdef DEBUG
  std::cout << "Parametric constructor activated (" << this << ")\n";
#endif
  this->reallocate(new_capacity);
}

template<typename T>
MyVector<T>::~MyVector() {
  /*for (std::size_t i = 0; i < this->size; i++) {
        free(this->v[i]);
  }
   free(this->v);
   This is not required because if the user creates a vector of pointers, it is the user's responsibility to free the
   pointers inside the vector, it is not the data structure responsibility!
  */

  //delete[] this->v; //this would be ok if we didn't need to implement pop_back() and clear() functions. Since we need
  // to implement those, we cannot rely on delete[] to call the destructor for the objects that were initially
  // allocated by the new[], since some destructors have already been called and it will result in an illegal double
  // free! We need to to allocate and release memory without implicitly calling the constructors!

  this->clear(); // this calls all the destructors
  ::operator delete(this->v, this->capacity * sizeof(T)); // deletes section of memory withou calling destructors

}

template<typename T>
void MyVector<T>::reallocate(std::size_t new_capacity) {
  // 1. Allocate new block of memory (We are too low level for smart pointers)
  //A.
  // T *newBlock = new(std::nothrow) T[new_capacity]; // This triggers the default constructors, and must be freed using
  // delete[], which will trigger the default destructors: this would be ok if we didn't need to implement pop_back()
  // and clear() functions. Since we need to implement those, we cannot rely on delete[] to call the destructor for
  // the objects that were initially allocated by the new[], since some destructors have already been called and it
  // will result in an illegal double free! We need to allocate and release memory without implicitly calling the
  // constructors!

  //The new operator invokes the function operator new. For arrays of any type, and for objects that aren't class,
  // struct, or union types, a global function, ::operator new, is called to allocate storage. Class-type objects can
  // define their own operator new static member function on a per-class basis.
  //
  //When the compiler encounters the new operator to allocate an object of type T, it issues a call to T::operator
  // new( sizeof(T) ) or, if no user-defined operator new is defined, ::operator new( sizeof(T) ). It's how the new
  // operator can allocate the correct amount of memory for the object.
  // T* newBlock= (T*) new(new_capacity*sizeof(T)); // error, no new function defined for the template
  T *newBlock =
      (T *) ::operator new(new_capacity * sizeof(T));  // This does not trigger the default constructor, it just
  // reserves space for the element which will later be placed there!
  if (newBlock == nullptr) {
    std::cerr << "Error: There is not enough memory available to reserve!\n";
    return;
  }

  if (new_capacity < this->size)
    this->size = new_capacity;  // this would probably be better if done externally!

  // 2. move old elements into the new block (to avoid wasted computation)
  //A. SHALLOW COPY
  /*memcpy(this->v, newBlock, this->size);*/
  // Ok for primitive types or for user-defined types that do not store anything on the heap, since this won't call
  // the copy constructor of each object: it will just perform a shallow copy!
  // If the variables of an object have been dynamically allocated then it is required to do a Deep Copy in order to
  // create a copy of the object.
  // Note: C++ compiler implicitly creates a copy constructor and overloads assignment operator in order to perform
  // shallow copy at compile time.

  //B. DEEP COPY
  //std::copy(this->v, this->v + this->size, newBlock); This calls the copy constructor for all the underlying
  // objects, the behavior is the same as
  /*  for (std::size_t i = 0; i < this->size; i++) {
      newBlock[i] = this->v[i];
    }*/

  //C. MOVE
  for (std::size_t i = 0; i < this->size; i++) {
    newBlock[i] = std::move(this->v[i]);
  }
  //3. Delete old block
  //delete[] this->v; // This calls the destructor for all the elements!
  for (size_t i = 0; i < this->size; i++) {
    this->v[i].~T();
  } //calls the destructor of all the previous elements
  ::operator delete(this->v, this->capacity * sizeof(T)); // deletes section of memory withou calling destructors
  this->v = newBlock;
  this->capacity = new_capacity;
#ifdef DEBUG
  std::cout << "Space for " << this->capacity << " elements reserved!\n";
#endif
}

template<typename T>
void MyVector<T>::push_back(const T &new_element) {
  if (this->size == this->capacity)
    this->reallocate((this->capacity > 0) ? this->capacity * 2 : 1);  // if capacity was 0, 0*2=0!

  //inserisco nuovo elemento al fondo
  // this->v[this->size++] = T(new_element); triggers 1 assignment by copy constructor and immediately destroys the
  // new object
  this->v[this->size++] = new_element;
}
template<typename T>
void MyVector<T>::push_back(T &&new_element) {
  //new_element is an lvalue now!
  if (this->size == this->capacity)
    this->reallocate((this->capacity > 0) ? this->capacity * 2 : 1);  // if capacity was 0, 0*2=0!

  //inserisco nuovo elemento al fondo
  this->v[this->size++] = std::move(new_element); // triggers
}

template<typename T>
template<typename... Args>
T &MyVector<T>::emplace_back(Args &&... args) {
  //A template parameter pack is a template parameter that accepts zero or more template arguments (non-types, types,
  // or templates). A function parameter pack is a function parameter that accepts zero or more function arguments.
  //A template with at least one parameter pack is called a variadic template.

  if (this->size == this->capacity)
    this->reallocate((this->capacity > 0) ? this->capacity * 2 : 1);  // if capacity was 0, 0*2=0!

  // Pack expansion
  //A pattern followed by an ellipsis, in which the name of at least one parameter pack appears at least once, is
  // expanded into zero or more comma-separated instantiations of the pattern, where the name of the parameter pack
  // is replaced by each of the elements from the pack, in order.

  //https://stackoverflow.com/questions/3582001/what-are-the-main-purposes-of-using-stdforward-and-which-problems-it-solves
  // When emplace_back receives an lvalue, T gets an lvalue. When emplace_back receives an rvalue, T gets an rvalue.
  // The implementation is something similar to:
  // template <typename T>
  // void f(T&& a){ E(static_cast<T&&>(a)); }
  // which has this behaviour:
  // if(is_lvalue_reference<T>::value) return a;
  // else return std::move(a);

  // std::move should be called without explicit template arguments and always results in an rvalue, while
  // std::forward may end up as either. Use std::move when you know you no longer need the value and want to move it
  // elsewhere, use std::forward to do that according to the values passed to your function template.

  // this->v[this->size] = T(std::forward<Args>(args)...); // this allocates the newly created T in the stack frame of
  // the emplace_back function and then moves it inside this->v, performing an assignment by movement and a
  // destruction! But this is exactly what we wanted to avoid!

  // To construct object in-place we need to use:
  // Placement new
  //If placement-params are provided, they are passed to the allocation function as additional arguments. Such
  // allocation functions are known as "placement new", after the standard allocation function void* operator new
  // (std::size_t, void*), which simply returns its second argument unchanged. This is used to construct objects in
  // allocated storage:
  // // within any block scope...
  //{
  //    alignas(T) unsigned char buf[sizeof(T)];
  //    // Statically allocate the storage with automatic storage duration
  //    // which is large enough for any object of type `T`.
  //    T* tptr = new(buf) T; // Construct a `T` object, placing it directly into your
  //                          // pre-allocated storage at memory address `buf`.
  //    tptr->~T();           // You must **manually** call the object's destructor
  //                          // if its side effects is depended by the program.
  //}                         // Leaving this block scope automatically deallocates `buf`.
  //new (starting memory address where we want to place our new element) what we want to construct
  new(&this->v[this->size])T(std::forward<Args>(args)...); // (There is no need to use std::nothrow in a placement
  // new expression. unlike new/new[], placement new/new[] are already defined as being noexcept)
  return this->v[this->size++];
}

template<typename T>
void MyVector<T>::pop_back() {
  if (this->size > 0) {
    this->size--;
    this->v[size].~T(); // Call the destructor of the object -> now the user gave us the responsibility to free the
    // object that he previously placed in the vector! Not only make it logically erased, but physically erase it for
    // safety reasons, to nullify it and release all resources that it might own!
  }
}

template<typename T>
void MyVector<T>::clear() {
  while (this->size > 0) {
    pop_back();
  }
  //delete[] this->v;
  //this->v = nullptr;
  this->size = 0;
}

template<typename T>
T &MyVector<T>::at(std::size_t pos) {
  /*  if (pos >= this->size)
        throw std::out_of_range;*/

  return this->v[pos];
}

template<typename T>
T &MyVector<T>::front() {
  return this->v[0];
}

template<typename T>
T &MyVector<T>::back() {
  return this->v[this->size - 1];
}

template<typename T>
bool MyVector<T>::empty() {
  return this->size;
}

template<typename T>
size_t MyVector<T>::Size() const {
  return this->size;
}
template<typename T>
size_t MyVector<T>::Capacity() const {
  return this->capacity;
}

template<typename T>
T *MyVector<T>::data() {
  return this->v;
}

template<typename T>
const T &MyVector<T>::operator[](std::size_t pos) const {
  return this->v[pos];
}
template<typename T>
T &MyVector<T>::operator[](std::size_t pos) {
  return this->v[pos];
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const MyVector<T> &m) {
  for (std::size_t i = 0; i < m.Size(); i++) {
    out << "{" << m[i] << "}" << ((i < m.Size() - 1) ? ", " : "");
  }
  out << '\n';
  return out;
}
template<typename T>
void MyVector<T>::reserve(const std::size_t new_capacity) {
  //Since size_t is an unsigned int, this is not even checked by the std::Vector, which just let the program
  // terminate with an error.
  if (new_capacity < 0) {
#ifdef DEBUG
    std::cout << "Capacity cannot be < 0.\n";
#endif
    return;
  }

  if (new_capacity <= this->capacity) {
#ifdef DEBUG
    std::cout << "New capacity is not greater than previous capacity: " << this->capacity << "\n";
#endif
    return;
  }
  this->reallocate(new_capacity);

}

template<typename T>
MyVector<T>::MyVector(const MyVector<T> &source) : v(nullptr), size(0), capacity(0) {
#if DEBUG
  std::cout << "Copy constructor activated\n";
#endif
  this->reserve(source.capacity); // initializes v and set this->capacity
  //this->capacity = source.capacity;
  for (std::size_t i = 0; i < source.size; i++) {
    this->v[i] = source.v[i];
  }

  std::copy(source.v, source.v + source.size, this->v);
  this->size = source.size;
}

template<typename T>
void MyVector<T>::swapVector(MyVector<T> &a, MyVector<T> &b) {
  std::swap(a.v, b.v);
  std::swap(a.size, b.size);
  std::swap(a.capacity, b.capacity);
}

template<typename T>
MyVector<T> &MyVector<T>::operator=(const MyVector<T> &source) {
  //Copy assignment operator. Replaces the contents with a copy of the contents of source.
#ifdef DEBUG
  std::cout << "Assignment operator by copy activated(" << this << ")\n";
#endif
  if (this != &source) {
    MyVector<T> temp = MyVector(source);
    MyVector::swapVector(*this, temp);
  } else
    std::cerr << "Can't assign one object to itself!\n";
  return *this;
}

template<typename T>
MyVector<T>::MyVector(MyVector<T> &&source) noexcept: v(nullptr), size(0), capacity(0) {
#ifdef DEBUG
  std::cout << "Movement constructor activated!\n";
#endif
  MyVector::swapVector(*this, source);
}

template<typename T>
MyVector<T> &MyVector<T>::operator=(MyVector<T> &&source) noexcept {
#ifdef DEBUG
  std::cout << "Assignment operator by movement activated!\n";
#endif

  if (this != &source) {
    if (this->v != nullptr) {
#ifdef DEBUG
      std::cout << "Deleting destination memory (" << this->v << ") before copying by movement\n";
#endif
      delete[] this->v; //evitare leak se oggetto destinazione conteneva già memoria dinamica
    }
    this->capacity = 0;
    this->size = 0;
    this->v = nullptr;
    MyVector::swapVector(*this, source);
    //Non liberiamo delete[] source.data_!

  } else
    std::cerr << "Can't assign by movement one object inside itself!\n";

  return *this;
}

/*template<typename T>
void MyVector<T>::emplace_back(const T &&new_element) {
  if (size == capacity)
    this->reserve((this->capacity > 0) ? this->capacity * 2 : 1);  // if capacity was 0, 0*2=0!

  //inserisco nuovo elemento al fondo
  this->v[this->size++] = new_element;
}*/

template<typename T>
void MyVector<T>::assign(std::size_t count, const T &element) {
  this->reserve(count); //includes delete v[] and update this->capacity
  for (this->size = 0; this->size < count; this->size++) {
    this->v[this->size] = element;
  }
}

template<typename T>
void MyVector<T>::shrink_to_fit() {
  //copy of reserve, improvable!
  T *newBlock = new(std::nothrow) T[this->size]; //I can't use new[] if I will change the size of the array, I
  // must delete the previous and allocate new space every time!
  if (newBlock == nullptr) {
    std::cerr << "Error: There is not enough memory available to reserve!\n";
    return;
  }
  for (std::size_t i = 0; i < this->size; i++) {
    newBlock[i] = this->v[i];
  }

  std::copy(this->v, this->v + this->size, newBlock);
  delete[] this->v;
  this->v = newBlock;
  this->capacity = this->size;
#ifdef DEBUG
  std::cout << "Space for " << this->capacity << " elements reserved!\n";
#endif
}
template<typename T>
T *MyVector<T>::getV() const {
  return this->v;
}
template<typename T>
std::size_t MyVector<T>::getSize() const {
  return this->size;
}
template<typename T>
std::size_t MyVector<T>::getCapacity() const {
  return this->capacity;
}

#endif //MYVECTOR_H_
