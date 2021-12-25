#pragma once

#include "helper.h"

class Linear_regresser {
public:
    Linear_regresser(const matrix& y_data, const matrix& X_data) /*throws(bad_dimension, non_rect)*/;

    const matrix& fit();
    const matrix& get_coeff() const;
private:
    matrix coeff;
    const matrix& X;
    const matrix& y;
};
