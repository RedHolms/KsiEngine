#pragma once
#include "KsiMain.hpp"

#include <DirectXMath.h>

#include "Vector.hpp"

KSI_START

class Camera {
public:
   Camera(Vector3 pos, Vector3 rot = {0, 0, 0})
      : m_position(pos), m_forward({0, 0, -1}), m_rotation(rot) {}

private:
   Vector3 m_position;
   Vector3 m_forward;
   Vector3 m_rotation;

public:
   KSI_API void GetMatrix(DirectX::XMMATRIX* outM);
};

KSI_END