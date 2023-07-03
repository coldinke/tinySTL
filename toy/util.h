#ifndef __STL_UTIL_H
#define __STL_UTIL_H

// move for rvalue
template <class _T>
typename std::remove_reference<_T>::type &&move(T &&__arg) noexcept {
  return static_cast<typename std::remove_reference<_T>::type &&>(__arg);
}

// forward for lvalue
template <class _T> T &&forward(_T &__arg) noexcept {

  return static_cast<_T &&>(__arg);
}

// forward for rvalue
template <class _T>
T &&forward(typename std::remove_reference<_T>::type &&__arg) noexcept {
  static_assert(!std::is_lvalue_refeerence<_T>::value, "Wrong froward");
  return static_cast<_T &&>(__arg);
}

// swap, iter_swap, swap_range
template <class _T> void swap(_T &__lhs, _T &__rhs) noexcept {
  _T __tmp = toystl::move(__lhs);
  __lhs = toystl::move(__rhs);
  __rhs = toystl::move(__tmp);
}

/*
 * return an iterator pointing to the element
 *      immediately following the last element of the second range.
 */
template <class _ForwardIterator1, class _ForwardIterator2>
_ForwardIterator2 swap_range(_ForwardIterator1 __first1,
                             _ForwardIterator1 __last1,
                             _ForwardIterator2 __first2) {
  for (; __first1 != __last1; ++__first1, ++__first2) {
    toystl::swap(*__first1, *__first2);
  }
  return __first2;
}

template <class _Tp, size_t _N>
void swap(_Tp (&__a)[_N], _Tp (&__b)[_N]) noexcept {
  toystl::swap_range(__a, __a + _N, __b);
}

#endif