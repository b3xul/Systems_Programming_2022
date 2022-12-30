// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef ES1_MAPPERINPUT_HPP
#define ES1_MAPPERINPUT_HPP

#include "Serializable.hpp"
//#include <boost/json/src.hpp>
#include "json.hpp"

template<typename T>
class MapperInput {
 public:
  MapperInput() = default;
  MapperInput(T value) : _value(std::move(value)) {
  };

  T get_value() const {
    return _value;
  };

  /*  void tag_invoke(const boost::json::value_from_tag &, const boost::json::value &jv, const MapperInput &obj) {

      T val = obj.get_value();
      jv = {{"_value", val}};
    }*/

  static void to_json(nlohmann::json &j, const MapperInput &m) {
    j = nlohmann::json{{"_value", m.get_value()}};
  }

  static void from_json(const nlohmann::json &j, MapperInput &m) {
    m._value = j.at("_value").get<T>();
  }

  std::vector<char> serialize() const {
    //deserialize will:
    // 1. read the first < and ignore it
    // 2. read the number of bytes of the string following the >
    // 3. resize string to contain the serialized object between < and >
    // 4. allocate space for all the fields
    const MapperInput &map_to_serialize = *this;
    /*
        boost::json::value jv = {{"_value", map_to_serialize.get_value()}};
        std::string serialized_attribute1_string = boost::json::serialize(jv);
    */
    nlohmann::json j;
    MapperInput::to_json(j, map_to_serialize);
    std::string serialized_object_string = j.dump();
    std::size_t serialized_object_length = serialized_object_string.size();
    std::vector<char> v(serialized_object_length);
    std::copy(serialized_object_string.begin(), serialized_object_string.end(), v.begin());
    /*    std::string serialized_attribute1_string = j.dump();
        std::size_t attribute1_length = serialized_attribute1_string.size();
        std::size_t attribute1_length_length = std::to_string(attribute1_length).size();

        std::size_t following_length = attribute1_length_length + attribute1_length;
        std::size_t following_length_length = std::to_string(following_length).size();

        std::string final =
            std::to_string(following_length) + ' ' + std::to_string(attribute1_length) + serialized_attribute1_string;
        std::vector<char> v(following_length_length + 1 + following_length);
        std::copy(final.begin(), final.end(), v.begin());*/

    //89 87{"_value":"209.17.96.34 - - [01/Jan/2020:00:19:59 +0100] \"GET / HTTP/1.1\" 200 20744"}
    return v;
  }
  void deserialize(std::shared_ptr<char *> serialized_object) {
    //89 87{"_value":"209.17.96.34 - - [01/Jan/2020:00:19:59 +0100] \"GET / HTTP/1.1\" 200 20744"}

    nlohmann::json j;
    char **tmp = serialized_object.get();
    std::string string_serialized_object(*tmp);
    //std::cout << "[ MAPPER ] Trying to deserialize: " << string_serialized_object << std::endl;
    j = nlohmann::json::parse(string_serialized_object);
    from_json(j, *this);
    //std::cout << "[ MAPPER ] Deserialized: " << this->get_value() << std::endl;

    //const boost::json::object &obj = .as_object();
    //this->_value = value_to<T>(obj.at("_value");

  }
 private:
  T _value;
};

#endif //ES1_MAPPERINPUT_HPP
