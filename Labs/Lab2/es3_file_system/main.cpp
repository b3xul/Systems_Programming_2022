// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <memory>
#include <filesystem>

#include "Directory.h"
#include "File.h"

int main() {
  /*PARTE 1*/
  std::shared_ptr<Directory> root = Directory::getRoot();
  std::shared_ptr<Directory> roott = Directory::getRoot();
  root->ls(0);
  std::shared_ptr<Directory> child = root->addDirectory("prova2");
  root->ls(0);
  std::shared_ptr<Directory> child2 = root->addDirectory("prova1");
  std::shared_ptr<Directory> child3 = child->addDirectory("prova3");
  std::shared_ptr<Directory> child4 = child3->addDirectory("prova4");
  std::shared_ptr<Directory> child5 = child3->addDirectory("prova5");
  std::shared_ptr<Directory> child6 = child2->addDirectory("prova6");
  std::shared_ptr<Directory> child7 = child2->addDirectory("prova7");
  std::shared_ptr<Directory> child8 = roott->addDirectory("prova8");
  root->ls(0);
  if (root->get("dfsjl") == nullptr)
    std::cout << "Not found" << '\n';
  if (root->get("..") == nullptr)
    std::cout << "Not found" << '\n';
  root->get(".")->ls(1);
  root->get("prova1")->ls(2);

  std::shared_ptr<Directory> d = root->getDir("prova2");
  d->ls(3);
  root->ls(3);

  if (root->getDir("fdhjskf") == nullptr)
    std::cout << "Dir not found" << '\n';

  root->getDir("prova2")->getDir("prova3")->remove("prova4"); //-V807
  root->ls(0);
  root->getDir("prova2")->remove("prova3");
  root->ls(0);
  if (!root->getDir("prova2")->remove("prova3"))
    std::cout << "Base not found" << '\n';
  std::shared_ptr<File> file1 = child2->addFile("file1", 100);
  file1->ls(3);
  root->ls(0);
  std::shared_ptr<File> file1bis = child2->getFile("file1");
  file1bis->ls(3);
  std::shared_ptr<Directory> tmp = child3->getRoot();

  std::string file2s{"file2"};
  std::shared_ptr<File> file2 = child2->addFile(file2s, 200);
  file2->ls(0);
  std::shared_ptr<File> file3 = child2->addFile("file3", 200);
  file3->ls(0);

  std::shared_ptr<Directory> file4 = child2->getDir("file1");
  //file4->ls(3);
  std::shared_ptr<Directory> file5 = root->getDir("..");
  //file5->ls(3);
  /*
   * PARTE 2
   * std::shared_ptr<Directory> root = Directory::getRoot();
    std::weak_ptr<Directory> tmp;
    std::shared_ptr<Directory> tmpS;
    std::shared_ptr<Base> next;

    for (const std::filesystem::directory_entry &it: std::filesystem::recursive_directory_iterator(
        "/mnt/26158F5879578F52/Università/Magistrale/Programmazione_di_sistema/Programmazione/Labs/Lab2/es3_file_system")) {
      tmp = root;
      for (const std::filesystem::path &element: it.path()) {
        if (element != "/") {
          tmpS = tmp.lock();
          next=tmpS->get(element);
          if (next == nullptr) {
            //directory/file not present
            if (it.is_directory()) {
              tmp=tmpS->addDirectory(element);
            }else if (it.is_regular_file())
              tmpS->addFile(element, std::filesystem::file_size(it));
          } else if (tmpS->myType() == 0) //directory already exists
            tmp = std::dynamic_pointer_cast<Directory>(next);
        }
      }

    }
    root->ls(0);*/

  return 0;
}