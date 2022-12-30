#include <iostream>
#include <memory>

#include "myList.h"

using namespace std;

struct AllocationMetrics {
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
void operator delete(void *memory, size_t size) {
  s_AllocationMetrics.TotalFreed += size;
#if DEBUG
  cout << "Deallocated " << size << " bytes\n";
#endif
  free(memory);
}

static void PrintMemoryUsage() {
  cout << "Current memory usage: " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}

#define DEBUG 0

int main() {

  /*
    list<int> a{4, 1, 2, 3};
    a.sort();
    for (const auto &item : a)
      cout << item << ",";
      */
#if DEBUG
  PrintMemoryUsage();
#endif

  myClass items[5] = {Message(10), Message(11), Message(12), Message(13), Message(14)};

  myList a;
  a.printList();
  a.push_front(items[0]);
  a.push_front(items[1]);
  a.push_front(items[2]);
  a.popNode();
  a.popNode();
  a.popNode();
  a.push_back(items[0]);
  a.push_back(items[1]);
  a.push_back(items[2]);
  a.insertNode(items[3], 2);
  a.insertNode(items[3], 0);
  a.insertNode(items[3], 1);
  a.insertNode(items[3], 3);
  a.swapNodes(1, 4);
  a.swapNodes(1, 2);
  a.swapNodes(0, 2);
  a.swapNodes(1, 6);
  a.swapNodes(0, 6);
  cout << "INCREASING LIST\n";
  myList b;
/*  b.insertIncOrderedNode(items[3]);
  b.insertIncOrderedNode(items[4]);
  b.insertIncOrderedNode(items[0]);
  b.insertIncOrderedNode(items[1]);
  b.insertIncOrderedNode(items[1]);*/
/*  b.deleteNodeByVal(1);
  b.deleteNodeByVal(0);
  b.deleteNodeByVal(4);
  b.deleteNodeByVal(1);*/
  cout << "DECREASING LIST\n";
  myList c;
/*  c.insertDecOrderedNode(items[3]);
  c.insertDecOrderedNode(items[4]);
  c.insertDecOrderedNode(items[0]);
  c.insertDecOrderedNode(items[1]);*/

  c.deleteNodeByPos(2);
  c.deleteNodeByPos(0);
  c.deleteNodeByPos(1);
  c.deleteNodeByPos(0);
  return 0;
}
