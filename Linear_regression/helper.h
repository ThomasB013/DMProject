#include "../Matrix/matrix.h"

using matrix = Matrix<double>;

struct non_invertible : std::exception {};

const double EPS = 1e-09;

bool iin(double v, double n, double e); //Is in neighbourhood.

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

//For gaussian elimination.
void row_add(matrix& A, matrix::size_type r1, double c, matrix::size_type r2); //r1 += c * r2.
void row_swap(matrix& A, matrix::size_type r1, matrix::size_type r2);

matrix get_rref(const matrix& A);
void to_rref(matrix& A);

matrix inv(const matrix& A) /*throws(non_invertible)*/;

