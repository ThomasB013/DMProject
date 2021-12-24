This file keeps track of design choices.

I keep everything in a .h because of linking issues. The only thing I found so far is doing tricks as Matrix/int/ in a header file if you need matrix for integers, but I don't like this.

Regarding Matrix.h
1. A matrix is a collection of rows. (Or rows of rows.)
2. C++17 does not allow for throw declarations. I keep them in comments because I find it a useful addition to the interface.
3. Make Matrix all public (struct), because 1. Matrix[3] would return the row anyways, thus allowing the users to heavily interfere. 2. The only real constraint is that a matrix should be rectangular at all times. 
4. To make up for the loss of guaranty, define throwing alternatives for each function.

Regarding chm_iterator.h:
1. CHM stands for consecutive homogeneous memory. This allows for a single iterator which I implemented here. 
2. For operations I looked at https://www.cplusplus.com/reference/iterator/RandomAccessIterator/. I will not define friend Chm_iterator operator+(difference_type n, const Chm_iterator& it), as it does not add any functionality.
Furthermore p + 4 should in my opinion be viewed as 'starting address -> amount of steps', and not the other way around (so definining this will only allow for bad readability).
3. std::iterator<std::random_access_iterator_tag, T> does not allow me to use pointer, etc in the class definition. So I ommitted it.
4. For reference operator[](difference_type i) I chose difference_type rather than size_type to allow [-4] etc.

Regarding my_vec.h:
1. This is an imitation of the real vector (yes I know that the real vector is better than mine).
2. It is mainly for exercise. 
3. A problem for Matrix is that Matrix[3] will return a vector, representing the row. But this allows the user to do things as Matrix[3].push_back("I am messing with the squaredness");
4. DEFAULT_SIZE and GROWING_RATIO are constants that determine initializing and push_back.
5. I think some functions could have a template for their allocator. For example to compare elements we don't need the allocators to be of the same type.
I don't implement this for the same reason I don't use namespaces: this is a try-out project more than a try-to-be-complete project.

Regarding mem_base.h:
1. This is based on page 397 (section 13.6.2) of C++ Programing by Bjarne Stroustrup.
2. We hold the last element and total for each row in our matrix. This is double as each row has the same size and fill.
I still keep this because it allows for easier function and smaller scope, hence the code is easier.
3. In the = operator for R-value reference we assume that the destructor will get called, hence we don't need to call the destructor on other. 
The same goes for My_vec. If someone is smart enough to call std::move, then that person should also be smart enough to call the destructor, which is public, if 
they wanted to have the memory freed.
