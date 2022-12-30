/*
//
// Created by bexul on 11/01/22.
//

#ifndef ES2_PROGRAMMAZIONE_LABS_LAB4_PARTE2_SERIALIZABLE_HPP_
#define ES2_PROGRAMMAZIONE_LABS_LAB4_PARTE2_SERIALIZABLE_HPP_

#include <memory>
#include <boost/json/src.hpp>

template<typename T>
class Serializable {
 private:
 public:
  std::vector<char> serialize() const {
    std::cout << boost::json::serialize(boost::json::value_from(this));
    return std::vector<char>{'a', 'b'};
  }
  void deserialize(std::shared_ptr<char *>);
};
#endif //ES2_PROGRAMMAZIONE_LABS_LAB4_PARTE2_SERIALIZABLE_HPP_
*/
