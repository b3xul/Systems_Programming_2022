//
// Created by bexul on 07/04/21.
//

#define DEBUG

#include "message.h"

using namespace std;

// L'inizializzazione va fatta fuori dalla classe, inteso che la variabile è definita (e non le assegni il valore)
// all'interno della classe, quindi la sua inizializzazione la fai in un modulo .cpp riferendoti alla variabile come
// <tipo> {const} <nome_classe>::<nome_var_statica> = <valore>;
//posso dichiarare un membro privato static in modo che sia comunque non accessibile da altri oltre che dai metodi della classe
//però essendo static posso inizializzarlo ad un valore predefinito senza dover passare dal costruttore
long Message::allocated_messages_ = 0;

/* Costruttore senza parametri (Strategia 6) */
Message::Message() : id_(-1), data_(nullptr), size_(0) {
#ifdef DEBUG
  cout << "Default constructor activated (" << this << ")\n";
#endif
}
/* Costruttore senza parametri (Strategia 5)
Message::Message(long id_, char *data_, size_t size_) {
  #ifdef DEBUG
  cout << "Generating Memory\n";
#endif
if (size_ != 0) {
#ifdef DEBUG
  cout << "Generating Memory\n";
#endif
    data_ = new(nothrow) char[size_];
    if (data_ == nullptr) {
      cerr << "error while creating the dynamic mem in Message\n";
    }
  } else data_ = nullptr;
} */

/* Costruttore con parametri(id+size)
Message::Message(long id, size_t size) : id_(id), size_(size) {
  allocated_messages_++;
#ifdef DEBUG
  cout << "Parametric constructor(id+size) activated (" << this << ")\n";
#endif
  if (size_ != 0) {
#ifdef DEBUG
    cout << "Generating Memory... " << this << '\n';
#endif
    data_ = mkMessage(size_);
#ifdef DEBUG
    cout << "Generated Message: " << string_message(data_) << "\n";
#endif
  } else
    data_ = nullptr;
}*/

/* Costruttore con parametri(size only) */
Message::Message(size_t size) : id_(allocated_messages_++), size_(size) {
#ifdef DEBUG
  cout << "Parametric constructor(size only) activated (" << this << ")\n";
#endif
  if (size_ > 0) {
#ifdef DEBUG
    cout << "Generating Memory... " << this << '\n';
#endif
    data_ = mkMessage(size_);
#ifdef DEBUG
    /*
        cout << "Generated Message: " << string_message(data_) << "\n";
    */
#endif
  } else {
    id_ = -1;
    size_ = 0;
    data_ = nullptr;
    allocated_messages_--;
  }
}

/* Costruttore di copia */
Message::Message(const Message &source) {
#ifdef DEBUG
  cout << "Copy constructor activated(" << this << ")\n";
#endif
  if (source.data_ != nullptr) {
#ifdef DEBUG
    cout << "Generating Memory... " << this << '\n';
#endif
    data_ = new(nothrow) char[source.size_ + 1];
    if (data_ != nullptr) {
      memcpy(data_, source.data_, source.size_);
      // strcpy(this->data, source.data);
      // copy(source.data, source.data+source.size+1, this->data); meglio! Passo puntatore(o iteratore) di inizio e
      // fine dell'area di memoria da copiare e poi puntatore di destinazione
#ifdef DEBUG
      /*
            cout << "Generated Message: " << string_message(data_) << "\n";
      */
#endif
      id_ = source.id_;
      size_ = source.size_;
      allocated_messages_++;
    }
  } else
    cerr << "error while creating the dynamic mem in Message inside the copy constructor\n";

}

/* Usando paradigma Copy & Swap */
void swapMessages(Message &a, Message &b) {
  swap(a.id_, b.id_);
  swap(a.size_, b.size_);
  swap(a.data_, b.data_);
}

/* Costruttore di movimento */
/*Message::Message(Message &&source) {
  #ifdef DEBUG
  cout << "Movement constructor activated!\n";
#endif
  id_ = source.id_;
  source.id_ = -1;
  size_ = source.size_;
  source.size_ = 0;
  data_ = source.data_;
  source.data_ = nullptr;
  //Non liberiamo delete[] source.data_!
}*/
Message::Message(Message &&source) noexcept: id_(-1), size_(0), data_(nullptr) {
#ifdef DEBUG
  cout << "Movement constructor activated!\n";
#endif
  swapMessages(*this, source);
}
/* Operatore di assegnazione per copia*/
/* OPZ.1 complex and error prone classic implementation */
/*Message &Message::operator=(const Message &source) {
#ifdef DEBUG
  cout << "Assignment operator activated(" << this << ")\n";
#endif
  if (this != &source) {
    if (data_ != nullptr) {
#ifdef DEBUG
      cout << "Deleting destination memory (" << string_message(data_) << ") before copying\n";
#endif
      delete[] data_; //evitare leak se oggetto destinazione contenteva già memoria dinamica
    }
    //N.B. If data_ is a null-pointer (nullptr), the function does nothing! (non serve controllo!)
    data_ = nullptr;

    id_ = source.id_;
    size_ = source.size_;
    if (source.data_ != nullptr) {
#ifdef DEBUG
      cout << "Generating Memory... " << this << '\n';
#endif
      data_ = new(nothrow) char[size_ + 1];
      if (data_ == nullptr) {
        cerr << "error while creating the dynamic mem in Message inside the assignment operator\n";
      }
      memcpy(data_, source.data_, source.size_ + 1);
#ifdef DEBUG
      cout << "Generated Message: " << string_message(data_) << "\n";
#endif
    }
  } else
    cerr << "Can't assign one object to itself!\n";
  return *this;
}*/
/* OPZ.2 Using copy and Swap paradigm and avoiding wasted computation in m6=m6 copies! */
Message &Message::operator=(const Message &source) {
#ifdef DEBUG
  /*cout << "Assignment operator by copy activated(" << this << ")\n";*/
#endif
  if (this != &source) {
    //allocated_messages_++; Already counted in the copy constructor
    Message temp = Message(source);
    swapMessages(*this, temp);
  } else
    cerr << "Can't assign one object to itself!\n";
  return *this;
}

