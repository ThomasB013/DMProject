#include "../Matrix/matrix.h"

using matrix = Matrix<double>;

matrix operator+(const matrix& A, const matrix& B) /*throws(non_matching_col_row_dim)*/;
matrix operator+(matrix&& A, const matrix& B) /*throws(non_matching_col_row_dim)*/;
matrix operator+(const matrix& A, matrix&& B) /*throws(non_matching_col_row_dim)*/;

matrix operator-(const matrix& A, const matrix& B) /*throws(non_matching_col_row_dim)*/;
matrix operator-(matrix&& A, const matrix& B) /*throws(non_matching_col_row_dim)*/;
matrix operator-(const matrix& A, matrix&& B) /*throws(non_matching_col_row_dim)*/;

matrix operator*(double v, const matrix& B);
matrix operator*(double v, matrix&& B);

matrix operator/(const matrix& B, double v);
matrix operator/(matrix&& B, double v);

matrix operator*(const matrix& A, const matrix& B) /*throws(non_matching_dim)*/;