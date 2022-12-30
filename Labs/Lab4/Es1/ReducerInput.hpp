//
// Created by Nicolò Maunero on 19/04/21.
//

#ifndef ES1_REDUCERINPUT_HPP
#define ES1_REDUCERINPUT_HPP

template<typename KeyT, typename ValT, typename AccT>
class ReducerInput {
public:
    ReducerInput(KeyT key, ValT val, AccT acc):_key(std::move(key)), _val(std::move(val)), _acc(std::move(acc)){};
    KeyT get_key() const {return _key;};
    ValT get_val() const {return _val;};
    AccT get_accumulator() const {return _acc;};
private:
    KeyT _key;
    ValT _val;
    AccT _acc;
};


#endif //ES1_REDUCERINPUT_HPP
