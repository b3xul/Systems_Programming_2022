// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 05/01/22.
//

#include "Base.h"

//Clang-Tidy: Pass by value and use std::move -> not necessary since we overloaded functions:
// optimal in terms of copies/moves, but requires code repetition.

Base::Base(const std::string &name) : name(name) {
  // A passed lvalue binds to name, then is copied into this->name
}
Base::Base(std::string &&rname) : name(std::move(rname)) {
  // A passed rvalue binds to rname, then is moved into this->name
}

std::string Base::getName() const {
  return this->name;
}
