#pragma once

#include "my_vec.h"
#include <vector>
#include <iostream>
#include <exception>

struct bad_dimension : std::exception {};
struct non_rect : bad_dimension {};
struct non_square : bad_dimension {};
struct no_rows : bad_dimension {};

struct non_matching_dim : bad_dimension {};
struct non_matching_row_dim : non_matching_dim {};
struct non_matching_col_dim : non_matching_dim {};
struct non_matching_col_row_dim : non_matching_dim {};

template<typename T>
struct Matrix{
    template<typename U>
    using vector = My_vec<U>;

    using size_type = typename vector<T>::size_type; //Copy from the vector we're using.
    using value_type = typename vector<vector<T>>::value_type;
    using reference = typename vector<vector<T>>::reference;
    using const_reference = typename vector<vector<T>>::const_reference;
    using pointer = typename vector<vector<T>>::pointer;
    using const_pointer = typename vector<vector<T>>::const_pointer;
    using iterator = typename vector<vector<T>>::iterator;
    using const_iterator = typename vector<vector<T>>::const_iterator;

    Matrix() : data{vector<vector<T>>(0, vector<T>(0))} {}
    Matrix(size_type rows, size_type cols, const T& val = T{}) : data{vector<vector<T>>(rows, vector<T>(cols, val))} {}
    Matrix(const vector<vector<T>>& d) :data{d} {}
    Matrix(std::initializer_list<vector<T>> d) :data{d} {}


    size_type row_count() const { return data.size(); }
    size_type col_count() const { /*throw(no_rows)*/
        if (row_count() ==0) throw no_rows {}; return data[0].size(); }
    size_type get_col_count() const { /*throw(no_rows, non_rect)*/
        if (row_count() == 0) throw no_rows {}; assert_rect(); return data[0].size(); };

    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }
    
    bool operator==(const Matrix<T>& v) const { return data == v.data; }
    bool operator!=(const Matrix<T>& v) const { return data != v.data; }

    reference operator[](size_type i) { return data[i]; }
    const_reference operator[](size_type i) const { return data[i]; }

    bool is_rect() const;
    void assert_rect() const /*throw(non_rect)*/;
    bool is_square() const;
    void assert_square() const /*throw(non_square)*/;

    Matrix<T> col_select(const vector<size_type>& selection) const;
    Matrix<T> cols(const vector<bool>& selection) const /*throw(non_matching_col_dim)*/;

    Matrix<T> row_select(const vector<size_type>& selection) const;
    Matrix<T> rows(const vector<bool>& selection) const /*throw(non_matching_row_dim)*/;

    Matrix<T> t() const;

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


template<typename T>
Matrix<T> Matrix<T>::col_select(const vector<size_type>& selection) const {
    Matrix<T> ans(row_count(), selection.size());
    for(size_type i = 0; i != ans.row_count(); ++i)
        for(size_type j = 0; j != ans.col_count(); ++j)
            ans[i][j] = (*this)[i][selection[j]]; //Got syntax error on this->[i], (this->operator[](i)) worked, but that's ugly. Could also do this->data[i]
    return ans;
}

template<typename T>
Matrix<T> Matrix<T>::cols(const vector<bool>& selection) const {
    if (col_count() != selection.size())
        throw non_matching_col_dim {};
    
    size_type col_size = 0;
    for (bool b : selection)
        if (b)
            col_size++;

    Matrix<T> ans(row_count(), col_size);
    
    for(size_type i = 0; i != ans.row_count(); ++i) {
        size_type col = 0;
        for(size_type j = 0; j != selection.size(); ++j)
            if (selection[j]){
                ans[i][col] = (*this)[i][j];
                col++;
            }
    }
    return ans;
}

template<typename T>
Matrix<T> Matrix<T>::row_select(const vector<size_type>& selection) const {
    Matrix<T> ans(selection.size(), col_count());
    for(size_type i = 0; i != ans.row_count(); ++i)
        for(size_type j = 0; j != ans.col_count(); ++j)
            ans[i][j] = (*this)[selection[i]][j];
    return ans;
}

template<typename T>
Matrix<T> Matrix<T>::rows(const vector<bool>& selection) const {
    if (row_count() != selection.size())
        throw non_matching_row_dim{};
    
    size_type row_size = 0;
    for (bool b : selection)
        if (b) //Seems better than pushing_back or wasting allocated memory.
            row_size++;
    
    Matrix<T> ans (row_size, col_count());
    size_type i = 0;
    for (size_type j = 0; j != selection.size(); ++j)
        if (selection[j]){
            ans[i] = (*this)[j];
            ++i;
        }
    return ans;    
}

template<typename T>
Matrix<T> Matrix<T>::t() const {
    Matrix<T> ans(col_count(), row_count());
    for(size_type i = 0; i != row_count(); ++i)
        for(size_type j = 0; j != col_count(); ++j)
            ans[j][i] = (*this)[i][j];
    return ans;
}