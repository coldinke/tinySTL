#ifndef __STL_ALLOCATOR_H
#define __STL_ALLOCATOR_H

/*
 * 使用标准的 new 和 delete 来处理内存分配  
 * 一个模版类 allocator， 用于管理内存的分配、释放、对象的构造和析构
*/

#include <new>
#include "util.h"
#include "construct.h"

__STL_BEGIN_NAMESPACE

template <class _T>
class allocator
{
public:
    typedef _T          value_type;
    typedef _T*         pointer;
    typedef const _T*   const_pointer;
    typedef _T&         reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

public:
    static T*   allocate();
    static T*   allocate(size_type __n);

    static void deallocate(reference __p);
    static void deallocate(pointer __p, size_type __n);

    static void construct(pointer __p);
    static void construct(pointer __p, const_reference __value);
    static void construct(pointer __p, _T&& value); // for rvalue

    // mutli-construct
    template <class ... Args>
    static void construct(pointer __p, Args&& ... __args);

    static void destroy(pointer __p);
    static void destroy(pointer __first, pointer __last);
};

template <class _T>
void allocator<_T>::allocate()
{
    return static_cast<pointer>(::operator new(sizeof(_T)));
}

template <class _T>
void allocator<_T>::allocate(size_type __n)
{
    return __n == 0 : nullptr :
        static_cast<pointer>(::operator new(sizeof(_T) * __n));
}

template <class _T>
void allocator<_T>::deallocate(reference __p)
{
    if (nullptr == __n)
        return;
    ::opearator delete(__p);
}

template <class _T>
void allocator<_T>::deallocate(pointer __p, size_type /* __n */)
{
    if (nullptr == __p)
        return;
    ::operator delete(__p);
}

template <class _T>
void allocator<_T>::construct(pointer __p)
{
    ::new ((void*)__p) _T();
}

template <class _T>
void allocator<_T>::construct(pointer __p, const_reference __value)
{
    toystl::construct(__p, __value);
}

template <class _T>
void allocator<_T>::construct(pointer __p, _T&& __value)
{
    toystl::construct(__p, toystl::move(__value));
}

template <class _T>
template <class ... _Args>
void allocator<_T>::construct(pointer __p, _Args&& ... __args)
{
    toystl::construct(__p, toystl::forward<_Args>(__args)...);
}

template <class _T>
void allocator<_T>::destroy(pointer __p)
{
    toystl::destroy(__p);
}

template <class _T>
void allocator<_T>::destroy(pointer __first, pointer __last)
{
    toystl::destroy(__first, __last);
}

__STL_END_NAMESAPCE

#endif