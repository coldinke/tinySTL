#ifndef __STL_UNINITIALIZED_H
#define __STL_UNINITIALIZED_H

#include "stl_algobase.h"
#include "stl_config.h"
#include "stl_iterator.h"
#include "stl_pair.h"

__STL_BEGIN_NAMESPACE

// uninitialized_copy
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last,
                                  _ForwardIter __result, std::true_type) {
  return toystl::copy(__first, __last, __result);
}

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last,
                                  _ForwardIter __result, std::false_type) {
  _ForwardIter __cur = __result;
  try {
    for (; __first != __last; ++__first, ++__cur)
      toystl::construct(&*__cur, *__first);
    return __cur;
  } catch (...) {
    toystl::destroy(__result, __cur);
    throw;
  }
}

template <class _InputIter, class _ForwardIter>
_ForwardIter uninitialized_copy(_InputIter __first, _InputIter __last,
                                _ForwardIter __result) {
  typedef typename std::is_trivially_copy_assignable<
      typename iterator_traits<_InputIter>::value_type>::type _Trivial;
  return __uninitialized_copy(__first, __last, __result, _Trivial());
}

// uninitialized_copy_n
template <class _InputIter, class _Size, class _ForwardIter>
_ForwardIter __uninitialized_copy_n(_InputIter __first, _Size __n,
                                    _ForwardIter __result, std::true_type) {
  return toystl::copy_n(__first, __n, __result).second;
}

template <class _InputIter, class _Size, class _ForwardIter>
_ForwardIter __uninitialized_copy_n(_InputIter __first, _Size __n,
                                    _ForwardIter __result, std::false_type) {
  _ForwardIter __cur = __result;
  try {
    for (; __n > 0; --__n, ++__first, ++__cur)
      toystl::construct(&*__cur, *__first);
  } catch (...) {
    toystl::destroy(__result, __cur);
    throw;
  }
  return __cur;
}

template <class _InputIter, class _Size, class _ForwardIter>
_ForwardIter uninitialized_copy_n(_InputIter __first, _Size __n,
                                  _ForwardIter __result) {
  typedef typename std::is_trivially_copy_assignable<
      typename iterator_traits<_InputIter>::value_type>::type _Trivial;
  return __uninitialized_copy_n(__first, __n, __result, _Trivial());
}

// uninitialized_fill
template <class _ForwardIter, class _Tp>
void __uninitialized_fill(_ForwardIter __first, _ForwardIter __last,
                          const _Tp &__x, std::true_type) {
  toystl::fill(__first, __last, __x);
}

template <class _ForwardIter, class _Tp>
void __uninitialized_fill(_ForwardIter __first, _ForwardIter __last,
                          const _Tp &__x, std::false_type) {
  _ForwardIter __cur = __first;
  try {
    for (; __cur != __last; ++__cur)
      toystl::construct(&*__cur, __x);
  } catch (...) {
    toystl::destroy(__first, __cur);
    throw;
  }
}

template <class _ForwardIter, class _Tp>
void uninitialized_fill(_ForwardIter __first, _ForwardIter __last,
                        const _Tp &__x) {
  typedef typename std::is_trivially_copy_assignable<_Tp>::type _Trivial;
  __uninitialized_fill(__first, __last, __x, _Trivial());
}

__STL_END_NAMESAPCE

#endif