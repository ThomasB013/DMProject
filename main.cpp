#include <iostream>
#include <string>

#include "Matrix/matrix.h"
using namespace std;
#include <iomanip>

#include "Linear_regression/data_frame.h"

#include "Linear_regression/linear_regression.h"

int main(){
    


    matrix D {
       {3, 6},
       {5, 7},
       {-10, -7},
       {-14, 13},
       {-5, 10},
       {-14, 12},
       {-11, 3},
        {0, 5}
    };
    

    data_frame frame;
    frame.data = D;
    frame.col_names = {"x", "y"};
    cout << frame;   
    print_summary(cout, frame);
    //{""}

    const auto y_data = D.col_select(frame.get_indices({"y"}));
    auto x_data = D.col_select(frame.get_indices({"x"}));
    for(auto& row : x_data){
        row.push_back(row[0]*row[0]*row[0]).push_back(row[0]*row[0]).push_back(row[0]).push_back(1);
        row[0] *= row[0] * row[0] * row[0];
    }
    
    data_frame x_d;
    x_d.data = x_data;
    x_d.col_names = {"x^4", "x^3", "x^2", "x^1", "b"};
    cout << x_d;

    Linear_regresser lr(y_data, x_data);
    lr.fit();

    cout << lr.get_coeff();

    return 0;
}