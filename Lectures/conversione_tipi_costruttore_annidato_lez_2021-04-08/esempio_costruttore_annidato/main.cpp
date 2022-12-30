#include <iostream>

class Base {
 public:
  Base() : _code(0) {
    std::cout << "Costruttore Base (" << this << ") " << _code << "\n";
  };
  virtual void baz() {
    print_in_baz();
  };
  virtual ~Base() {
    std::cout << "Distruttore Base\n";
  }

 protected:
  int _code;
  void print_in_baz() { /* Se provo a eseguire dalla classe Der, Base::baz(){ print_in_baz()}, con print_in_baz()
 * private, funziona, perchè baz() è nello stesso contesto di print_in_baz(). Quello che non posso fare è richiamare
 * print_in_baz() direttamente da Der, perchè sono in un altro contesto! */
    std::cout << "Baz base (" << this << ") " << _code << "\n";
  }
};

class Der : public Base {
 public:
  Der() : Base() {
    _code = 10; // posso accedere a _code perchè protected (o public)
    std::cout << "Costruttore Der (" << this << ") " << _code << "\n";
    /*Base::*/baz(); // se non avessi ridefinito virtual void Base::baz(), non mi avrebbe permesso di eseguirlo,
    // perchè virtuale!

    //print_in_baz();
  }
  void baz() override { // override perchè sta implementando la funzione VIRTUAL baz() di Base
    std::cout << "baz() di Der\n";
    print_in_baz();
  };

  /*float() {
    sono in grado di definire una conversione a un tipo standard?
  };*/

  ~Der() override {
    std::cout << "Distruttore Der\n";
  }
};

int main() {
  Der *obj;
  Base obj2;
  std::cout << sizeof(Base) << '\n'; // = 4 senza metodi virtuali, = 16 con 1 o più metodi virtuali!
  std::cout << sizeof(Der)<< '\n';//  = 4 senza metodi virtuali, = 16 con 1 o più metodi virtuali!

  try {
    obj = new Der();
  } catch (std::bad_alloc &e) {
    std::cerr << "Bad allocation for the object: " << e.what() << '\n';
    return -1;
  }
  obj->baz();
  delete obj;

  //obj = &obj2; // trovare il modo di effettuare questa conversione:
  obj = static_cast<Der *>(&obj2);
  obj->baz();

  return 0;
}
