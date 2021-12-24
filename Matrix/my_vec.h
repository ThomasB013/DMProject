#pragma once

#include "mem_base.h"
#include "chm_iterator.h"

#include <iostream>
#include <memory>
#include <utility>

const size_t GROWING_RATIO = 2;
const size_t DEFAULT_SIZE = 4;


template<typename T, typename A = std::allocator<T>>
class My_vec{ 
public:
    using size_type = size_t;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = Chm_iterator<T>;
    using const_iterator = const_Chm_iterator<T>;
   
    explicit My_vec();
    explicit My_vec(size_type n, const T& val = T{}, const A& a = A{});
    My_vec(std::initializer_list<T> l);
    My_vec(My_vec&& v);
    My_vec(const My_vec& v);
    ~My_vec() { destroy_elements(); }

    size_type size() const;
    size_type capacity() const;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    bool operator==(const My_vec<T, A>& v) const;
    bool operator!=(const My_vec<T, A>& v) const;

    reference operator[](size_type i);
    const_reference operator[](size_type i) const;

    My_vec<T, A>& operator=(My_vec<T, A>&& v);
    My_vec<T, A>& operator=(const My_vec<T, A>& v);

    My_vec<T, A>& push_back(T&& val);
    My_vec<T, A>& push_back(const T& val);
    My_vec<T, A>& pop_back();

    My_vec<T, A>& reserve(size_type n);

private:
    Mem_base<T, A> b;
    void destroy_elements();
    void resize(size_type new_size);
};

//*****************************
//      PUBLIC
//*****************************

template<typename T, typename A>
My_vec<T, A>::My_vec() :b{A{}, 0, DEFAULT_SIZE} {}

template<typename T, typename A>
My_vec<T, A>::My_vec(size_type n, const T& val, const A& a) :b{a, n} {
    std::uninitialized_fill(b.elem, b.space, val);    
}

template<typename T, typename A>
My_vec<T, A>::My_vec(std::initializer_list<T> l) :b{A{}, l.size()} {
    std::uninitialized_copy(l.begin(), l.end(), b.elem);
}

template<typename T, typename A>
My_vec<T, A>::My_vec(My_vec&& v) :b{move(v.b)} { }

template<typename T, typename A>
My_vec<T, A>::My_vec(const My_vec& v) :b{v.b.alloc, v.size()} {
    std::uninitialized_copy(v.b.elem, v.b.last, b.elem);
}

template<typename T, typename A>
typename My_vec<T, A>::size_type My_vec<T, A>::size() const {
    return b.last - b.elem;
}

template<typename T, typename A>
typename My_vec<T, A>::size_type My_vec<T, A>::capacity() const {
    return b.space - b.elem;
}

template<typename T, typename A>
typename My_vec<T, A>::iterator My_vec<T, A>::begin() {
    return iterator{b.elem};
}

template<typename T, typename A>
typename My_vec<T, A>::iterator My_vec<T, A>::end() {
    return iterator{b.last};
}

template<typename T, typename A>
bool My_vec<T, A>::operator==(const My_vec<T, A>& v) const {
    if (size() != v.size())
        return false;
    
    if(this == &v)
        return true;

    for(size_type i = 0; i != v.size(); ++i)
        if (b.elem[i] != v.b.elem[i])
            return false;
    return true;
}

template<typename T, typename A>
bool My_vec<T, A>::operator!=(const My_vec<T, A>& v) const {
    return !(*this == v);
}

template<typename T, typename A>
typename My_vec<T, A>::const_iterator My_vec<T, A>::begin() const {
    return const_iterator{b.elem};
}

template<typename T, typename A>
typename My_vec<T, A>::const_iterator My_vec<T, A>::end() const {
    return const_iterator{b.last};
}

template<typename T, typename A>
typename My_vec<T, A>::reference My_vec<T, A>::operator[](size_type i) {
    return b.elem[i];
}

template<typename T, typename A>
typename My_vec<T, A>::const_reference My_vec<T, A>::operator[](size_type i) const {
    return b.elem[i];
}

template<typename T, typename A>
My_vec<T, A>& My_vec<T, A>::operator=(My_vec<T, A>&& v) {
    swap(b, v.b);
    return *this;
}

template<typename T, typename A>
My_vec<T, A>& My_vec<T, A>::operator=(const My_vec<T, A>& v) {
    if (this == &v)
        return *this;
    
    if (size() >= v.size()) {
        destroy_elements(); //Clear the elements
        std::uninitialized_copy(v.b.elem, v.b.elem + v.size(), b.elem); //And fill what you need.
        b.last = b.elem + v.size();
    } else {
        My_vec<T, A> temp {v}; //Copy v completely, including allocated size.
        swap(*this, temp);
    }
    return *this;    
}

template<typename T, typename A>
My_vec<T, A>& My_vec<T, A>::push_back(T&& val) {
    if (b.last == b.space)
        resize(max(DEFAULT_SIZE, size() * GROWING_RATIO));
    *b.last = val;
    b.last++;
    return *this;
}

template<typename T, typename A>
My_vec<T, A>& My_vec<T, A>::push_back(const T& val) {
    T copy {val};
    return push_back(std::move(copy));
}

template<typename T, typename A>
My_vec<T, A>& My_vec<T, A>::pop_back() {
    if (b.last == b.elem) //This also checks for nullpointers.
        return *this;
    b.last--;
    b.last->~T();
    return *this;
}

template<typename T, typename A>
My_vec<T, A>& My_vec<T, A>::reserve(size_type n) {
    if (capacity() < n)
        resize(n);
}

//*****************************
//      PRIVATE
//*****************************

template<typename T, typename A>
void My_vec<T, A>::destroy_elements() {
    for(auto p = b.elem; p != b.last; ++p)
        p->~T();
}

template<typename T, typename A>
void My_vec<T, A>::resize(size_type new_size) {
    while(size() > new_size)
        pop_back();

    Mem_base<T, A> m {b.alloc, new_size};
    m.last = m.elem + size();
    std::uninitialized_move(b.elem, b.last, m.elem);
    swap(b, m);
}