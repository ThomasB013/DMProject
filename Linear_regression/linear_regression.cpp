#include "linear_regression.h"

Linear_regresser::Linear_regresser(const matrix& y_data, const matrix& X_data) : X{X_data}, y{y_data} {
    X_data.assert_rect("X_data needs to be rectangular.");
    y_data.assert_rect("y_data needs to be rectangular.");
    if (y_data.col_count() != 1)
        throw bad_dimension {"y_data needs to have dimension n x 1."};
    if (y_data.row_count() != X_data.row_count())
        throw bad_dimension {"y_data and X_data need to have the same amount of observations."};
}

const matrix& Linear_regresser::fit() {
    coeff = inv(X.t()*X)*X.t()*y;
    return coeff;
}

const matrix& Linear_regresser::get_coeff() const {
    return coeff;
}

matrix Linear_regresser::predict(const matrix& X) const {
    return X*coeff;
}