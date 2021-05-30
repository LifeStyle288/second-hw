#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <string>
#include <vector>

// #define PR(x) #x << " = " << x << ", "
// #define PE(x) #x << " = " << x

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
        if (m_ptr != nullptr)
        {
            std::free(m_ptr);
        }
        if (m_prev != nullptr)
        {
            std::free(m_prev);
        }
    }

    template <typename U, int M> 
    MyAllocator(const MyAllocator<U, M>&) {}

    T* allocate(std::size_t n)
    {
        // std::cout << PR(m_cnt) << PE(n) << std::endl;
        if (n > N)
        {
            throw std::bad_alloc();
        }
        void* p;
        if (m_cnt == 0)
        {
            alloc_mem();
            p = m_ptr;
        }
        else if ((m_cnt + n) <= N)
        {
            p = (void*)((uint8_t*)m_ptr + sizeof(T) * (m_cnt + n - 1));
        }
        else if (n <= N && m_cnt < N)
        {
            m_prev = m_ptr;
            alloc_mem();
            p = m_ptr;
            m_cnt = 0;
        }
        else
        {
            throw std::bad_alloc();
        }
        // std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
        m_cnt += n;
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T *p, std::size_t n)
    {
        UNUSED(n);
        // std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
        if (p == m_ptr)
        {
            m_cnt = 0;
            std::free(m_ptr);
            m_ptr = nullptr;
        }
        else if (p == m_prev)
        {
            std::free(m_prev);
            m_prev = nullptr;
        }
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
    void alloc_mem()
    {
        m_ptr = std::malloc(sizeof(T) * N);
        if (!m_ptr)
        {
            throw std::bad_alloc();
        }
    }
    size_t m_cnt = 0;
    void* m_ptr = nullptr;
    void* m_prev = nullptr;
};

template <typename T, typename U, int K>
using MyAllocatorMap = MyAllocator<std::pair<const T, const U>, K>;
template <typename T, int K>
using MyAllocatorVec = MyAllocator<T, K>;

#endif // MY_ALLOCATOR_H
