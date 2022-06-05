#ifndef KsiEngine_KsiMain_hpp_
#define KsiEngine_KsiMain_hpp_

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

#if !defined(_HRESULT_DEFINED)
   #define _HRESULT_DEFINED
   typedef long HRESULT;
#endif

#if !defined(STB_IMAGE_IMPLEMENTATION)
   #define STB_IMAGE_IMPLEMENTATION
#endif

#define _KSI KsiEngine
#define _KSI_RENDER Render
#define _KSI_MATH Math

#define _KSI_ ::_KSI
#define _KSI_RENDER_ _KSI_::_KSI_RENDER
#define _KSI_MATH_ _KSI_::_KSI_MATH

#define KSI_START namespace _KSI {
#define KSI_END }

#define KSI_RENDER_START KSI_START namespace _KSI_RENDER {
#define KSI_RENDER_END } KSI_END

#define KSI_MATH_START KSI_START namespace _KSI_MATH {
#define KSI_MATH_END } KSI_END

#if defined(_MSC_BUILD)
   #define KsiInterface __interface
   #define KsiInterface_begin public:
   #define KsiInterface_method_ virtual
   #define _KsiInterface_method = 0
#else
   #define KsiInterface struct
   #define KsiInterface_begin
   #define KsiInterface_method_ virtual
   #define _KsiInterface_method = 0
#endif

#define EMPTY_FUNCTION {}

#if !defined(_XM_NO_INTRINSICS_)
   #define _XM_NO_INTRINSICS_
#endif

template <typename Base, typename Derived>
struct is_base {
   constexpr static bool check(Base*)  { return true; }
   constexpr static bool check(...)    { return false; }
   enum : bool { value = check(static_cast<Derived*>(0)) };
}; 

template <typename Type>
struct is_pointer {
   constexpr static bool check(Type)  { return true; }
   constexpr static bool check(...)   { return false; }
   enum : bool { value = check(nullptr) };
};

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

KSI_API void DirectXAssertImpl(HRESULT hr, const char* expr, const char* file, uint32_t line);

/* some utils */
#define _ConstArraySize(p) (sizeof(p)/sizeof(*p))
#define _ClearMemory(p, s) memset(p, 0, s)
#define _ClearStructure(v) _ClearMemory(&v, sizeof(v))
#define _SafeRelease(p) if (p) p->Release(), p = nullptr
#define _SafeDelete(p) if (p) delete p, p = nullptr
#define _RectWidth(r) (r.right - r.left)
#define _RectHeight(r) (r.bottom - r.top)
#define _UnconditionalCast(t, v) (*((t*)&(v)))
#define _MemberOffset(s, m) ((uintptr_t)&(((s*)0)->m))
#define _Max(a, b) (a >= b ? a : b)
#define _Min(a, b) (a >= b ? b : a)
#define _Clamp(n, mi, ma) (_Max(_Min(n, ma), mi))
#define _UnpackVec2(v) v.x, v.y
#define _UnpackVec3(v) v.x, v.y, v.z
#define _UnpackVec4(v) v.x, v.y, v.z, v.w
#define _GetFPS(dt) (1.0f/dt)
#define _GetDeltaTime(fps) (1.0f/fps)
#define _DirectXAssert(expr) DirectXAssertImpl(expr, #expr, __FILE__, __LINE__)

#endif // KsiEngine_KsiMain_hpp_