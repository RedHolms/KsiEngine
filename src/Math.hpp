#pragma once
#include "KsiMain.hpp"

#include <DirectXMath.h>

#include "Vector.hpp"

/* operators */
inline _KSI_::Vector2 operator-(const _KSI_::Vector2& a, const _KSI_::Vector2& b) {
   return _KSI_::Vector2(
      a.x - b.x,
      a.y - b.y
   );
}

inline _KSI_::Vector2 operator*(const _KSI_::Vector2& vec, float scaler) {
   return _KSI_::Vector2(
      vec.x * scaler,
      vec.y * scaler
   );
}

inline _KSI_::Vector2 operator/(const _KSI_::Vector2& vec, float scaler) {
   return _KSI_::Vector2(
      vec.x / scaler,
      vec.y / scaler
   );
}

inline _KSI_::Vector3 operator-(const _KSI_::Vector3& vec) {
   return _KSI_::Vector3(
      -vec.x,
      -vec.y,
      -vec.z
   );
}

inline _KSI_::Vector3& operator+=(_KSI_::Vector3& a, const _KSI_::Vector3& b) {
   a.x += b.x;
   a.y += b.y;
   a.z += b.z;
   return a;
}

inline _KSI_::Vector3& operator-=(_KSI_::Vector3& a, const _KSI_::Vector3& b) {
   a.x -= b.x;
   a.y -= b.y;
   a.z -= b.z;
   return a;
}

inline _KSI_::Vector3 operator*(const _KSI_::Vector3& vec, float scaler) {
   return _KSI_::Vector3(
      vec.x * scaler,
      vec.y * scaler,
      vec.z * scaler
   );
}

inline _KSI_::Vector3 operator/(const _KSI_::Vector3& vec, float scaler) {
   return _KSI_::Vector3(
      vec.x / scaler,
      vec.y / scaler,
      vec.z / scaler
   );
}

KSI_MATH_START

inline float Vector2Length(const Vector2& vec);
inline Vector2 Vector2Normalize(const Vector2& vec);

inline float Vector3Length(const Vector3& vec);
inline Vector3 Vector3Normalize(const Vector3& vec);
inline Vector3 Vector3Cross(const Vector3& a, const Vector3& b);

inline Vector3 EulerNormalize(const Vector3& rot);

inline Vector3 MatrixGetForwardVector(const DirectX::XMMATRIX& M);

KSI_MATH_END

#include "Math.inl"