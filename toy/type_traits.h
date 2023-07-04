#ifndef _STL_TYPE_TRAITS_H
#define _STL_TYPE_TRAITS_H

// use standard header for type_tratis
#include <type_traits>

__STL_BEGIN_NAMESPACE

template <class _T, _T v> struct _integral_constant {
  static constexpr _T value = v;
};

template <bool b> using _bool_constant = _integral_constant<bool, b>;

typedef _bool_constant<true> true_type;
typedef _bool_constant<false> false_type;

__STL_END_NAMESAPCE

#endif