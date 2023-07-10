#ifndef __STL_VECTOR_H
#define __STL_VECTOR_H

// #include <initializer_list>
#include <cassert>  // for assert
#include <stdexcept> // for out_of_range

#include "stl_allocator.h"
#include "stl_algobase.h"
#include "stl_algo.h"
#include "stl_config.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"

__STL_BEGIN_NAMESPACE

template <class _T>
class vector {
  static_assert(!std::is_same<bool, _T>::value, "vector<bool> is not supported");

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

  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef toystl::reverse_iterator<iterator> reverse_iterator;
  typedef toystl::reverse_iterator<const_iterator> const_reverse_iterator;

  allocator_type get_allocator() const { return data_allocator(); }

 private:
  iterator __start;
  iterator __finish;
  iterator __end_of_storage;

 public:
  iterator begin() noexcept { return __start; }
  const_iterator begin() const noexcept { return __start; }
  iterator end() noexcept { return __finish; }
  const_iterator end() const noexcept { return __finish; }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  size_type size() const { return static_cast<size_type>(end() - begin()); }
  size_type capacity() const {
    return static_cast<size_type>(__end_of_storage - begin());
  }
  size_type max_size() const { return static_cast<size_type>(-1) / sizeof(_T); }
  bool empty() const { return begin() == end(); }

  void shrink_to_fit();
  void reserve(size_type __n);

