#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#define V4

int main() {

#ifdef V1
  int a = 0;  // normalmente dovrebbe essere globale per far sì che i thread possano accedervi! In questo caso invece
  // usiamo la funzione di cattura della funzione lambda per riferimento per riuscire a catturarla anche se è locale
  // al main! In questo modo entrambi i thread riescono comunque ad accedervi anche senza che sia globale
  // Bello ma complicato perchè non è in un punto che rappresenta chiaramente cos'è davvero in condivisione!
  auto thread_fn = [&a]() {
    for (int i = 0; i < 100000; i++) {
      a++;
    }
  };
  std::thread t1(thread_fn);

  std::thread t2(thread_fn);

  //Se provo ad assegnarli:
  //t1=t2; operator = is a private member of std::thread (è ancora presente ma usa il movimento, non la copia,
  // perchè costruttore di copia=delete
  //t1=std::move(t2); accettabile, ma che senso ha?

  // senza t1.join() e t2.join() CRASH : terminate called without an active exception
  t1.join();
  t2.join();

  std::cout << a << '\n';
  //1. Con un solo thread stampa 100000

  //2. Se facessi   std::thread t1(thread_fn);
  //  std::thread t2(thread_fn);
  //  t1.join();
  //  std::thread t2(thread_fn);
  //  t2.join();
  // stamperebbe 200000 (prima finisco un thread, poi eseguo l'altro

  //3. Invece eseguendoli contemporaneamente stampa un numero casuale!!! (124331,125320,102812,...)
#endif
#ifdef V2
  int a = 0;
  std::mutex m; // Stesso oggetto m condiviso da entrambi i thread.

  auto thread_fn = [&a, &m]() {
    for (int i = 0; i < 100000; i++) {
      m.lock(); // Posso usare oggetto per fare lock e unlock e in questo modo proteggo a in lettura e scrittura,
      // garantendo la correttezza del programma! Questo però rischia ancora di creare delle problematiche
      a++;
      m.unlock();
    }
  };

  std::thread t1(thread_fn);
  std::thread t2(thread_fn);

  t1.join();
  t2.join();
  std::cout << a << '\n';
#endif
#ifdef V3
  int a = 0;
  std::mutex m;

  auto thread_fn = [&a, &m](int tn) {
    for (int i = 0; i < 100000; i++) {
      std::lock_guard<std::mutex> lockG(m); // Oggetto che sfrutta il paradigma RAII: alla costruzione fa una lock e
      // alla distruzione fa una unlock. Ad ogni iterazione crea e distrugge un oggetto lockG (prende mutex per
      // riferimento in lock, unlock). Esecuzione e' inframezzata, perche' se dopo che 1 thread rilascia il mutex e
      // prima che lo riprenda, l'altro thread riesce a prendere il mutex, viene eseguito l'altro! Vero parallelismo!
      // N.B. se ho un blocco di esecuzione molto grande prima di raggiungere la fine della lifetime del lock_guard,
      // e solo una parte di quel blocco di esecuzione deve essere locked, e' piu' conveniente usare .lock(), .unlock
      // () per ridurre il numero di istruzioni da bloccare il piu' possibile. In realta' anche in questo caso basta
      // semplicemente contenere la critical seection tra 2 { } in modo da gestire noi dove vogliamo che sia il fine
      // vita!

      // Molto piu' leggibile rispetto alla sintassi POSIX.

      // N.B. zona da bloccare deve essere la piu' piccola possibile: se avessi messo lock_guard fuori dal ciclo
      // avrei bloccato completamente un thread finche' l'altro non avesse completamente terminato! Prima tutti 0 e
      // poi tutti 1 (il primo che prende il lock blocca l'altro)

      // Complessita' di comprensione e' data dalla programmazione concorrente!
      a++;
      std::cout << tn << ' '; // Thread number
      std::flush(std::cout);

      // non si alternano 0 e 1 piu' frequentemente probabilmente a causa della cout e della flush che rendono il
      // codice un po' piu' lento e aggiungono la necessita' per i thread di prendere il controllo del buffer
    }
  };

  std::thread t1(thread_fn, 0);
  std::thread t2(thread_fn, 1);

  t1.join();
  t2.join();
  std::cout << a << '\n';
#endif
  // Per questo programma probabilmente sarebbe piu' veloce l'esecuzione sequenziale, ma per istruzioni matematiche
  // ci sara' anche un'altro costrutto (istruzioni atomiche) che non richiedono l'uso di mutex ma garantiscono
  // parallelismo

#ifdef V4
  int a = 0;
  std::mutex m;

  std::condition_variable check;
  auto n_core = std::thread::hardware_concurrency() -1 ; // Funzione statica che permette di scoprire a runtime quanti
  // core si hanno a disposizione. Quanti ne possiamo sfruttare? Alcuni fanno n_core-1 (1 core sicuramente sarà
  // riservato al SO) , altri fanno n_core/2, non c'è una strategia fissa (tuning dipende da applicazione per
  // applicazione!). Prossima volta vedremo thread pool e asynch, meccanismi a livello più alto che vincolano il
  // numero di thread che effettivamente girano a quelli che l'architettura può supportare.
  // Perchè? Perchè se ho 10 core e creo 1000 thread con tutta probabilità il programma andrà più lento di
  // un'esecuzione monothread, perchè come per il multiprocesso, ogni movimento di scheduling richiede tempo extra
  // (overhead di sincronizzazione supera benefici del parallelismo perchè non va di pari passo con il parallelismo
  // fornito dalla mia esecuzione). Oggi spesso parallelismo non parla di multiprocess e multithread, ma parla di GPU
  // e simili proprio perchè quelle architetture si prestano meglio a parallelizzare l'esecuzione sui dati. Lì la
  // sincronizzazione non esiste, ma esiste la divergenza: codice invece di eseguire 10 istruzioni una dopo l'altra,
  // contiene un if (punto di divergenza: equivalente al nostro punto di sincronizzazione nel senso che lì rallenta
  // l'esecuzione). La cosa bella è che tutto questo ci permette di avere un aggancio con l'architettura! Poi
  // bisognera' fare tuning che pero' dipende strettamente dall'applicazione che si va a eseguire! Per alcune meglio
  // non superare x core, per altre piu' core si hanno e piu' si riesce a migliorare!
  std::cout << n_core << '\n';  // 8

  auto thread_fn = [&a, &m](int tn) {
    for (int i = 0; i < 100000; i++) {
      std::lock_guard<std::mutex> lockG(m);
      a++;
      std::cout << tn << ' ';
      std::flush(std::cout);
    }
  };

  std::thread t1(thread_fn, 0);
  std::thread t2(thread_fn, 1);

  t1.join();
  t2.join();
  std::cout << a << '\n';
#endif

  return 0;
}
