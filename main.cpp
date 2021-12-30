#include <iostream>
#include <fstream>
#include "Linear_regression/data_frame.h"
#include <random>
using namespace std;

void csat_example();
void data_frame_example();
void outlier_correct_example();
void outlier_incorrect_example();
void gen_data_points(string file_name, int fun_points =200, int ran_points =25, int var =25, int max_x =27, int max_y =150);

int main(){
    csat_example();
    data_frame_example();
    
    outlier_correct_example();
    outlier_incorrect_example();
    return 0;
}

void csat_example() {
    cout << "\n\n****************************************************************\n";
    cout << "csat_example";
    cout << "\n****************************************************************\n\n";
    
    data_frame df("../Data/csat_data.txt"); //Relative to the build folder.
    
    //Inspect the layout
    cout << "Data Frame:\n";
    cout << df; 
    
    //Inspect the data
    cout << "\n\nSummary Ouput:\n";
    print_summary(df, cout); 
    cout << "\n\nRegression Ouput:\n";

    //Add an intercept columnn named cons.
    df.add_col("cons", "1");
    df.regress("csat", "expense percent income high college cons");

    /*
    Output:

    Data Frame:
    id              csat            percent         expense         income          high            college
    0.000000e+00    9.910000e+02    8.000000e+00    3.627000e+03    2.749800e+01    6.690000e+01    1.570000e+01
    1.000000e+00    9.200000e+02    4.100000e+01    8.330000e+03    4.825400e+01    8.660000e+01    2.300000e+01
    2.000000e+00    9.320000e+02    2.600000e+01    4.309000e+03    3.209300e+01    7.870000e+01    2.030000e+01
    3.000000e+00    1.005000e+03    6.000000e+00    3.700000e+03    2.464300e+01    6.630000e+01    1.330000e+01
    .
    .
    4.700000e+01    9.130000e+02    4.900000e+01    5.000000e+03    3.633800e+01    8.380000e+01    2.290000e+01
    4.800000e+01    9.260000e+02    1.700000e+01    4.911000e+03    2.423300e+01    6.600000e+01    1.230000e+01
    4.900000e+01    1.023000e+03    1.100000e+01    5.871000e+03    3.430900e+01    7.860000e+01    1.770000e+01
    5.000000e+01    9.800000e+02    1.300000e+01    5.723000e+03    3.157600e+01    8.300000e+01    1.880000e+01


    Summary Ouput:
    Summary         id              csat            percent         expense         income          high            college
    Min             0.000000e+00    8.320000e+02    4.000000e+00    2.960000e+03    2.346500e+01    6.430000e+01    1.230000e+01
    Max             5.000000e+01    1.093000e+03    8.100000e+01    9.259000e+03    4.861800e+01    8.660000e+01    3.330000e+01
    Mean            2.500000e+01    9.440980e+02    3.576471e+01    5.235961e+03    3.395657e+01    7.626078e+01    2.002157e+01
    Std Dev         1.486607e+01    6.693497e+01    2.619281e+01    1.401155e+03    6.423134e+00    5.588741e+00    4.165780e+00
    Number of observations: 51


    Regression Ouput:
    csat            expense         percent         income          high            college         cons
    Coeff           3.352826e-03    -2.618177e+00   1.055852e-01    1.630841e+00    2.030894e+00    8.515649e+02
    Std Dev         4.470883e-03    2.538491e-01    1.166094e+00    9.922470e-01    1.660118e+00    5.929228e+01
    t-stat          7.499248e-01    -1.031391e+01   9.054611e-02    1.643584e+00    1.223343e+00    1.436215e+01
    P > |t|         4.572030e-01    1.935119e-13    9.282551e-01    1.072320e-01    2.275677e-01    4.440892e-16
    [95% Conf       -5.659356e-03   -3.129873e+00   -2.244968e+00   -3.692809e-01   -1.315489e+00   7.320465e+02
    interval]       1.236501e-02    -2.106480e+00   2.456138e+00    3.630963e+00    5.377278e+00    9.710833e+02

    # Obs: 51               # Reg: 6
    SSE 1.846633e+05        SSR 3.935120e+04
    SST 2.240145e+05
    */
}


