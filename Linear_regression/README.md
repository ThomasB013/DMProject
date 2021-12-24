
Helper.h contains the interface for matrix<double> operations.

The pattern that is worked out for operator+ and others is that these operators require both matrices to have the same size;
Hence if one of the matrices is an R-value reference we can 'steal' the memory frame and this saves us from deallocating and reallocating.