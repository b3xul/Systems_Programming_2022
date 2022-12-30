//
// Created by Nicolò Maunero on 19/04/21.
//

#ifndef ES1_RESULT_HPP
#define ES1_RESULT_HPP

template<typename KeyT, typename ValueT>
class Result {
public:
    Result() = default;  //ho bisogno del costruttore di default se uso gli iteratori
    Result(KeyT key):_key(std::move(key)){};
    Result(KeyT key, ValueT value): _value(std::move(value)),_key(std::move(key)){};
    KeyT get_key() const {return _key;};
    ValueT get_value() const {return _value;};
private:
    KeyT _key;
    ValueT _value;
};


#endif //ES1_RESULT_HPP
