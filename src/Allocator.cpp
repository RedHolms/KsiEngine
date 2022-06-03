#include "Allocator.hpp"

KSI_START

void* BasicAllocator::alloc(size_t size) {
   return new char[size];
}

void BasicAllocator::free(void* data) {
   delete[] data;
}

void* BasicAllocator::realloc(void* data, size_t newSize) {
   void* newData = this->alloc(newSize);
   memcpy(newData, data, newSize);
   this->free(data);
   return newData;
}

KSI_END