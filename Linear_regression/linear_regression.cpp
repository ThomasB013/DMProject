#include "linear_regression.h"

#include <math.h>

Linear_regresser::Linear_regresser() {}

void Linear_regresser::fit(const matrix& y_data, const matrix& X_data) {
    X_data.assert_rect("X_data needs to be rectangular.");
    y_data.assert_rect("y_data needs to be rectangular.");
    if (y_data.col_count() != 1)
        throw bad_dimension {"y_data needs to have dimension n x 1."};
    if (y_data.row_count() != X_data.row_count())
        throw bad_dimension {"y_data and X_data need to have the same amount of observations."};
    
    n = y_data.row_count();
    k = X_data.col_count();
    XtX_inv = inv(X_data.t()*X_data);
    coeff = XtX_inv*X_data.t()*y_data;
    
    const auto y_hat = predict(X_data);
    const double y_mean = col_means(y_data)[0];
    SSE = sum_squared(y_hat - y_mean);
    SSR = sum_squared(y_data - y_hat);
    SST = sum_squared(y_data - y_mean);

    est_sd = std::sqrt(SSR / (n - k)); 
    set_coeff_std_dev();
}

const matrix& Linear_regresser::get_coeff() const {
    return coeff;
}

double Linear_regresser::get_est_sd() const {
    return est_sd;
}

double Linear_regresser::get_SST() const {
    return SST;
}

double Linear_regresser::get_SSE() const {
    return SSE;
}

double Linear_regresser::get_SSR() const {
    return SSR;
}

size_t Linear_regresser::get_no_obs() const {
    return n;
}

size_t Linear_regresser::get_no_reg() const {
    return k;
}

double Linear_regresser::get_coeff_std_dev(matrix::size_type i) const {
    return coeff_std_dev[i];
}

matrix::vector<double> Linear_regresser::get_coeff_std_dev() const {
    return coeff_std_dev;
}

double Linear_regresser::get_coeff_t_stat(matrix::size_type i) const {
    return coeff[i][0] / coeff_std_dev[i];
}

matrix::vector<double> Linear_regresser::get_coeff_t_stat() const {
    matrix::vector<double> coeff_t_stat(coeff.row_count());
    for (matrix::size_type i = 0; i != coeff_t_stat.size(); ++i)
        coeff_t_stat[i] = get_coeff_t_stat(i);
    return coeff_t_stat;
}

double Linear_regresser::get_t_test(matrix::size_type i) const {
    return two_sided_t_test(n-k, get_coeff_t_stat(i));
}

matrix::vector<double> Linear_regresser::get_t_test() const {
    matrix::vector<double> t_test(coeff.row_count());
    for (matrix::size_type i = 0; i != t_test.size(); ++i)
        t_test[i] = get_t_test(i);
    return t_test;
}

interval Linear_regresser::get_95_conf_coeff(matrix::size_type i) const {
    return {coeff[i][0] - student_t_0_975(n - k) * coeff_std_dev[i]
        , coeff[i][0] + student_t_0_975(n - k) * coeff_std_dev[i]};
}

matrix::vector<interval> Linear_regresser::get_95_conf_coeff() const {
    matrix::vector<interval> conf_int(coeff.row_count());
    for (matrix::size_type i = 0; i != conf_int.size(); ++i)
        conf_int[i] = get_95_conf_coeff(i);
    return conf_int;
}

matrix Linear_regresser::predict(const matrix::vector<double>& obs) const {
    return predict(matrix{{obs}});
}

matrix Linear_regresser::predict(const matrix& X) const {
    return X*coeff;
}

interval Linear_regresser::get_95_conf_predict(const matrix::vector<double>& obs) const {
    const double Y_HAT = predict(obs)[0][0]; //Will return a 1x1 matrix.

    const auto OBS = matrix{{obs}};

    const auto DIF = student_t_0_975(n-k) * est_sd * std::sqrt((OBS * XtX_inv * OBS.t())[0][0] + 1);

    return { Y_HAT - DIF, Y_HAT + DIF};
}

matrix::vector<interval> Linear_regresser::get_95_conf_predict(const matrix& obs) const {
    matrix::vector<interval> intervals(obs.row_count());
    for (matrix::size_type i = 0; i != intervals.size(); ++i)
        intervals[i] = get_95_conf_predict(obs[i]);
    return intervals;
}


void Linear_regresser::set_coeff_std_dev() {
    matrix::vector<double> vec(coeff.row_count());
    for (matrix::size_type i = 0; i != vec.size(); ++i)
        vec[i] = est_sd * std::sqrt(XtX_inv[i][i]);
    coeff_std_dev = vec;
}