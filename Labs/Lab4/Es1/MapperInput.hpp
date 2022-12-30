//
// Created by Nicolò Maunero on 19/04/21.
//

#ifndef ES1_MAPPERINPUT_HPP
#define ES1_MAPPERINPUT_HPP

template <typename T>
class MapperInput {
public:
    MapperInput(T value):_value(std::move(value)){};

    T get_value() const {return _value;};
private:
    T _value;
};

#endif //ES1_MAPPERINPUT_HPP
