#include <iostream>
#include <memory>

// Per capire meglio smart pointer possiamo provare a fare
class dummy {
 private:
  static int count_;
  int ref_;
 public:
  dummy() {
    ref_ = count_++;
    std::cout << "@dummy created " << ref_ << '\n';
  }
  ~dummy() {
    std::cout << "@dummy destroyed " << ref_ << '\n';
  }
};
int dummy::count_ = 0;
int main() {

  int var;
  std::shared_ptr<int *> a(std::make_shared<int *>(&var));
  //std::shared_ptr<int> a2(&var); //NO! Non si può chiamare il distruttore di un'oggetto non allocato sull'heap (int
  // var)

  std::cout << "&var=" << &var << '\n';
  std::cout << "sizeof(int)=" << sizeof(int) << '\n'; //4
  std::cout << "&a=" << &a << '\n'; // entrambi sullo stack ma distanze variabili!

  std::shared_ptr<dummy *> b(std::make_shared<dummy *>(new dummy)); //NO!
  {
    std::unique_ptr<dummy *> c(std::make_unique<dummy *>(new dummy)); //NO!
  }
  // non viene chiamato distruttore di b e c, perchè ho creato uno smart pointer a un puntatore a un oggetto, invece
  // che creare uno smart pointer a un oggetto!

  std::shared_ptr<dummy> d(new dummy);
  {
    std::unique_ptr<dummy> e(new dummy);
  }
  // viene chiamato prima il distruttore di e e poi quello di d!

  std::unique_ptr<dummy> f(nullptr);  // lo lascia fare, ma non viene chiamato costruttore
  std::shared_ptr<dummy> g(nullptr);  // lo lascia fare, ma non viene chiamato costruttore
  // std::weak_ptr<dummy> g(nullptr); error: no matching function for call to ‘std::weak_ptr<dummy>::weak_ptr(std::nullptr_t)’

  std::shared_ptr<dummy> h = std::make_shared<dummy>();
  std::shared_ptr<int> i = std::make_shared<int>();


  return 0;
}
