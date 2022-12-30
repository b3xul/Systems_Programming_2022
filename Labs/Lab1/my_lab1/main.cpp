#include <iostream>
#include <vector>
#include "message.h"
#include "DurationLogger.h"
#include "MessageStore.h"

using namespace std;

#define ES2
#define N_BUFF 1000

void assignment_by_copy(Message *a, Message *b);
void assignment_by_movement(Message *a, Message *b);
void test_store();

struct AllocationMetrics {
  uint32_t TotalAllocated = 0;
  uint32_t TotalFreed = 0;
  uint32_t CurrentUsage() {
    return TotalAllocated - TotalFreed;
  }
};
static AllocationMetrics s_AllocationMetrics;

void *operator new(size_t size) {
  s_AllocationMetrics.TotalAllocated += size;
  cout << "Allocated " << size << " bytes\n";
  return malloc(size);
}
void operator delete(void *memory, size_t size) {
  s_AllocationMetrics.TotalFreed += size;
  cout << "Deallocated " << size << " bytes\n";
  free(memory);
}

static void PrintMemoryUsage() {
  cout << "Memory usage: " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}

int main() {

  using namespace std;

#ifdef ES1
  /*
    //1. costruttore di default
    Message m1 = Message(); //equivalente a Message m1;

    cout << "m1:" << m1 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';
    cout << "-----------------------\n";

    //2. costruttore con parametri(size only)
    Message m2{10};  //equivalente a Message m2=Message(10);
    cout << "m2:" << m2 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';
    cout << "-----------------------\n";

    //3. costruttore di copia
    Message m3{m2};
    //equivalente a Message m3=Message(m2); N.B. equivalente a Message m3 = m2;->NON USA ASSIGNMENT OPERATOR!
    cout << "m2:" << m2 << '\n';
    cout << "m3:" << m3 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';
    cout << "-----------------------\n";

    //4. costruttore di movimento
    Message m4{move(m3)};
    //non ci sono sintassi equivalenti perchè std::move è l'unica funzione in grado di convertire un lvalue in un xvalue
    cout << "m3=" << m3 << '\n';
    cout << "m4=" << m4 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';
    cout << "-----------------------\n";

    //5. operatore di assegnazione
    //Message m5 = m4; Usa direttamente Copy constructor
    Message m5; // Default constructor
    cout << "m5=" << m5 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';

    m5 = m4; // Copy constructor

    cout << "m4=" << m4 << '\n';
    cout << "m5=" << m5 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';

    Message m6{5};
    cout << "m6=" << m6 << '\n';
    m6 = m5;
    cout << "m6=" << m6 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';

    m6 = m6; // Wasted computation!
    cout << "m6=" << m6 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';
    cout << "-----------------------\n";

    //6. operatore di assegnazione per movimento
    Message m7;
    cout << "m7=" << m7 << '\n';
    m7 = move(m6);
    cout << "m6=" << m6 << '\n';
    cout << "m7=" << m7 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';

    Message m8{7};
    cout << "m8=" << m8 << '\n';
    m8 = move(m7);
    cout << "m7=" << m7 << '\n';
    cout << "m8=" << m8 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';

    m8 = move(m8);
    cout << "m8=" << m8 << '\n';
    cout << "allocated_messages: " << Message::GetAllocatedMessages() << '\n';
    cout << "-----------------------\n";
  */

  //Spunti di riflessione

  /*
   // 1. In entrambi i casi si usa il costruttore di default, ma...
    Message buff1[N_BUFF];  //This is allocated on the stack and it is automatically deleted at the end of the main because
    // its lifecycle ends with the function where it is created.
    Message *buff2 = new Message[N_BUFF]; //This is dinamic memory, the main allocated the array (on the heap!), so it is
    // its responsibility to free it properly!

    //2. Parametric constructor activated, Assignment operator by movement activated! Se
    // disponibile si usa l'operatore di assegnazione per movimento!

    //3. Non serve fare la delete del primo buffer perche' verra' fatta in automatico al termine del main, mentre per
    // il secondo e' necessario
    delete[] buff2;

    buff1[0] = Message(100);
    cout << buff1[0] << '\n';

    buff1[3] = Message(100);
    cout << buff1[3] << '\n';

    4. Tutto ok
    5. Se commento il costruttore di default no matching function for call to ‘Message::Message()’
    6. Se lo rendo privato ‘Message::Message()’ is private within this context
    7. Se rimuovo l'operatore di assegnazione per copia, usa quello di movimento (lo userebbe comunque)! Assignment
    operator by movement activated!

    Se rimuovo l'operatore di assegnazione per movimento, usa quello di copia! Assignment operator by copy activated!

    Se li rimuovo entrambi, se e' stato definito move constructor fallisce compilazione (ma come, hai definito il
    move constructor ma non gli operatori di movimento/copia??)
    error: use of deleted function ‘constexpr Message& Message::operator=(const Message&)’

    Se li rimuovo entrambi e anche il costruttore di movimento, crea Message(100) usando il costruttore parametrico, poi
    lo copia in buff1[0] come farebbe con qualunque altro oggetto, compreso il puntatore all'oggetto dinamico
    allocato in Message(100), se non fosse che poi esso viene liberato, avendo solo un puntatore che punta a garbage
    dopo l'assegnazione!
    Si rischia anche una free(): double free detected in tcache 2
    Perche' la stessa stringa viene liberata sia dopo la morte dell'oggetto Message(100) che dopo la morte di
    buff1[0]! Ci sono 2 funzioni (costruttore parametrico e main) entrambi con il possesso di quella memoria dinamica
    e entrambi cercano di liberarla! Per questo nel nostro operatore di assegnazione facciamo memcpy (cosi' quando il
    costruttore termina, libera la sua stringa, ma rimane la stringa appartenente al main, che quindi deve anch'esso
    liberarla!) e invece nell'operatore di assegnazione per movimento rubiamo la proprieta' della stringa, che quindi
    non viene liberata dall'operatore, ma solo dal main in seguito!) */

  //Message m=22; not allowed with explicit keyword, you need Message m
  Message m2(22); //always allowed
  Message m3=Message(22); //always allowed
  Message buff3[N_BUFF], buff4[N_BUFF], buff5[N_BUFF];
  int i;
  for (i = 0; i < N_BUFF; i++)
    buff3[i] = Message(100);

  cout << "-----------------\n";

  /*  assignment_by_copy(buff3, buff4);*/
  assignment_by_movement(buff3, buff5);

  return 0;
#endif

#ifdef ES2
  MessageStore ms0{-4};

  MessageStore ms1{2};

  Message m1{5};
  ms1.add(m1);
  Message m2{6};
  ms1.add(m2);
  Message m3{7};
  ms1.add(m3);
  Message m4{8};
  ms1.add(m4);
  ms1.displayMessages();

  ms1.remove(6);
  ms1.remove(1);
  ms1.displayMessages();

  cout << *(ms1.get(2)) << '\n';
  cout << *(ms1.get(8)) << '\n';

  tuple<int, int> stats = ms1.stats();
  cout << "messaggi validi: " << get<0>(stats) << "\nposizioni libere: " << get<1>(stats) << '\n';

  ms1.compact();
  ms1.displayMessages();
  cout << "--------------------------\n";

  test_store();
  /* Spunti di riflessione (problematiche)
   * Message* get(long id); -> perdita possesso puntatore
• Message get(long id); -> se id non presente serve tornare un dummy message
• Message& get(long id); -> può essere nullptr?
  std::optional<Message> get(long id); -> bisogna estrarre Message da optional usando * per usarlo
• void get(int id, Message message); -> copia inutile del message passato come parametro, non viene aggiornato
• void get(int id, Message *message); -> non best practice risultato tra i parametri, perdita possesso puntatore
• void get(int id, Message **message); -> non best practice risultato tra i parametri, perdita possesso puntatore
• void get(int id, Message &message); -> non può essere reference a nullptr
• void get(int id, const Message &message); -> non può dare risultato
   */
#endif
}

