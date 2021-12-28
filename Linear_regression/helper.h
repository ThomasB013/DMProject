#pragma once

#include "../Matrix/matrix.h"
#include <functional>

using matrix = Matrix<double>;

struct non_invertible : std::runtime_error { non_invertible(const char* msg) : std::runtime_error(msg) {}};

const double EPS = 1e-09;

bool iin(double v, double n, double e); //Is in neighbourhood.
bool is_one(double v); //With precision = EPS.
bool is_zero(double v); //With precision = EPS.

matrix operator+(const matrix& A, const matrix& B);
matrix operator+(matrix&& A, const matrix& B);
matrix operator+(const matrix& A, matrix&& B);

matrix operator+(const matrix& A, double b);

matrix operator-(const matrix& A, const matrix& B);
matrix operator-(matrix&& A, const matrix& B);
matrix operator-(const matrix& A, matrix&& B);

matrix operator-(const matrix& A, double b);

matrix operator*(double v, const matrix& B);
matrix operator*(double v, matrix&& B);

matrix operator/(const matrix& B, double v);
matrix operator/(matrix&& B, double v);

matrix operator*(const matrix& A, const matrix& B);

matrix identity(matrix::size_type n);

//For gaussian elimination.
void row_add(matrix& A, matrix::size_type r1, double c, matrix::size_type r2); //r1 += c * r2.
void row_multiply(matrix& A, matrix::size_type r, double c);
void row_swap(matrix& A, matrix::size_type r1, matrix::size_type r2);

matrix get_rref(const matrix& A); //Reduced Row Echelon Form
void to_rref(matrix& A);

matrix inv(const matrix& A);

matrix::vector<double> col_means(const matrix& A);
matrix::vector<double> col_var(const matrix& A, const matrix::vector<double>& col_means);
matrix::vector<double> col_std_dev(const matrix& A, const matrix::vector<double>& col_means);
matrix::vector<double> col_mins(const matrix& A);
matrix::vector<double> col_max(const matrix& A);

double sum_squared(const matrix& a);
double sum_diff_squared(const matrix& a, const matrix& b);

double integral(std::function<double(double)> f, double a, double b, int n =1000); //integral by n rectangles.

double first_part_student_t(int freedom); //Independent of x, can speed up integrals over x.
double second_part_student_t(int freedom, double x);
double student_t_pdf(int freedom, double x); 
double student_t_cdf(int freedom, double a, double b);
double two_sided_t_test(int freedom, double t); //calculates the change to have even extermer values than |t|. 
double student_t_0_975(int freedom); //Choses a close value from https://www.sjsu.edu/faculty/gerstman/StatPrimer/t-table.pdf in the column t.975