#pragma once
#include "KsiMain.hpp"

#include "Renderer.hpp"
#include "Texture.hpp"
#include "Vector.hpp"
#include "Mesh.hpp"
#include "Math.hpp"

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
   Vector3& GetRotation()                          { return m_rotation; }
   void     SetRotation(const Vector3& rotation)   { m_rotation = _KSI_MATH_::EulerNormalize(rotation); }

   Vector3& GetPosition()                          { return m_position; }
   void     SetPosition(const Vector3& position)   { m_position = position; }

   Texture& GetTexture()                           { return m_texture; }
   void     SetTexture(const Texture& texture)     { m_texture = texture; }

   Mesh&    GetMesh()                              { return m_mesh; }
   void     SetMesh(const Mesh& mesh)              { m_mesh = mesh; }

   KSI_API const _KSI_RENDER_::DrawObject& GetDrawObject();
};

KSI_END