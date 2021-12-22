#include "mem_base.h"
#include "chm_iterator.h"

const size_t DOUBLING_RATIO = 2;



template<typename T, typename A = std::allocator<T>>
class My_vec{ 
public:
    using size_type = size_t;


    explicit My_vec();
    explicit My_vec(size_type n, const T& val = T{}, const A& a = A{});
    //My_vec(std::initializer_list<T>)

    Chm_iterator<T> begin();
    Chm_iterator<T> end();

private:
    Mem_base<T, A> b;
    void destroy_elements();
};



template<typename T, typename A>
My_vec<T, A>::My_vec(size_type n, const T& val, const A& a) :b{a, n} {
    std::uninitialized_fill(b.elem, b.space, val);    
}

template<typename T, typename A>
Chm_iterator<T> My_vec<T, A>::begin() {
    return Chm_iterator<T>(b.elem);
}

template<typename T, typename A>
Chm_iterator<T> My_vec<T, A>::end() {
    return Chm_iterator<T>(b.last);
}
