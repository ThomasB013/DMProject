#pragma once
#include <iterator>

template<typename T>
class Chm_iterator  {
public:
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	Chm_iterator() = delete;
	Chm_iterator(pointer p_) : p(p_) {}
	
	Chm_iterator& operator++();
	Chm_iterator operator++(int);
	Chm_iterator& operator--();
	Chm_iterator operator--(int);

	reference operator*();
	reference operator[](difference_type i); 
	pointer operator->();

	bool operator==(const Chm_iterator& it) const;
	bool operator!=(const Chm_iterator& it) const;
	bool operator<(const Chm_iterator& it) const;
	bool operator<=(const Chm_iterator& it) const;
	bool operator>(const Chm_iterator& it) const;
	bool operator>=(const Chm_iterator& it) const;

	Chm_iterator& operator+=(difference_type n);
	Chm_iterator& operator-=(difference_type n);
	difference_type operator-(const Chm_iterator& it) const;
	Chm_iterator operator+(difference_type n) const; 
	Chm_iterator operator-(difference_type n) const;
private:
	pointer p;
};

template<typename T>
Chm_iterator<T>& Chm_iterator<T>::operator++() {
	p++;
	return *this;
}

template<typename T>
Chm_iterator<T> Chm_iterator<T>::operator++(int) {
	const Chm_iterator temp(*this);
	p++;
	return temp;
}

template<typename T>
Chm_iterator<T>& Chm_iterator<T>::operator--() {
	p--;
	return *this;
}

template<typename T>
Chm_iterator<T> Chm_iterator<T>::operator--(int) {
	const Chm_iterator temp(*this);
	p--;
	return temp;
}

template<typename T>
typename Chm_iterator<T>::reference Chm_iterator<T>::operator*() {
	return *p;
}

template<typename T>
typename Chm_iterator<T>::reference Chm_iterator<T>::operator[](difference_type i) {
	return *(p + i);
}

template<typename T>
typename Chm_iterator<T>::pointer Chm_iterator<T>::operator->() {
	return p;
}

template<typename T>
bool Chm_iterator<T>::operator==(const Chm_iterator<T>& it) const {
	return p == it.p;
}

template<typename T>
bool Chm_iterator<T>::operator!=(const Chm_iterator<T>& it) const {
	return p != it.p;
}

template<typename T>
bool Chm_iterator<T>::operator<(const Chm_iterator<T>& it) const {
	return p < it.p;
}

template<typename T>
bool Chm_iterator<T>::operator<=(const Chm_iterator<T>& it) const {
	return p <= it.p;
}

template<typename T>
bool Chm_iterator<T>::operator>(const Chm_iterator<T>& it) const {
	return p > it.p;
}

template<typename T>
bool Chm_iterator<T>::operator>=(const Chm_iterator<T>& it) const {
	return p >= it.p;
}

template<typename T>
Chm_iterator<T>& Chm_iterator<T>::operator+=(typename Chm_iterator<T>::difference_type n){
	p += n;
	return *this;
}

template<typename T>
Chm_iterator<T>& Chm_iterator<T>::operator-=(typename Chm_iterator<T>::difference_type n) {
	p -= n;
	return *this;
}

template<typename T>
typename Chm_iterator<T>::difference_type Chm_iterator<T>::operator-(const Chm_iterator<T>& it) const {
	return p - it.p;
}

template<typename T>
typename Chm_iterator<T>::Chm_iterator Chm_iterator<T>::operator+(typename Chm_iterator<T>::difference_type n) const {
	return Chm_iterator(p + n);
}

template<typename T>
Chm_iterator<T> Chm_iterator<T>::operator-(typename Chm_iterator<T>::difference_type n) const {
	return Chm_iterator(p - n);
}