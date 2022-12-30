#include <iostream>
#include <vector>
#include <any>
#include <functional>
#include <mutex>
using namespace std;

// typename... Args significa che Args e' una std::tuple (come pair ma di quanti elementi si vuole. non si usa .first
// e .second, ma si accede ai campi usando .get(1), .get(2), .get(3)...
template<typename F, typename...Args>
class MyPackagedTask {  // la classe della standard library PackageTask funzionerà con lo stesso funzionamento di
  // questa, gestita in modo più ottimizzato (sicuramente come struttura dati contenitore, poi internamente anche?)
  F &_f;
  std::tuple<Args...> _args;
  typedef decltype(apply(_f, _args)) R; // la funzione standard apply utilizza la funzione invoke per lanciare _f con
  // i parametri _args.
  // R e' un tipo che definisce un oggetto che corrisponde all'invocazione di una funzione!

  // typedef decltype significa definiscimi il tipo R come il tipo che ho dichiarato come ritorno dell'apply.
 public:
  explicit MyPackagedTask(F &&fn, Args &&... args) : _f(std::forward<F>(fn)), _args(std::forward<Args>(args)...) {
    // Passiamo al costruttore puntatore a funzione/oggetto funzionale/f lambda come primo argomento e il secondo
    // parametro e' una tupla contenente tutti gli argomenti di quella funzione.
    // Per riempire le variabili nell'oggetto da creare uso la funzione forward che preleva la reference passata come
    // parametro e restituendola come ritorno.
  }
  R operator()() {
    return apply(_f, _args); // qua non sto invocando _f! Sto RITORNANDO l'invocazione della funzione!
  }
};

void test(int a, int b) {
  cout << a << " vs " << b << '\n';
}

int test2(int a, int b) {
  cout << "I'm running...\n";
  return a - b;
}

void test3(int a) {
  cout << "solo: " << a << '\n';
}

#define V1

int main() {
#ifdef V1
  MyPackagedTask prova1(test, 10, 20);  // passo puntatore a funzione e i suoi parametri (numero puo' variare tra un
  // MyPackagedTask e un altro!!!
  MyPackagedTask prova2(test2, 20, 50);
  MyPackagedTask prova3(test3, 60);

  prova1(); // richiamo un oggettoMyPackageTask senza parametri, perche' andra' a usare quelli che ha salvato nell'
  // oggetto stesso! Equivalente a fare test(10,20)
  std::cout << prova2() << std::endl; // MyPackagedTask riesce a inferire dalla funzione test2 il tipo di ritorno R
  // dell'invocazione dell'oggetto funzionale MyPackagedTask
  auto res = prova2();
  std::cout << res << std::endl; // res e' un int!
  prova3();
#endif
#ifdef V2
  // std::vector<std::function<Come posso gestire il fatto che le funzioni da lanciare abbiano tipo di ritorno
  // diverso?>> tasks; Si'! In alcuni casi...
  std::vector<std::function<std::any()>> tasks; // Memorizza un vettore di functional object in cui la funzione che
  // deve eseguire non ha parametri e ha un ritorno di tipo any: ritorno GENERICO che deve essere inserito in un
  // container di tipo any e che fornisce metodi per capire di che tipo e' il ritorno e come deve essere fatto il
  // cast per gestire quel dato

  //tasks.emplace_back(MyPackagedTask(test, 10, 20)); ANY NON SUPPORTA FUNZIONI CON RITORNO DI TIPO VOID, NEANCHE
  // USANDO OPTIONAL (significa che ritorno è opzionale, non che ritorno possa esssere vuoto)
  tasks.emplace_back(MyPackagedTask(test2, 20, 50));

#endif
#ifdef V3
  std::vector<std::function<void()>> tasks; // Per i nostri thread basta far sì che il valore di ritorno sia vuoto!

  tasks.emplace_back(MyPackagedTask(test, 10, 20)); // inserisco nel mio contenitore di tasks (BlockingQueue nel
  // disegno) il mio oggetto MyPackagedTask contenente una funzione con un numero variabile di parametri
  tasks.emplace_back(MyPackagedTask(test2, 20, 50));
  tasks.emplace_back(MyPackagedTask(test3, 60));

  // Per eseguirli devo prelevarli dal contenitore (per coda .top+.pop)

  auto task_temp = std::bind(test2, 70, 80);  // creare versioni particolari di funzioni: bind costruisce un oggetto
  // funzionale in cui il primo parametro è la funzione da eseguire, e i successivi parametri sono i variadic da
  // usare come parametri. task_temp=test2(70,80); (task_temp non contiene il valore di ritorno di test2(70,80), ma
  // l'invocazione stessa!
  tasks.emplace_back(task_temp);
  // Sia myPackageTask (tramite l'operatore ()) che task_temp ritornano un'invocazione a una funzione con valore di
  // ritorno vuoto, quindi possono essere richiamate entrambe estraendole dal contenitore.

  for (const auto &t: tasks) { // => while(!vector.empty()) -> thread dorme (attende)
    t(); // quando viene inserito nuovo elemento in coda, viene notificato thread, si risveglia, estrae t e la esegue
  }

  auto f1 = std::bind(test, 80, std::placeholders::_1);
  // nuovo uso del bind: crea funzione test(80, parametro variabile)
  // 80 è un parametro di default
  f1(90); // richiama test(80, 90)

#endif
  // Nella coda vera usata dai thread pool, invece del for avrei:
#ifdef V4
  std::vector<std::function<void()>> tasks;
  std::mutex m;
  std::unique_lock l(m); //m mutex per proteggere l'accesso al contenitore e impedire che tutti i thread ci accedano
  // contemporaneamente!
    auto t=tasks.back();
    tasks.pop_back();
  l.unlock();
  t();

   //E devo ricordarmi di proteggere anche le varie emplace usando
  std::unique_lock ul(m);
    tasks.emplace_back(MyPackagedTask(test, 10, 20));
  ul.unlock();

   // Alternativa molto più efficace è quella di non usare i contenitori della STL, ma di creare noi dei contenitori già
   // thread-safe come ci servono oppure di usarne di già fatti, in modo che ogni operazione che viene fatta su di essi
   // è già thread-safe senza bisogno di ricordarsi di fare lock() e unlock() prima di ogni operazione
#endif

  return 0;
}
