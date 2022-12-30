#include <iostream>
#include <memory>

#include "myList.h"
#include "instrumentor.h"

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
  Instrumentor::Instance().beginSession("Profile1");
  {
    PROFILE_SCOPE("main");
    myClass items[5] = {0, 1, 2, 3, 4};

    int k = 0;
    while (k++ < 100000) {
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
      b.insertIncOrderedNode(items[3]);
      b.insertIncOrderedNode(items[4]);
      b.insertIncOrderedNode(items[0]);
      b.insertIncOrderedNode(items[1]);
      b.insertIncOrderedNode(items[1]);
      b.deleteNodeByVal(1);
      b.deleteNodeByVal(0);
      b.deleteNodeByVal(4);
      b.deleteNodeByVal(1);
      cout << "DECREASING LIST\n";
      myList c;
      c.insertDecOrderedNode(items[3]);
      c.insertDecOrderedNode(items[4]);
      c.insertDecOrderedNode(items[0]);
      c.insertDecOrderedNode(items[1]);

      c.deleteNodeByPos(2);
      c.deleteNodeByPos(0);
      c.deleteNodeByPos(1);
      c.deleteNodeByPos(0);
    }

    // With const int& pos -> 36.705 seconds
    // With const int && pos -> 31.227 seconds
    // With int && pos -> 31.002 seconds
    // With const int pos -> 31.221 seconds
    // With int pos -> 38.447 seconds
  }
  Instrumentor::Instance().endSession();
  return 0;
}
