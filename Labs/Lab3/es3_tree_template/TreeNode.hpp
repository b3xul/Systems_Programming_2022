//
// Created by bexul on 04/01/22.
//

#ifndef LAB3_ES1_PROGRAMMAZIONE_LABS_LAB2_ES3_FILE_SYSTEM_Tree_H_
#define LAB3_ES1_PROGRAMMAZIONE_LABS_LAB2_ES3_FILE_SYSTEM_Tree_H_

#include <map>
#include <memory>
#include <iostream>

template<typename T>
class TreeNode : public std::enable_shared_from_this<T> {
 private:
  /* Static variables */
  static std::weak_ptr<T> root;
  /*  // Root cannot be a std::shared_ptr, because we will to use shared_from_this() to avoid the situation:
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
    // If we created it as shared ptr, it complicates the creation of a new TreeNode because we cannot just pass
    // shared_from_this(), otherways another shared pointer to the same root raw pointer would be created! We would
    // need to add a check for which TreeNode is trying to create a child TreeNode (not sure if that would work)*/

  /* Normal variables */
  T element;
  std::weak_ptr<T> parent; // weak because if I delete a father, all the subdirectories must be deleted, the
  // father must not be still alive as the father of others!
  std::weak_ptr<T> self;
  std::map<std::string, std::shared_ptr<T>> children;

  /* Static methods */
  static std::shared_ptr<T> makeSubTree(const std::string &name, const std::weak_ptr<T> &parent);
  // both private because we use the Named Constructor Idiom to guarantee that all the instances of TreeNode will be
  // allocated on the heap

  /* Normal methods */
  explicit TreeNode(const std::string &name);
  explicit TreeNode(std::string &&rname);
  // No public constructor, only a factory function,
  // so there's no way to have getRoot return nullptr.
 public:
  /* Static methods */
  [[nodiscard]] static std::shared_ptr<TreeNode<T>> getRoot(const T &rootNode);

  //restituisce il nome dell’oggetto
  [[nodiscard]] std::string getName() const;

  // stampa name con indent spazi d'indentazione (incrementa con i figli)
  void ls(int indent) const;

  // called by the user, internally calls makeTree(). crea TreeNode e lo aggiunge come figlio.
  std::shared_ptr<T> addSubTree(const std::string &name);

  // ottiene puntatore alla TreeNode con il nome passato
  std::shared_ptr<T> get(const std::string &name);

  bool remove(const std::string &name);
};
/*
template<typename T> std::weak_ptr<T> TreeNode<T>::root;

template<typename T>
TreeNode<T>::TreeNode(const std::string &name) : name(name) {
}
template<typename T>
TreeNode<T>::TreeNode(std::string &&rname): name(std::move(rname)) {

}
template<typename T>
std::string TreeNode<T>::getName() const {
  return this->name;
}

template<typename T>
void TreeNode<T>::ls(int indent) const {
  //((this->myType()==0)? "[D] ":"") this is not necessary because it is handled by the polymorphism
  std::cout << std::string(indent, ' ') << "[D] " << this->getName() << '\n'; //-V106
  // for (const auto&[nameKey, dir]: children) {
  for (const auto &keyValuePair: children) {
    keyValuePair.second->ls(indent + 4); //-V112
  }
}
template<typename T>
std::shared_ptr<T> TreeNode<T>::addSubTree(const std::string &name) {
  if (name == "." || name == ".." || this->get(name) != nullptr)
    return {nullptr};
  // equivalent to return std::shared_ptr<T>(nullptr);

  // call function to create subTree and adds it as child of the parent tree
  //std::weak_ptr<T> tmpParent = shared_from_this();
  //std::shared_ptr<T> subDir = makeSubTree(name, tmpParent);
  //return subDir;
  return {nullptr}
}
template<typename T>
std::shared_ptr<T> TreeNode<T>::makeSubTree(const std::string &name, const std::weak_ptr<T> &parent) {
  // calls constructor of subtree and returns shared pointer to it.
  std::shared_ptr<T> subDir(new T(name)); //-V824
  subDir->parent = parent;
  subDir->self = subDir;
  if (std::shared_ptr<T> parentShared = parent.lock())
    parentShared->children.insert(std::make_pair(name, subDir));
  return subDir;
}
template<typename T>
std::shared_ptr<T> TreeNode<T>::get(const std::string &name) {
  if (name == ".")
    return this->self.lock();
  else if (name == "..")
    return this->parent.lock();

  std::shared_ptr<T> element(nullptr);
  //std::map<std::basic_string<char>, std::shared_ptr<T>>::iterator
  auto it = this->children.find(name);
  if (it != this->children.end())
    element = it->second;
  return element;
}
template<typename T>
bool TreeNode<T>::remove(const std::string &name) {
  if (name == "." || name == "..")
    return false;

  return this->children.erase(name); // returns number of elements removed (0 or 1)
}*/
template<typename T>
std::shared_ptr<TreeNode<T>> TreeNode<T>::getRoot(const T &rootNode) {
  //      shared_ptr<_Tp>
  //      lock() const noexcept
  //      { return shared_ptr<_Tp>(*this, std::nothrow); }
  std::shared_ptr<T> current_root = TreeNode<T>::root.lock();
  // We need to create a shared ptr because a weak ptr cannot exist alone. If it is the first time we'll create a new
  // shared ptr, assign the weak ptr of TreeNode<T>::root to it and return it.
  // If it already exists we return the TreeNode<T>::root.lock() (shared ptr extracted from the weak ptr)
  //std::cout << root.use_count() << '\n'; 0, 1
  if (current_root == nullptr) {
    // TreeNode<T>::root not yet initialized
    current_root = std::shared_ptr<T>(rootNode); //-V824
    current_root->parent = std::shared_ptr<T>(nullptr);
    current_root->self = current_root;
    // Not using std::make_shared<Best> because the constructor is private.
    TreeNode<T>::root = current_root;
  }
  //std::cout << current_root.use_count() << '\n'; 1, 2
  return current_root;
}

#endif //LAB3_ES1_PROGRAMMAZIONE_LABS_LAB2_ES3_FILE_SYSTEM_Tree_H_
