#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H

#include <cstddef>
#include <algorithm>
#include <memory>
#include <initializer_list>

template <typename T, typename Alloc = std::allocator<T>>
class SimpleVector
{
    using allocator_type = Alloc;
public:
    SimpleVector() :
        m_alloc(Alloc()),
        m_size(0),
        m_capacity(0),
        m_data(m_alloc.allocate(m_capacity))
    {}
    explicit SimpleVector(const size_t capacity) : 
        m_alloc(Alloc()),
        m_size(0), 
        m_capacity(capacity),
        m_data(m_alloc.allocate(m_capacity))
    {}
    SimpleVector(std::initializer_list<T> il) :
        m_alloc(Alloc()),
        m_size(0),
        m_capacity(il.size()),
        m_data(m_alloc.allocate(m_capacity))
    {
        typename std::initializer_list<T>::const_iterator it = il.begin();
        while (it < il.end())
        {
            push_back(*it);
            ++it;
        }
    }
    ~SimpleVector() 
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            m_alloc.destroy(m_data + i);
        }
        m_alloc.deallocate(m_data, m_size);
    }

    void reserve(const size_t capacity)
    {
        m_alloc.deallocate(m_data, m_size);
        m_capacity = capacity;
        m_data = m_alloc.allocate(m_capacity);
    }

    T& operator[](const size_t index) 
    {
        return m_data[index];
    }

    T* begin() 
    {
        return m_data;
    }
    T* end() 
    {
        return m_data + m_size;
    }

    const T* begin() const 
    {
        return m_data;
    }
    const T* end() const 
    {
        return m_data + m_size;
    }

    allocator_type get_allocator() const
    {
        return m_alloc;
    }

    size_t size() const
    {
        return m_size;
    }
    size_t capacity() const
    {
        return m_capacity;
    }
    void push_back(const T& value)
    {
        if (m_size >= m_capacity)
        {
            auto new_cap = m_capacity == 0 ? 1 : 2 * m_capacity;
            auto new_data = m_alloc.allocate(new_cap);
            for (size_t i = 0; i < m_size; ++i)
            {
                m_alloc.construct(new_data + i, *(m_data + i));
            }
            for (size_t i = 0; i < m_size; ++i)
            {
                m_alloc.destroy(m_data + i);
            }
            if (m_size > 0)
            {
                m_alloc.deallocate(m_data, m_size);
            }
            m_data = new_data;
            m_capacity = new_cap;
        }
        m_alloc.construct(m_data + m_size, value);
        ++m_size;
    }

private:
    Alloc m_alloc;
    size_t m_size = 0;
    size_t m_capacity = 0;
    T* m_data = nullptr;
};

#endif
