#ifndef __STL_ALGO_H
#define __STL_ALGO_H

#include <cstddef>
#include <ctime>

#include "stl_algobase.h"
#include "stl_config.h"

__STL_BEGIN_NAMESPACE
/**********
 * all_of
 * 检查[first, last)区间内的所有元素是否都满足一元操作 unary_pred
 * 如果都满足，返回 true，否则返回 false
 */
template <class _InputIterator, class _UnaryPredicate>
bool all_of(_InputIterator __first, _InputIterator __last, _UnaryPredicate __unary_pred)
{
    for (; __first != __last; ++__first)
        if (!__unary_pred(*__first))
            return false;
    return true;
}

/**********
 * any_of
 * 检查[first, last)区间内是否有元素满足一元操作 unary_pred
 * 如果有，返回 true，否则返回 false
 */
template <class _InputIterator, class _UnaryPredicate>
bool any_of(_InputIterator __first, _InputIterator __last, _UnaryPredicate __unary_pred)
{
    for (; __first != __last; ++__first)
        if (__unary_pred(*__first))
            return true;
    return false;
}

/**********
 * none_of
 * 检查[first, last)区间内元素是否都不满足一元操作 unary_pred
 * 如果是，返回 true，否则返回 false
 */
template <class _InputIterator, class _UnaryPredicate>
bool none_of(_InputIterator __first, _InputIterator __last, _UnaryPredicate __unary_pred)
{
    for (; __first != __last; ++__first)
        if (__unary_pred(*__first))
            return false;
    return true;
}

/**********
 * find
 * 在[first, last)区间内找到等于 value 的元素
 * 返回指向该元素的迭代器
 */
template <class _InputIterator, class _Tp>
_InputIterator
find(_InputIterator __first, _InputIterator __last, const _Tp &__value)
{
    while (__first != __last && *__first != __value)
        ++__first;
    return __first;
}

/**********
 * find_if
 * 在[first, last)区间内找到第一个满足一元操作 unary_pred 的元素，即返回结果为 true 时
 * 返回指向该元素的迭代器
 */
template <class _InputIterator, class _UnaryPredicate>
_InputIterator
find_if(_InputIterator __first, _InputIterator __last, _UnaryPredicate __unary_pred)
{
    while (__first != __last && !__unary_pred(*__first))
        ++__first;
    return __first;
}

/**********
 * find_if_not
 * 在[first, last)区间内找到第一个不满足一元操作 unary_pred 的元素，即返回结果为 false 时
 * 返回指向该元素的迭代器
 */
template <class _InputIterator, class _UnaryPredicate>
_InputIterator
find_if_not(_InputIterator __first, _InputIterator __last, _UnaryPredicate __unary_pred)
{
    while (__first != __last && __unary_pred(*__first))
        ++__first;
    return __first;
}

/**********
 * reverse
 * 将[first, last)区间内的元素反转
 */
template <class _BidirectionalIterator>
void __reverse(_BidirectionalIterator __first, _BidirectionalIterator __last, toystl::bidirectional_iterator_tag)
{
    while (true)
        if (__first == __last || __first == --__last)
            return;
        else
            iter_swap(__first++, __last);
}

template <class _RandomAccessIterator>
void __reverse(_RandomAccessIterator __first, _RandomAccessIterator __last, toystl::random_access_iterator_tag)
{
    while (__first < __last)
        iter_swap(__first++, --__last);
}

template <class _BidirectionalIterator>
void reverse(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
    __reverse(__first, __last, iterator_category(__first));
}

__STL_END_NAMESAPCE

#endif