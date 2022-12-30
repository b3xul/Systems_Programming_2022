// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

#include "Matrix.hpp"
#include "../../Lab1/my_lab1/message.h"

int main() {

  Matrix<Message> me(2, 5);
  std::cout << me[0][1] << '\n';
  me[0][1] = Message(5);
  std::cout << me[0][1] << '\n';

/*  std::vector<std::string> a;
  a=std::vector<std::string> (3);*/

  Matrix<int> m(3, 3);
      int k = 0;

      for(int i=0; i<3; i++) {
          for(int j=0; j<3; j++) {
              m[i][j] = k;
              k++;
          }
      }

      for(int i=0; i<3; i++) {
          for(int j=0; j<3; j++) {
              std::cout << m[i][j] << " ";
          }
        std::cout << '\n';
      }

        if(m.find_auto(19)) {
          std::cout << "Found!" << '\n';
        }
        else {
          std::cout << "Not found." << '\n';
        }

  return 0;
}
