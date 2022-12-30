// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 08/01/22.
//
#ifndef ES2_MATRIX__MATRIX_HPP_
#define ES2_MATRIX__MATRIX_HPP_

#include <vector>
// Try to include your vector !
//#include "/mnt/26158F5879578F52/Università/Magistrale/Programmazione_di_sistema/Programmazione/Labs/Lab3/es1_my_vector_template/MyVector.hpp"
/*template<typename T>
class RowPointer {
 private:
  std::vector<T> row;
  int cols;
 public:
  RowPointer() {
  };
  RowPointer(int cols) : cols(cols) {
    this->row.reserve(cols);
  };

*//*  ~RowPointer(){
      delete this->row;
  }*//*
  T &operator[](int index) {
    return this->row[index];
  }
};*/

template<typename T>
class Matrix {
 private:
  std::vector<std::vector<T>> matrix;
  //RowPointer<T> *matrix;
  int rows;
  int cols;
 public:

  Matrix(int rows, int cols) {
    /*  Preallocate default items
        this->matrix = std::vector<std::vector<T>> (rows);
          for(int i=0;i<rows;i++){
            this->matrix[i] = std::vector<T> (cols);
          }*/
    this->matrix.reserve(rows);
    for (int i = 0; i < rows; i++) {
      this->matrix[i].reserve(cols);
    }
  };
  /*  Matrix(int rows, int cols) : rows(rows), cols(cols) {
      this->matrix = new RowPointer<T>[rows];
      for (int i = 0; i < rows; i++) {
        this->matrix[i] = RowPointer<T>(cols);
      }
    };*/
  ~Matrix() {
    /*    for (int i = 0; i < rows; i++) {
          ~this->matrix[i];
        }*/
    //delete [] matrix;
  }
  // Doppio subscripting
  //T& operator () (int rows, int cols){} -> too easy!

  // L'operatore di subscripting della matrice non ritornerà direttamente l'elemento, ma una riga della matrice, poi...
  std::vector<T> &operator[](int index) {
    return this->matrix[index];
  }
  /*  RowPointer<T> &operator[](int index) {
      return this->matrix[index];
    }*/
  // ...se che anche l'oggetto riga (nel nostro caso abbiamo usato un oggetto già fatto std::vector, ma potremmo
  // creare noi il nostro oggetto riga, l'importante è ridefinire l'operatore []) a sua volta fa l'overload
  // dell'operatore [], ritornando l'elemento, il compilatore estrarrà prima la riga, poi l'elemento e siamo riusciti
  // a usare il doppio subscripting!

  /*
    T& operator [] (int index){
      return row[index];
    }
  */

  // add row -> add 1 vector to vector of vectors

  // add column -> pass through all the vectors of the vector and increase every size by 1! Very poor performance! At
  // least manage it as the vector resizing (log)!


  bool find_it(const T& val) {
      for(int i=0; i<this->rows; i++) {
          for(typename std::vector<T>::iterator it = this->matrix[i].begin(); it != this->matrix[i].end(); it++) {
              if(*it == val) {
                  return true;
              }
          }
      }
      return false;
  }

  bool find_auto(const T& val) {
      for(const auto& r : this->matrix) {
          for(const auto& el : r) {
              if(el == val) {
                  return true;
              }
          }
      }
      return false;
  }
};

#endif //ES2_MATRIX__MATRIX_H_

