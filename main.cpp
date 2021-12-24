#include <iostream>
#include <string>

#include "Matrix/matrix.h"
using namespace std;
#include <iomanip>


struct data_frame {
    My_vec<std::string> col_names;
    Matrix<double> data;
};


#include "Linear_regression/helper.h"
//#include "Linear_regression/test2.h"



int main(){
    /*matrix A {{1, 2}, {3, 4}};
    matrix B {{1, 2}, {3, 4}};
    cout << B + A + A + B;
    cout << 4 * B;

    auto C { 4 * B };

    auto Z = C;
    cout << "true: " << (Z == 4 * B) << '\n';
    
    cout << B * A;

    cout << A.col_select({0, 0, 1, 1});
    cout << A.col_select({0, 0, 1, 1}).t();
*/
    matrix D {
        {1, 3, 1, 9},
        {1, 1, 1, 1},
        {3, 11, 5, 35},
        {4, 3, 2, 1}};
    cout << D;



    cout << "RREF: \n" << get_rref(D);
    


    cout << "Inv: \n" << inv(D);
    cout << "Inv * A:\n" << inv(D) * D;

    return 0;
    cout << D;
    row_multiply(D, 1, 1/D[1][1]);
    //D.row_add(1, -1, 0).row_add(2, -3, 0);
    cout << D;
    row_add(D, 0, -D[0][1]/D[1][1], 1);
    row_add(D, 2, -D[2][1]/D[1][1], 1);
    cout << D;
    row_swap(D, 1, 1);
    cout << D;

  //  cout << scientific << setw(12) << 14124.242422452266 << " f\n";

  //  cout << boolalpha << (1/5 - 5 * 1 / 7 * (1/(25*7)) == 0);
}