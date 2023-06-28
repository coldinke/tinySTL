#ifndef __STL_CONSTRUCT_H
#define __STL_CONSTRUCT_H

__STL_BEGIN_NAMESPACE

#include <new>

template <class _T1>
void construct(T* __p) {
    ::new ((void*)__p) _T1();
}

template <class _T1, class _T2>
void construct(T1* __p, const T2& __v) {
    ::new ((void*)__p) T1(__v);
}

template <class _T1, class ... Args>
void construct(_T1* __p, Args&& ... __args)
{
    ::new((void*)__p) _T1(forward<Args>(__args)...);
}

template <class _T>
void _Destroy_one(_T*, std::true_type) {}

template <class _T>
void _Destroy_one(_T* __p, std::false_type) {
    if (__p != nullptr) {
        __p->~_T();
    }
}

__STL_END_NAMESAPCE


#endif