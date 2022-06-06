#pragma once
#include "KsiMain.hpp"

KSI_START

struct Vector2 {
   float x, y;

   Vector2() : x(0), y(0) {}
   Vector2(float _x, float _y) : x(_x), y(_y) {}

   inline bool operator==(const Vector2& other) {
      return
         x == other.x &&
         y == other.y;
   }
};

struct Vector3 {
   float x, y, z;

   Vector3() : x(0), y(0), z(0) {}
   Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

   inline bool operator==(const Vector3& other) {
      return
         x == other.x &&
         y == other.y &&
         z == other.z;
   }
};

KSI_END