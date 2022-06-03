#pragma once
#include "KsiMain.hpp"

#include "Allocator.hpp"

KSI_START

template <class _Ty, class _Allocator = BasicAllocator>
class Array {
   static_assert(is_allocator<_Allocator>::value, "Template argument '_Allocator' is required to be an allocator");

public:
   Array()
      : m_items(nullptr), m_reserved(10), m_count(0)
   {
      _realloc();
   }

   Array(size_t res)
      : m_items(nullptr), m_reserved(res), m_count(0)
   {
      _realloc();
   }

   Array(const _Ty* items, size_t count, size_t res = count + 10)
      : m_items(nullptr), m_reserved(res), m_count(count)
   {
      _realloc();
      memcpy(m_items, items, count * sizeof(_Ty));
   }

   ~Array() {
      _free();
   }

private:
   _Allocator m_allocator;
   size_t m_reserved;
   size_t m_count;
   _Ty* m_items;

public:
   void push_back(const _Ty& item) {
      push_back(&item, 1);
   }
   void push_back(const _Ty* items, size_t count) {
      if (m_reserved <= m_count + count) {
         m_reserved += count + 10;
         _realloc();
      }

      memcpy(m_items + m_count, items, count * sizeof(_Ty));
      m_count += count;
   }
   void pop_back(size_t count = 1) {
      m_count -= count <= m_count ? count : m_count;
   }
   void clear() {
      m_count = 0;
   }

   size_t count() { return m_count; }
   _Ty* data() { return m_items; }

   _Ty& operator[](size_t idx) { return m_items[idx]; }
private:
   void _free() {
      if (m_items)
         m_allocator.free(m_items), m_items = nullptr;
      m_count = 0;
   }
   void _realloc() {
      if (m_reserved == 0)
         _free();
      else {
         if (m_items)
            m_items = (_Ty*) m_allocator.realloc(m_items, m_reserved * sizeof(_Ty));
         else
            m_items = (_Ty*) m_allocator.alloc(m_reserved * sizeof(_Ty));
      }
   }
};

KSI_END