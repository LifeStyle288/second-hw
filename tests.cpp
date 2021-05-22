#define BOOST_TEST_MODULE tests

#include <boost/test/unit_test.hpp>

#include <map>

#include "my_allocator.h"
#include "simple_vector.h"
#include "factorial.h"

#define MAX_ELEMENTS 10

BOOST_AUTO_TEST_SUITE(tests)

BOOST_AUTO_TEST_CASE(test_map_allocate)
{
    BOOST_TEST_MESSAGE("Start test_map_allocate...");
    using TestMap = std::map<int, int, std::less<int>, MyAllocatorMap<int, int, MAX_ELEMENTS>>;
    TestMap m;
    for (size_t i = 0; i < MAX_ELEMENTS; ++i)
    {
        m.insert({i, factorial(i)});
    }
    const TestMap sample = {{0, 1}, {1, 1}, {2, 2}, {3, 6}, {4, 24}, {5, 120}, {6, 720}, {7, 5040}, {8, 40320}, {9, 362880}};
    TestMap::const_iterator it1 = m.begin();
    TestMap::const_iterator it2 = sample.begin();
    for (; it1 != m.end(); ++it1, ++it2) 
    {
        BOOST_CHECK(*it1 == *it2);
    }
    BOOST_TEST_MESSAGE("Checks true...");
    BOOST_CHECK_THROW(m.insert({10, factorial(10)}), std::bad_alloc);
    BOOST_TEST_MESSAGE("Stop test_map_allocate...");
}

BOOST_AUTO_TEST_CASE(test_vector_allocate)
{
    BOOST_TEST_MESSAGE("Start test_vector_allocate...");
    using TestVec = std::vector<int, MyAllocatorVec<int, MAX_ELEMENTS>>;
    TestVec v;
    for (int i = 0; i < MAX_ELEMENTS; ++i)
    {
        if (i > 0)
        {
            BOOST_CHECK_THROW(v.push_back(i), std::bad_alloc);
            break;
        }
        else
        {
            v.push_back(i);
        }
    }
    BOOST_TEST_MESSAGE("Stop test_vector_allocate...");
}

BOOST_AUTO_TEST_CASE(test_my_vector_allocate)
{
    BOOST_TEST_MESSAGE("Start test_my_vector_allocate...");
    using TestVec = SimpleVector<int, MyAllocatorVec<int, MAX_ELEMENTS>>;
    TestVec v(MAX_ELEMENTS);
    for (int i = 0; i < MAX_ELEMENTS; ++i)
    {
        v.push_back(i);
    }
    const TestVec sample = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    BOOST_CHECK_EQUAL_COLLECTIONS(v.begin(), v.end(), sample.begin(), sample.end());
    BOOST_TEST_MESSAGE("Check equal collections true...");
    BOOST_CHECK_THROW(v.push_back(111), std::bad_alloc);
    BOOST_TEST_MESSAGE("Stop test_my_vector_allocate...");
}

BOOST_AUTO_TEST_SUITE_END()
