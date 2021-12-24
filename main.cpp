#include <iostream>
#include <string>

#include "Matrix/matrix.h"
using namespace std;



struct data_frame {
    My_vec<std::string> col_names;
    Matrix<double> data;
};


#include "Linear_regression/helper.h"
//#include "Linear_regression/test2.h"

int main(){
    matrix A {{1, 2}, {3, 4}};
    matrix B {{1, 2}, {3, 4}};
    cout << B + A + A + B;
    cout << 4 * B;

    cout << B * A;

    cout << A.col_select({0, 0, 1, 1});
    cout << A.col_select({0, 0, 1, 1}).t();

   /*Matrix<int> A {{1, 2}, {3, 4}};
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
*/
}