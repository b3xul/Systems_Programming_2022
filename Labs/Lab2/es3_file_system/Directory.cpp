// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 04/01/22.
//

#include "Directory.h"

#include <memory>

std::weak_ptr<Directory> Directory::root;
//int Directory::current_indent_level = 0;


/*
std::shared_ptr<Directory> &Directory::getRoot() {
  //std::cout << root.use_count() << '\n';
  root = std::make_shared<Directory>("/");
  //std::cout << root.use_count() << '\n';

  return root;
}

std::shared_ptr<Directory> Directory::addDirectory(const std::string &name) {
  // call function to create subdirectory and adds it as child of the parent directory
  std::weak_ptr<Directory> parent = shared_from_this(); // Constructing a std::shared_ptr for an object that is
  // already managed by another std::shared_ptr will not consult the internally stored weak reference and thus will
  // lead to undefined behavior.
  std::shared_ptr<Directory> new_child = makeDirectory(name, parent);
  this->children.insert(std::make_pair(name, new_child));
  return new_child;
}
std::shared_ptr<Directory> Directory::makeDirectory(const std::string &name, std::weak_ptr<Directory> parent) {
  // calls constructor of subdirectory and returns shared pointer to it.
  return std::shared_ptr<Directory>(new Directory(name, parent));
}
Directory::Directory(const std::string &name, std::weak_ptr<Directory> parent)
    : name(name), parent(parent), self(shared_from_this()) {
  // constructs subdirectory
}
 */

Directory::Directory(const std::string &name) : Base(name) {
  // The constructor of Directory will call a constructor of Base; there's no way you can prevent it. If you do
  // nothing, it will call the default constructor (Base()); if you need to pass arguments, you'll
  // have to specify the base class in the initializer list -> Base(name)
}
std::shared_ptr<Directory> Directory::getRoot() {
  //      shared_ptr<_Tp>
  //      lock() const noexcept
  //      { return shared_ptr<_Tp>(*this, std::nothrow); }
  std::shared_ptr<Directory> current_root = Directory::root.lock();
  // We need to create a shared ptr because a weak ptr cannot exist alone. If it is the first time we'll create a new
  // shared ptr, assign the weak ptr of Directory::root to it and return it.
  // If it already exists we return the Directory::root.lock() (shared ptr extracted from the weak ptr)
  //std::cout << root.use_count() << '\n'; 0, 1
  if (current_root == nullptr) {
    // Directory::root not yet initialized
    current_root = std::shared_ptr<Directory>(new Directory("/")); //-V824
    current_root->parent = std::shared_ptr<Directory>(nullptr);
    current_root->self = current_root;
    // Not using std::make_shared<Best> because the constructor is private.
    Directory::root = current_root;
  }
  //std::cout << current_root.use_count() << '\n'; 1, 2
  return current_root;
}

int Directory::myType() const {
  return 0;
}

void Directory::ls(int indent) const {
  //((this->myType()==0)? "[D] ":"") this is not necessary because it is handled by the polymorphism
  std::cout << std::string(indent, ' ') << "[D] " << this->getName() << '\n'; //-V106
  // for (const auto&[nameKey, dir]: children) {
  for (const auto &keyValuePair: children) {
    keyValuePair.second->ls(indent + 4); //-V112
  }
}

std::shared_ptr<Directory> Directory::addDirectory(const std::string &name) {
  if (name == "." || name == ".." || this->get(name) != nullptr)
    return {nullptr};
  // equivalent to return std::shared_ptr<Directory>(nullptr);

  // call function to create subdirectory and adds it as child of the parent directory
  std::weak_ptr<Directory> tmpParent = shared_from_this();
  std::shared_ptr<Directory> subDir = makeDirectory(name, tmpParent);
  return subDir;
}
std::shared_ptr<File> Directory::addFile(const std::string &name, uintmax_t size) {
  if (name == "." || name == ".." || this->get(name) != nullptr)
    return {nullptr};
  //equivalent to return std::shared_ptr<File>(nullptr);

  std::shared_ptr<File> newFile = std::make_shared<File>(name, size); // since constructor is public, I can use
  // make_shared

  std::weak_ptr<Directory> me = shared_from_this();
  std::shared_ptr<Directory> meShared = me.lock();
  meShared->children.insert(std::make_pair(name, newFile));

  return newFile;
}

std::shared_ptr<File> Directory::addFile(std::string &&name, uintmax_t size) {
  if (name == "." || name == ".." || this->get(name) != nullptr)
    return {nullptr};
  //equivalent to return std::shared_ptr<File>(nullptr);
  std::shared_ptr<File> newFile = std::make_shared<File>(std::move(name), size); // since constructor is public, I can
  // use
  // make_shared

  std::weak_ptr<Directory> me = shared_from_this();
  std::shared_ptr<Directory> meShared = me.lock();
  meShared->children.insert(std::make_pair(newFile->getName(), newFile));

  return newFile;
}

std::shared_ptr<Directory> Directory::makeDirectory(const std::string &name, const std::weak_ptr<Directory> &parent) {
  // calls constructor of subdirectory and returns shared pointer to it.
  std::shared_ptr<Directory> subDir(new Directory(name)); //-V824
  subDir->parent = parent;
  subDir->self = subDir;
  if (std::shared_ptr<Directory> parentShared = parent.lock())
    parentShared->children.insert(std::make_pair(name, subDir));
  return subDir;
}

std::shared_ptr<Base> Directory::get(const std::string &name) {
  if (name == ".")
    return this->self.lock();
  else if (name == "..")
    return this->parent.lock();

  std::shared_ptr<Base> element(nullptr);
  //std::map<std::basic_string<char>, std::shared_ptr<Directory>>::iterator
  auto it = this->children.find(name);
  if (it != this->children.end())
    element = it->second;
  return element;
}
std::shared_ptr<Directory> Directory::getDir(const std::string &name) {
  // Creates a new instance of std::shared_ptr whose stored pointer is obtained from r's stored pointer using a cast expression.
  //
  //If r is empty, so is the new shared_ptr (but its stored pointer is not necessarily null). Otherwise, the new
  // shared_ptr will share ownership with the initial value of r, except that it is empty if the dynamic_cast
  // performed by dynamic_pointer_cast returns a null pointer.
  std::shared_ptr<Base> b = this->get(name);
  if (b != nullptr && b->myType() == 0) // directory
    return std::dynamic_pointer_cast<Directory>(b);
  // else if(b==nullptr || b->myType()==1) file
  return {nullptr};
}
std::shared_ptr<File> Directory::getFile(const std::string &name) {

  return std::dynamic_pointer_cast<File>(this->get(name));
}
bool Directory::remove(const std::string &name) {
  if (name == "." || name == "..")
    return false;

  return this->children.erase(name); // returns number of elements removed (0 or 1)
}

