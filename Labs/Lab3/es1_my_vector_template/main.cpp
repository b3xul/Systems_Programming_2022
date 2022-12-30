// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

#include "../../../Personal_Study/Timing_Visual_Profiler/instrumentor.h"
#include "MyVector.hpp"

using namespace std;

void myVectorUsage();

int main() {

  // https://www.youtube.com/watch?v=PocJ5jXv8No&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=47
  // STL vector is not designed to be fast, but flexible!
  // often gaming companies or others that need fast performances reimplement their own version of the STL, like the EA
  // STL (https://github.com/electronicarts/EASTL)!

  Instrumentor::Instance().beginSession("Profile1");
#if DEBUG
  //PrintMemoryUsage();
#endif
  myVectorUsage();
  Instrumentor::Instance().endSession();

  return 0;
}

void myVectorUsage() {
  PROFILE_FUNCTION();
  /*  MyVector<std::string> v0;
    v0.push_back("primo");
    v0.push_back("secondo");
    v0.push_back("terzo");
    std::cout << v0;*/
  MyVector<Vertex> v1;
  v1.push_back(Vertex(1, 2, 3));
  std::cout << "-----------\n";
  // Vertex(1, 2, 3)= Parametric constructor + v1.push_back(const T &new_element)= assignment operator by copy / +
  // v1.push_back (T &&new_element)= assignment operator by movement  + destructor
  v1.push_back(Vertex());
  std::cout << "-----------\n";
  v1.push_back(Vertex(7));
  std::cout << "-----------\n";
  // Vertex(7)= scalar constructor + 4 default constructor for the new array + 2 destructor to remove old array +
  // v1.push_back(const T &new_element)=assignment operator by copy /v1.push_back (T &&new_element)= assignment
  // operator by movement+ destructor + assignment operator by copy (B.) or by movement (C.) for moving the old block
  // to the new block depends from the push_back implementation!
  Vertex a(3, 2, 1);
  v1.push_back(a);  // This must call the assignment operator by copy, not the one by movement, since a exists also
  // outside of the push_back parameter -> cause a warning about accessing
  std::cout << "-----------\n";
  v1.pop_back();
  std::cout << v1;
  std::cout << "-----------\n";
  v1.push_back(Vertex(8, 9, 10));
  std::cout << v1;

  std::cout << "-----------\n";
  v1.emplace_back(8, 9, 10);  // The behaviour is the same as v1.push_back(Vertex(8, 9, 10)), but it doesn't save it
  // on the main, it saves the vertex directly inside the array!
  std::cout << v1;
  std::cout << "-----------\n";
  v1.pop_back();
  std::cout << v1;
  std::cout << "-----------\n";
  v1.emplace_back(1, 9, 10);
  std::cout << v1;
  std::cout << "-----------\n";
  // v1.pop_back();
  std::cout << v1;
  std::cout << "-----------\n";
  // v1.pop_back();
  std::cout << v1;
  std::cout << "-----------\n";
  // v1.pop_back();
  std::cout << v1;
  std::cout << "-----------\n";
  v1.clear();
  std::cout << v1;
  std::cout << "-----------\n";
  v1.emplace_back(1, 9, 10);
  std::cout << v1;
  std::cout << "-----------\n";
  //MyVector<Vertex> v2(-3);
  std::vector<Vertex> v2;
  /* std::cout << "-----------\n";
  v2.push_back(Vertex(8, 9, 10));
  std::cout << v2[0];
  //MyVector<Vertex> v2(-3);
  std::cout << "-----------\n";
  v2.emplace_back(8, 9, 10);
  std::cout << v2[1];*/
  //MyVector<Vertex> v2(-3);
  /*  std::cout << "-----------\n";
    MyVector<Vertex> v3(5);
    std::cout << v3;*/
  /*  v3.reserve(5);
    v3.reserve(4);
    v3.reserve(6);
    v3.push_back(Vertex(1, 2, 3));
    v3.push_back(Vertex(4, 5, 6));
    v3.push_back(Vertex(7, 8, 9));
    v3.reserve(7);
    std::cout << v3;*/
  /*  MyVector<Vertex>v4 = v3; // Copy constructor
    std::cout << v4;
    MyVector<Vertex>v5(1);
    v5.push_back(Vertex(11, 12, 13));
    MyVector<Vertex>v6;
    v6.push_back(Vertex(11, 12, 13));
    std::cout << "v6:" << v6;
    v5 = v4; // Assignment operator by copy
    std::cout << "v5:" << v5;
    MyVector<Vertex>v7{move(v5)}; // Move constructor
    std::cout << "v7:" << v7;
    MyVector<Vertex>v8;
    v8 = move(v7); // Move assignment
    std::cout << "v8:" << v8;
  #if DEBUG
    PrintMemoryUsage();
  #endif
    v8.push_back(Vertex(21, 22, 23)); // PERCHE' QUESTO NON ALLOCA NUOVA MEMORIA?
    std::cout << "v8:" << v8;
  #if DEBUG
    PrintMemoryUsage();
  #endif
    v8.emplace_back(Vertex(31, 32, 33));
    std::cout << "v8:" << v8;
  #if DEBUG
    PrintMemoryUsage();
  #endif
    v8.assign(6, Vertex(41, 42, 43));
    std::cout << "v8:" << v8;
  #if DEBUG
    PrintMemoryUsage();
  #endif
    v8.assign(10, Vertex(41, 42, 43));
    std::cout << "v8:" << v8;
  #if DEBUG
    PrintMemoryUsage();
  #endif
    std::cout << "v3 front:" << v3.front() << "\nv3 back:" << v3.back() << "\nv3 at 2:" << v3.at(2) << "\nv3 data:"
              << v3.data() << "\nv3 empty:" << v3.empty();
    v8.clear();
    std::cout << "\nv8:" << v8;*/
}