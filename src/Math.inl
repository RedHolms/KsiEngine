#pragma once
#include "KsiMain.hpp"
#include "Math.hpp"

#include <math.h>

KSI_MATH_START

inline float Vector2Length(const Vector2& vec) {
   return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

inline Vector2 Vector2Normalize(const Vector2& vec) {
   float l = Vector2Length(vec);
   if (l == 0) return Vector2(0.0f, 0.0f);
   
   return Vector2(
      vec.x / l,
      vec.y / l
   );
}

inline float Vector3Length(const Vector3& vec) {
   return sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

inline Vector3 Vector3Normalize(const Vector3& vec) {
   float l = Vector3Length(vec);
   return Vector3(
      vec.x / l,
      vec.y / l,
      vec.z / l
   );
}

inline Vector3 Vector3Cross(const Vector3& a, const Vector3& b) {
   return Vector3(
      a.z * b.y - a.y * b.z,
      a.x * b.z - a.z * b.x,
      a.y * b.x - a.x * b.y
   );
}

inline Vector3 EulerNormalize(const Vector3& rot) {
   Vector3 result(rot);
   // We can't get mod, because it's a float
   while (result.x >= 360.0f) result.x -= 360.0f;
   while (result.y >= 360.0f) result.y -= 360.0f;
   while (result.z >= 360.0f) result.z -= 360.0f;
   return result;
}

inline Vector3 MatrixGetForwardVector(const DirectX::XMMATRIX& M) {
   return Vector3(
      -M.m[2][0],
      -M.m[2][1],
      -M.m[2][2]
   );
}

KSI_MATH_END