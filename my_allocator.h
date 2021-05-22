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
        void* p;
        if (m_cnt == 0)
        {
            m_ptr = std::malloc(sizeof(T) * N);
            if (!m_ptr)
            {
                throw std::bad_alloc();
            }
            p = m_ptr;
        }
        else if (m_cnt < N)
        {
            p = (void*)((uint8_t*)m_ptr + sizeof(T) * m_cnt);
        }
        else
        {
            throw std::bad_alloc();
        }
        std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
        ++m_cnt;
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T *p, std::size_t n)
    {
        UNUSED(n);
        std::free(p);
        m_cnt = 0;
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
private:
    void* m_ptr;
    size_t m_cnt = 0;
};

template <typename T, typename U, int K>
using MyAllocatorMap = MyAllocator<std::pair<const T, const U>, K>;
template <typename T, int K>
using MyAllocatorVec = MyAllocator<T, K>;

#endif // MY_ALLOCATOR_H
