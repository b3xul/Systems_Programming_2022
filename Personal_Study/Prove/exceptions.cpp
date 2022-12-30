// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 15/01/22.
//

#include <iostream>
#include <exception>

double f1(double a, double b) {
  if (b == 0)
    throw std::runtime_error("division by 0");
  double c = a / b;
  return a;
}
void f2() {
  f1(7, 0);
}
void f3() {
  f2();
}
int main() {
  int retry = 2;
  while (retry > 0) {
    try {
      f3();
      break;
    } catch (std::exception &e) {
      //ReleaseExtraRes(); //funzione di supporto che re-imposta le variabili toccate da 'try'
      retry--;
      std::cerr << e.what() << std::endl;
      if (!retry)
        throw; //eccezione catturata e mandata al chiamante

    }
  }
}