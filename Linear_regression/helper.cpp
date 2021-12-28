#include "helper.h"

#include <utility>
#include <limits>
#include <math.h>

const double PI = std::atan(1)*4;

bool iin(double v, double n, double e) { 
    return (n - e < v && v < n + e);
}

bool is_one(double v) {
    return iin(v, 1, EPS);
}

bool is_zero(double v){
    return iin(v, 0, EPS);
}

matrix operator+(const matrix& A, const matrix& B) {
    matrix copy {B};
    return operator+(A, std::move(copy));
}

matrix operator+(matrix&& A, const matrix& B) {
    return operator+(B, std::move(A));;
}

matrix operator+(const matrix& A, matrix&& B) {
    if (A.row_count() != B.row_count() || A.col_count() != B.col_count())
        throw non_matching_col_row_dim {"To execute A+B matrix A and B need to have matching dimensions."};
    for (matrix::size_type i = 0; i != A.row_count(); ++i)
        for(matrix::size_type j = 0; j != A.col_count(); ++j)
            B[i][j] += A[i][j];
    return B;
}

matrix operator+(const matrix& A, double b) {
    auto copy {A};
    for(auto& row : copy)
        for (double& x : row)
            x += b;
    return copy;
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
        throw non_matching_col_row_dim {"To execute A-B matrix A and B need to have matching dimensions."};
    for (matrix::size_type i = 0; i != A.row_count(); ++i)
        for(matrix::size_type j = 0; j != A.col_count(); ++j)
            B[i][j] -= A[i][j];
    return B;
}

matrix operator-(const matrix& A, double b) {
    auto copy {A};
    for(auto& row : copy)
        for (double& x : row)
            x -= b;
    return copy;
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
        throw non_matching_dim {"To execute A*B matrix A's column count needs to match matrix B's row count."};
    matrix ans {A.row_count(), B.col_count(), 0};
    for(matrix::size_type i = 0; i != ans.row_count(); ++i)
        for(matrix::size_type j = 0; j != ans.col_count(); ++j)
            for (matrix::size_type k = 0; k != B.row_count(); ++k)
                ans[i][j] += A[i][k] * B[k][j];
    return ans;
}

matrix identity(matrix::size_type n) {
    matrix ans(n, n, 0);
    for (matrix::size_type i = 0; i != n; ++i)
        ans[i][i] = 1;
    return ans;
}

void row_add(matrix& A, matrix::size_type r1, double c, matrix::size_type r2) {
    for (matrix::size_type j = 0; j != A.col_count(); ++j)
        A[r1][j] += c * A[r2][j];
}

void row_multiply(matrix& A, matrix::size_type r, double c) {
    for (auto& val : A[r])
        val *= c;
}

void row_swap(matrix& A, matrix::size_type r1, matrix::size_type r2) {
    std::swap(A[r1], A[r2]);
}

matrix get_rref(const matrix& A) {
    matrix copy {A};
    to_rref(copy); 
    return copy;
}

matrix::size_type find_non_zero(matrix& A, matrix::size_type start, matrix::size_type col) {
    while (start != A.row_count() && is_zero(A[start][col]))
        ++start;
    return start;
}

void to_rref(matrix& A) {
    using st = matrix::size_type;
    st pi = 0, pj = 0; //Pivot i, j

    while(pi != A.row_count() && pj != A.col_count()){
        st nz = find_non_zero(A, pi, pj);
        if (nz != A.row_count()){ //We have a nonzero.
            if (nz != pi)
                row_swap(A, nz, pi);
            row_multiply(A, pi, 1.0/A[pi][pj]);
            A[pi][pj] = 1; //Avoid rounding errors around pivots.
            for (st i = 0; i != A.row_count(); ++i)
                if (i != nz)
                    row_add(A, i, -A[i][pj], pi);
            ++pi;
        }
        ++pj;
    }

    for (auto& row : A)
        for (auto& v : row)
            v = is_zero(v) ? 0 : v; //Avoid rounding errors for zeroes.
}

matrix inv(const matrix& A) {/*throws(non_invertible, non_square)*/
    using st = matrix::size_type;
    A.assert_square("Only a square matrix can be inverted.");

    const st N = A.row_count();
    matrix A_I(N, 2 * N, 0);
    for (st i = 0; i != N; ++i)
        for (st j = 0; j != N; ++j){
            A_I[i][j] = A[i][j];
            if (i == j)
                A_I[i][N + j] = 1;
        }
    to_rref(A_I);
    if (!is_one(A_I[N-1][N-1])) //Safely assume N >= 1.
        throw non_invertible {"Inv() failed because the matrix was not full rank."};

    matrix::vector<st> v(A.col_count());
    for (st i = 0; i != N; ++i)
        v[i] = N + i;
    return A_I.col_select(v);
}

matrix::vector<double> col_means(const matrix& A) {
    matrix::vector<double> means(A.col_count(), 0);
    for (matrix::size_type j = 0; j != A.col_count(); ++j){
        for (matrix::size_type i = 0; i != A.row_count(); ++i)
            means[j] += A[i][j];
        means[j] /= A.row_count();
    }
    return means;
}

