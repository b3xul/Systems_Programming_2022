//
// Created by bexul on 04/01/22.
//

#ifndef LAB3_ES1_PROGRAMMAZIONE_LABS_LAB2_ES3_FILE_SYSTEM_DIRECTORY_H_
#define LAB3_ES1_PROGRAMMAZIONE_LABS_LAB2_ES3_FILE_SYSTEM_DIRECTORY_H_

#include <map>
#include <memory>
#include <iostream>
#include "Base.h"
#include "File.h"
class Directory : public std::enable_shared_from_this<Directory>, public Base {
 private:
  /* Static variables */
  static std::weak_ptr<Directory> root;
  // Root cannot be a std::shared_ptr, because we will to use shared_from_this() to avoid the situation:
  // struct Bad
  //{
  //    std::shared_ptr<Bad> getptr() {
  //        return std::shared_ptr<Bad>(this);
  //    }
  //    ~Bad() { std::cout << "Bad::~Bad() called\n"; }
  //};
  //    std::shared_ptr<Bad> bad0 = std::make_shared<Bad>();
  //    std::shared_ptr<Bad> bad1 = bad0->getptr();
  // The key point is that the "obvious" technique of just returning shared_ptr<Y>(this) is broken, because this
  // ends up creating multiple distinct shared_ptr objects with separate reference counts. For this reason you must
  // never create more than one shared_ptr from the same raw pointer.
  // If we created it as shared ptr, it complicates the creation of a new directory because we cannot just pass
  // shared_from_this(), otherways another shared pointer to the same root raw pointer would be created! We would
  // need to add a check for which directory is trying to create a child directory (not sure if that would work)

  //static int current_indent_level;

  /* Normal variables */
  std::weak_ptr<Directory> parent; // weak because if I delete a father, all the subdirectories must be deleted, the
  // father must not be still alive as the father of others!
  std::weak_ptr<Directory> self;
  std::map<std::string, std::shared_ptr<Base>> children;

  /* Static methods */
  static std::shared_ptr<Directory> makeDirectory(const std::string &name, const std::weak_ptr<Directory> &parent);
  // both private because we use the Named Constructor Idiom to guarantee that all the instances of Directory will be
  // allocated on the heap

  /* Normal methods */
  explicit Directory(const std::string &name);
  // No public constructor, only a factory function,
  // so there's no way to have getRoot return nullptr.
 public:
  /* Static methods */
  [[nodiscard]] static std::shared_ptr<Directory> getRoot();

  /* Normal Methods */
  int myType() const override;

  // stampa name con indent spazi d'indentazione (incrementa con i figli)
  void ls(int indent) const override;

  // called by the user, internally calls makeDirectory(). crea Directory e l'aggiunge come figlia.
  std::shared_ptr<Directory> addDirectory(const std::string &name);
  std::shared_ptr<File> addFile(const std::string &name, uintmax_t size);
  std::shared_ptr<File> addFile(std::string &&name, uintmax_t size);
  // ottiene puntatore alla directory con il nome passato
  std::shared_ptr<Base> get(const std::string &name);
  std::shared_ptr<Directory> getDir(const std::string &name);
  std::shared_ptr<File> getFile(const std::string &name);

  bool remove(const std::string &name);
};

#endif //LAB3_ES1_PROGRAMMAZIONE_LABS_LAB2_ES3_FILE_SYSTEM_DIRECTORY_H_
