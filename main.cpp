#include <iostream>
#include <string>

#include "Matrix/Matrix.h"
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
    
    My_vec<int> row {1, 2, 3, 4, 5};
    for (int i = 6; i <= 10; ++i){
        row.push_back(i);
    }

    print(row);
    row.push_back(7);
    print(row);
}