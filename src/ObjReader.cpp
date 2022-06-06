#include "ObjReader.hpp"

#include <stdio.h>

#include <windows.h>

#define _CurChar(s) (*s->p)
#define _Inc(s) (++s->p)
#define _IsEol(s) (_CurChar(s) == '\n')
#define _IsSpace(s) (_CurChar(s) == ' ' || _CurChar(s) == '\t')
#define _IsStrChar(s) ((_CurChar(s) >= 'A' && _CurChar(s) <= 'Z') || (_CurChar(s) >= 'a' && _CurChar(s) <= 'z'))
#define _BytesReaded(s) (size_t)(s->p - s->buff)
#define _AssertOrCorrupted(s, expr, info) (void)((!!(expr)) || (_KSI_NC_::ObjReader_Corrupted(s, info), 0))

KSI_NC_START

enum class ObjReader_token {
   VERTEX,
   TEXTURE_UV,
   NORMAL,
   FACE,
   OBJECT,
   SMOOTHING
};

static void ObjReader_Corrupted(_KSI_NC_::ObjReaderState* s, const char* info);

static void ObjReader_readFile(ObjReaderState* s) {
   FILE* f = fopen(s->filename, "rb");
   assert(f && "Can't open obj file. Does it exist?");
   fseek(f, 0, SEEK_END);
   s->buffSize = ftell(f);
   rewind(f);
   s->buff = new uint8_t[s->buffSize + 1];
   size_t readed = fread(s->buff, sizeof(uint8_t), s->buffSize, f);
   s->buffSize = readed;
   fclose(f);
}

static inline void ObjReader_skipSpaces(ObjReaderState* s) {
   while (_IsSpace(s)) _Inc(s);
}

static inline void ObjReader_skipLine(ObjReaderState* s) {
   while (_IsEol(s)) _Inc(s);
}

static inline void ObjReader_skipNonReadable(ObjReaderState* s) {
   while (_IsSpace(s) || _IsEol(s)) _Inc(s);
}

static inline int ObjReader_getLine(ObjReaderState* s) {
   int line = 0;
   uint8_t* p = s->buff;
   while (p != s->p) {
      while (*p != '\n') {
         if (p == s->p) break;
         ++p;
      }
      if (p != s->p) ++p;
      line++;
   }
   return line;
}

static inline int ObjReader_getLineChar(ObjReaderState* s) {
   uint8_t* p = s->p;
   uint8_t* lineStart = nullptr;
   while (*p != '\n') {
      --p;
   }
   lineStart = p+1;
   return (size_t)(s->p - lineStart);
}

static ObjReader_token ObjReader_getToken(ObjReaderState* s) {
   ObjReader_skipNonReadable(s);

   switch (_CurChar(s)) {
      case 'v': {
         _Inc(s);
         switch (_CurChar(s)) {
            case ' ':
               ObjReader_skipSpaces(s);
               return ObjReader_token::VERTEX;
            case 't':
               _Inc(s);
               _AssertOrCorrupted(s, _CurChar(s) == ' ', "Unknown token");
               ObjReader_skipSpaces(s);
               return ObjReader_token::TEXTURE_UV;
            case 'n':
               _Inc(s);
               _AssertOrCorrupted(s, _CurChar(s) == ' ', "Unknown token");
               ObjReader_skipSpaces(s);
               return ObjReader_token::NORMAL;
            default:
               break;
         }
      } break;
      case 's': {
         _Inc(s);
         _AssertOrCorrupted(s, _CurChar(s) == ' ', "Unknown token");
         ObjReader_skipSpaces(s);
         return ObjReader_token::SMOOTHING;
      } break;
      case 'f': {
         _Inc(s);
         _AssertOrCorrupted(s, _CurChar(s) == ' ', "Unknown token");
         ObjReader_skipSpaces(s);
         return ObjReader_token::FACE;
      } break;
      case 'o': {
         _Inc(s);
         _AssertOrCorrupted(s, _CurChar(s) == ' ', "Unknown token");
         ObjReader_skipSpaces(s);
         return ObjReader_token::OBJECT;
      } break;
      default:
         break;
   }
   ObjReader_Corrupted(s, "Unknown token");
}

