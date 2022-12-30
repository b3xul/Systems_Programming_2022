//
// Created by Alessandro on 24/04/21.
//
#include <iostream>
#include <memory>

template<typename T>
class Vect {
 private:
  std::size_t _n;
  std::size_t _allocated_n;
  std::unique_ptr<T[]> _ptr; // cosa succede mettendo invece shared pointer?

 public:
  // Non ho bisogno di implementare un distruttore, perche' quando il costruttore di default prova a cancellare _ptr,
  // va a chiamare il distruttore di default della classe T

  // mettendo explicit davanti obbliga il compilatore a usare il movimento invece della copia
  Vect(int n = 10) : _n(n), _ptr(new T[n]), _allocated_n(n) {
  }

  Vect(const Vect &src) = delete; // Impedire costruttore di copia

  // Impedire anche operatore di copia

  //  Vect(const Vect &&src) non va bene perche' se passo una const reference l'unica cosa che posso farne e' una
  //  copia, non posso farne move
  Vect(Vect &&src) { // parametro passato come reference
    //_ptr=src._ptr; non si puo' fare perche' sarebbe una copia

    /* _ptr = std::move(src._ptr);  Questo si puo' fare solo se src non e' const. Ma come viene gestita la memoria
     * dinamica che stava in _ptr? Avremmo memory leak! Meglio liberare _ptr e poi fare swap
     */

    _ptr.swap(src._ptr);

    _n = src._n;
    _allocated_n = src._allocated_n;

    src._ptr.reset(); // libera memoria precedentemente puntata da _ptr per evitare memory leak
    src._n = 0; // resetto la memoria precedentemente puntata da _ptr cosicche' rimanga un oggetto vuoto, dummy
    src._allocated_n = 0;

  }

  //operatore di movimento...

  //operatore di accesso in subscripting, per poter accedere agli oggetti contenuto nel vettore (ritorna reference
  // all'oggetto).
  T &operator[](std::size_t index) {
    return _ptr[index];
  }

  //"regola dei 2", questi 2 operatori devono andare in coppia
  //operatore di accesso in subscripting COSTANTE
  const T &operator[](std::size_t index) const {
    return _ptr[index];
  }

  bool resize(std::size_t new_size) { // tipi di base passati per valore
    if (new_size > _allocated_n) {
      // Approccio che non usa puntatori grezzi, si potrebbe anche gestire diversamente. Minimo numero di operazioni
      std::unique_ptr<T[]> new_mem(new T[new_size]); // servirebbe try catch intorno perche' uso new che throwa
      // memcpy copia i Byte (shallow copy)
      // std::copy si muove elemento per elemento e richiama il costruttore di copia.
      // copy (puntatore (iteratore) di inizio, puntatore di arrivo, puntatore della zona di memoria di output)

      //std::copy(_ptr, _ptr + _n, new_mem); //error: no match for ‘operator+’ (operand types are ‘std::unique_ptr<int
      // [], std::default_delete<int []> >’ and ‘std::size_t’ {aka ‘long unsigned int’})
      // candidate: ‘operator+(int, std::size_t {aka long unsigned int})’ <built-in>
      // no known conversion for argument 1 from ‘std::unique_ptr<int [], std::default_delete<int []> >’ to ‘int’
      // Fondamentalmente non e' in grado di usare l'operatore + con un puntatore smart!

      //usando .get() accedo alla memoria direttamente (puntatore grezzo) per cui in questo caso l'operatore + e'
      // definito e si puo' usare
      std::copy(_ptr.get(), _ptr.get() + _n, new_mem.get());

      _ptr.swap(new_mem); // dopo questa istruzione new_mem puntera' alla memoria che era prima contenuta in _ptr.
      // Essendo unique pointer siamo sicuri che, al termine della funzione resize, new_mem raggiunga la fine della
      // sua lifetime e venga per questo liberata!
      _n = new_size;
      _allocated_n = new_size;
      return true;
    } else {
      _n = new_size; // evito di implementare politica di resize a stringere, cosi' se in futuro mi serve nuovamente
      // lo spazio che gia' avevo allocato, non devo chiederlo di nuovo
      return false;
    }
  }

  void printPTR() {
    std::cout << _ptr.get() << '\n';  // get() ritorna puntatore const (sola lettura)
  }
};

int main() {
  Vect<int> v;
  /*Vect<int> v2 = v;
   se commento costruttore di copia:
   * error: use of deleted function ‘Vect<T>::Vect(const Vect<T>&) [with T = int]’ +
   * ‘Vect<int>::Vect(const Vect<int>&)’ is implicitly declared as deleted because ‘Vect<int>’ declares a move
   * constructor or move assignment operator
   se elimino costruttore di copia: error: use of deleted function ‘Vect<T>::Vect(const Vect<T>&) [with T = int]’
   se uso costruttore di copia Vect(const Vect &src) {}: v contiene il suo valore, mentre v2 contiene nullptr (copia
   fallita, ma costruttore di default funziona!)
   */
  Vect<int> v2 = std::move(v);

  v.printPTR(); // svuotato, ora v contiene nullptr
  v2.printPTR();

  v2[0] = 10; // operatore di accesso in subscripting (lettura+scrittura)
  v2.resize(12);
  v2.printPTR(); // l'indirizzo in cui si trova v2 e' cambiato, perche' la resize e' avvenuta correttamente,
  // spostando la memoria contenuta nel vettore
  std::cout << v2[0] << '\n'; // operatore di accesso in subscripting COSTANTE (sola lettura)
  return 0;
}