void assignment_by_copy(Message *a, Message *b) {
  DurationLogger copy{"Assignment by copy"};  //Senza darle il nome viene immediatamente distrutta!
  for (int i = 0; i < N_BUFF; i++)
    b[i] = a[i];  //<<<<<<<<<<<<<< ending [ Assignment by copy ] after [0.011926 seconds]
}

void assignment_by_movement(Message *a, Message *b) {
  DurationLogger movement("Assignment by movement");
  for (int i = 0; i < N_BUFF; i++)
    b[i] = move(a[i]);  //<<<<<<<<<<<<<< ending [ Assignment by movement ] after [0.000073 (7.3e-05) seconds]!!!
}

void test_store() {
  MessageStore store(10);
  std::vector<long> ids; // remember message ids for testing deletes

  for (int i = 0; i < 100; i++) {
    Message m(1000000);
    ids.push_back(m.GetId());
    store.add(m);
    Message m1(1000000);
    ids.push_back(m1.GetId());
    store.add(m1);
  }
  std::cout << "dimension: " << store.GetDim() << std::endl;
  int available = 0, vacancies = 0;
  tie(available, vacancies) = store.stats();
  cout << "valid " << available << " vacancies " << vacancies << endl;
  // remove odd ids (50 ids)
  for (auto id: ids) {
    if (id % 2) {
      store.remove(id);
    }
  }
  std::cout << "dimension: " << store.GetDim() << std::endl;
  available = 0;
  vacancies = 0;
  tie(available, vacancies) = store.stats();
  cout << "valid " << available << " vacancies " << vacancies << endl;
  store.compact();
  std::cout << "dimension: " << store.GetDim() << std::endl;
  available = 0;
  vacancies = 0;
  tie(available, vacancies) = store.stats();
  cout << "valid " << available << " vacancies " << vacancies << endl;
}