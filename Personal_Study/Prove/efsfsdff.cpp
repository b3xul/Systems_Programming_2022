// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 26/12/21.
//

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by bexul on 22/12/21.
//

#include <iostream>
#include <string>

using namespace std;

class Entity {
 private:
  int m_X, m_Y;
  int *foo() {
    return nullptr;
  }
 public:
  int GetX() {
    return m_X;
  }

  void bar() {
    int *buffer = foo();
    buffer[0] = 0;
  }
};
void PrintEntity(Entity &e) {

  cout << e.GetX() << '\n';
}
int main() {

  int aa = 2;
  if (aa == 3)
    cout << "ciaos";

}