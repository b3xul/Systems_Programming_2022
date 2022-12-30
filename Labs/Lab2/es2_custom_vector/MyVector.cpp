// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 27/12/21.
//

#include "MyVector.h"
#include <cstring>

#define DEBUG 1

MyVector::MyVector() : v(nullptr), size(0), capacity(0) {
#ifdef DEBUG
  std::cout << "Default constructor activated (" << this << ")\n";
#endif
}

MyVector::MyVector(const std::size_t new_capacity) : v(nullptr), size(0), capacity(0) {
#ifdef DEBUG
  std::cout << "Parametric constructor activated (" << this << ")\n";
#endif
  this->reserve(new_capacity);
}

MyVector::~MyVector() {
  //delete[] v; this is not enough! how does it know how much to delete???
  /*    for (std::size_t i = 0; i < this->size; i++) {
        free(this->v[i]);
      }*/
  //free(this->v);
  delete[] this->v;
}

void MyVector::reserve(const std::size_t new_capacity) {
  /*
   Since size_t is an unsigned int, this is not even checked by the std::Vector, which just let the program terminate
   with an error.
  if (new_capacity < 0) {
  #ifdef DEBUG
      cout <<  "Capacity cannot be < 0.\n";
  #endif
      return;
    }*/
  if (new_capacity <= this->capacity) {
#ifdef DEBUG
    std::cout << "New capacity is not greater than previous capacity: " << this->capacity << "\n";
#endif
    return;
  }

  // 1. Using REALLOC creates very ugly C code where we would need to manually reinitialize every object of the array
  // after having copied that! Weird!

  //If ptr is NULL, the behavior is the same as calling malloc(new_size).
  //MyType *tmp = (MyType *) realloc(this->v, new_capacity * sizeof(MyType));

  //https://stackoverflow.com/questions/22932984/is-it-fair-to-always-recommend-stdvector-over-realloc
  //https://stackoverflow.com/questions/8003233/does-stdvector-have-to-move-objects-when-growing-capacity-or-can-allocator?lq=1
  //https://isocpp.org/wiki/faq/freestore-mgmt#realloc-and-renew
  // Can I use realloc() on pointers allocated via new?
  //No!
  //When realloc() has to copy the allocation, it uses a bitwise copy operation, which will tear many C++ objects to shreds. C++ objects should be allowed to copy themselves. They use their own copy constructor or assignment operator.
  //https://pvs-studio.com/en/docs/warnings/v630/
  //  When memory is allocated for the class in this way, the code does not call the constructor. When memory is
  //  released through the 'free()' function, the code does not call the destructor. This is quite odd: such a code
  //  might cause handling uninitialized variables and other errors.
  // class CL
  //{
  //  int num;
  //public:
  //  CL() : num(0) {...}
  //  ...
  //};
  //...
  //CL *pCL = (CL*)malloc(sizeof(CL) * 10);
  // As a result, the 'num' variable won't be initialized. Of course, you can call the constructor for each object "manually", but a more correct way is to use the 'new' operator.
  // Besides all that, the heap that new uses may not be the same as the heap that malloc() and realloc() use!

  //Why doesn’t C++ have an equivalent to realloc()?
  //If you want to, you can of course use realloc(). However, realloc() is only guaranteed to work on arrays allocated by malloc() (and similar functions) containing objects without user-defined copy constructors. Also, please remember that contrary to naive expectations, realloc() occasionally does copy its argument array.
  //In C++, a better way of dealing with reallocation is to use a standard library container, such as vector, and let it grow naturally.

  // 2. Using new[] will result in more memory fragmentation but will result in a better and simpler code, little
  // tradeoff!
  //MyType *tmp_vector = new(std::nothrow) MyType;
  MyType *tmp = new(std::nothrow) MyType[new_capacity]; //I can't use new[] if I will change the size of the array, I
  // must delete the previous and allocate new space every time!
  if (tmp == nullptr) {
    std::cerr << "Error: There is not enough memory available to reserve!\n";
    return;
  }
/*    for (std::size_t i = 0; i < this->size; i++) {
      tmp[i] = this->v[i];
    }*/

  std::copy(this->v, this->v + this->size, tmp);
  //memcpy(this->v, tmp, this->size);
  delete[] this->v;
  this->v = tmp;
  this->capacity = new_capacity;
#ifdef DEBUG
  std::cout << "Space for " << this->capacity << " elements reserved!\n";
#endif

}

