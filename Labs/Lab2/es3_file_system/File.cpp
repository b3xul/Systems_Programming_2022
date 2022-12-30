// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 07/01/22.
//

#include "File.h"

File::File(const std::string &name, uintmax_t size) : Base(name), size(size) {
}
File::File(std::string &&name, uintmax_t size) : Base(std::move(name)), size(size) {
}
void File::ls(int indent) const {
  std::cout << std::string(indent + 4, ' ') << this->getName() << " - " << this->getSize() << " Bytes\n"; //-V106
}
uintmax_t File::getSize() const {
  return this->size;
}
int File::myType() const {
  return 1;
}
