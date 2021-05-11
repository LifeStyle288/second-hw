#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <string>
#include <vector>

#define UNUSED(val) (void)val

template <typename T, int N>
class MyAllocator
{
    struct Chunk
    {
        char data[sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*)];
    };
public:
    using value_type = T;

    template <typename U>
    struct rebind
    {
        using other = MyAllocator<U, N>;
    };

    MyAllocator() : m_buffer(N), m_next(nullptr)
    {
        CreateDependencies();
    }
    ~MyAllocator() = default;

    template <typename U, int M> 
    MyAllocator(const MyAllocator<U, M>&) : m_buffer(N), m_next(nullptr)
    {
        CreateDependencies();
    }

    T* allocate(std::size_t n)
    {
        UNUSED(n);
        // std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
        if (!m_next)
        {
            throw std::bad_alloc();
        }
        void* next = m_next;
        m_next = Convert(*m_next);
        return reinterpret_cast<T*>(next);
    }

    void deallocate(T *p, std::size_t n)
    {
        UNUSED(n);
        // std::cout << __PRETTY_FUNCTION__ << "[p = " << p << "]" << std::endl;
        Chunk* chunk = reinterpret_cast<Chunk*>(p);
        Convert(*chunk) = m_next;
        m_next = chunk;
        p = reinterpret_cast<T*>(Convert(*m_next));
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
private:
    Chunk*& Convert(Chunk& chunk) const
    {
        return reinterpret_cast<Chunk*&>(chunk.data);
    }

    void CreateDependencies()
    {
        // build list
        for (size_t i = 1; i < m_buffer.size(); ++i)
        {
            // std::cout << "[m_buffer[i] = " << (int*)&m_buffer[i].data[0] << "]" << std::endl;
            Convert(m_buffer[i - 1]) = &m_buffer[i];
        }

        m_next = &m_buffer[0];
    }

    std::vector<Chunk> m_buffer;
    Chunk* m_next;
};

#endif // MY_ALLOCATOR_H
