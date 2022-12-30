//
// Created by Alessandro on 16/03/21.
//

#include "dynamicMemClass.h"

dynamicMemClass::dynamicMemClass(std::size_t size) : _size(size) {
	if (_size != 0) {
		std::cout << "Generating Memory\n";
		_ptr = new (std::nothrow) int [size];
		if (_ptr == nullptr) {
			std::cerr << "error while creating the dynamic mem in dynamicMemClass\n";
		}
	}
	else _ptr = nullptr;
}
dynamicMemClass::dynamicMemClass(const dynamicMemClass& src): _size(src._size) {
	std::cout << "Copy activated\n";
	_ptr = nullptr;
	if (_size > 0) {
		_ptr = new (std::nothrow) int [_size];
		if (_ptr == nullptr) {
			std::cerr << "error while creating the dynamic mem in dynamicMemClass\n";
		} else {
			std::memcpy(_ptr, src._ptr, _size * sizeof (int));
		}
	}
}
dynamicMemClass::dynamicMemClass(dynamicMemClass&& src) {
	std::cout << "Movement activated\n";
	_size = src._size;
	_ptr = src._ptr;
	src._ptr = nullptr;
	src._size = 0;
	// src esiste ancora
}

dynamicMemClass::~dynamicMemClass() {
	std::cout << "Activated delete: " << this << '\n';
	if (_ptr != nullptr) {
		delete[] _ptr;
		std::cout << "Deleting Memory " << _ptr << " (" << *_ptr << ")\n";
		_ptr = nullptr;
	}
}

size_t dynamicMemClass::getActualSize() const {
	return _size;
}

const int * dynamicMemClass::getPtr() {
    return _ptr;
}