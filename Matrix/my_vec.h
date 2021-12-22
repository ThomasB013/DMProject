#include "mem_base.h"
#include "chm_iterator.h"

const size_t GROWING_RATIO = 2;
const size_t MIN_SIZE = 4; //Make sure that ground(MIN_SIZE * GROWING_RATIO) > MIN_SIZE.


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

   

    explicit My_vec();
    explicit My_vec(size_type n, const T& val = T{}, const A& a = A{});
    My_vec(std::initializer_list<T> l);
    My_vec(My_vec&& v);
    My_vec(const My_vec& v);

    size_type size() const;
    iterator begin();
    iterator end();

    reference operator[](size_type i);
    const_reference operator[](size_type i) const;

protected:
    
private:
    Mem_base<T, A> b;
    void destroy_elements();
    void resize(size_type new_size);
};

//*****************************
//      PUBLIC
//*****************************

template<typename T, typename A>
My_vec<T, A>::My_vec() :b{A{}, 0, MIN_SIZE} {}

template<typename T, typename A>
My_vec<T, A>::My_vec(size_type n, const T& val, const A& a) :b{a, n} {
    std::uninitialized_fill(b.elem, b.space, val);    
}

template<typename T, typename A>
My_vec<T, A>::My_vec(std::initializer_list<T> l) :b{A{}, l.size()} {
    std::uninitialized_copy(l.begin(), l.end(), b.elem);
}

template<typename T, typename A>
My_vec<T, A>::My_vec(My_vec&& v)  {
    b = move(v.b);
}

template<typename T, typename A>
My_vec<T, A>::My_vec(const My_vec& v) :b{v.b.alloc, v.size()} {
    std::uninitialized_copy(v.b.elem, v.b.last, b.elem);
}

template<typename T, typename A>
typename My_vec<T, A>::size_type My_vec<T, A>::size() const {
    return b.last - b.elem;
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
typename My_vec<T, A>::reference My_vec<T, A>::operator[](size_type i) {
    return b.elem[i];
}

template<typename T, typename A>
typename My_vec<T, A>::const_reference My_vec<T, A>::operator[](size_type i) const {
    return b.elem[i];
}


//*****************************
//      PROTECTED
//*****************************

//*****************************
//      PRIVATE
//*****************************
template<typename T, typename A>
void My_vec<T, A>::destroy_elements(){
    for(auto p = b.elem; p != b.last; ++p)
        p->~T();
}