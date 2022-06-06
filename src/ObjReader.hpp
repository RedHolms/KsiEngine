#pragma once
#include "KsiMain.hpp"

#include "Array.hpp"

KSI_NC_START

struct ObjV {
   float x, y, z;
};

struct ObjVT {
   float u, v;
};

struct ObjVN {
   float x, y, z;
};

struct ObjFace {
   uint16_t vertexIdx;
   uint16_t uvIdx;
   uint16_t normIdx;
};

struct ObjReaderResult {
   Array<ObjV> vertices;
   Array<ObjVT> uvs;
   Array<ObjVN> norms;
   Array<ObjFace> faces;
};

struct ObjReaderState {
   ObjReaderResult* result;
   const char* filename;
   uint8_t* buff;
   uint8_t* p;
   size_t buffSize;
};

KSI_API ObjReaderResult ObjRead(const char* filename);

KSI_NC_END