#include <iostream>

#include "stl_vector.h"

int main() {
  int a[] = {1, 2, 3, 4, 5};
  toystl::vector<int> v1;
  toystl::vector<int> v2(10);
  // this constructor is not right implemented, so it can not pass the compilation
  // toystl::vector<int> v3(10, 1);
  toystl::vector<int> v4(a, a + 5);
  toystl::vector<int> v5(v2);
  toystl::vector<int> v6(std::move(v2));
  toystl::vector<int> v7{1, 2, 3, 4, 5, 6, 7, 8, 9};
  toystl::vector<int> v8, v9, v10;
  v8 = v7;
  v9 = std::move(v7);
  v10 = {1, 2, 3, 4, 5, 6, 7, 8, 9};  
}