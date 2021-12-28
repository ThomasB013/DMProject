#pragma once

#include "helper.h"
/*

In this file I implement a linear regresser. 

I tried to do a lot of things for a single instance / index and then 
use these functions to fill a matrix of instances.

Not used: 
struct not_fitted : std::runtime_error { not_fitted(std::string msg) :std::runtime_error(msg) {} };
*/

struct interval {
    interval() {}
    interval(double l, double r) :left(l), right(r) {}
    bool contains(double v) const;
    
    double left;
    double right;
};

class Linear_regresser {
public:
    explicit Linear_regresser();
    void fit(const matrix& y_data, const matrix& X_data);
  

    const matrix& get_coeff() const;
    double get_est_sd() const;
    double get_SST() const;
    double get_SSE() const;
    double get_SSR() const;

    size_t get_no_obs() const;
    size_t get_no_reg() const;

    double get_coeff_std_dev(matrix::size_type i) const;
    matrix::vector<double> get_coeff_std_dev() const;
    
    //The following 4 functions account to test against \beta_i = coeff[i][0] = 0.
    double get_coeff_t_stat(matrix::size_type i) const;
    matrix::vector<double> get_coeff_t_stat() const;
    double get_t_test(matrix::size_type i) const;
    matrix::vector<double> get_t_test() const;

    interval get_95_conf_coeff(matrix::size_type i) const;
    matrix::vector<interval> get_95_conf_coeff() const;

    //These functions predict a single observation row or an observation matrix.
    matrix predict(const matrix::vector<double>& obs) const;
    matrix predict(const matrix& X) const;

    //These functions return the 95% confidence interval of a prediction given an observation.
    interval get_95_conf_predict(const matrix::vector<double>& obs) const;
    matrix::vector<interval> get_95_conf_predict(const matrix& obs) const; //In this case obs needs to be a 1 times k matrix.

private:
    bool fitted;
    void assert_fitted() const;
    void set_coeff_std_dev();
    matrix XtX_inv;
    matrix coeff;
    matrix::vector<double> coeff_std_dev;
    double est_sd; //estimated standard deviation
    double SST;
    double SSE;
    double SSR;
    size_t n;
    size_t k;
};
