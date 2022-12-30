// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef ES1_REDUCERINPUT_HPP
#define ES1_REDUCERINPUT_HPP

template<typename KeyT, typename ValT, typename AccT>
class ReducerInput {
 public:
  ReducerInput() = default;
  ReducerInput(KeyT key) : _key(std::move(key)) {
  };
  ReducerInput(KeyT key, ValT val, AccT acc) : _key(std::move(key)), _val(std::move(val)), _acc(std::move(acc)) {
  };
  KeyT get_key() const {
    return _key;
  };
  ValT get_val() const {
    return _val;
  };
  AccT get_accumulator() const {
    return _acc;
  };
  //    boost::json::value jv = {{"_value", map_to_serialize.get_value()}, {"_value2", map_to_serialize.get_value()}};
  static void to_json(nlohmann::json &j, const ReducerInput &m) {
    j = nlohmann::json{{"_key", m.get_key()}, {"_val", m.get_val()}, {"_acc", m.get_accumulator()}};
  }

  static void from_json(const nlohmann::json &j, ReducerInput &m) {
    m._key = j.at("_key").get<KeyT>();
    m._val = j.at("_val").get<ValT>();
    m._acc = j.at("_acc").get<AccT>();
  }

  std::vector<char> serialize() const {
    //deserialize will:
    // 1. read the first < and ignore it
    // 2. read the number of bytes of the string following the >
    // 3. resize string to contain the serialized object between < and >
    // 4. allocate space for all the fields
    const ReducerInput &reducer_to_serialize = *this;
    /*
        boost::json::value jv = {{"_value", map_to_serialize.get_value()}};
        std::string serialized_attribute1_string = boost::json::serialize(jv);
    */
    nlohmann::json j;
    ReducerInput::to_json(j, reducer_to_serialize);
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
    //std::cout << "[ REDUCER ] Trying to deserialize: " << string_serialized_object << std::endl;
    j = nlohmann::json::parse(string_serialized_object);
    from_json(j, *this);
/*    std::cout << "[ REDUCER ] Deserialized: " << this->get_key() << " - " << this->get_val() << " - "
              << this->get_accumulator() << " - " << std::endl;*/

    //const boost::json::object &obj = .as_object();
    //this->_value = value_to<T>(obj.at("_value");

  }
 private:
  KeyT _key;
  ValT _val;
  AccT _acc;
};

#endif //ES1_REDUCERINPUT_HPP
