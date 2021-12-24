#include <iostream>
#include <string>

#include "Matrix/matrix.h"
using namespace std;

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

    
    Matrix<int> B {{3, 4, 5}, {4, 5, 6}, {7, 8, 9}};

    cout << B;
    
    auto C = B.col_select({0, 1, 1, 1, 2});
    cout << C;

    cout << B.row_select({2, 1, 0});

    auto D =  C.cols({true, false, true, false, true});

    cout << D;

    cout << std::boolalpha << (B == D);


    //cout << std::boolalpha << B.is_rect() << '\t' << B.is_square() << '\n';
//    Matrix<int> A {{1, 2}, {3, 4}};

    //for(const auto& r : A.data){
    //    for(int x : r){
    //        cout << x << ' ';
    //    }
    //    cout << '\n';
    //}
    
}