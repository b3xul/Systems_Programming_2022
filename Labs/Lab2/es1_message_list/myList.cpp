//
// Created by bexul on 27/08/21.
//

#include "myList.h"

#define DEBUG 1

using namespace std;

myList::myList() : head(nullptr), tail(nullptr), current_el(nullptr), size(0) {
#ifdef DEBUG
  cout << "New list constructed\n";
#endif
}
myList::~myList() {
  node *x = head, *t;

  while (x != nullptr) {
    t = x;
    x = x->next;
    delete t;
  }
#ifdef DEBUG
  cout << "List destroyed\n";
#endif
}

myList::node *myList::new_node() {
  size++;
  return new node;
}

myList::node *myList::build_node(const myClass &val, node *next) {
  node *x = new node;
  x->value = val;
  x->next = next;
  size++;
  return x;
}
void myList::push_front(const myClass &val) {
  // List before: |nullptr|
  // push_front
  // List after: |node1| -> |nullptr|
  if (head == nullptr) //empty list
    head = tail = current_el = build_node(val, nullptr);
    // List before: |node1| -> |nullptr|
    // push_front
    // List after: |node2| -> |node1| -> |nullptr|

    // List before: |node2| -> |node1| -> |nullptr|
    // push_front
    // List after: |node3| -> |node2| -> |node1| -> |nullptr|
  else {
    head = current_el = build_node(val, head);
  }
  printList();
}
myClass myList::popNode() {
  if (head == nullptr)
    return dummyValue;

  myClass res = head->value;
  node *t = head;
  if (head->next == nullptr)
    tail = nullptr;
  current_el = head = head->next;
  delete t;
  size--;
  printList();
  return res;
}
void myList::push_back(const myClass &val) {
  // List before: |nullptr|
  // push_back
  // List after: |node1| -> |nullptr|
  if (head == nullptr) //empty list
    head = tail = current_el = build_node(val, nullptr);
    // List before: |node1| -> |nullptr|
    // push_back
    // List after: |node1| -> |node2| -> |nullptr|

    // List before: |node1| -> |node2| -> |nullptr|
    // push_back
    // List after: |node1| -> |node2| -> |node3| -> |nullptr|
  else {
    node *new_tail = build_node(val, nullptr);
    tail->next = new_tail;
    tail = current_el = new_tail;
  }
  printList();
}
void myList::insertNode(const myClass &val, int pos) {
  if (pos == 0) {
    push_front(val);
  } else if (pos == size)
    push_back(val);
  else if (pos > size)
    return;
  else {
    int i = 0;
    for (node *x = head; x != nullptr; x = x->next, i++) {
      if (i == pos - 1) { // insert t after x at pos i
        node *t = build_node(val, x->next);
        x->next = t;
        current_el = t;
        printList();
        break;
      }
    }
  }
}
/*void myList::insertIncOrderedNode(const myClass &val) {
// comparison must change since it is a comparison on the value
  if (head == nullptr || val < head->value)
    push_front(val);
  else if (val >= tail->value)
    push_back(val);
  else {
    for (node *x = head; x != nullptr; x = x->next) {
      if (val < x->next->value) { // if val is lower than the next node insert it after the current node
        node *t = build_node(val, x->next);
        x->next = t;
        current_el = t;
        printList();
        break;
      }
    }
  }
}
void myList::insertDecOrderedNode(const myClass &val) {
  if (head == nullptr || val > head->value)
    push_front(val);
  else if (val <= tail->value)
    push_back(val);
  else {
    for (node *x = head; x != nullptr; x = x->next) {
      if (val > x->next->value) { // if val is lower than the next node insert it after the current node
        node *t = build_node(val, x->next);
        x->next = t;
        current_el = t;
        printList();
        break;
      }
    }
  }
}*/
void myList::deleteNodeByPos(const int &pos) {
  if (head == nullptr || pos > size)
    return;
  if (pos == 0) {
    node *t = head;
    head = current_el = head->next;
    if (t == tail)
      tail = head;
    delete t;
    size--;
  } else {
    int i = 0;
    for (node *x = head; x != nullptr; x = x->next, i++) {
      if (i == pos - 1) { // delete t after x at pos i
        node *t = x->next;
        x->next = x->next->next;
        if (t == tail)
          tail = x;
        delete t;
        size--;
        current_el = x;
        break;
      }
    }
  }
  printList();
}
/*
void myList::deleteNodeByVal(const myClass &val) {
  if (head == nullptr)
    return;
  if (head->value == val) {
    node *t = head;
    head = current_el = head->next;
    delete t;
    size--;
  } else {
    node *p = head;
    node *x = head;
    while (x != nullptr) {
      while (x != nullptr && x->value == val) { // delete t after x
        node *t = x;
        if (x == head) {
          head = head->next;
        }
        if (x == tail)
          tail = p;

        p->next = x->next;
        x = x->next;

        delete t;
        size--;
        current_el = p;
      }
      p = x;
      if (x != nullptr)
        x = x->next;
    }

  }
  printList();
}
*/

void myList::swapNodes(const int &pos0, const int &pos1) {
  int i = 0;
  if (pos0 == pos1 || pos0 > size || pos1 > size)
    return;
  int first = min(pos0, pos1);
  int second = max(pos0, pos1);

  node *p0, *p1, *x0, *x1;

  for (node *x = head; x != nullptr; x = x->next, i++) {
    if (i == first - 1) {
      p0 = x;
      x0 = x->next;
    } else if (i == second - 1) {
      p1 = x;
      x1 = x->next;
    }
  }
  if (first == 0) { //p0 and x0 are uninitialised
    p1->next = head; //p1 points to old head
    node *t = head->next;
    head->next = x1->next; //x0 points to the successor of x1
    x1->next = t; // x1 points to the successor of x0
    if (tail == x1) {
      tail = head;
    }
    head = x1; // update head
    current_el = head;

  } else {
    p1->next = x0; //p1 points to x0
    node *t = x0->next;
    x0->next = x1->next; //x0 points to the successor of x1
    x1->next = t; // x1 points to the successor of x0
    p0->next = x1; //p0 points to x1
    current_el = x1;
    if (tail == x1) {
      tail = x0;
    }
  }

  printList();
}
myClass myList::findNodeByPos(const int &pos) {
  int i = 0;
  for (node *x = head; x != nullptr; x = x->next, i++) {
    if (i == pos) {
      current_el = x;
      return x->value;
    }
  }
  return dummyValue;
}
void myList::printList() {
  cout << "head: " << (head ? head->value.GetData() : "") << "\ttail: " << (tail ? tail->value.GetData() : "")
       << "\tcurrent_el: " << (current_el ? current_el->value.GetData() : "") << '\n';
  for (node *t = head; t != nullptr; t = t->next) {
    cout << "|" << t->value << "| -> ";
  }
  cout << "nullptr\n";
}
