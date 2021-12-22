This file keeps track of design choices.

I keep everything in a .h because of linking issues. The only thing I found so far is doing tricks as Matrix<int> in a header file if you need matrix, but I don't like this.

Regarding Matrix.h
1. A matrix is a collection of rows. (Or rows of rows.)

Regarding chm_iterator.h:
1. CHM stands for consecutive homogeneous memory. This allows for a single iterator which I implemented here. 
2. For operations I looked at https://www.cplusplus.com/reference/iterator/RandomAccessIterator/. I will not define friend Chm_iterator operator+(difference_type n, const Chm_iterator& it), as it does not add any functionality.
Furthermore p + 4 should in my opinion be viewed as 'starting address -> amount of steps', and not the other way around (so definining this will only allow for bad readability).
3. std::iterator<std::random_access_iterator_tag, T> does not allow me to use pointer, etc in the class definition. So I ommitted it.
4. For reference operator[](difference_type i) I chose difference_type rather than size_type to allow [-4] etc.

Regarding my_vec.h:
1. This is an imitatino of the real vector. (Of course I know that the real vector is better than mine)
2. It is mainly for exercise. 
3. All size changing functionalities (push_back, operator=, pop_back) are protected. This will allow matrix (which has my_vec<my_vec<T>>) to make a my_vec<T>& operator[](size_type i)
without worring about the user changing the size of single rows.

Regarding mem_base.h:
1. This is based on page 397 (section 13.6.2) of C++ Programing by Bjarne Stroustrup.
2. We know hold the last element and total for each row in our matrix. This is double.
I still keep this because it allows for easier function and smaller scope, hence the code is easier.
3. In the = operator for R-value reference we assume that the destructor will get called, hence we don't need to call the destructor on other.