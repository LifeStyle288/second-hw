#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <string>
#include <vector>

#define UNUSED(val) (void)val

template <typename T, int N>
struct MyAllocator
{
    using value_type = T;

    template <typename U>
    struct rebind
    {
        using other = MyAllocator<U, N>;
    };

    MyAllocator() = default;
    ~MyAllocator()
    {
    }

    template <typename U, int M> 
    MyAllocator(const MyAllocator<U, M>&) {}

    T* allocate(std::size_t n)
    {
        if (n > N)
        {
            throw std::bad_alloc();
        }
        auto p = std::malloc(sizeof(T) * n);
        // std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T *p, std::size_t n)
    {
        UNUSED(n);
        // std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
        std::free(p);
    }

    template <typename U, typename ...Args>
    void construct(U *p, Args &&...args)
    {
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p) 
    {
        p->~T();
    }
};

template <typename T, typename U, int K>
using MyAllocatorMap = MyAllocator<std::pair<const T, const U>, K>;
template <typename T, int K>
using MyAllocatorVec = MyAllocator<T, K>;

#endif // MY_ALLOCATOR_H
