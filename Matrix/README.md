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
