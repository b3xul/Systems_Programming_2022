//
// Created by bexul on 05/01/22.
//

#ifndef LAB2_ES3__BASE_H_
#define LAB2_ES3__BASE_H_
#include <string>

class Base {
 private:
  std::string name;
 public:
  explicit Base(const std::string& name);
  explicit Base(std::string &&rname);
  //restituisce il nome dell’oggetto
  [[nodiscard]] std::string getName() const;
  // metodo virtuale puro di cui fare override nelle classi derivate; restituisce il tipo dell’istanza (Directory o
  // File) codificato come un intero.
  [[nodiscard]] virtual int myType() const = 0;
  //metodo virtuale puro di cui fare override nelle classi derivate.
  virtual void ls(int indent) const = 0;

};

#endif //LAB2_ES3__BASE_H_
