#ifndef QUEUEBUFFER_H
#define QUEUEBUFFER_H

#include <cstddef>
#include <exception>
#include <cstdlib>
#include <cstring>

template<typename T>
class QueueBuffer
{
private:
    T* m_buffer;
    size_t m_buffer_size, m_read_pos, m_write_pos;

public:
    inline QueueBuffer()
    {
        m_buffer_size = 1024;
        m_buffer = (T*) malloc(sizeof (T) * m_buffer_size);
        if (m_buffer == NULL)
            throw std::bad_alloc();
        m_read_pos = 0;
        m_write_pos = 0;
    }
    inline ~QueueBuffer()
    {
        free(m_buffer);
    }
    inline T* Reserve(size_t count)
    {
        if (m_write_pos + count > m_buffer_size) {
            size_t new_buffer_size = (m_write_pos - m_read_pos + count) * 2;
            if (new_buffer_size > m_buffer_size) {
                T* new_buffer = (T*) malloc(sizeof(T) * new_buffer_size);
                if (new_buffer == NULL)
                    throw std::bad_alloc();
                memcpy(m_buffer, m_buffer + m_read_pos, sizeof(T) * (m_write_pos - m_read_pos));
                free(m_buffer);
                m_buffer = new_buffer;
                m_buffer_size = new_buffer_size;
            } else {
                memmove(m_buffer, m_buffer + m_read_pos, sizeof(T) * (m_write_pos - m_read_pos));
            }
            m_write_pos -= m_read_pos;
            m_read_pos = 0;
        }
        return m_buffer + m_write_pos;
    }
    inline void Push(size_t count)
    {
        assert(m_write_pos + count <= m_buffer_size);
        m_write_pos += count;
    }
    inline void Push(const T* data, size_t count)
    {
        T* target = Reserve(count);
        memcpy(target, data, sizeof(T) * count);
        Push(count);
    }
    inline void Pop(size_t count)
    {
        assert(m_read_pos + count <= m_write_pos);
        m_read_pos += count;
        if (m_read_pos == m_write_pos) {
            m_read_pos = 0;
            m_write_pos = 0;
        }
    }
    inline void Pop(T* data, size_t count)
    {
        assert(count <= GetSize());
        memcpy(data, GetData(), sizeof(T) * count);
        Pop(count);
    }
    inline void Clear()
    {
        m_read_pos = 0;
        m_write_pos = 0;
    }

public:
    inline T* GetData() {return m_buffer + m_read_pos; }
    inline size_t GetSize() {return m_write_pos - m_read_pos; }
    inline bool isEmpty() {return (m_read_pos == m_write_pos);}
    inline T& operator[](size_t i) {return m_buffer[m_read_pos + i]; }

    //nocopyable
    QueueBuffer(const QueueBuffer&) = delete;
    QueueBuffer& operator=(const QueueBuffer&) = delete;
};



#endif // QUEUEBUFFER_H
