#include <memory>

template<typename T, typename A = std::allocator<T>>
struct Mem_base{
    A alloc;
    T* elem;
    T* space;
    T* last;

    Mem_base(const A& a, typename A::size_type n, typename A::size_type m =0)
        :alloc{a}, elem{alloc.allocate(n+m)}, space{elem + n}, last{elem + n + m} {}
    ~Mem_base() { alloc.deallocate(elem, last-elem) }

    Mem_base(const Mem_base&) = delete;
    Mem_base& operator=(const Mem_base&) = delete;
    
    Mem_base(Mem_base&& other) :alloc{other.a}, elem{other.elem}, space{other.space}, last{other.last} {
        other.elem = other.space = other.last = nullptr;    
    }

    Mem_base& operator=(Mem_base&& other){
        swap(alloc, other.alloc);
        swap(elem, other.elem);
        swap(space, other.space);
        swap(last, other.last);
    }
};