#pragma once
#include "KsiMain.hpp"

KSI_START

KsiInterface IAllocator {
KsiInterface_begin

   KsiInterface_method_
      void* alloc(size_t size)
   _KsiInterface_method;

   KsiInterface_method_
      void free(void* data)
   _KsiInterface_method;

   KsiInterface_method_
      void* realloc(void* data, size_t newSize)
   _KsiInterface_method;
};

class BasicAllocator : public IAllocator {
public:
   KSI_API void* alloc(size_t size);
   KSI_API void free(void* data);
   KSI_API void* realloc(void* data, size_t newSize);
};

template <class _Ty>
using is_allocator = is_base<IAllocator, _Ty>;

KSI_END