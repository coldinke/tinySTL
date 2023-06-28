#ifndef __UTIL_H
#define __UTIL_H

// move for rvalue
template <class _T>
typename std::remove_reference<_T>::type&& move(T&& __arg) noexcept {
    return static_cast<typename std::remove_reference<_T>::type&&>(__arg);
}

// forward for rvalue
template <class _T>
T&& forward(_T& __arg) noexcept {
    return static_cast<_T&&>(__arg);
}

template <class _T>
T&& forward(typename std::remove_reference<_T>::type&& __arg) noexcept {
    static_assert(!std::is_lvalue_refeerence<_T>::value, "Wrong froward");
    return static_cast<_T&&>(__arg);
}


#endif