#include "helper.h"

#include <utility>

matrix operator+(const matrix& A, const matrix& B) {
    matrix copy {B};
    return operator+(A, std::move(copy));
}

matrix operator+(matrix&& A, const matrix& B) {
    return operator+(B, std::move(A));;
}

matrix operator+(const matrix& A, matrix&& B) {
    if (A.row_count() != B.row_count() || A.col_count() != B.col_count())
        throw non_matching_col_row_dim {};
    for (matrix::size_type i = 0; i != A.row_count(); ++i)
        for(matrix::size_type j = 0; j != A.col_count(); ++j)
            B[i][j] += A[i][j];
    return B;
}

matrix operator-(const matrix& A, const matrix& B) {
    matrix copy {B};
    return operator-(A, std::move(copy));
}

matrix operator-(matrix&& A, const matrix& B) {
    return operator-(B, std::move(A));
}

matrix operator-(const matrix& A, matrix&& B) {
    if (A.row_count() != B.row_count() || A.col_count() != B.col_count())
        throw non_matching_col_row_dim {};
    for (matrix::size_type i = 0; i != A.row_count(); ++i)
        for(matrix::size_type j = 0; j != A.col_count(); ++j)
            B[i][j] -= A[i][j];
    return B;
}

matrix operator*(double v, const matrix& B){
    matrix copy {B};
    return operator*(v, std::move(copy));
}

matrix operator*(double v, matrix&& B) {
    for (auto& row : B)
        for (double& a : row)
            a *= v;
    return B;
}

matrix operator/(const matrix& B, double v) {
    matrix copy {B};
    return operator/(std::move(copy), v);
}

matrix operator/(matrix&& B, double v) {
    for (auto& row : B)
        for (double& a : row)
            a /= v;
    return B;
}

matrix operator*(const matrix& A, const matrix& B) {
    if (A.col_count() != B.row_count())
        throw non_matching_dim {};
    matrix ans {A.row_count(), B.col_count(), 0};
    for(matrix::size_type i = 0; i != ans.row_count(); ++i)
        for(matrix::size_type j = 0; j != ans.col_count(); ++j)
            for (matrix::size_type k = 0; k != B.row_count(); ++k)
                ans[i][j] += A[i][k] * B[k][j];
    return ans;
}