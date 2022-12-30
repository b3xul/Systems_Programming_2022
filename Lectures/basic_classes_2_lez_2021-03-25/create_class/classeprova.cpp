//
// Created by Alessandro on 12/03/21.
//

#include "classeprova.h"

namespace mynamespace {
/*classeprova::classeprova() {
	std::cout << "Creazione Obj Classe prova (" << this << ")\n";
	dato1 = dato2 = 0;
}*/

	classeprova::classeprova(int d1, int d2) {
		cout << "Creazione Obj Classe prova da costruttore 2 (" << this << ")\n";
		dato1 = d1;
		dato2 = d2;

	}

	void classeprova::stampa_dati() {
		std::cout << "indirizzo dati: " << &dato1 << "(" << dato1 << "), " << &dato2 << "(" << dato2 << ")\n";
	}
}