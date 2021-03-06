#pragma once
#include "KsiMain.hpp"

KSI_START

template <class _Ty>
class Array {
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

   Array(const _Ty* items, size_t count, size_t res = -1)
      : m_items(nullptr), m_reserved(res == -1 ? count + 10 : res), m_count(count)
   {
      _realloc();
      memcpy(m_items, items, count * sizeof(_Ty));
   }

   Array(const Array& other) {
      operator=(other);
   }

   ~Array() {
      _free();
   }

   const Array& operator=(const Array& other) {
      m_items = nullptr;
      m_reserved = other.m_reserved;
      m_count = other.m_count;
      _realloc();
      memcpy(m_items, other.m_items, other.m_count * sizeof(_Ty));
      return other;
   }

private:
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

      memcpy(end(), items, count * sizeof(_Ty));
      m_count += count;
   }
   void pop_back(size_t count = 1) {
      m_count -= count <= m_count ? count : m_count;
   }
   void clear() {
      m_count = 0;
   }

   size_t find(const _Ty& v) {
      for (size_t i = 0; i < m_count; i++)
         if (m_items[i] == v) return i;
      return -1;
   }

   _Ty* begin() { return m_items; }
   _Ty* end() { return m_items + m_count; }

   size_t count() { return m_count; }
   _Ty* data() { return m_items; }

   _Ty& operator[](size_t idx) { return m_items[idx]; }

private:
   void _free() {
      if (m_items) {
         _SafeDelete(m_items);
      }
      m_count = 0;
   }
   void _realloc() {
      if (m_reserved == 0)
         _free();
      else {
         if (m_items) {
            _Ty* temp = new _Ty[m_reserved];
            memcpy(temp, m_items, m_count * sizeof(_Ty));
            delete[] m_items;
            m_items = temp;
         } else
            m_items = new _Ty[m_reserved];
      }
   }
};

KSI_END