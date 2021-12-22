#include <iostream>
#include <string>

#include "Matrix/Matrix.h"
using namespace std;

#include "Matrix/chm_iterator.h"

#include "Matrix/my_vec.h"

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
    My_vec<int> v{10, 4};
    for(int x : v){
        cout << x << ',';
    }
}