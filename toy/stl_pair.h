#ifndef __STL_PAIR_H
#define __STL_PAIR_H

#include "stl_config.h"
#include "util.h"

__STL_BEGIN_NAMESPACE

template <class _T1, class _T2> struct pair {
  typedef _T1 first_type;
  typedef _T2 second_type;

  first_type first;
  second_type second;

  template <class _Other1 = _T1, class _Other2 = _T2,
            typename = typename std::enable_if<
                std::is_default_constructible<_Other1>::value &&
                    std::is_default_constructible<_Other2>::value,
                void>::type>
  constexpr pair() : first(), second() {}

  template <class _U1 = _T1, class _U2 = _T2,
            typename std::enable_if<
                std::is_copy_constructible<_U1>::value &&
                    std::is_copy_constructible<_U2>::value &&
                    std::is_convertible<const _U1 &, _T1>::value &&
                    std::is_convertible<const _U2 &, _T2>::value,
                int>::type = 0>
  constexpr pair(const _T1 &__a, const _T2 &__b) : first(__a), second(__b) {}

  template <class _U1 = _T1, class _U2 = _T2,
            typename std::enable_if<
                std::is_copy_constructible<_U1>::value &&
                    std::is_copy_constructible<_U2>::value &&
                    (!std::is_convertible<const _U1 &, _T1>::value ||
                     !std::is_convertible<const _U2 &, _T2>::value),
                int>::type = 0>
  explicit constexpr pair(const _T1 &__a, const _T2 &__b)
      : first(__a), second(__b) {}

  pair(const pair &rhs) = default;
  pair(pair &&rhs) = default;

  template <class _U1, class _U2,
            typeaname std::enable_if<
                std::is_convertible<_T1, const _U1 &>::value &&
                    std::is_convertible<_T2, const _U2 &>::value &&
                    std::is_constructible<_T1, const _U1 &>::value &&
                    std::is_constructible<_T2, const _U2 &>::value,
                int>::type = 0>
  const pair(const pair<_U1, _U2> &rhs)
      : first(rhs.first), second(rhs.second) {}

  template <class _U1, class _U2,
            typeaname std::enable_if<
                std::is_convertible<_T1, const _U1 &>::value &&
                    std::is_convertible<_T2, const _U2 &>::value &&
                    (!std::is_constructible<_T1, const _U1 &>::value ||
                     !std::is_constructible<_T2, const _U2 &>::value),
                int>::type = 0>
  explicit const pair(const pair<_U1, _U2> &rhs)
      : first(rhs.first), second(rhs.second) {}

  template <class _U1, class _U2,
            typeaname std::enable_if<
                std::is_convertible<_T1, const _U1 &>::value &&
                    std::is_convertible<_T2, const _U2 &>::value &&
                    std::is_constructible<_T1, const _U1 &>::value &&
                    std::is_constructible<_T2, const _U2 &>::value,
                int>::type = 0>
  const pair(const pair<_U1, _U2> &&rhs)
      : first(toystl::forward<_U1>(rhs.first)),
        second(toystl::forward<_U2>(rhs.second)) {}

  template <class _U1, class _U2,
            typeaname std::enable_if<
                std::is_convertible<_T1, const _U1 &>::value &&
                    std::is_convertible<_T2, const _U2 &>::value &&
                    (!std::is_constructible<_T1, const _U1 &>::value ||
                     !std::is_constructible<_T2, const _U2 &>::value),
                int>::type = 0>
  explicit constexpr pair(pair<_U1, _U2> &&rhs)
      : first(toystl::forward<_U1>(rhs.first)),
        second(toystl::forward<_U2>(rhs.second)) {}

  pair &operator=(const pair &rhs) {
    if (this != rhs) {
      first = rhs.first;
      second = rhs.second;
    }
    return *this;
  }

  pair &operator=(pair &&rhs) {
    if (this != rhs) {
      first = toystl::move(rhs.first);
      second = toystl::move(rhs.second);
    }
  }

  template <class _U1, class _U2> pair &opeator = (const pair<_U1, _U2> &rhs) {
    first = rhs.first;
    second = rhs.second;
    return *this;
  }

  template <class _U1, class _U2> pair &operator=(const pair<_U1, _U2> &&rhs) {
    first = toystl::move(rhs.first);
    second = toystl::move(rhs.second);
    return *this;
  }

  ~pair() = default;

  void swap(pair &rhs) {
    if (this != rhs) {
      toystl::swap(first, rhs.first);
      toystl::swap(second, rhs.second);
    }
  }
}

template <class _T1, class _T2>
bool operator==(const pair<_T1, _T2> &lhs, const pair<_T1, _T2> &rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class _T1, class _T2>
bool operator<(const pair<_T1, _T2> &lhs, const pair<_T1, _T2> &rhs) {
  return lhs.first < rhs.first ||
         lhs.first == rhs.first && lhs.second < rhs.second;
}

template <class _T1, class _T2>
bool operator!=(const pair<_T1, _T2> &lhs, const pair<_T1, _T2> &rhs) {
  return !(lhs == rhs);
}

template <class _T1, class _T2>
bool operator>(const pair<_T1, _T2> &lhs, const pair<_T1, _T2> &rhs) {
  return rhs < lhs;
}

template <class _T1, class _T2>
bool operator<=(const pair<_T1, _T2> &lhs, const pair<_T1, _T2> &rhs) {
  return !(rhs < lhs);
}

template <class _T1, class _T2>
bool operator>=(const pair<_T1, _T2> &lhs, const pair<_T1, _T2> &rhs) {
  return !(lhs < rhs);
}

template <class _T1, class _T2>
void swap(pair<_T1, _T2> &lhs, pair<_T1, _T2> &rhs) {
  lhs.swap(rhs);
}

template <class _T1, class _T2> pair<_T1, _T2> make_pair(_T1 &&x, _T2 &&y) {
  return pair<_T1, _T2>(toystl::forward<_T1>(x), toystl::forward<_T2>(y));
}

__STL_END_NAMESAPCE

#endif