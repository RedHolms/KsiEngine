#pragma once
#include "KsiMain.hpp"

#include "Renderer.hpp"
#include "Vector.hpp"

KSI_START

class GameObject {
public:
   KSI_API GameObject(const Vector3& position, const Vector3& rotation);
   KSI_API ~GameObject();

private:
   Vector3 m_position;
   Vector3 m_rotation;
   _KSI_RENDER_::DrawObject m_dro;

public:
   Vector3& GetRotation() { return m_rotation; }

   KSI_API const _KSI_RENDER_::DrawObject& GetDrawObject();
};

KSI_END