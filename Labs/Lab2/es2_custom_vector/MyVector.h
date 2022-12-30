//
// Created by bexul on 27/12/21.
//

#ifndef MYVECTOR_H_
#define MYVECTOR_H_

#include <iostream>

class Vertex {
 private:
  float x, y, z;
  std::string a;
 public:
  Vertex() : x(0), y(0), z(0), a("nullptr") {
  }
  Vertex(float x, float y, float z) : x(x), y(y), z(z), a("nullptr") {
    if (x == 4) {
      this->a = std::string("ciao");
    }
  }
  ~Vertex() = default;

  Vertex(const Vertex &vertex) : x(vertex.x), y(vertex.y), z(vertex.z), a(std::string(vertex.a)) {
    std::cout << "Vertex copy constructor activated!\n";
  }
  friend std::ostream &operator<<(std::ostream &stream, const Vertex &vertex) {
    stream << vertex.x << ", " << vertex.y << ", " << vertex.z << " - " << vertex.a;
    return stream;
  }
};

typedef Vertex MyType;

class MyVector {
 private:
  MyType *v; //-V122
  std::size_t size; // /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h -> #define __SIZE_TYPE__ long unsigned int
  // -V122
  std::size_t capacity; //-V122
 public:
  MyVector();
  explicit MyVector(std::size_t capacity);

  ~MyVector();

  MyVector(const MyVector &source); // copy constructor
  friend void swapVector(MyVector &a, MyVector &b);
  MyVector &operator=(const MyVector &source); // copy assignment
  MyVector(MyVector &&source) noexcept; // movement constructor
  MyVector &operator=(MyVector &&source) noexcept; // movement assignment

  void reserve(std::size_t capacity);
  void shrink_to_fit();

  void push_back(const MyType &new_element);
  void emplace_back(const MyType &&new_element);

  void assign(std::size_t count, const MyType &element);

  MyType &at(std::size_t pos);
  MyType &front();
  MyType &back();
  MyType *data();

  bool empty();

  void clear();

  MyType &operator[](std::size_t pos) const;
  friend std::ostream &operator<<(std::ostream &out, const MyVector &m);
};

#endif //MYVECTOR_H_
