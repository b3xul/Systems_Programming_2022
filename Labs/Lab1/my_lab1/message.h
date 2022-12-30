//
// Created by bexul on 07/04/21.
//

#ifndef LAB1_P_MESSAGE_H
#define LAB1_P_MESSAGE_H

#include <iostream> //size_t
#include <cstring> //memcpy

class Message {
 public :
  /* construttore senza parametri, da richiamare con Message m1, equivalente a scrivere Message m1=Message();
  1. Message(); (default);
  2.h Message() : id_(0), data_(nullptr), size_(0) { std::cout << "Generating Memory\n" };
  3.h Message(long id_ = 0, char *data_ = nullptr, std::size_t size_ = 0) {std::cout << "Generating Memory\n";  };
  4.h Message(long id_ = 0, char *data_ = nullptr, std::size_t size_ = 0); N.B. DEFAULT VALUES IN .H!
  4.cpp Message::Message(long id_, char *data_, std::size_t size_) {  cout << "Generating Memory\n";};
  5.h Message();
  5.cpp Message::Message() {long id_ = 0; char *data_ = nullptr; std::size_t size_ = 0; cout << "Generating
  Memory\n";};
  6.h Message();
  6.cpp Message::Message() : id_(0), data_(nullptr), size_(0){cout << "Generating Memory\n";  };
  USIAMO LA STRATEGIA 6. PER AVERE TUTTO IL CODICE NEL .cpp ed essere più pulito
  */
  Message();

  /* costruttore con parametri(id+size)
  Message(long id, std::size_t size);*/

  /* Prefixing the explicit keyword to the constructor prevents the compiler from using that constructor for implicit
   * conversions. Adding it to the above class will create a compiler error at the function call DoBar (42). It is
   * now necessary to call for conversion explicitly with DoBar (Foo (42))
   */
  /* costruttore con parametri(size only) */
  explicit Message(std::size_t size);

  /* costruttore di copia */
  Message(const Message &source);

  /* Per paradigma Copy and Swap */
  friend void swapMessages(Message &a, Message &b);

  /* noexcept: since you should be just be copying a few basic types and nulling out the values in the other object,
   * those operations shouldn't throw. */
  /* costruttore di movimento */
  Message(Message &&source) noexcept;

  /* operatore di assegnazione */
  Message &operator=(const Message &source);
  /*Message &operator=(Message source);*/

  /* operatore di assegnazione per movimento*/
  Message &operator=(Message &&source) noexcept;

  /* OPZ 1: operator defined inside the object
friend serve per dire al compilatore che implemento in message una funzione (operator) che pero' non e' un metodo della classe message
(non va chiamata usando Message::operator), e' una funzione che puo' interagire DIRETTAMENTE con la classe!
(e' una funzione amica che non ha bisogno dei getter e setter) */
  friend std::ostream &operator<<(std::ostream &out, const Message &m);

  /* If a function declared nodiscard or a function returning an enumeration or class declared nodiscard by value is
   * called from a discarded-value expression other than a cast to void, the compiler is encouraged to issue a
   * warning.
   * Se provo a fare m1.GetId(); senza usarne il risultato ottengo come warning: Ignoring return value of function
   * declared with 'nodiscard' attribute*/
  /* Getters */
  /* Quando un metodo di una classe non cambia alcun attributo (anche potenzialmente variabile) della classe, lo si
  marca con la keyword const alla fine della definizione del metodo (es. int Message::getId() const;). Questo
  permette al compilatore di fare un controllo aggiuntivo in fase di compilazione per aiutarci a evitare possibili
  errori.
  Ritornare un valore const/usare come parametro un valore const significa che stai ritornando/usando un
  oggetto/variabile immutabile (dall'esterno), che ha un significato diverso dal fatto che il metodo internamente
  non va a modificare nessun campo della classe. */
  [[nodiscard]] long GetId() const;
  [[nodiscard]] const char *GetData() const;  //così non passa di proprietà a chi lo richiede, controllo in più! (ma funziona?)
  [[nodiscard]] size_t GetSize() const;
  static long GetAllocatedMessages(); // const non va bene perchè error: static member function ‘static long int
  // Message::GetAllocatedMessages()’ cannot have cv-qualifier
  /* Distruttore */
  ~Message();

 private:
  long id_;
  char *data_;
  std::size_t size_;
  static long allocated_messages_;

  std::string string_message(char *m) const;
  char *mkMessage(size_t n);
};

/* OPZ 2: operator defined outside the object
std::ostream &operator<<(std::ostream &out, const Message &m);*/

#endif //LAB1_P_MESSAGE_H
