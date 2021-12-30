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
#include "Linear_regression/parser.h"


int main(){


    data_frame df;
    df.data = matrix{ matrix::vector<double>{}, matrix::vector<double>{}, matrix::vector<double>{}, matrix::vector<double>{} };
    // +   2^2 - 1 + 4-2*2/(3 - 2)
    try {
        df.add_col("four", "lg(4 - 2 - 3^0) + 2^2 - 1 + 4-2*2/(3 - 2)").add_col("id", "$i^2").add_col("id2", "id-2");
        df.add_col("_id3", "id2 * 4");
    }
    catch (exception& e){
        cout << e.what() << '\n';
    }
    cout << df;
    return 1;



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