MyVector::MyVector(const MyVector &source) : v(nullptr), size(0), capacity(0) {
#if DEBUG
  std::cout << "Copy constructor activated\n";
#endif
  this->reserve(source.capacity); // initializes v and set this->capacity
  //this->capacity = source.capacity;
  /*  for (std::size_t i = 0; i < source.size; i++) {
      this->v[i] = source.v[i];
    }*/
  std::copy(source.v, source.v + source.size, this->v);
  this->size = source.size;
}

void swapVector(MyVector &a, MyVector &b) {
  std::swap(a.v, b.v);
  std::swap(a.size, b.size);
  std::swap(a.capacity, b.capacity);
}

MyVector &MyVector::operator=(const MyVector &source) {
  //Copy assignment operator. Replaces the contents with a copy of the contents of source.
#ifdef DEBUG
  std::cout << "Assignment operator by copy activated(" << this << ")\n";
#endif
  if (this != &source) {
    MyVector temp = MyVector(source);
    swapVector(*this, temp);
  } else
    std::cerr << "Can't assign one object to itself!\n";
  return *this;
}
void MyVector::push_back(const MyType &new_element) {
  if (size == capacity)
    this->reserve((this->capacity > 0) ? this->capacity * 2 : 1);  // if capacity was 0, 0*2=0!

  //inserisco nuovo elemento al fondo
  this->v[this->size++] = Vertex(new_element);
}

MyVector::MyVector(MyVector &&source) noexcept: v(nullptr), size(0), capacity(0) {
#ifdef DEBUG
  std::cout << "Movement constructor activated!\n";
#endif
  swapVector(*this, source);
}
MyVector &MyVector::operator=(MyVector &&source) noexcept {
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
    swapVector(*this, source);
    //Non liberiamo delete[] source.data_!

  } else
    std::cerr << "Can't assign by movement one object inside itself!\n";

  return *this;
}
void MyVector::emplace_back(const MyType &&new_element) {
  if (size == capacity)
    this->reserve((this->capacity > 0) ? this->capacity * 2 : 1);  // if capacity was 0, 0*2=0!

  //inserisco nuovo elemento al fondo
  this->v[this->size++] = new_element;
}

void MyVector::assign(std::size_t count, const MyType &element) {
  this->reserve(count); //includes delete v[] and update this->capacity
  for (this->size = 0; this->size < count; this->size++) {
    this->v[this->size] = element;
  }
}
MyType &MyVector::at(std::size_t pos) {
  /*  if (pos >= this->size)
      throw std::out_of_range;*/
  return this->v[pos];
}

MyType &MyVector::operator[](std::size_t pos) const {
  return this->v[pos];
}

std::ostream &operator<<(std::ostream &out, const MyVector &m) {

  for (std::size_t i = 0; i < m.size; i++) {
    out << "{" << m[i] << "}" << ((i < m.size - 1) ? ", " : "");
  }
  out << '\n';
  return out;
}
MyType &MyVector::front() {
  return this->v[0];
}
MyType &MyVector::back() {
  return this->v[this->size - 1];
}
MyType *MyVector::data() {
  return this->v;
}
bool MyVector::empty() {
  return this->size;
}
void MyVector::shrink_to_fit() {
  //copy of reserve, improvable!
  MyType *tmp = new(std::nothrow) MyType[this->size]; //I can't use new[] if I will change the size of the array, I
  // must delete the previous and allocate new space every time!
  if (tmp == nullptr) {
    std::cerr << "Error: There is not enough memory available to reserve!\n";
    return;
  }
  /*  for (std::size_t i = 0; i < this->size; i++) {
      tmp[i] = this->v[i];
    }*/
  std::copy(this->v, this->v + this->size, tmp);
  delete[] this->v;
  this->v = tmp;
  this->capacity = this->size;
#ifdef DEBUG
  std::cout << "Space for " << this->capacity << " elements reserved!\n";
#endif
}
void MyVector::clear() {
  delete[] this->v;
  this->v = nullptr;
  this->size = 0;
}