matrix::vector<double> col_var(const matrix& A, const matrix::vector<double>& col_means) {
    matrix::vector<double> vars(A.col_count(), 0);
    for (matrix::size_type j = 0; j != A.col_count(); ++j){
        for (matrix::size_type i = 0; i != A.row_count(); ++i)
            vars[j] += (A[i][j] - col_means[j]) * (A[i][j] - col_means[j]);
        vars[j] /= (A.row_count() - 1);
    }
    return vars;
}

matrix::vector<double> col_std_dev(const matrix& A, const matrix::vector<double>& col_means) { 
    auto vars = col_var(A, col_means);
    for (double& d : vars)
        d = std::sqrt(d);
    return vars;
}

matrix::vector<double> col_mins(const matrix& A) {
    matrix::vector<double> mins(A.col_count(), std::numeric_limits<double>::max());
    for (matrix::size_type j = 0; j != A.col_count(); ++j)
        for (matrix::size_type i = 0; i != A.row_count(); ++i)
            mins[j] = std::min(mins[j], A[i][j]);
    return mins;
}

matrix::vector<double> col_max(const matrix& A) {
    matrix::vector<double> max(A.col_count(), std::numeric_limits<double>::min());
    for (matrix::size_type j = 0; j != A.col_count(); ++j)
        for (matrix::size_type i = 0; i != A.row_count(); ++i)
            max[j] = std::max(max[j], A[i][j]);
    return max;
}

double sum_squared(const matrix& a) {
    if (a.col_count() != 1)
        throw bad_dimension {"For sum_squared the input need to have dimension n x 1."}; 
    
    double sum = 0;
    for(const auto& r : a)
        sum += r[0] * r[0]; 

    return sum;
}

double sum_diff_squared(const matrix& a, const matrix& b) {
    if (a.col_count() != 1 || b.col_count() != 1)
        throw bad_dimension {"For diff_squared both input matrices need to have dimension n x 1."};
    if (a.row_count() != b.row_count())
        throw non_matching_row_dim {"For diff_squared both input matrices need to have the same number of observations."};

    return sum_squared(a - b);
}


double integral(std::function<double(double)> f, double a, double b, int n) {
    double dx = (b - a) / n;
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        double height = f(a + i*dx + dx/2);
        sum += height * dx;
    } 
    return sum;
}

double first_part_student_t(int freedom){
    return std::tgamma((freedom + 1.0)/2.0) / (std::sqrt(PI * freedom) * std::tgamma(freedom/2.0));
}

double second_part_student_t(int freedom, double x){
    return std::pow(1 + x*x/freedom, -(freedom+1.0)/2.0);
}

double student_t_pdf(int freedom, double x) {
    return first_part_student_t(freedom) * second_part_student_t(freedom, x);
}

double student_t_cdf(int freedom, double a, double b) {
    const double FIRST = first_part_student_t(freedom);
    
    const double INT = integral([=](double x) -> double {
        return second_part_student_t(freedom, x);
    }, a, b);
       
    return INT * FIRST;
}

double two_sided_t_test(int freedom, double t){
    return 1 - 2 * student_t_cdf(freedom, 0, std::abs(t));
}

double student_t_0_975(int freedom) {
    const double VALUES_F_30[] {
        12.71, 4.303, 3.182, 2.776, 2.571, 2.447, 2.365, 2.306, 2.262, 2.228, 
        2.201, 2.179, 2.16, 2.145, 2.131, 2.12, 2.11, 2.101, 2.093, 2.086, 
        2.08, 2.074, 2.069, 2.064, 2.06, 2.056, 2.052, 2.048, 2.045, 2.042};
    const double VALUES_F_40_60_80_100[] {2.021, 2, 1.99, 1.984};
    const double F_1000 = 1.962;
    const double Z =  1.96;
    if (freedom <= 30)
        return VALUES_F_30[freedom - 1];
    if (freedom <= 40) {//between 30 en 40.
        const double F_30 = VALUES_F_30[29];
        const double F_40 = VALUES_F_40_60_80_100[0];
        const double alpha = (freedom - 30) / 10.0;
        return F_30 * (1 - alpha) + F_40 * alpha;
    }
    if (freedom < 100) {
        const int L = (freedom-40)/20, R = (freedom-40)/20 + 1;
        const double F_L = VALUES_F_40_60_80_100[L];
        const double F_R = VALUES_F_40_60_80_100[R];
        const double alpha = (freedom % 20) / 20.0;
        return F_L * (1 - alpha) + F_R * alpha;
    }
    if (freedom <= 1000) {//between 30 en 40.
        const double F_100 = VALUES_F_40_60_80_100[3];
        const double alpha = (freedom - 100) / 900.0;
        return F_100 * (1 - alpha) + F_1000 * alpha;
    }
    return Z;
}