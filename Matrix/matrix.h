#pragma once

#include "my_vec.h"
#include <vector>
#include <iostream>
#include <exception>

struct bad_dimension : std::exception {};
struct non_rect : bad_dimension {};
struct non_square : bad_dimension {};


template<typename T>
class Matrix{
public:
    template<typename U>
    using vector = My_vec<U>;

    using size_type = typename vector<T>::size_type;

    Matrix() : data{vector<vector<T>>(0, vector<T>(0))} {}
    Matrix(const vector<vector<T>>& d) :data{d} {}
    Matrix(std::initializer_list<vector<T>> d) :data{d} {}


    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);

    bool is_rect() const;
    void assert_rect() const /*throw(non_rect)*/;
    bool is_square() const;
    void assert_square() const /*throw(non_square)*/;


private:
    vector<vector<T>> data;
};



template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m){
    for(const auto& row : m.data){
        for(const auto& entry : row)
            os << entry << ' ';
        os << '\n';
    }
    return os;
}

template<typename T>
bool Matrix<T>::is_rect() const {
    if (data.size() == 0)
        return true;

    for (size_type i = 1; i != data.size(); ++i)
        if (data[i - 1].size() != data[i].size())
            return false;
    return true;
}

template<typename T>
void Matrix<T>::assert_rect() const {
    if (!is_rect())
        throw non_rect {};
}

template<typename T>
bool Matrix<T>::is_square() const {
    const size_type M = data.size();
    for (const auto& row : data)
        if (row.size() != M)
            return false;
    return true;
}

template<typename T>
void Matrix<T>::assert_square() const {
    if (!is_square())
        throw non_square {};
}