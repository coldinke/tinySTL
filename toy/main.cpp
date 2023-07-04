#include <iostream>
#include "stl_vector.h"

int main() {
    bronya_stl::vector<int> Test(5);
    bronya_stl::vector<int> Test2;
    typedef bronya_stl::vector<int>::iterator itr;
    Test.push_back(4);
    Test.push_back(5);
    Test2 = Test;
    printf("%d\n", Test2[0]);
    return 0;
}