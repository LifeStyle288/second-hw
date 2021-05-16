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
    ~MyAllocator() = default;

    template <typename U, int M> 
    MyAllocator(const MyAllocator<U, M>&) {}

    T* allocate(std::size_t n)
    {
        UNUSED(n);
        auto p = std::malloc(sizeof(T) * N);
        if (!p)
        {
            throw std::bad_alloc();
        }
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T *p, std::size_t n)
    {
        UNUSED(n);
        std::free(p);
    }

    std::size_t max_size() const
    {
        return N;
    }

    template <typename U, typename ...Args>
    void construct(U *p, Args &&...args)
    {
        // std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p) 
    {
        // std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
        p->~T();
    }
};

template <typename T, typename U, int K>
using MyAllocatorMap = MyAllocator<std::pair<const T, const U>, K>;
template <typename T, int K>
using MyAllocatorVec = MyAllocator<T, K>;

#endif // MY_ALLOCATOR_H
