#ifndef __STL_VECTOR_H
#define __STL_VECTOR_H

// #include <initializer_list>
#include <cassert>  // for assert

#include "stl_allocator.h"
#include "stl_config.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"

__STL_BEGIN_NAMESPACE

template <class _T>
class vector {
  static_cast(!std::is_same<bool, _T>::value, "vector<bool> is not supported");

 public:
  typedef toystl::allocator<_T> allocator_type;
  typedef toystl::allocator<_T> data_allocator;

  typedef typename allocator_type::value_type value_type;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type size_type;
  typedef typename allocator_type::difference_type difference_type;

  typedef value_type *iterator;
  typedef const value_type *const_iterator;
  typedef toystl::reverse_iterator<iterator> reverse_iterator;
  typedef toystl::reverse_iterator<const_iterator> const_reverse_iterator;

 private:
  iterator __start;
  iterator __finish;
  iterator __end_of_storage;

 public:
  iterator begin() { return __start; }
  const_iterator begin() const { return __start; }
  iterator end() { return __finish; }
  const_iterator end() const { return __finish; }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  size_type size() const { return static_cast<size_type>(end() - begin()); }
  size_type capacity() const {
    return static_cast<size_type>(__end_of_storage - begin());
  }
  bool empty() const { return begin() == end(); }

  reference operator[](size_type __n) {
    assert(__n < size());
    return *(begin() + __n);
  }
  const_reference operator[](size_type __n) const {
    assert(__n < size());
    return *(begin() + __n);
  }

  vector() noexcept { init_16(); }

  explicit vector(size_type __n) { fill_init(__n, value_type()); }

  vector(size_type __n, const value_type &__value) { fill_init(__n, __value); }

  vector(vector &&__rhs) noexcept
      : __start(__rhs.__start),
        __finish(__rhs.__finish),
        __end_of_storage(__rhs.__end_of_storage) {
    __rhs.__start = nullptr;
    __rhs.__finish = nullptr;
    __rhs.__end_of_storage = nullptr;
  }

  vector(std::initializer_list<value_type> __ilist) {
    init_space(__ilist.size(), __ilist.size());
    toystl::uninitialized_copy(__ilist.begin(), __ilist.end(), __start);
  }

  ~vector() {
    destroy_and_recover(__start, __finish, __end_of_storage - __start);
    __start = __finish = __end_of_storage = nullptr;
  }

  reference at(size_type __n) {
    if (__n >= size()) throw std::out_of_range("vector");
    return (*this)[__n];
  }
  const_reference at(size_type __n) const {
    if (__n >= size()) throw std::out_of_range("vector");
    return (*this)[__n];
  }

  reference front() {
    assert(!empty());
    return *begin();
  }

  const_reference front() const {
    assert(!empty());
    return *begin();
  }

  reference back() {
    assert(!empty());
    return *(end() - 1);
  }

  const_reference back() const {
    assert(!empty());
    return *(end() - 1);
  }

  void assign(size_type __n, const value_type &__value) {
    if (__n > capacity()) {
      vector tmp(__n, __value);
      swap(tmp);
    } else if (__n > size()) {
      toydtl::fill(begin(), end(), __value);
      __finish = toystl::uninitialized_fill_n(__finish, __n - size(), __value);
    } else {
      // erase(toystl::fill_n(begin(), __n, __value), end());
    }
  }

 private:
  // helper function

  // initialize and destroy
  void init_16() noexcept;
  void init_space(size_type __n, size_type __cap);
  void fill_init(size_type __n, const value_type &__value);
  void destroy_and_recover(iterator __first, iterator __last, size_type __n);
};

// helper function

template <class _T>
void vector<_T>::init_16() noexcept {
  try {
    __start = data_allocator::allocate(16);
    __finish = __start;
    __end_of_storage = __start + 16;
  } catch (...) {
    __start = nullptr;
    __finish = nullptr;
    __end_of_storage = nullptr;
  }
}

template <class _T>
void vector<_T>::init_space(size_type __n, size_type __cap) {
  try {
    __start = data_allocator::allocate(__cap);
    __finish = __start + __n;
    __end_of_storage = __start + __cap;
  } catch (...) {
    __start = nullptr;
    __finish = nullptr;
    __end_of_storage = nullptr;
    throw;
  }
}

template <class _T>
void vector<_T>::fill_init(size_type __n, const value_type &__value) {
  const size_type init_size = toystl::max(static_cast<size_type>(16), __n);
  init_space(__n, init_size);
  toystl::uninitialized_fill_n(__start, __n, __value);
}

template <class _T>
void vector<_T>::destroy_and_recover(iterator __first, iterator __last,
                                     size_type __n) {
  data_allocator::destroy(__first, __last);
  data_allocator::deallocate(__first, __n);
}

__STL_END_NAMESPACE

#endif