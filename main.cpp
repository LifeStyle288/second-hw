#include <iostream>
#include <map>

#include "my_allocator.h"
#include "factorial.h"
#include "simple_vector.h"
#include "printer.h"

#define MAX_ELEMENTS 10

using MyAllocatorMap = MyAllocator<std::pair<const int, const int>, MAX_ELEMENTS>;
using MyAllocatorVec = MyAllocator<int, MAX_ELEMENTS>;

int main(int, char *[]) 
{
    std::map<int, int> m1;
    std::map<int, int, std::less<int>, MyAllocatorMap> m2;
    for (size_t i = 0; i < MAX_ELEMENTS; ++i)
    {
        m1.insert({i, factorial(i)});
        m2.insert({i, factorial(i)});
    }
    print_map(m1);
    print_map(m2);

    SimpleVector<int> vec1;
    std::vector<int, MyAllocatorVec> vec2;
    for (size_t i = 0; i < MAX_ELEMENTS; ++i) 
    {
        vec1.push_back(i);
        vec2.push_back(i);
    }
    print_vec(vec1);
    print_vec(vec2);

    return 0;
}