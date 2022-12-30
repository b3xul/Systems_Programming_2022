//
// Created by bexul on 27/08/21.
//

#ifndef LAB2_P__MYLIST_H_
#define LAB2_P__MYLIST_H_

/*#include "../Lab1_P/my_lab1/message.h"

typedef Message myClass;*/
#include <iostream>

typedef int myClass;
const myClass dummyValue = -1; //could be replaced with Optional

class myList {
 private :
  typedef struct listNode {
    myClass value;
    struct listNode *next;
  } node;

  node *head, *tail, *current_el;
  int size;

  // Return the pointer to a new node
  node *new_node();

  node *build_node(const myClass &val, node *next);

 public :
  myList();
  ~myList();
  /* Insert an element at the head of the list. Current element moved to
  the new head.*/
  void push_front(const myClass &val);
  /* Return the value of the node at the head of the list and delete the
  head node. Current element moved to the new head. */
  myClass popNode();
  /* Insert an element at the tail of the list. Current element moved to
  the tail. */
  void push_back(const myClass &val);
  /* Insert an element at the pos -th element of the list. Current
  element moved to the new element */
  void insertNode(const myClass &val, const int && pos);
  /* Insert an element in a sorted list with increasing values. Current
  element moved to the new element.
   N.B. I assume that the user only uses insertIncOrderedNode to insert nodes if he wants to maintain the increasing
   ordering. */
  void insertIncOrderedNode(const myClass &val);
  /* Insert an element in a sorted list with decreasing values. Current
  element moved to the new element.*/
  void insertDecOrderedNode(const myClass &val);
  /* Delete the pos -th node of the list. Current element moved to the
  pos -th previous element. */
  void deleteNodeByPos(const int && pos);
  /* Delete all the node of the list containing val. Current element
  moved to the previous element of the deleted element.*/
  void deleteNodeByVal(const myClass &val);

  /* Swap the content of node at pos0 and pos1. Current element moved
  to the pos0 -th element. */
  void swapNodes(const int && pos0, const int && pos1);
  /* Return the value of the pos -th node. Current element moved to the
  found element.*/
  myClass findNodeByPos(const int && pos);
  /* Return the position of the first occurrence of a node containing
  val. Current element moved to the found element. */
  int findNodeByVal(const char &val);

  void printList();

};

#endif //LAB2_P__MYLIST_H_
