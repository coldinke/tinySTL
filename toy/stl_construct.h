#ifndef __STL_CONSTRUCT_H
#define __STL_CONSTRUCT_H

#include <new>
#include "stl_config.h"
#include "utlity.h"
#include "type_traits.h"
#include "stl_iterator.h"

__STL_BEGIN_NAMESPACE

// construct
template <class _T1>
void construct(_T1 *__p)
{
  ::new ((void *)__p) _T1();
}

template <class _T1, class _T2>
void construct(_T1 *__p, const _T2 &__value)
{
  ::new ((void *)__p) _T1(__value);
}

template <class _T1, class... Args>
void construct(_T1 *__p, Args &&...__args)
{
  ::new ((void *)__p) _T1(toystl::forward<Args>(__args)...);
}

// destroy
template <class _T>
void _Destroy_one(_T *, std::true_type) {}

template <class _T>
void _Destroy_one(_T *__p, std::false_type)
{
  if (__p != nullptr)
  {
    __p->~_T();
  }
}

template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last,
                   std::true_type)
{
}

template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last,
                   std::false_type)
{
  for (; __first != __last; ++__first)
  {
    destory(&*__first);
  }
}

template <class _Tp>
void destroy(_Tp *__pointer)
{
  _Destroy_one(__pointer, std::is_trivially_destructible<_Tp>{});
}

template <class _ForwardIterator>
void destroy(_ForwardIterator __first, _ForwardIterator __last)
{
  __destroy_aux(__first, __last,
                std::is_trivially_destructible<
                    typename iterator_traits<_ForwardIterator>::value_type>{});
}

__STL_END_NAMESAPCE

#endif