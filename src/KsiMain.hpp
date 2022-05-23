#pragma once

#if !defined(__cplusplus)
   #error "Ksi engine is cpp-only"
#endif

#if defined(KSI_BUILD)
   #if defined(KSI_STATIC)
      #define KSI_API
   #else
      #define KSI_API __declspec(dllexport)
   #endif
#else
   #if defined(KSI_STATIC)
      #define KSI_API
   #else
      #define KSI_API __declspec(dllimport)
   #endif
#endif

#define _KSI_ KsiEngine

#define KSI_START namespace _KSI_ {
#define KSI_END }

#define KSI_RENDER_START KSI_START namespace Render {
#define KSI_RENDER_END } KSI_END

/* some utils */
#define _ConstArraySize(p) (sizeof(p)/sizeof(*p))