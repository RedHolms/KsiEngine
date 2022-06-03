#include "Camera.hpp"

#include <DirectXMath.h>

#include "Vector.hpp"

KSI_START

Camera::Camera(const Vector3& position, const Vector3& rotation) {
   m_position = position;
   m_rotation = rotation;
}

Vector3 Camera::GetForwardDirection() {
   DirectX::XMVECTOR xvec = DirectX::XMVector3Rotate(
      DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f),
      DirectX::XMQuaternionRotationRollPitchYaw(
         DirectX::XMConvertToRadians(m_rotation.x),
         DirectX::XMConvertToRadians(m_rotation.y),
         DirectX::XMConvertToRadians(m_rotation.z)
      )
   );
   return Vector3(
      xvec.vector4_f32[0],
      xvec.vector4_f32[1],
      xvec.vector4_f32[2]
   );
}

DirectX::XMMATRIX Camera::GetViewMatrix() {
   return 
      DirectX::XMMatrixLookToLH(
         DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 1.0f),
         (
            DirectX::XMVector3Rotate(
               DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f),
               DirectX::XMQuaternionRotationRollPitchYaw(
                  DirectX::XMConvertToRadians(m_rotation.x),
                  DirectX::XMConvertToRadians(m_rotation.y),
                  DirectX::XMConvertToRadians(m_rotation.z)
               )
            )
         ),
         DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
      );
      // DirectX::XMMatrixLookToLH(
      //    DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 0.0f),
      //    DirectX::XMVector3Rotate(
      //       DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),
      //       DirectX::XMQuaternionRotationRollPitchYaw(
      //          DirectX::XMConvertToRadians(m_rotation.x),
      //          DirectX::XMConvertToRadians(m_rotation.y),
      //          DirectX::XMConvertToRadians(m_rotation.z)
      //       )
      //    ),
      //    DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
      // );
}

KSI_END