  reference operator[](size_type __n) {
    assert(__n < size());
    return *(begin() + __n);
  }
  const_reference operator[](size_type __n) const {
    assert(__n < size());
    return *(begin() + __n);
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

  pointer data() noexcept { return __start; }
  const_pointer data() const noexcept { return __start; }

  // assign

  void assign(size_type __n, const value_type& __value) {
    fill_assign(__n, __value);
  }

  template <class _InputIter,
            typename std::enable_if<
                toystl::is_input_iterator<_InputIter>::value, int>::type = 0>
  void assign(_InputIter __first, _InputIter __last) {
    assert(!(__last < __first));  // !(last < first) = last >= first
    copy_assign(__first, __last, toystl::iterator_category(__first));
  }

  void assign(std::initializer_list<value_type> __ilist) {
    copy_assign(__ilist.begin(), __ilist.end(), toystl::forward_iterator_tag());
  }

  // emplace // emplac_back

  template <class... Args>
  iterator emplace(const_iterator __pos, Args&&... __args);

  template <class... Args>
  void emplace_back(Args&&... __args);

  // push_bakc / pop_back

  void push_back(const value_type& __value);
  void push_back(value_type&& __value) { emplace_back(toystl::move(__value)); }

  void pop_back();

  // insert

  iterator insert(const_iterator __pos, const value_type& __value);
  iterator insert(const_iterator __pos, value_type&& __value) {
    return emplace(__pos, toystl::move(__value));
  }

  iterator insert(const_iterator __pos, size_type __n,
                  const value_type& __value) {
    assert(__pos >= begin() && __pos <= end());
    return fill_insert(const_cast<iterator>(__pos), __n, __value);
  }

  template <class _InputIter,
            typename std::enable_if<
                toystl::is_input_iterator<_InputIter>::value, int>::type = 0>
  void insert(const_iterator __pos, _InputIter __first, _InputIter __last) {
    assert(__pos >= begin() && __pos <= end() && !(__last < __first));
    copy_insert(const_cast<iterator>(__pos), __first, __last);
  }

  // earse / clear
  iterator erase(const_iterator __pos);
  iterator erase(const_iterator __first, const_iterator __last);

  void clear() { erase(begin(), end()); }

  // resize / reverse
  void resize(size_type __new_size) { return resize(__new_size, value_type()); }
  void resize(size_type __new_size, const value_type& __value);

  void reverse() { toystl::reverse(begin(), end()); }

  // swap
  void swap(vector& __rhs) noexcept;

 public:
  vector() noexcept { init_16(); }

  explicit vector(size_type __n) { fill_init(__n, value_type()); }

  vector(size_type __n, const value_type& __value) { fill_init(__n, __value); }

  template <class _Iter, typename std::enable_if<
    toystl::is_input_iterator<_Iter>::value, int>::type = 0>
  vector(_Iter __first, _Iter __last) {
    assert(!(__last < __first));
    range_init(__first, __last);
  }

  vector(const vector& __rhs) { range_init(__rhs.begin(), __rhs.end()); }

  vector(vector&& __rhs) noexcept
      : __start(__rhs.__start),
        __finish(__rhs.__finish),
        __end_of_storage(__rhs.__end_of_storage) {
    __rhs.__start = nullptr;
    __rhs.__finish = nullptr;
    __rhs.__end_of_storage = nullptr;
  }

  vector(std::initializer_list<value_type> __ilist) {
    range_init(__ilist.begin(), __ilist.end());
  }

  ~vector() {
    destroy_and_recover(__start, __finish, __end_of_storage - __start);
    __start = __finish = __end_of_storage = nullptr;
  }

 public:
  vector& operator=(const vector& __rhs);
  vector& operator=(vector&& __rhs) noexcept;

  vector& operator=(std::initializer_list<value_type> __ilist) {
    vector tmp(__ilist.begin(), __ilist.end());
    swap(tmp);
    return *this;
  }

 private:
  // helper function

  // initialize and destroy
  void init_16() noexcept;
  void init_space(size_type __n, size_type __cap);
  void fill_init(size_type __n, const value_type& __value);
  template <class _Iter>
  void range_init(_Iter __first, _Iter __last);

  void destroy_and_recover(iterator __first, iterator __last, size_type __n);

  // assign

  void fill_assign(size_type __n, const value_type& __value);

  template <class _InputIter>
  void copy_assign(_InputIter __first, _InputIter __last,
                   toystl::input_iterator_tag);

  template <class _ForwardIter>
  void copy_assign(_ForwardIter __first, _ForwardIter __last,
                   toystl::forward_iterator_tag);

  // reallocate

  template <class... Args>
  void reallocate_emplace(iterator __pos, Args&&... args);
  void reallocate_insert(iterator __pos, const value_type& __value);

  // insert 
  
  iterator fill_insert(iterator __pos, size_type __n, const value_type& __value);
  template <class _InputIter>
  void     copy_insert(iterator __pos, _InputIter __first, _InputIter __last);

  // shrink_to_fit

  void reinsert(size_type size);
};

template <class _T>
vector<_T>& vector<_T>::operator=(const vector& __rhs) {
  if (this != &__rhs)
  {
    const auto len = __rhs.size();
    if (len > capacity())
    {
      vector tmp(__rhs.begin(), __rhs.end());
      swap(tmp);
    }
    else if (size() >= len)
    {
      auto i = toystl::copy(__rhs.begin(), __rhs.end(), begin());
      data_allocator::destroy(i, __finish);
      __finish = __start + len;
    }
    else 
    {
      toystl::copy(__rhs.begin(), __rhs.end() + size(), __start);
      toystl::uninitialized_copy(__rhs.begin() + size(), __rhs.end(), __finish);
      __end_of_storage = __finish = __start + len;
    }
  }
  return *this;
}

template <class _T>
vector<_T>& vector<_T>::operator=(vector&& __rhs) noexcept {
  destroy_and_recover(__start, __finish, __end_of_storage - __start);
  __start = __rhs.__start;
  __finish = __rhs.__finish;
  __end_of_storage = __rhs.__end_of_storage;
  __rhs.__start = nullptr;
  __rhs.__finish = nullptr;
  __rhs.__end_of_storage = nullptr;
  return *this;
}

template <class _T>
void vector<_T>::reserve(size_type __n)
{
  if (capacity() < __n)
  {
    const auto old_size = size();
    auto tmp = data_allocator::allocate(__n);
    toystl::uninitialized_move(__start, __finish, tmp);
    destroy_and_recover(__start, __finish, __end_of_storage - __start);
    __start = tmp;
    __finish = tmp + old_size;
    __end_of_storage = __start + __n;
  }
}

template <class _T>
void vector<_T>::resize(size_type __new_size, const value_type& __value)
{
  if (__new_size < size())
  {
    erase(begin() + __new_size, end());
  }
  else
  {
    insert(end(), __new_size - size(), __value);
  }
}

template <class _T>
void vector<_T>::shrink_to_fit()
{
  if (__finish < __end_of_storage)
  {
    reinsert(size());
  }
}

template <class _T>
template <class... Args>
typename vector<_T>::iterator vector<_T>::emplace(const_iterator __pos,
                                                  Args&&... __args) {
  assert(__pos >= begin() && __pos <= end());
  iterator xpos = const_cast<iterator>(__pos);
  const size_type n = xpos - __start;
  if (__finish != __end_of_storage && xpos == end()) {
    data_allocator::construct(toystl::address_of(*__finish),
                              toystl::forward<Args>(__args)...);
    ++__finish;
  } else if (__finish != __end_of_storage) {
    auto new_end = __finish;
    data_allocator::construct(toystl::address_of(*__finish), *(__finish - 1));
    ++new_end;
  toystl:
    copy_backward(xpos, __finish - 1, __end_of_storage);
    *xpos = value_type(toystl::forward<Args>(__args)...);
    __finish = new_end;
  } else {
    reallocate_emplace(xpos, toystl::forward<Args>(__args)...);
  }
  return begin() + n;
}

template <class _T>
template <class... Args>
void vector<_T>::emplace_back(Args&&... args) {
  if (__finish < __end_of_storage) {
    data_allocator::construct(toystl::address_of(*__finish),
                              toystl::forward<Args>(args)...);
    ++__finish;
  } else {
    reallocate_emplace(__finish, toystl::forward<Args>(args)...);
  }
}

template <class _T>
void vector<_T>::push_back(const value_type& __value) {
  if (__finish != __end_of_storage) {
    data_allocator::construct(toystl::address_of(*__finish), __value);
    ++__finish;
  } else {
    reallocate_emplace(__finish, __value);
  }
}

template <class _T>
void vector<_T>::pop_back() {
  assert(!empty());
  data_allocator::destroy(__finish - 1);
  --__finish;
}

template <class _T>
typename vector<_T>::iterator vector<_T>::insert(const_iterator __pos,
                                                 const value_type& __value) {
  assert(__pos >= begin() && __pos <= end());
  iterator xpos = const_cast<iterator>(__pos);
  const size_type n = __pos - __start;
  if (__finish != __end_of_storage && xpos == __finish) {
    data_allocator::construct(toystl::address_of(*__finish), __value);
    ++__finish;
  } else if (__finish != __end_of_storage) {
    auto new_finish = __finish;
    data_allocator::construct(toystl::address_of(*__finish), *(__finish - 1));
    ++new_finish;
    auto value_copy = __value;
    toystl::copy_backward(xpos, __finish - 1, __finish);
    *xpos = toystl::move(value_copy);
    __finish = new_finish;
  } else {
    reallocate_insert(xpos, __value);
  }
  return begin() + n;
}

template <class _T>
typename vector<_T>::iterator 
vector<_T>::erase(const_iterator __pos) {
  assert(__pos >= begin() && __pos <= end());
  iterator xpos = const_cast<iterator>(begin() + (__pos - begin()));
  toystl::move(xpos + 1, __finish, xpos);
  data_allocator::destroy(__finish - 1);
  --__finish;
  return xpos;
}

template <class _T>
typename vector<_T>::iterator 
vector<_T>::erase(const_iterator __first, const_iterator __last) {
  assert(__first >= begin() && __last <= end() && !(__last < __first));
  const auto n = __first - begin();
  iterator r = begin() + n;
  destroy_and_recover(r + (__last - __first), __finish, __last - __first);
  __finish = __finish - (__last - __first);
  return __start + n;
}

template <class _T>
void vector<_T>::swap(vector<_T>& __rhs) noexcept {
  if (this != &__rhs) {
    toystl::swap(__start, __rhs.__start);
    toystl::swap(__finish, __rhs.__finish);
    toystl::swap(__end_of_storage, __rhs.__end_of_storage);
  }
}
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
void vector<_T>::fill_init(size_type __n, const value_type& __value) {
  const size_type init_size = toystl::max(static_cast<size_type>(16), __n);
  init_space(__n, init_size);
  toystl::uninitialized_fill_n(__start, __n, __value);
}

template <class _T>
template <class _Iter>
void vector<_T>::range_init(_Iter __first, _Iter __last) {
  const size_type distance = toystl::distance(__first, __last);
  const size_type init_size = toystl::max(
      static_cast<size_type>(16), static_cast<size_type>(__last - __first));
  init_space(distance, init_size);
  toystl::uninitialized_copy(__first, __last, __start);
}

template <class _T>
void vector<_T>::destroy_and_recover(iterator __first, iterator __last,
                                     size_type __n) {
  data_allocator::destroy(__first, __last);
  data_allocator::deallocate(__first, __n);
}

template <class _T>
void vector<_T>::fill_assign(size_type __n, const value_type& __value) {
  if (__n > capacity()) {
    vector tmp(__n, __value);
    swap(tmp);
  } else if (__n > size()) {
    toystl::fill(begin(), end(), __value);
    __finish = toystl::uninitialized_fill_n(__finish, __n - size(), __value);
  } else {
    erase(toystl::fill_n(begin(), __n, __value), end());
  }
}

template <class _T>
template<class _InputIter> void vector<_T>::copy_assign(
    _InputIter __first, _InputIter __last, toystl::input_iterator_tag) {
  auto __cur = begin();
  for (; __first != __last && __cur != end(); ++__cur, ++__first) {
    *__cur = *__first;
  }
  if (__first == __last) {
    erase(__cur, end());
  } else {
    insert(end(), __first, __last);
  }
}

template <class _T>
template <class _ForwardIter>
void vector<_T>::copy_assign(_ForwardIter __first, _ForwardIter __last,
                             toystl::forward_iterator_tag) {
  const size_type distance = toystl::distance(__first, __last);
  if (distance > capacity()) {
    vector tmp(__fill, __last);
    swap(tmp);
  } else if (size() >= distance) {
    auto new_end = toystl::copy(__first, __last, __start);
    data_allocator::destroy(new_end, __end_of_storage);
    __end_of_storage = new_end;
  } else {
    auto mid = __first;
    toystl::advance(mid, size());
    toystl::copy(__first, mid, __start);
    auto new_end = toystl::uninitialized_copy(mid, __last, __finish);
    __end_of_storage = new_end;
  }
}

template <class _T>
template <class... Args>
void vector<_T>::reallocate_emplace(iterator __pos, Args&&... args) {
  const size_type old_size = size();
  const size_type new_size = old_size ? 2 * old_size : 1;
  auto new_start = data_allocator::allocate(new_size);
  auto new_finish = new_start;
  try {
    new_finish = toystl::uninitialized_move(__start, __pos, new_start);
    data_allocator::construct(toystl::address_of(*new_finish),
                              toystl::forward<Args>(args)...);
    ++new_finish;
    new_finish = toystl::uninitialized_move(__pos, __finish, new_finish);
  } catch (...) {
    data_allocator::deallocate(new_start, new_size);
    throw;
  }
  destroy_and_recover(__start, __finish, __end_of_storage - __start);
  __start = new_start;
  __finish = new_finish;
  __end_of_storage = new_start + new_size;
}

template <class _T>
void vector<_T>::reallocate_insert(iterator __pos, const value_type& __value) {
  const size_type old_size = size();
  const size_type new_size = old_size ? 2 * old_size : 1;
  auto new_start = data_allocator::allocate(new_size);
  auto new_finish = new_start;
  const value_type& value_copy = __value;
  try {
    new_finish = toystl::uninitialized_move(__start, __pos, new_start);
    data_allocator::construct(toystl::address_of(*new_finish), value_copy);
    ++new_finish;
    new_finish = toystl::uninitialized_move(__pos, __finish, new_finish);
  } catch (...) {
    data_allocator::deallocate(new_start, new_size);
    throw;
  }
  destroy_and_recover(__start, __finish, __end_of_storage - __start);
  __start = new_start;
  __finish = new_finish;
  __end_of_storage = new_start + new_size;
}


// overload the vector compare operation.
template <class _T>
bool operator==(const vector<_T>& __lhs, const vector<_T>& __rhs) {
  return __lhs.size() == __rhs.size() &&
         toystl::equal(__lhs.begin(), __lhs.end(), __rhs.begin());
}

template <class _T>
bool operator<(const vector<_T>& __lhs, const vector<_T>& __rhs) {
  return toystl::lexicographical_compare(__lhs.begin(), __lhs.end(), __rhs.begin(),
                                         __rhs.end());
}

template <class _T>
bool operator!=(const vector<_T>& __lhs, const vector<_T>& __rhs) {
  return !(__lhs == __rhs);
}

template <class _T>
bool operator>(const vector<_T>& __lhs, const vector<_T>& __rhs) {
  return __rhs < __lhs;
}

template <class _T>
bool operator<=(const vector<_T>& __lhs, const vector<_T>& __rhs) {
  return !(__rhs < __lhs);
}

template <class _T>
bool operator>=(const vector<_T>& __lhs, const vector<_T>& __rhs) {
  return !(__lhs < __rhs);
}

__STL_END_NAMESAPCE

#endif