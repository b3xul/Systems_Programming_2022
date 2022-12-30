#include <iostream>

#include "../../../Personal_Study/Timing_Visual_Profiler/instrumentor.h"
#include "MyVector.h"

using namespace std;

#define DEBUG 0

/*struct AllocationMetrics {
  uint32_t TotalAllocated = 0;
  uint32_t TotalFreed = 0;
  uint32_t CurrentUsage() {
    return TotalAllocated - TotalFreed;
  }
};
static AllocationMetrics s_AllocationMetrics;

void *operator new(size_t size) {
  s_AllocationMetrics.TotalAllocated += size;
#if DEBUG
  cout << "Allocated " << size << " bytes\n";
#endif
  return malloc(size);
}
void *operator new[](size_t size) {
  s_AllocationMetrics.TotalAllocated += size;
#if DEBUG
  cout << "Allocated chunk of " << size << " bytes\n";
#endif
  return malloc(size);
}

void operator delete(void *memory, size_t size) {
  s_AllocationMetrics.TotalFreed += size;
#if DEBUG
  cout << "Deallocated " << size << " bytes\n";
#endif
  free(memory);
}

void operator delete[](void *memory, size_t size) {
  s_AllocationMetrics.TotalFreed += size;
#if DEBUG
  cout << "Deallocated chunk of " << size << " bytes\n";
#endif
  free(memory);
}

static void PrintMemoryUsage() {
  cout << "Current memory usage: " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}*/

void stdVectorPushBack();
void stdVectorReserved();
void stdVectorReservedEmplaced();
void myVectorUsage();

int main() {

  // Si implementi un contenitore C++ per la gestione di vettori, si realizzi il contenitore in modo che, lato utente,
  //sia possibile fare tutto quello che il contenitore vector consente di fare, esclusa la gestione degli iteratori.
  //L’aggiunta di un nuovi elementi deve corrispondere alla allocazione di ulteriore spazio in memoria per il
  //vettore, sono possibili due approcci:
  //• Allocare solamente lo spazio necessario all’aggiunta dei nuovi elementi
  //• Allocare lo spazio per chunk di memoria. In questo modo il vettore avr`a una dimensione fissa di
  //partenza size e, qualora l’aggiunta di un nuovo elemento richieda l’aggiunta di memoria si proceder`a
  //ad allocare un nuovo vettore di dimensione dim_vettore + size.
  //Si ragioni specialmente sul significato e sulle operazione di resize, valutando costi e benefici dei due approcci,
  //e su tutti i costruttori di copia ed operatori di assegnazione.
  //Al fine di provare il funzionamento del contenitore, si scriva una funzione di ordinamento per un vettore.
  //Per il momento si utilizzi per il contenitore un tipo di dato fisso, scelto dal programmatore, i.e., un vettore
  //di interi, piuttosto che di caratteri.

  // https://www.youtube.com/watch?v=PocJ5jXv8No&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=47
  // STL vector is not designed to be fast, but flexible!
  // often gaming companies or others that need fast performances reimplement their own version of the STL, like the EA
  // STL (https://github.com/electronicarts/EASTL)!


  Instrumentor::Instance().beginSession("Profile1");
#if DEBUG
  PrintMemoryUsage();
#endif
  stdVectorPushBack();
  /*  stdVectorReserved();
   stdVectorReservedEmplaced();*/
  myVectorUsage();
  Instrumentor::Instance().endSession();
  return 0;
}

