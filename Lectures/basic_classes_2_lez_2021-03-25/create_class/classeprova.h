//
// Created by Alessandro on 12/03/21.
//

#ifndef INC_04_C___CLASSEPROVA_H
#define INC_04_C___CLASSEPROVA_H

#include <iostream>

using namespace std;

namespace mynamespace {

	class classeprova {
	public:
		//classeprova();
		classeprova(int d1 = 0, int d2 = 0);

		void stampa_dati();

	private:
		int dato1, dato2;
	};
}



#endif //INC_04_C___CLASSEPROVA_H
