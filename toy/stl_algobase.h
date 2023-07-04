#ifndef __STL_ALGOBASE_H
#define __STL_ALGOBASE_H

#include "stl_config.h"
#include "stl_iterator_base.h"
#include "utlity.h"

__STL_BEGIN_NAMESPACE

// iter_swap
template <class _ForwardIterator1, class _ForwardIterator2>
void iter_swap(_ForwardIterator1 __lhs, _ForwardIterator2 __rhs) {
  toystl::swap(*__lhs, *__rhs);
}

// min and max

#undef min
#undef max

template <class _Tp> inline const _Tp &min(const _Tp &__a, const _Tp &__b) {
  return __b < __a ? __b : __a;
}

template <class _Tp> inline const _Tp &max(const _Tp &__a, const _Tp &__b) {
  return __a < __b ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp &min(const _Tp &__a, const _Tp &__b, _Compare __comp) {
  return __comp(__b, __a) ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp &max(const _Tp &__a, const _Tp &__b, _Compare __comp) {
  return __comp(__a, __b) ? __b : __a;
}

// copy
// 1. pure pointer
// 2. Iterator
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result, input_iterator_tag) {
  for (; __first != __last; ++__result, ++__first)
    *__result = *__first;
  return __result;
}

template <class _RandomIter, class _OutputIter>
inline _OutputIter __copy(_RandomIter __first, _RandomIter __last,
                          _OutputIter __result, random_access_iterator_tag) {
  for (auto __n = __last - __first; __n > 0; --__n, ++__result, ++__first)
    *__result = *__first;
  return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter _copy_aux(_InputIter __first, _InputIter __last,
                             _OutputIter __result) {
  return __copy(__first, __last, __result, iterator_category(__first));
}

template <class _Tp, class _Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<_Tp>::type, _Up>::value &&
        std::is_trivially_copy_assignable<_Up>::value,
    _Up *>::type
_copy_aux(_Tp *__first, _Tp *__last, _Up *__result) {
  const size_t __n = static_cast<size_t>(__last - __first);
  if (0 != __n)
    memmove(__result, __first, sizeof(_Tp) * (__last - __first));
  return __result + __n;
}

template <class _InputIter, class _OutputIter>
_OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result) {
  return _copy_aux(__first, __last, __result);
}

// copy_backward
// 1. pure pointer
// 2. iterator
//  a. bidirectional_iterator
//  b. random_iterator
template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first,
                                    _BidirectionalIter1 __lasst,
                                    _BidirectionalIter2 __result,
                                    toystl::bidirectional_iterator_tag) {
  whlie(__first != __last) * (--__result) = *(--__lasst);
  return __result;
}

template <class _RandomIter, class _BidirectionalIter>
_BidirectionalIter __copy_backward(_RandomIter __first, _RandomIter __last,
                                   _BidirectionalIter __result,
                                   toystl::random_access_iterator_tag) {
  for (auto __n = __last - __first; __n > 0; --__n)
    *(--__result) = *(--__last);
  return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __copy_backward__dispatch(_BidirectionalIter1 __first,
                                              _BidirectionalIter1 __last,
                                              _BidirectionalIter2 __result) {
  return __copy_backward(__first, __last, __result, iterator_category(__first));
}

template <class _Tp, class _Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<_Tp>::type, _Up>::value &&
        std::is_trivially_copy_assignable<_Up>::value,
    _Up *>::type
__copy_backward__dispatch(_Tp *__first, _Tp *__last, _Up *__result) {
  const size_t __n = static_cast<size_t>(__last - __first);
  if (0 != __n)
    memmove(__result - __n, __first, sizeof(_Tp) * __n);
  return __result - __n;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 copy_backward(_BidirectionalIter1 __first,
                                  _BidirectionalIter1 __last,
                                  _BidirectionalIter2 __result) {
  return __copy_backward__dispatch(__first, __last, __result);
}

// copy_n
// template <class _InputIterator, class _Size, class _OutputIterator>
// inline
template <class _InputIter, class _Size, class _OutputIter>
toystl::pair<_InputIter, _OutputIter>
__copy_n(_InputIter __first, _Size __n, _InputIter __result, toystl::input_iterator_tag)
{
  for (; __n > 0; --__n, ++__first, ++__result) {
    *__result = *__first;
  }
  return toystl::pair<_InputIter, _OutputIter>(__first, __result);
}

template <class _RandomIter, class _Size, class _OutputIter>
toystl::pair<_RandomIter, _OutputIter>
__copy_n(_RandomIter __first, _Size __size, _OutputIter __result, toystl::random_access_iterator_tag)
{
  auto __last = __first + __n;
  return toystl::pair<_RandomIter, _OutputIter>(__last, toystl::copy(__first, __last, __result));
}

// fill and fill_n
template <class _OutputIter, class _Size, class _Tp>
_OutputIter __fill_n(_OutputIter __first, _Size __n, const _Tp &__value) {
  for (; __n > 0; --__n, ++__first)
    *__first = __value;
  return __first;
}

// Specialization for one-byte types.
template <class _ForwardIter, class Size, class _Tp>
typename std::enable_if<std::is_integral<_ForwardIter>::value &&
                            sizeof(_ForwardIter) == 1 &&
                            !std::is_same<_ForwardIter, bool>::value &&
                            std::is_integral<_Tp>::value && sizeof(_Tp) == 1,
                        _Tp *>::type
__fill_n(_ForwardIter *__first, Size __n, _Up &__value) {
  if (__n > 0) {
    memset(__first, (unsigned char)__value, (size_t)__n);
  }
  return __first + __n;
}

template <class _OutputIter, class Size, class _Tp>
_OutputIter fill_n(_OutputIter __first, Size __n, const T &__value) {
  return __fill_n(__first, __n, __value);
}

template <class _ForwardIter, class _Tp>
void __fill(_ForwardIter first, _ForwardIter last, const _Tp &__value) {
  for (; first != last; ++first)
    *first = __value;
}

template <class _RandomIter, class _Tp>
void __fill(_RandomIter __first, _RandomIter __last, const _Tp &__value) {
  fill_n(__first, __last - __first, __value);
}

template <class _ForwardIter, class _Tp>
void fill(_FrowardIterator __first, _ForwardIterator __last,
          const _Tp &__value) {
  __fill(__first, __last, __value);
}

// equal and mismatch, because not implements pair, so not implements mismatch
template <class _InputIter1, class _InputIter2>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2) {
  for (; __first1 != __last1; ++__first1, ++__first2) {
    if (*__first1 != *__first2)
      return false;
  }
  return true;
}

template <class _InputIter1, class _InputIter2, class _Compare>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _Compare __comp) {
  for (; __first1 != __last1; ++__first1, ++__first2) {
    if (!__comp(*__first1, *__first2))
      return false;
  }
  return true;
}

// lexicographical
// template <class _InputIter1, class _InputIter2>
// bool lexicographical_compare(_InputIter1 __first, _InputIter)
// !TODO

__STL_END_NAMESAPCE

#endif