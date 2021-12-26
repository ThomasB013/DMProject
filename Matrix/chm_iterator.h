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
Chm_iterator<T>& Chm_iterator<T>::operator+=(difference_type n){
	p += n;
	return *this;
}

template<typename T>
Chm_iterator<T>& Chm_iterator<T>::operator-=(difference_type n) {
	p -= n;
	return *this;
}

template<typename T>
typename Chm_iterator<T>::difference_type Chm_iterator<T>::operator-(const Chm_iterator<T>& it) const {
	return p - it.p;
}

template<typename T>
Chm_iterator<T> Chm_iterator<T>::operator+(difference_type n) const {
	return Chm_iterator(p + n);
}

template<typename T>
Chm_iterator<T> Chm_iterator<T>::operator-(difference_type n) const {
	return Chm_iterator(p - n);
}

//*****************************
//      SAME BUT FOR READ ONLY:
//*****************************

template<typename T>
class const_Chm_iterator  {
public:
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	const_Chm_iterator() = delete;
	const_Chm_iterator(pointer p_) : p(p_) {}
	
	const_Chm_iterator& operator++();
	const_Chm_iterator operator++(int);
	const_Chm_iterator& operator--();
	const_Chm_iterator operator--(int);

	const_reference operator*() const;
	const_reference operator[](difference_type i) const; 
	const_pointer operator->() const;

	bool operator==(const const_Chm_iterator& it) const;
	bool operator!=(const const_Chm_iterator& it) const;
	bool operator<(const const_Chm_iterator& it) const;
	bool operator<=(const const_Chm_iterator& it) const;
	bool operator>(const const_Chm_iterator& it) const;
	bool operator>=(const const_Chm_iterator& it) const;

	const_Chm_iterator& operator+=(difference_type n);
	const_Chm_iterator& operator-=(difference_type n);
	difference_type operator-(const const_Chm_iterator& it) const;
	const_Chm_iterator operator+(difference_type n) const; 
	const_Chm_iterator operator-(difference_type n) const;
private:
	const_pointer p;
};

template<typename T>
const_Chm_iterator<T>& const_Chm_iterator<T>::operator++() {
	p++;
	return *this;
}

template<typename T>
const_Chm_iterator<T> const_Chm_iterator<T>::operator++(int) {
	const const_Chm_iterator temp(*this);
	p++;
	return temp;
}

template<typename T>
const_Chm_iterator<T>& const_Chm_iterator<T>::operator--() {
	p--;
	return *this;
}

template<typename T>
const_Chm_iterator<T> const_Chm_iterator<T>::operator--(int) {
	const const_Chm_iterator temp(*this);
	p--;
	return temp;
}

template<typename T>
typename const_Chm_iterator<T>::const_reference const_Chm_iterator<T>::operator*() const {
	return *p;
}

template<typename T>
typename const_Chm_iterator<T>::const_reference const_Chm_iterator<T>::operator[](difference_type i) const {
	return *(p + i);
}

template<typename T>
typename const_Chm_iterator<T>::const_pointer const_Chm_iterator<T>::operator->() const {
	return p;
}

template<typename T>
bool const_Chm_iterator<T>::operator==(const const_Chm_iterator<T>& it) const {
	return p == it.p;
}

template<typename T>
bool const_Chm_iterator<T>::operator!=(const const_Chm_iterator<T>& it) const {
	return p != it.p;
}

template<typename T>
bool const_Chm_iterator<T>::operator<(const const_Chm_iterator<T>& it) const {
	return p < it.p;
}

template<typename T>
bool const_Chm_iterator<T>::operator<=(const const_Chm_iterator<T>& it) const {
	return p <= it.p;
}

template<typename T>
bool const_Chm_iterator<T>::operator>(const const_Chm_iterator<T>& it) const {
	return p > it.p;
}

template<typename T>
bool const_Chm_iterator<T>::operator>=(const const_Chm_iterator<T>& it) const {
	return p >= it.p;
}

template<typename T>
const_Chm_iterator<T>& const_Chm_iterator<T>::operator+=(difference_type n){
	p += n;
	return *this;
}

template<typename T>
const_Chm_iterator<T>& const_Chm_iterator<T>::operator-=(difference_type n) {
	p -= n;
	return *this;
}

template<typename T>
typename const_Chm_iterator<T>::difference_type const_Chm_iterator<T>::operator-(const const_Chm_iterator<T>& it) const {
	return p - it.p;
}

template<typename T>
const_Chm_iterator<T> const_Chm_iterator<T>::operator+(difference_type n) const {
	return const_Chm_iterator(p + n);
}

template<typename T>
const_Chm_iterator<T> const_Chm_iterator<T>::operator-(difference_type n) const {
	return const_Chm_iterator(p - n);
}