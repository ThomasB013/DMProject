#include <iostream>
#include <string>

#include "Matrix/matrix.h"
using namespace std;
#include <iomanip>

#include "Linear_regression/data_frame.h"

#include "Linear_regression/linear_regression.h"

#include <algorithm>

#include "Linear_regression/helper.h"

double square(double x){
    return x*x;
}



int main(){
    double t;
    int freedom;
    data_frame f {"C:\\Users\\thoma\\Documents\\Computing Science\\Year 2\\Semester 1\\Data Mining\\DMProject\\data.txt"};
    
    for (auto& row : f.data)
        row.push_back(1);
    
    f.col_names.push_back("cons");


    cout << f << '\n';
    

    //cout << f.data.row_count();

    //print_summary(cout, f);
    //f.regress("wdi_lifexp", "cons p_polity2 gle_rgdpc");
    f.regress("csat", "expense percent income high college cons");
    //f.regress("sat_system", "female age high_educ inc_categ cons");

    return 0;
}