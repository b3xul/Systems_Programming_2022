//
// Created by bexul on 07/01/22.
//

#ifndef LAB2_ES3__FILE_H_
#define LAB2_ES3__FILE_H_

#include "iostream"
#include "Base.h"

class File : public Base {  //public is essential!
  // class Base {
  //    public:
  //        int publicMember;
  //    protected:
  //        int protectedMember;
  //    private:
  //        int privateMember;
  //};
  //Everything that is aware of Base is also aware that Base contains publicMember.
  //Only the children (and their children) are aware that Base contains protectedMember.
  //No one but Base is aware of privateMember.
  //By "is aware of", I mean "acknowledge the existence of, and thus be able to access".
  //
  //The same happens with public, private and protected inheritance. Let's consider a class Base and a class File
  // that inherits from Base.
  //
  //If the inheritance is public, everything that is aware of Base and File (Directory imports both Base and File) is
  // also aware that File inherits from Base (and so they know they can insert a File inside a Base object, like
  // for our map!!)
  //If the inheritance is protected, only File, and its children, are aware that they inherit from Base.
  //If the inheritance is private (default behaviour!), no one other than File is aware of the inheritance.
 private:
  uintmax_t size; //-V122
 public:
  File(const std::string &name, uintmax_t dimension);
  File(std::string &&name, uintmax_t size);
  [[nodiscard]] uintmax_t getSize() const;
  [[nodiscard]] int myType() const override;
  void ls(int indent) const override;

};

#endif //LAB2_ES3__FILE_H_
