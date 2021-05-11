#include "factorial.h"

size_t factorial(const size_t n)
{
    if (n > 1)
        return n * factorial(n - 1);
    else
        return 1;
}
