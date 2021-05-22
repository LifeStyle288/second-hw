#define BOOST_TEST_MODULE tests

#include <boost/test/unit_test.hpp>

#include <map>

#include "my_allocator.h"
#include "simple_vector.h"
#include "factorial.h"

BOOST_AUTO_TEST_SUITE(tests)

BOOST_AUTO_TEST_CASE(test_map_allocate)
{
    using TestMap = std::map<int, int, std::less<int>, MyAllocatorMap<int, int, 10>>;
    TestMap m;
    for (size_t i = 0; i < 10; ++i)
    {
        m.insert({i, factorial(i)});
    }
    const TestMap sample = {{0, 1}, {1, 1}, {2, 2}, {3, 6}, {4, 24}, {5, 120}, {6, 720}, {7, 5040}, {8, 40320}, {9, 362880}};
    TestMap::const_iterator it1 = m.begin();
    TestMap::const_iterator it2 = sample.begin();
    for (; it1 != m.end(); ++it1, ++it2) 
    {
        BOOST_CHECK_EQUAL(*it1 == *it2);
    }
    BOOST_CHECK_THROW(m.insert({10, factorial(10)}), std::bad_alloc);
}

BOOST_AUTO_TEST_SUITE_END()