/* OPZ.3 Using copy and Swap paradigm and wasting computation in m6=m6 copies! */
/* Message &Message::operator=(Message source) {
#ifdef DEBUG
  cout << "Assignment operator activated(" << this << ")\n";
#endif
  swapMessages(*this, source);
  return *this;
}
 * The Advantage is that the compiler takes care of the copy itself, but the
 * Only defect is that copies data even when it would not be necessary!! For this reason if we want to assign an
 * object to another we can do that using assignment by movement, without wasting computation. This can be done
 * using the copy and swap paradigm. If we want to make an hard copy, we could do that without using the copy and
 * swap mechanism so that we can avoid hard-copying the same data in case of m6=m6 assignment.
 */

/* Operatore di assegnazione per movimento */
Message &Message::operator=(Message &&source) noexcept {
#ifdef DEBUG
  cout << "Assignment operator by movement activated!\n";
#endif

  if (this != &source) {
    if (data_ != nullptr) {
#ifdef DEBUG
      cout << "Deleting destination memory (" << string_message(data_) << ") before copying by movement\n";
#endif
      delete[] data_; //evitare leak se oggetto destinazione contenteva già memoria dinamica
      allocated_messages_--;
    }
    id_ = -1;
    size_ = 0;
    data_ = nullptr;
    swapMessages(*this, source);
    //Non liberiamo delete[] source.data_!

  } else
    cerr << "Can't assign by movement one object inside itself!\n";

  return *this;
}

/* OPZ 1: operator defined inside the object*/
ostream &operator<<(ostream &out, const Message &m) {
  out << "(" << &m << ")" << "[id:" << m.id_ << "]" << "[size:" << m.size_ << "]" << (m.string_message(m.data_));
  return out;
}

/* OPZ 2: operator defined outside the object
ostream &operator<<(ostream &out, const Message &m) {
  string s(m.GetData());
  out << "[id:" << m.GetId() << "]" << "[size :" << m.GetSize()
      << "]" << "[data :" << s.substr(0, 20) << "]";
  return out;
}*/

// {const} <tipo> <nome_classe>::<nome_funzione> {const} { code...};
long Message::GetId() const {
  return id_;
}
const char *Message::GetData() const {
  return data_;
}
size_t Message::GetSize() const {
  return size_;
}
long Message::GetAllocatedMessages() {
  return allocated_messages_;
}

Message::~Message() {
  //Nel distruttore ha poco senso rimettere i valori di default/puntatori a memoria dinamica a null,
  // poiché distruggendo l'oggetto si perde anche il puntatore contenuto al suo interno, ma male non fa
#ifdef DEBUG
  /*cout << "Activated delete: " << *this << '\n';*/
#endif
  if (data_ != nullptr) {
    //Non serve per la delete, ma per aggiornare correttamente allocated_messages_!
    delete[] data_;
#ifdef DEBUG
    cout << "Delete completed" << '\n';
#endif
    // Non strettamente necessario resettare altri campi dato che verrà eliminata la memoria, ma dato che lo si fa
    // per data_ per non lasciare memoria inizializzata, tanto vale farlo anche per gli altri
    id_ = -1;
    size_ = 0;
    data_ = nullptr;
    allocated_messages_--;
  }
#ifdef DEBUG
  cout << "allocatedMessages: " << allocated_messages_ << '\n';
#endif

}

std::string Message::string_message(char *m) const {
  string out;
  out = "[data:" + ((m != nullptr) ? string(m) : "") + "]";
  return out;
}

char *Message::mkMessage(size_t n) {
  string vowels = "aeiou";
  string consonants = "bcdfghlmnpqrstvz";
  char *m = new(nothrow) char[n + 1]; // space for \0
  if (m != nullptr) {
#ifdef DEBUG
    cout << "Memory available!\nGenerating Message...\n";
#endif
    for (int i = 0; i < n; i++) {
      m[i] = i % 2 ? vowels[rand() % vowels.size()] : consonants[rand() % consonants.size()];
    }
    m[n] = '\0';
  } else
    cerr << "Error while creating the dynamic memory in Message inside the mkMessage function (called by the copy "
            "constructor)\n";
  return m;
}