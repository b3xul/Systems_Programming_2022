// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#ifndef ES1_RESULT_HPP
#define ES1_RESULT_HPP

template<typename KeyT, typename ValueT>
class Result {
 public:
  Result() = default;  //ho bisogno del costruttore di default se uso gli iteratori
  Result(KeyT key) : _key(std::move(key)) {
  };
  Result(KeyT key, ValueT value) : _value(std::move(value)), _key(std::move(key)) {
  };
  KeyT get_key() const {
    return _key;
  };
  ValueT get_value() const {
    return _value;
  };
  static void to_json(nlohmann::json &j, const Result &m) {
      j = nlohmann::json{{"_key",m.get_key()},{"_value", m.get_value()}};
    }

    static void from_json(const nlohmann::json &j, Result &m) {
      m._key = j.at("_key").get<KeyT>();
      m._value = j.at("_value").get<ValueT>();
    }

    std::vector<char> serialize() const {
      //deserialize will:
      // 1. read the first < and ignore it
      // 2. read the number of bytes of the string following the >
      // 3. resize string to contain the serialized object between < and >
      // 4. allocate space for all the fields
      const Result &result_to_serialize = *this;
      /*
          boost::json::value jv = {{"_value", map_to_serialize.get_value()}};
          std::string serialized_attribute1_string = boost::json::serialize(jv);
      */
      nlohmann::json j;
      Result::to_json(j, result_to_serialize);
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
      //std::cout << "Trying to deserialize: " << string_serialized_object << std::endl;
      j = nlohmann::json::parse(string_serialized_object);
      from_json(j, *this);
/*
      std::cout << "Deserialized: " << this->get_value() << std::endl;
*/

      //const boost::json::object &obj = .as_object();
      //this->_value = value_to<T>(obj.at("_value");

    }
 private:
  KeyT _key;
  ValueT _value;
};

#endif //ES1_RESULT_HPP
