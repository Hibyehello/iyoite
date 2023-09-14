#pragma once

#include <cstddef>
#include <memory>
#include <new>

class ArenaAllocator {
public:
  inline ArenaAllocator(size_t bytes) : m_size(bytes) {
    m_buf = static_cast<std::byte *>(operator new(bytes));
    m_offset = m_buf;
  }
  ~ArenaAllocator() { operator delete(m_buf); }

  template <typename T> inline T *alloc() {
    void *offset = m_offset;
    m_offset += sizeof(T);
    return (T *)offset;
  }

private:
  size_t m_size;
  std::byte *m_buf;
  std::byte *m_offset;
};