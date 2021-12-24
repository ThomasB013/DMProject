
Helper.h contains the interface for matrix<double> operations.

The pattern that is worked out for operator+ and others is that these operators require both matrices to have the same size;
Hence if one of the matrices is an R-value reference we can 'steal' the memory frame and this saves us from deallocating and reallocating.

I will not return the references to the objects that are being modified. I do find the following syntax good:
D.row_add(1, -1, 0).row_add(2, -3, 0); (Read: row1 += -1 * row0, row2 += -3 * row0) But since we are not working with classes I can't do that. The equivalent of that is then:
row_add(row_add(D, 1, -1, 0), 2, -3, 0); which is a lot less pretty than just doing two seperate lines. 


