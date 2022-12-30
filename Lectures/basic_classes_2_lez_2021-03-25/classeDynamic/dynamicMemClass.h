//
// Created by Alessandro on 16/03/21.
//

#ifndef INC_04_C___DYNAMICMEMCLASS_H
#define INC_04_C___DYNAMICMEMCLASS_H

#include <cstddef>
#include <memory>
#include <cstring>
#include <iostream>

class dynamicMemClass {
public:
	dynamicMemClass(std::size_t size = 0);
	dynamicMemClass(const dynamicMemClass& src);
	dynamicMemClass(dynamicMemClass&& src);

	~dynamicMemClass();

	std::size_t getActualSize() const;
    const int * getPtr(); //NON VA ASSOLUTAMENTE BENE, perche' nel main potrei fare:
    //STOLENPTR=obj.getPtr();
    //Delete[] Stolenptr ->ho rubato la proprieta' del puntatore e l'ho liberato io, ma doveva essere di proprieta' dell'oggetto!
private:
	int * _ptr;
	std::size_t _size;
};


#endif //INC_04_C___DYNAMICMEMCLASS_H
