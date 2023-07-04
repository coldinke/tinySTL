#ifndef __STL_VECTOR_H
#define __STL_VECTOR_H

#include "stl_allocator.h"
#include "stl_config.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"

__STL_BEGIN_NAMESPACE

template <class _Tp, class _Alloc> class _Vecotr_Base {
public:
  typedef _Alloc allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }

  _Vecotr_Base(const allocator_type &) : __begin(0), __end(0), __cap(0) {}
  _Vecotr_Base(size_t __n, const allocator_type &)
      : __begin(0), __end(0), __cap(0) {
    __begin = _allocate(__n);
    __end = __begin;
    __cap = __begin + __n;
  }
  ~_Vecotr_Base() { _deallocate(__begin, __cap - __begin) }

protected:
_T *__begin;
_T *__end;
_T *__cap;

typedef simple_alloc<_T, _Alloc> _Data_Alloc_Type;

_T *_allocate(size_t __n) {
  return __n == 0 ? nullptr : _Data_Alloc_Type::allocate(__n);
}

void _deallocate(_T *__p, size_t __n) {
  _Data_Alloc_Type::deallocate(__p, __n);
}
};

  template<class _T, class _Alloc = alloc>
    class vector : protected _Vector_Base<_T, _Alloc> {
    private:
        typedef _Vector_Base<_T, _Alloc> _Base;
    public:
        /* vector 嵌套型别定义 */
        typedef _T value_type;

        typedef value_type                          *pointer;
        typedef value_type                          *iterator;
        typedef value_type                          &reference;
        typedef reverse_iterator<iterator>          __reverse_iterator;

        typedef const value_type                    *const_pointer;
        typedef const value_type                    *const_iterator;
        typedef const value_type                    &const_reference;
        typedef reverse_iterator<const_iterator>    __const_reverse_iterator;

        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;

        typedef typename _Base::allocator_type      allocator_type;

        allocator_type get_allocater() const { return _Base::get_allocater(); }
    protected:
        using _Base::__begin;
        using _Base::__end;
        using _Base::__cap;

        using _Base::_allocate;
        using _Base::_deallocate;
    protected:
        void _insert_aux(iterator __position);
        void _insert_aux(iterator __position, const _T &__x);
        void _fill_insert(iterator __position, size_type __n, const _T &__value);

        template<class _InputIterator>
        void _range_insert(iterator __position, _InputIterator __first, _InputIterator __last, input_iterator_tag);

        template<class _ForwardIterator>
        void _range_insert(iterator __postion, _ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag);

        template<class _ForwardIterator>
        iterator _allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last) {
            iterator __result = _allocate(__n);
            try {
                uninitialized_copy(__first, __last, __result);
                return __result;
            }
            catch(...) {
                _deallocate(__result, __n);
                throw;
            }
        }

    public:
        /* vector类的构造和析构 */
        explicit vector(const allocator_type &__a = allocator_type()) : _Base(__a) {}
        vector(size_type __n, const _T &__value, const allocator_type &__a = allocator_type()) : _Base(__n, __a) {
            uninitialized_fill_n(__begin, __n, __value);
        }

        explicit vector(size_type __n) : _Base(__n, allocator_type()) {
            uninitialized_fill_n(__begin, __n, _T());
        }

        vector(const vector<_T, _Alloc> &__x) : _Base(__x.size(), __x.get_allocater()) {
            uninitialized_copy(__x.begin(), __x.__end(), __begin);
        }

        ~vector() { destroy(__begin, __end); }
    public:
        /* 迭代器相关操作 */

        /* 返回vector头指针 */
        iterator     begin()             { return __begin; }
        /* 返回vector尾指针 */
        iterator     end()               { return __end; }
        /* 返回reverse_vector头指针 */
        iterator     r_begin()           { return __reverse_iterator(end()); }
        /* 返回reverse_vector尾指针 */
        iterator     r_end()             { return __reverse_iterator(begin()); }

        /* 返回const类型的vector头指针 */
        const_iterator c_begin()  const  { return __begin; }
        /* 返回const类型的vector尾指针 */
        const_iterator c_end()    const  { return __end; }
        /* 返回const类型的reverse_vector头指针 */
        const_iterator c_rbegin() const  { return __reverse_iterator(end()); }
        /* 返回const类型的reverse_vector尾指针 */
        const_iterator c_rend()   const  { return __reverse_iterator(begin()); }

        /* 容量相关操作 */

        /* 返回vector大小 */
        size_type    size()       const  { return size_type(c_end() - c_begin()); }
        /* 返回最大容量 */
        size_type    max_size()   const  { return size_type(-1) / sizeof(_T); }
        /* 返回vector储存的容量 */
        size_type    capacity()   const  { return size_type(__cap - __begin); }
        /* 返回vector是否为空 */
        bool         empty()      const  { return begin() == end(); }

        /* 访问元素相关操作 */

        /* 检测vector范围 */
        void _Range_check(size_type __n) const {
            if(__n >= size()) {
                throw;
            }
        }
        
        /* 重载[]符号 */
        reference operator[](size_type __n) { return *(begin() + __n); }
        /* 返回vector头指针的内容 */
        reference front() { return *begin(); }
        /* 返回vector尾指针的内容 */
        reference back() { return *(end() - 1); }
        /* 返回vector第n个指针的内容 */
        reference at(size_type __n) {
            _Range_check(__n);
            return (*this)[__n];
        }

        /* 重载[]符号 */
        const_reference operator[](size_type __n) const { return *(begin() + __n); }
        /* 返回const类型vector头指针的内容 */
        const_reference front() const { return *begin(); }
        /* 返回const类型vector尾指针的内容 */
        const_reference back() const { return *(end() - 1); }
        /* 返回const类型vector第n个指针的内容 */
        const_reference at(size_type __n) const {
            _Range_check(__n);
            return (*this)[__n];
        }

        /* 修改容器相关操作 */

        /* 交换两个vector */
        void swap(vector<_T, _Alloc> &__x) {
            if(this != &__x) {
                bronya_stl::swap(__begin, __x.__begin);
                bronya_stl::swap(__end, __x.__end);
                bronya_stl::swap(__cap, __x.__cap);
            }
        }
        /* 擦除单个元素 */
        iterator erase(iterator __position) {
            if(__position + 1 != end()) {
                copy(__position + 1, __end, __position);
            }
            --__end;
            destroy(__end);
            return __position;
        }
        /* 擦除 [ __first, __last ) 范围内元素 */
        iterator erase(iterator __first, iterator __last) {
            iterator __i = copy(__last, __end, __first);
            destroy(__i, __end);
            __end = __end - (__last - __first);
            return __first;
        }
        /* 调整vector大小,如果当前容器大小小于目标大小,在后面填充value */
        void resize(size_type __new_size, const _T &__value) {
            if(__new_size < size()) {
                erase(begin() + __new_size, end());
            }
            else {
                insert(end(), __new_size - size(), __value);
            }
        }
        /* 调整vector大小 */
        void resize(size_type __new_size) {
            resize(__new_size, _T());
        }
        /* 清除vector内所有的元素 */
        void clear() {
            erase(begin(), end());
        }
        /* 重载赋值操作符 */
        vector<_T, _Alloc> &operator=(vector<_T, _Alloc> &__other) {
            if(this != &__other) {
                const size_type __other_len = __other.size();
                if(__other_len > capacity()) {
                    iterator __tmp = _allocate_and_copy(__other_len, __other.begin(), __other.end());
                    destroy(__begin, __end);
                    _deallocate(__begin, __cap - __begin);
                    __begin = __tmp;
                    __cap = __begin + __other_len;
                }
                else if(__other_len <= size()) {
                    iterator __i = copy(__other.begin(), __other.end(), __other.begin());
                    destroy(__i, __end);
                }
                else {
                    copy(__other.begin(), __other.begin() + __other.size(), __end);
                    uninitialized_copy(__other.begin() + __other.size(), __other.end(), __end);
                }
                __end = __begin + __other_len;
            }
            return *this;
        }
        /* 向vectow尾部插入value */
        void push_back(const _T &__value) {
            if(__end != __cap) {
                construct(__end, __value);
                ++__end;
            }
            else {
                _insert_aux(end(), __value);    
            }
        }
        /* 弹出vector尾部 */
        void pop_back() {
            --__end;
            destroy(__end);
        }
       /* 将value插入到vector的postiow位置 */
        iterator insert(iterator __position, const _T &__value) {
            size_type __n = __position - begin();
            /* 如果插入到vectow尾部 */
            if(__end != __cap && __position == end()) {
                construct(__end, __value);
                ++__end;
            }
            else {
                _insert_aux(__position, __value);
            }
            return (begin() + __n);
        }

        template<class _InputIterator>
        void _insert_dispatch(iterator __position, _InputIterator __first, _InputIterator __last, __false_type) {
            _range_insert(__position, __first, __last, iterator_category(__first));
        }

        template<class _Integer>
        void _insert_dispatch(iterator __position, _Integer __n, _Integer __value, __true_type) {
            _fill_insert(__position, (size_type)__n, (_T)__value);
        }

        template<class _InputIterator>
        void insert(iterator __position, _InputIterator __first, _InputIterator __last) {
            typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
            _insert_dispatch(__position, __first, __last, _Integral());
        }

        void insert(iterator __position, size_type __n, const _T &__value) {
            _insert_dispatch(__position, __n, __value);
        }




    };


    template<class _T, class _Alloc>
    void vector<_T, _Alloc>::_insert_aux(iterator __position, const _T &__x) {
        if(__end != __cap) {
            construct(__end, *(__end - 1));
            ++__end;
            _T __x_copy = __x;
            copy_backward(__position, __end - 2, __end - 1);
            *__position = __x_copy;
        }
        else {
            const size_type __old_size = size();
            const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
            iterator __new_begin = _allocate(__len);
            iterator __new_end = __new_begin;
            try {
                __new_end = uninitialized_copy(__begin, __position, __new_begin);
                construct(__new_end, __x);
                ++__new_end;
                __new_end = uninitialized_copy(__position, __end, __new_end);
            }
            catch(...) {
                destroy(__new_begin, __new_end);
                _deallocate(__new_begin, __len);
                throw;
            }
            destroy(begin(), end());
            _deallocate(__begin, __cap - __begin);
            __begin = __new_begin;
            __end   = __new_end;
            __cap   = __begin + __len;
        }
    }

    template<class _T, class _Alloc>
    void vector<_T, _Alloc>::_insert_aux(iterator __position) {
        if(__end != __cap) {
            construct(__end, *(__end - 1));
            ++__end;
            copy_backward(__position, __end - 2, __end - 1);
            *__position = _T();
        }
        else {
            const size_type __old_size = size();
            const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
            iterator __new_begin = _allocate(__len);
            iterator __new_end = __new_begin;
            try {
                __new_end = uninitialized_copy(__begin, __position, __new_begin);
                construct(__new_end);
                ++__new_end;
                __new_end = uninitialized_copy(__position, __end, __new_end);
            }
            catch(...) {
                destroy(__new_begin, __new_end);
                _deallocate(__new_begin, __len);
                throw;
            }
            destroy(begin(), end());
            _deallocate(__begin, __cap - __begin);
            __begin = __new_begin;
            __end   = __new_end;
            __cap   = __begin + __len;
        }
    }

    template<class _T, class _Alloc>
    void vector<_T, _Alloc>::_fill_insert(iterator __position, size_type __n, const _T &__value) {
        if(__n != 0) {
            if(size_type(__cap - __end) >= __n) {
                _T __value_copy = __value;
                const size_type __elems_after = __end - __position;
                iterator __old_end = __end;
                if(__elems_after >= __n) {
                    uninitialized_copy(__end - __n, __end, __end);
                    __end += __n;
                    copy_backward(__position, __old_end - __n, __old_end);
                    fill(__position, __position + __n, __value_copy);
                }
                else {
                    uninitialized_fill_n(__end, __n - __elems_after, __value_copy);
                    __end += __n - __elems_after;
                    uninitialized_copy(__position, __old_end, __end);
                    __end += __elems_after;
                    fill(__position, __old_end, __value_copy);
                }
            }
            else {
                const size_type __old_size = size();
                const size_type __len = __old_size + max(__old_size, __n);
                iterator __new_begin = _allocate(__n);
                iterator __new_end = __new_begin;
                try {
                    __new_end = uninitialized_copy(__begin, __position, __new_begin);
                    __new_end = uninitialized_fill_n(__new_end, __n, __value);
                    __new_end = uninitialized_copy(__position, __end, __new_end);
                }
                catch(...) {
                    destroy(__new_begin, __new_end);
                    _deallocate(__new_begin, __len);
                    throw;
                }
                destroy(__begin, __end);
                _deallocate(__begin, __cap - __begin);
                __begin = __new_begin;
                __end = __new_end;
                __cap = __new_end + __len;
            }
        }
    }

    template<class _T, class _Alloc>
    template<class _InputIterator>
    void vector<_T, _Alloc>::_range_insert(iterator __position, _InputIterator __first, _InputIterator __last, input_iterator_tag) {
        for(; __first != __last; ++__first) {
            __position = insert(__position, *__first);
            ++__position;
        }
    }

    template<class _T, class _Alloc>
    template<class _ForwardIterator>
    void vector<_T, _Alloc>::_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag) {
        if(__first != __last) {
            size_type __n = 0;
            distance(__first, __last, __n);
            if(size_type(__cap - __end) >= __n) {
                const size_type __elems_after = __end - __position;
                iterator __old_end = __end;
                if(__elems_after > __n) {
                    uninitialized_copy(__end - __n, __end, __end);
                    __end += __n;
                    copy_backward(__position, __old_end - __n, __old_end);
                    copy(__first, __last, __position);
                }
                else {
                    _ForwardIterator __mid = __first;
                    advance(__mid, __elems_after);
                    uninitialized_copy(__mid, __last, __end);
                    __end += __n - __elems_after;
                    uninitialized_copy(__position, __old_end, __end);
                    __end += __elems_after;
                    copy(__first, __mid, __position);
                }
            }
            else {
                const size_type __old_size = size();
                const size_type __len = __old_size + max(__old_size, __n);
                iterator __new_begin = _allocate(__len);
                iterator __new_end = __new_begin;
                try {
                    __new_end = uninitialized_copy(__begin, __position, __new_begin);
                    __new_end = uninitialized_copy(__first, __last, __new_end);
                    __new_end = uninitialized_copy(__position, __end, __new_end);
                }
                catch(...) {
                    destroy(__new_begin, __new_end);
                    _deallocate(__new_begin, __len);
                    throw;
                }
                destroy(__begin, __end);
                _deallocate(__begin, __cap - __begin);
                __begin = __new_begin;
                __end = __new_end;
                __cap = __new_begin + __len;
            }
        }
    }


    /* 重载vector比较操作符 */
    template<class _T, class _Alloc>
    inline bool operator==(const vector<_T, _Alloc> &__x, const vector<_T, _Alloc> &__y) {
        return (__x.size() == __y.size() && equal(__x.begin(), __x.end(), __y.begin()));
    }

    template<class _T, class _Alloc>
    inline bool operator!=(const vector<_T, _Alloc> &__x, const vector<_T, _Alloc> &__y) {
        return !(__x == __y);
    }





__STL_END_NAMESPACE

#endif