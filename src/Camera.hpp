#pragma once
#include "KsiMain.hpp"

#include <DirectXMath.h>

#include "Vector.hpp"
#include "Math.hpp"

KSI_START

class Camera {
public:
   KSI_API Camera(const Vector3& position, const Vector3& rotation);

private:
   Vector3 m_position;
   Vector3 m_rotation;

public:
   Vector3& GetRotation()                          { return m_rotation; }
   void     SetRotation(const Vector3& rotation)   { m_rotation = _KSI_MATH_::EulerNormalize(rotation); }
   
   Vector3& GetPosition()                          { return m_position; }
   void     SetPosition(const Vector3& position)   { m_position = position; }

   KSI_API Vector3 GetForwardDirection();
   KSI_API DirectX::XMMATRIX GetViewMatrix();
};

KSI_END