void stdVectorPushBack() {
  PROFILE_FUNCTION();
  std::vector<Vertex> vertices;
  //std::cout << vertices.capacity() << '\n'; 0
  //std::cout << vertices.size() << '\n'; 0
  vertices.push_back(Vertex(1, 2, 3));
  //std::cout << vertices.capacity() << '\n'; 1
  //std::cout << vertices.size() << '\n'; 1
  vertices.push_back(Vertex(4, 5, 6));
  //std::cout << vertices.capacity() << '\n'; 2
  //std::cout << vertices.size() << '\n'; 2
  vertices.push_back(Vertex(7, 8, 9));
  //std::cout << vertices.capacity() << '\n'; 4
  //std::cout << vertices.size() << '\n'; 3
  vertices.push_back(Vertex(7, 8, 9));
#if DEBUG
  PrintMemoryUsage();
#endif
  vertices.push_back(Vertex(7, 8, 9));
  //std::cout << vertices.capacity() << '\n'; 8
  //std::cout << vertices.size() << '\n'; 5


  // 1. This calls the copy constructor 12 times! This is due to the fact that the Vertices are initially allocated on
  // the main stack, so you need to copy them from the main stack to the memory reserved for the vector (also it
  // will be on the main stack, but in a different location).
  // 2. Starting capacity of the vector is 0! Then size doubles each time! If we knew the environment better, we
  // could directly have a starting size for the vector bigger than 0 to avoid multiple reallocations (and consequent
  // copies!)

  // Create vertex1 -> resize vector (0->1) + copy to new vector the vertex 1
  // Create vertex 2 -> resize vector (1->2) + copy to new vector the vertex 1, 2
  // Create vertex 3 -> resize vector (2->4) + copy to new vector the vertex 1, 2, 3
  // Create vertex 4 -> copy to vector the vertex 4
  // Create vertex 5 -> resize vector (4->8) + copy to vector the vertex 1,2,3,4,5
  // Total copy constructor activated: 12!
  // 0.068ms
#if DEBUG
  PrintMemoryUsage();
#endif
}

void stdVectorReserved() {
  PROFILE_FUNCTION();
  // To reduce the number of resizes, we don't use neither vertices.resize(5) nor std::vector<Vertex> vertices(5),
  // which will try to construct the 5 vertices to fill the vector with! We just want to allocate enough memory to
  // hold them!
  std::vector<Vertex> vertices;
  vertices.reserve(5);
  //std::cout << vertices.capacity() << '\n'; 5
  vertices.push_back(Vertex(1, 2, 3));
  vertices.push_back(Vertex(4, 5, 6));
  vertices.push_back(Vertex(7, 8, 9));
  vertices.push_back(Vertex(10, 11, 12));
  vertices.push_back(Vertex(13, 14, 15));
  // Create vertex1 -> copy to new vector the vertex 1
  // Create vertex 2 -> copy to new vector the vertex 2
  // Create vertex 3 -> copy to new vector the vertex 3
  // Create vertex 4 -> copy to vector the vertex 4
  // Create vertex 5 -> copy to vector the vertex 5
  // Total copy constructor activated: 5!
  // 0.021ms
#if DEBUG
  PrintMemoryUsage();
#endif
}

void stdVectorReservedEmplaced() {
  PROFILE_FUNCTION();
  std::vector<Vertex> vertices;
  vertices.reserve(5);
  //std::cout << vertices.capacity() << '\n'; 5
  vertices.emplace_back(1, 2, 3);
  vertices.emplace_back(4, 5, 6);
  vertices.emplace_back(7, 8, 9);
  vertices.emplace_back(10, 11, 12);
  vertices.emplace_back(13, 14, 15);
  // Create vertices directly in the memory reserved for the vector, instead of allocating the in another location on
  // the main stack frame first, and then copying them to the right location!
  // Total copy constructor activated: 0!!!
  // 0.008ms
#if DEBUG
  PrintMemoryUsage();
#endif
}

void myVectorUsage() {
  //PROFILE_FUNCTION();
  MyVector v1;
  //MyVector v2(-3);
  MyVector v3(5);
  v3.reserve(5);
  v3.reserve(4);
  v3.reserve(6);
  v3.push_back(Vertex(1, 2, 3));
  v3.push_back(Vertex(4, 5, 6));
  v3.push_back(Vertex(7, 8, 9));
  v3.reserve(7);
  std::cout << v3;
  MyVector v4 = v3; // Copy constructor
  std::cout << v4;
  MyVector v5(1);
  v5.push_back(Vertex(11, 12, 13));
  MyVector v6;
  v6.push_back(Vertex(11, 12, 13));
  std::cout << "v6:" << v6;
  v5 = v4; // Assignment operator by copy
  std::cout << "v5:" << v5;
  MyVector v7{move(v5)}; // Move constructor
  std::cout << "v7:" << v7;
  MyVector v8;
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
  std::cout << "\nv8:" << v8;
}