static int ObjReader_getDigitsNumberInInt(int value) {
   size_t digits = value <= 0 ? 1 : 0;
   while (value > 0) value /= 10, digits++;
   return digits;
}

static int ObjReader_parseInt(ObjReaderState* s) {
   int result;
   sscanf((char*)s->p, "%i", &result);
   s->p += ObjReader_getDigitsNumberInInt(result);
   return result;
}

static float ObjReader_parseFloat(ObjReaderState* s) {
   float result;
   sscanf((char*)s->p, "%f", &result);
   s->p += 8;
   if (result < 0.0f) _Inc(s);
   return result;
}

static char* ObjReader_parseString(ObjReaderState* s) {
   uint8_t* start = s->p;
   while (_IsStrChar(s)) _Inc(s);
   size_t strSize = (size_t)(s->p - start);
   char* buff = new char[strSize + 1];
   buff[strSize] = 0;
   memcpy(buff, start, strSize);
   return buff;
}

static void ObjReader_start(ObjReaderState* s) {
   ObjReader_readFile(s);

   s->p = s->buff;
   while (_BytesReaded(s) < s->buffSize) {
      ObjReader_token token = ObjReader_getToken(s);

      switch (token) {
         case ObjReader_token::VERTEX: {
            ObjV v;
            v.x = ObjReader_parseFloat(s);
            ObjReader_skipSpaces(s);
            v.y = ObjReader_parseFloat(s);
            ObjReader_skipSpaces(s);
            v.z = ObjReader_parseFloat(s);
            s->result->vertices.push_back(v);
         } break;
         case ObjReader_token::TEXTURE_UV: {
            ObjVT vt;
            vt.u = ObjReader_parseFloat(s);
            ObjReader_skipSpaces(s);
            vt.v = ObjReader_parseFloat(s);
            s->result->uvs.push_back(vt);
         } break;
         case ObjReader_token::NORMAL: {
            ObjVN vn;
            vn.x = ObjReader_parseFloat(s);
            ObjReader_skipSpaces(s);
            vn.y = ObjReader_parseFloat(s);
            ObjReader_skipSpaces(s);
            vn.z = ObjReader_parseFloat(s);
            s->result->norms.push_back(vn);
         } break;
         case ObjReader_token::FACE: {
            while (!_IsEol(s)) {
               ObjFace face;
               face.vertexIdx = ObjReader_parseInt(s) - 1;
               _AssertOrCorrupted(s, _CurChar(s) == '/', "Expected '/'");
               _Inc(s);
               face.uvIdx = ObjReader_parseInt(s) - 1;
               _AssertOrCorrupted(s, _CurChar(s) == '/', "Expected '/'");
               _Inc(s);
               face.normIdx = ObjReader_parseInt(s) - 1;
               ObjReader_skipSpaces(s);
               s->result->faces.push_back(face);

               if (_BytesReaded(s) >= s->buffSize) break;
            }
         } break;
         case ObjReader_token::OBJECT: {
            ObjReader_skipSpaces(s);
            delete[] ObjReader_parseString(s);
         } break;
         case ObjReader_token::SMOOTHING: {
            (void)ObjReader_parseInt(s);
         } break;
      }
   }
}

static void ObjReader_cleanup(ObjReaderState* s) {
   delete[] s->buff;
   memset(s, 0, sizeof(*s));
}

ObjReaderResult ObjRead(const char* filename) {
   ObjReaderState state;
   _ClearStructure(state);
   ObjReaderResult result;
   _ClearStructure(result);

   state.filename = filename;
   state.result = &result;
   ObjReader_start(&state);
   ObjReader_cleanup(&state);

   return result;
}

static void ObjReader_Corrupted(_KSI_NC_::ObjReaderState* s, const char* info) {
   static char buff[512];
   snprintf(
      buff, _ConstArraySize(buff),
      "Can't load object file \"%s\".\n"
      "File corrupted or invalid: %s\n"
      "\n"
      "At line %i, char %i (%i)",
   s->filename, info, ObjReader_getLine(s), ObjReader_getLineChar(s), _BytesReaded(s) / sizeof(char));

   MessageBoxA(NULL, buff, "An object loading error", MB_ICONERROR);
   ExitProcess(3);
}

KSI_NC_END