void data_frame_example() {
    cout << "\n\n****************************************************************\n";
    cout << "data_frame_example";
    cout << "\n****************************************************************\n\n";
    
    data_frame f;
    f.data = matrix(5, 0); //5 empty rows.
    
    try {
        f.add_col("test_erro", "4+");
    }
    catch (exception& e){
        cout << e.what() << '\n';
    }
    try {
        f.add_col("test_error", "not_present");
    }
    catch (exception& e){
        cout << e.what() << '\n';
    }
    try {
        f.add_col("1nvalid_name", "1");
    }
    catch (exception& e) {
        cout << e.what() << '\n';
    }

    f.add_col("id", "$i");
    f.add_col("lg_id", "lg (id + 1)");
    f.add_col("x", "lg_id * 4 + e^(id*3)/100");
    f.add_col("id2", "e^lg_id + -1*400 - -5/1/80 + 1"); //Evaluated as: e^lg_id + -1*400 - (-5/(1/80) + 1); see parser.h
    f.add_col("sqrt_pi", "sqrt pi");
    f.add_col("_pi", "sqrt_pi^2");
    cout << f;

    /*
    Output:
    Expected a term.
    Column name not found: not_present
    Invalid column name: 1nvalid_name
    id              lg_id           x               id2             sqrt_pi         _pi
    0.000000e+00    0.000000e+00    1.000000e-02    0.000000e+00    1.772454e+00    3.141593e+00
    1.000000e+00    6.931472e-01    2.973444e+00    1.000000e+00    1.772454e+00    3.141593e+00
    2.000000e+00    1.098612e+00    8.428737e+00    2.000000e+00    1.772454e+00    3.141593e+00
    3.000000e+00    1.386294e+00    8.657602e+01    3.000000e+00    1.772454e+00    3.141593e+00
    4.000000e+00    1.609438e+00    1.633986e+03    4.000000e+00    1.772454e+00    3.141593e+00
    */
}


//This functions generates data points with some variance around the line y = 1/35x^3 - 15x + 10
//It draws fun_points random x points (uniform) between -max_x, max_x. Calculates the y value. And adds a point extracted from a normal distribution N(0, var) to add some variance.
//It then adds ran_points random uniform points. x in [-max_x, max_x] y in [-max_y, max_y].
void gen_data_points(string file_name, int fun_points, int ran_points, int var, int max_x, int max_y) {
    data_frame df;
    df.data = matrix(0, 2);
    df.col_names = {"x", "y"};

    default_random_engine generator;
    uniform_real_distribution<double> uniform_27(-max_x, max_x);
    uniform_real_distribution<double> uniform_150(-max_y, max_y);
    normal_distribution<double> normal(0, var);


    auto y = [](double x) -> double {
        return pow(x, 3)/35 -15*x + 10;
    };

    while (fun_points--) {
        double x = uniform_27(generator);
        df.data.data.push_back({x, y(x) + normal(generator)});
    }

    while (ran_points--) 
        df.data.data.push_back({uniform_27(generator), uniform_150(generator)});

    ofstream ofs {file_name};
    ofs << df.data;
}


void outlier_correct_example() {
    cout << "\n\n****************************************************************\n";
    cout << "outlier_correct_example";
    cout << "\n****************************************************************\n\n";
    

    data_frame df("../Data/data_points.txt");
    cout << df << "\n\n";
    /*
    x               y
    9.658690e+00    -1.146080e+02
    2.347340e+01    1.414740e+01
    1.603810e+00    -1.338480e+01
    -2.658430e+01   -1.232510e+02
    .
    .
    -9.021870e+00   1.078690e+01
    2.321410e+01    -7.703140e+01
    5.223050e+00    -7.590790e+01
    -1.640640e+01   1.007990e+02
    */
    
    //Let's use the fact that we know its a third degree polynomial:
    df.add_col("x3", "x^3").add_col("x2", "x^2");
    df.add_col("cons", "1");

    //Let's see how it goes:
    df.regress("y", "x3 x2 x cons");
    /*
    y               x3              x2              x               cons
    Coeff           2.945503e-02    -5.142709e-03   -1.298371e+01   1.134220e+01
    Std Dev         0.000000e+00    1.870172e-02    6.367851e-01    5.521023e+00
    t-stat          inf             -2.749858e-01   -2.038947e+01   2.054366e+00
    P > |t|         nan             7.835840e-01    -2.220446e-16   4.111681e-02
    [95% Conf       2.945503e-02    -4.219161e-02   -1.424521e+01   4.048203e-01
    interval]       2.945503e-02    3.190619e-02    -1.172221e+01   2.227958e+01

    # Obs: 225              # Reg: 4
    SSE 1.623851e+06        SSR 7.663757e+05
    SST 1.952178e+06
    */
    //Seems like we don't need to include x2, P value is greater than 0.05.

    //Find the outliers:
    auto outcome = df.find_outliers("y", "x3 x cons");

    ofstream ofs {"../Data/outlier_correct.txt"};
    for(int i : outcome)
        ofs << i << '\n';
}


void outlier_incorrect_example() {
    cout << "\n\n****************************************************************\n";
    cout << "outlier_incorrect_example";
    cout << "\n****************************************************************\n\n";

    data_frame df("../Data/data_points.txt");
    

    df.add_col("cons", "1");
    //Fit y = ax + b.
    df.regress("y", "x cons");
    
    //We try to find outliers with a wrong model.
    auto outcome = df.find_outliers("y", "x cons");
    ofstream ofs {"../Data/outlier_incorrect.txt"};
    for(int i : outcome)
        ofs << i << '\n';
}