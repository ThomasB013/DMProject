#pragma once

#include <memory>

template<typename T, typename A = std::allocator<T>>
struct Mem_base{
    A alloc;
    T* elem;
    T* last;
    T* space;

    Mem_base(const A& a, typename A::size_type n, typename A::size_type m =0)
        :alloc{a}, elem{alloc.allocate(n+m)}, last{elem + n}, space{elem + n + m} {}
    ~Mem_base() { alloc.deallocate(elem, last-elem); }

    Mem_base(const Mem_base&) = delete;
    Mem_base& operator=(const Mem_base&) = delete;
    
    Mem_base(Mem_base&& other) :alloc{other.alloc}, elem{other.elem}, last{other.last}, space{other.space} {
        other.elem = other.space = other.last = nullptr;    
    }

    Mem_base& operator=(Mem_base&& other){
        std::swap(alloc, other.alloc);
        std::swap(elem, other.elem);
        std::swap(last, other.last);
        std::swap(space, other.space);
        return *this;
    }
};