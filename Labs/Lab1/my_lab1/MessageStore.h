//
// Created by bexul on 09/04/21.
//

#ifndef ES2__MESSAGESTORE_H_
#define ES2__MESSAGESTORE_H_

#include <optional>
#include <tuple>
#include "message.h"

class MessageStore {
 public:
  explicit MessageStore(int n);

  void add(Message &m); // inserisce un nuovo messaggio (id viene autoincrementato)

  /* The class template std::optional manages an optional contained value, i.e. a value that may or may not be present.
  A common use case for optional is the return value of a function that may fail.
  optional handles expensive-to-construct objects well and is more readable, as the intent is expressed explicitly.
  Any instance of optional<T> at any given point in time either contains a value or does not contain a value */
  std::optional<Message> get(long id); // restituisce il messaggio con l'id richiesto, se presente

  bool remove(long id); // cancella il messaggio con l'id richiesto, se presente

  std::tuple<int, int> stats(); // restituisce il numero di messaggi validi e di elementi vuoti ancora disponibili

  void compact(); // compatta l’array (elimina le celle vuole e riporta l’array alla dimensione multiplo di n minima in
  // grado di contenere tutte le celle)

  void displayMessages() const;

  ~MessageStore();

  [[nodiscard]] Message *GetMessages() const;
  void SetMessages(Message *messages);
  [[nodiscard]] int GetN() const;
  void SetN(int n);
  [[nodiscard]] int GetDim() const;
  void SetDim(int dim);
  [[nodiscard]] int GetValidCounter() const;
  void SetValidCounter(int valid_counter);

 private:
  Message *messages_;
  int n_; // dimensione iniziale storage
  int dim_; // dimensione allocata in memoria array
  int valid_counter_; // numero di messaggi validi salvati

  int find_position(long id);
  void realloc();
};
#endif //ES2__MESSAGESTORE_H_
