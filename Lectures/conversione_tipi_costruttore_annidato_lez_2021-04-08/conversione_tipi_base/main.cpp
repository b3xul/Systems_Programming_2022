//
// Created by Alessandro on 31/03/21.
//
#include <iostream>
class Base {
 public:
  virtual void f() {
    std::cout << "sono base: " << std::endl;
  }
  virtual void f2() {
    std::cout << "sono base2: " << std::endl;
  }
};

class Derived : public Base {
 public:
  Derived() = default;
  void funDer() {
  }
  void f() override {
    std::cout << "sono der: " << std::endl;
  }
};
class AnotherDerived : public Base {
 private:
  int n = 4;
 public:
  AnotherDerived() : n(5) {
  }
  virtual void funAnoDer() {
    std::cout << "sono another: " << n << std::endl;
  }
  void f() override {
    std::cout << "sono another " << std::endl;
  }
};

int main() {
  int int1, converted_float1, *pointer_to_float1;
  float float1, converted_int1, *pointer_to_int1;

  std::cout << "inserire numero intero: ";
  std::cin >> int1; //12
  std::cout << "inserire numero floating point: ";
  std::cin >> float1; //1.3

  // N.B. legit vuol dire che non rischio crash, non che abbia senso l'operazione che sto facendo!

  /* C-style cast/raw casting*/
  float converted_int1_to_float = int1; //narrowing conversion
  //equivalent to
  float explicitly_converted_int1_to_float = (float) int1; //narrowing conversion

  //conversione del valore logico contenuto in int1 nella rappresentazione di quel valore logico su un float -> legit
  /*  std::cout << "Conversione implicita con float a=int b: ";
    std::cout << converted_int1_to_float << '\n'; //12*/
  std::cout << "Conversione esplicita con float a=int b: ";
  std::cout << explicitly_converted_int1_to_float << '\n'; //12
  //conversione del valore logico contenuto in float1 nella rappresentazione di quel valore logico su un int -> legit
  int converted_float1_to_int = (int) float1;
  std::cout << "Conversione implicita/esplicita con int a=float b: ";
  std::cout << converted_float1_to_int << '\n'; //1 (tronca parte decimale)

  /* Type punning */
  //rilettura dei 4 byte di memoria indirizzati da int1, negli 8 byte di memoria indirizzati da int1 -> sto leggendo
  // 4 B di memoria che non appartengono a int1!!! RISCHIO CRASH!!! Se poi provo a scriverci sopra rischio crash
  // ancora di più perchè vado a scrivere in memoria non mia!
  std::cout << "Reinterpretazione del puntatore a int1 come float: ";
  pointer_to_int1 = (float *) &int1;
  std::cout << "v1_float: " << *pointer_to_int1 << '\n'; //4 byte a caso, rischio crash
  //rilettura degli 8 byte di memoria indirizzati da float1, nei 4 byte di memoria indirizzati da int1 -> legit
  std::cout << "Reinterpretazione del puntatore a float1 come intero: ";
  pointer_to_float1 = (int *) &float1;
  std::cout << "v2_int: " << *pointer_to_float1 << '\n';

  //potrei usare struct da 2 int come un array facendo int* position =(int*)&mystruct; position[0], position[1],
  // senza bisogno di copiare dati/creare strutture dati aggiuntive nella struct!
  //oppure accedere al secondo elemento come int second=*(int*) ((char*)&e +4);

  /*C++ casting*/
  // C permette tutte le conversioni possibili, c++ ci permette:
  // 1. cercare nella codebase tutti i cast che sono stati fatti
  // 2. aggiungo un controllo aggiuntivo sull'operazione che sta venendo svolta, impedendo quelle impossibili
  // "invalid type conversion". Ad esempio provare a convertire un intero in una classe! non ha senso!
  // Il vantaggio di usare static e reinterpret cast (e dynamic e const) invece delle conversioni a basso livello del
  // C è che le nuove primitive consentono al compilatore di fare il check di quello che stiamo facendo, sia in fase
  // di compilazione che a runtime in alcuni casi (dynamic cast): consente graceful degradation!

  // equivalente a conversione logica di prima
  converted_int1 = static_cast<float>(int1);
  std::cout << "Conversione con static_cast float = static_cast<float>(int1): " << converted_int1 << '\n'; //12
  converted_float1 = static_cast<int>(float1);
  std::cout << "Conversione con static_cast int=static_cast<int>(float1): " << converted_float1 << '\n'; //1

  /* Equivalente a type punning di prima */

  //pointer_to_int1 = reinterpret_cast<float>(int1); //Reinterpret_cast from 'int' to 'float' is not allowed
  // Tratta indirizzo di int1 come un indirizzo contenente un floating point: reinterpreta la configurazione di bit che
  // rappresenta un 12 come la configurazione di bit che gestisce un numero float!
  pointer_to_int1 = reinterpret_cast<float *>(&int1);
  std::cout << "Conversione con reinterpret_cast pointer_to_int1 = reinterpret_cast<float *>(&int1): "
            << *pointer_to_int1 << '\n'; //1.68156e-44
  // reinterpreta la configurazione di bit che rappresenta un 1.3 come la configurazione di bit che gestisce un
  // numero int!
  pointer_to_float1 = reinterpret_cast<int *>(&float1);
  std::cout << "Conversione con reinterpret_cast pointer_to_float1 = reinterpret_cast<int *>(&float1): "
            << *pointer_to_float1 << '\n'; //1067869798

  /*std::cout << "Conversione con dynamic_cast non si può fare per tipi di base!: ";
    converted_int1 = dynamic_cast<float>(int1); // Invalid target type 'float' for dynamic_cast; target type must be a
    reference or pointer type to a defined class
    converted_int1 = dynamic_cast<float *>(&int1); // Float is not a class
    std::cout << "v1_float: " << converted_int1 << '\n';
    converted_float1 = dynamic_cast<int>(float1);
    std::cout << "v2_int: " << converted_float1 << '\n';

    std::cout << "Conversione con const_cast non si può usare per convertire tipi diversi: ";
    converted_int1 = const_cast<float>(int1); //Const_cast to 'float', which is not a reference, pointer-to-object, or pointer-to-data-member
    std::cout << "v1_float: " << converted_int1 << '\n';
    converted_float1 = const_cast<int>(float1);
    std::cout << "v2_int: " << converted_float1 << '\n';
  */

  //Lo static_cast<T>(p) è un template C++11 che converte il valore di "p" rendendolo di tipo "T" qualora esista un
  // meccanismo di conversione disponibile noto al compilatore. La conversione avviene in modo statico in fase di
  // compilazione, quindi non vi è nessun tipo di controllo di compatibilità a run-time. In presenza di conversione
  // illecita il risultato è impredicibile. Il vantaggio di static_cast è la sua efficienza visto che ha un impatto
  // in termini di esecuzione molto limitato ed il suo uso principale è per fare movimenti in verticale lungo l'asse
  // ereditario, in particolare
  // -->dalla classe Figlia a quella Padre (upcast) ossia "Padre * obj = static_cast<Padre*> (objFiglia)" <--
  // Questa istruzione funziona grazie alla presenza delle V-Table. Nella Fig.1 "b1" e "d" hanno lo
  // stesso indirizzo, solo che il compilatore ha informazioni diverse proprio per la presenza delle V-Table, una per
  // "der", una per "base1" ed una per "base2". Nella Fig.2 "b2" non ha lo stesso indirizzo di "d", ma un indirizzo
  // pari a
  // "d+offset" così che "b2" possa puntare alla sua V-Table. Il compilatore del C avrebbe assegnato a "b2" lo stesso
  // indirizzo di "d", con l'ovvia conseguenza di ottenere la V-Table di "b1" e non quella di "b2" comportando
  // l'esecuzione di codice diverso da quello voluto.
  Derived *der = new Derived();
  Base *base = static_cast<Base *>(der); // upcast da der a base è sempre possibile perchè base ha un sottoinsieme
  // delle cose di der, quindi basterà ignorare quelle.
  // base->funDer(); error: ‘class Base’ has no member named ‘funDer’

  // Il compilatore permette di utilizzare static_cast anche per scendere nell'ereditarietà (downcast), ma è
  // altamente sconsigliato visto che anotherDerived ha più cose rispetto a base!
  AnotherDerived *anoderReal = new AnotherDerived();
  //anoderReal->funAnoDer(); // RITORNA CORRETTAMENTE 5

  AnotherDerived *anoder = static_cast<AnotherDerived *> (base);
  der->funDer();
  //anoder->funAnoDer();  // RITORNA 0 INVECE DI 5, PERCHE' ALLA CLASSE BASE MANCA IL MEMBRO N! NON E' MAI STATO
  // INIZIALIZZATO! (CASTING NON PASSA DAL COSTRUTTORE!)
  // this works but it gives a warning ->  Clang-Tidy: Do not use static_cast to downcast from a base to a derived class


  //Il dynamic_cast<T>(p) è un template C++11 che fa più o meno le stesse cose di static_cast, ma in più effettua il
  // controllo a run-time dei tipi, il che permette di scendere oltre che salire nell'ereditarietà. In caso di
  // discesa in presenza di un puntatore, entra in gioco il RTTI (RunTime Type Identification) (RTTI could be
  // manually disabled if it adds too much overhead for very low computation environments! il quale verifica se
  // l'oggetto di partenza non è compatibile con quello di arrivo, in tale caso il dynamic_cast restituisce "null"
  // evitando così di accedere alla memoria altrui. In presenza di un riferimento, il quale non può mai valere "null"
  // per definizione, se il tentativo di downcast è incompatibile viene sollevata un'eccezione. In Fig.4 il
  // dynamic_cast può effettuare il downcast da una classe base virtuale ad una derivata, anche se l'istruzione "d =
  // dynamic_cast<Derivata*>(b1)" restituisce "null" grazie all'analisi della V-Table, infatti il meccanismo di RTTI
  // verifica se la discesa lungo l'asse verticale porta ad avere una V-Table identica a quella della classe
  // "Derivata". Questo controllo da parte del RTTI avviene con un semplice "if", quindi rispetto allo static_cast,
  // il dynamic_cast ha un costo di esecuzione maggiore. Identico discorso vale per "d = dynamic_cast<Derivata*>(b2)"

  Base *base_dynamic = dynamic_cast<Base *>(der);  //upcast sempre possibile (leggermente più lento rispetto a static)
  // Nella Fig.5 si effettua un'operazione di upcast "b2=dynamic_cast<Base2*>(d)", come in static_cast, la quale è
  // semanticamente lecita visto che Derivata <<is_a>> Base2 e nell'indirizzo di "b2" viene messo "d+offset", quindi
  // sarà possibile accedere ai metodi "p() e q()". Qualora si effettua l'operazione di upcast
  // "b1=dynamic_cast<Base1*>(d)" si potrà accedere ai metodi "m() e n()". L'operazione di "upcast" effettuata con
  // dynamic_cast ha un costo di esecuzione maggiore rispetto alla static_cast proprio per la presenza del RTTI, il
  // quale a sua volta rispetto alla Reflection di Java/C# ha costi di esecuzione nettamente minori

  AnotherDerived *anoder_dynamic = dynamic_cast<AnotherDerived *>(base_dynamic); //downcast non sempre possibile!
  if (anoder_dynamic == nullptr)
    std::cout << "Impossibile fare downcast!" << std::endl;
  else {
    //anoder_dynamic->funAnoDer();
    anoder_dynamic->f();
  }
  AnotherDerived *veroAnother = new AnotherDerived();
  Base *base_ma_con_vtable_di_AnotherDeriver_ma_da_cui_non_posso_accedere_metodi_non_di_Base = static_cast<Base *>
  (veroAnother);
  //N.B. Checks are done at runtime!!
  AnotherDerived *downcasted =
      dynamic_cast<AnotherDerived *>(base_ma_con_vtable_di_AnotherDeriver_ma_da_cui_non_posso_accedere_metodi_non_di_Base);
  if (downcasted == nullptr)
    std::cout << "Impossibile fare downcast!" << std::endl;
  else {
    std::cout << "Ora downcast possibile! Questa Base era in realtà un AnotherDerived!" << std::endl;

    //anoder_dynamic->funAnoDer();
    downcasted->f();
  }
  //upcast

  // struct Base
  //{
  //    virtual ~Base() {}
  //};
  //
  // struct Derived: Base
  //{
  //    virtual void name() {}
  //};
  //
  // Base* b2 = new Derived;
  //    if (Derived* d = dynamic_cast<Derived*>(b2); d != nullptr)
  //    {
  //        std::cout << "downcast from b2 to d successful\n";
  //        d->name(); // safe to call
  //    }
  //
  // downcast from b2 to d successful -> dato che b2 era nata come Derived, non perde la sua funzione name() quando
  // viene fatto il downcast
  //
  // In order for dynamic_cast to work the object needs to be polymorphic. The reason for this is that dynamic_cast
  // needs somewhere to store the type information that it will use to perform the cast, and it does this by storing
  // the information alongside the vtable for the class. In order for there to be a vtable you need to make at least
  // one of your methods virtual.
  // virtual = polymorphic;
  // abstract = interface behaviour (parent class has function =0 that must be overridden)
  //
  //The easiest way around this is to flag the base class destructor as virtual.
  Base mybase1 = Base();
  std::cout << sizeof(Base) << std::endl; // =1 senza metodi virtuali, =8 con metodi virtuali
  Derived myder1 = Derived();
  std::cout << sizeof(Derived) << std::endl;
  AnotherDerived myanoder1 = AnotherDerived(); // =1 se Base non ha metodi virtuali, =8 se Base metodi virtuali, =16
  // se Base e AnotherDerived hanno entrambi metodi virtuali!
  std::cout << sizeof(AnotherDerived) << std::endl;
}

