//
// Created by bexul on 09/04/21.
//

#include "MessageStore.h"

#define DEBUG

using namespace std;

MessageStore::MessageStore(int n) : messages_(nullptr), n_(n), dim_(n), valid_counter_(0) {
#ifdef DEBUG
  cout << "MessageStore Costructor activated! (" << this << ")\n";
#endif
  if (n > 0) {
    messages_ = new(nothrow) Message[n];
    if (messages_ == nullptr) {
      cerr << "error while creating the dynamic mem in MessageStore constructor\n";
      n_ = 0;
      dim_ = 0;
    }
  } else {
    n_ = 0;
    dim_ = 0;
  }
}

void MessageStore::add(Message &m) {
  if (m.GetId() == -1)  // non aggiungo messaggi vuoti al contenitore!
    return;

  int i;

  if (valid_counter_ == dim_) { // MessageStore is full
    //serve riallocare il vettore!
    Message *tmp = messages_;
    messages_ = nullptr;
    messages_ = new(nothrow) Message[dim_ + n_];
    if (messages_ == nullptr) {
      cerr << "error while creating the dynamic mem in MessageStore constructor\n";
      return;
    }
    for (i = 0; i < dim_; i++) {
      messages_[i] = move(tmp[i]);  // I use the move operator so the main does not have ownership of the Message
      // once it is placed inside the MessageStore
    }

    dim_ += n_;
    delete[] tmp;

    //inserisco nuovo elemento al fondo
    messages_[valid_counter_++] = move(m);

    return;
  }

  bool inserted = false;
  for (i = 0; i < dim_ && !inserted; i++) {
    if (messages_[i].GetId() == -1) {
      messages_[i] = move(m);
      valid_counter_++;
      inserted = true;
    }
    /* else if (m.GetId() == messages_[i].GetId()) {
      messages_[i] = move(m); //svuoto il messaggio precedente, che verra' poi cancellato, e lo sostituisco con il nuovo
      inserted = true;
    }*/
  }
}
int MessageStore::find_position(long id) {
  for (int i = 0; i < dim_; i++) {
    if (messages_[i].GetId() == id)
      return i;
  }
  return -1;
}
std::optional<Message> MessageStore::get(long id) {
  int position = find_position(id);
  if (position != -1) {
    return messages_[position];
  } else {
    return nullopt;
  }
}

bool MessageStore::remove(long id) {
  int position = find_position(id);
  if (position != -1) {
    messages_[position].~Message();
    valid_counter_--;
    return true;
  }
  return false;
}

std::tuple<int, int> MessageStore::stats() {
  return make_tuple(valid_counter_, dim_ - valid_counter_);
}
void MessageStore::compact() {
  int new_dim = n_;
  while (new_dim < valid_counter_) {
    new_dim += n_;
  }
  // oppure     new_dim = valid_messages%n==0 ? valid_messages : (valid_messages/n +1) * n;
  Message *new_messages = nullptr;
  new_messages = new(nothrow) Message[new_dim];
  if (messages_ == nullptr) {
    cerr << "error while creating the dynamic memory for compacting the MessageStore\n";
    return;
  }

  int j = 0;
  for (int i = 0; i < dim_; i++) {
    if (messages_[i].GetId() != -1) {
      new_messages[j++] = move(messages_[i]);
    }
  }
  messages_ = new_messages;
  dim_ = new_dim;
}

void MessageStore::displayMessages() const {
  cout << this << " contains:\n";
  for (int i = 0; i < dim_; i++) {
    cout << i << ':' << messages_[i] << '\n';
  }
}
MessageStore::~MessageStore() {
#ifdef DEBUG
  cout << "MessageStore Destructor activated! (" << this << ")\n";
#endif
  for (int i = 0; i < dim_; i++) {
    messages_[i].~Message();
  }
  /*
  delete [] this->messages;
  this->messages = nullptr;
   */
}

Message *MessageStore::GetMessages() const {
  return messages_;
}
void MessageStore::SetMessages(Message *messages) {
  messages_ = messages;
}
int MessageStore::GetN() const {
  return n_;
}
void MessageStore::SetN(int n) {
  n_ = n;
}
int MessageStore::GetDim() const {
  return dim_;
}
void MessageStore::SetDim(int dim) {
  dim_ = dim;
}
int MessageStore::GetValidCounter() const {
  return valid_counter_;
}
void MessageStore::SetValidCounter(int valid_counter) {
  valid_counter_ = valid_counter;
}
