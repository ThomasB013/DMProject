#include <iostream>
#include <string>

#include "Matrix/matrix.h"
using namespace std;

//#include "Matrix/chm_iterator.h"

#include "Matrix/my_vec.h"

template<typename T>
void print(My_vec<T> v){
    for(const auto x : v)
        cout << x << ' ';
    cout << '\n';
}

int main(){
    
    //My_vec<My_vec<int>> a {v1, v2};
    Matrix<int> A {{1, 2}, {3, 4}};
    cout << A;
    cout << std::boolalpha << A.is_rect() << '\t' << A.is_square() << '\n';

    
    Matrix<int> B {{3, 4, 5}, {4, 5, 6}};
    cout << B;
    cout << std::boolalpha << B.is_rect() << '\t' << B.is_square() << '\n';
//    Matrix<int> A {{1, 2}, {3, 4}};

    //for(const auto& r : A.data){
    //    for(int x : r){
    //        cout << x << ' ';
    //    }
    //    cout << '\n';
    //}
    
}