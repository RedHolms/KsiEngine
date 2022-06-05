#pragma once
#include "KsiMain.hpp"

#include "Renderer.hpp"
#include "Texture.hpp"
#include "Vector.hpp"
#include "Mesh.hpp"

KSI_START

class GameObject {
public:
   KSI_API GameObject(const Mesh& mesh, const Texture& texture, const Vector3& position, const Vector3& rotation);
   KSI_API ~GameObject();

private:
   Vector3 m_position;
   Vector3 m_rotation;
   Texture m_texture;
   Mesh m_mesh;
   _KSI_RENDER_::DrawObject m_dro;

public:
   Vector3& GetRotation() { return m_rotation; }

   KSI_API const _KSI_RENDER_::DrawObject& GetDrawObject();
};

KSI_END