#include <iostream>
#include <string>

#include "Matrix/Matrix.h"
using namespace std;

#include "Matrix/chm_iterator.h"


struct X{
    X(int a_ = 0) :a(a_), b(0) {}
    int a, b;
};

bool operator==(const X& a, const X& b){
    return a.a == b.a && a.b == b.b;
}


struct Y : public X {
    Y(int a, const char* c) :X(a), s(c) {}
    const char* s;
};



int main(){
    Y y1(5, "dikke herres"), y2(5, "ff");
    
    cout << boolalpha << (y1 == y2);

    X *p = new X[100];

    Chm_iterator<X> v {p};
    v->a = 4;
    v->b = 3;
    X copy = *v;
    auto zz = v + 4;
    zz->a = 10;
    zz->b = 40;
    cout << (p + 4)->a  << (v+=4)->b << '\n';
}