#include "GameObject.hpp"

#include "Engine.hpp"
#include "Renderer.hpp"

KSI_START

GameObject::GameObject(const Mesh& mesh, const Texture& texture, const Vector3& position, const Vector3& rotation) 
   : m_mesh(mesh), m_texture(texture)
{
   _ClearStructure(m_dro);

   m_position = position;
   m_rotation = rotation;
}

GameObject::~GameObject() {
   _SafeRelease(m_dro.buff);
}

const _KSI_RENDER_::DrawObject& GameObject::GetDrawObject() {
   ID3D11Device* d3ddevice = Engine::Get().GetRenderer()->GetDevice();

   _SafeRelease(m_dro.buff);

   m_dro.vertices = m_mesh.GetVertices();
   m_dro.indices = m_mesh.GetIndices();
   m_dro.indicesCount = m_mesh.GetIndicesCount();
   m_dro.textureView = m_texture.GetTextureView();
   
   D3D11_SUBRESOURCE_DATA subresourceData;
   D3D11_BUFFER_DESC objectBufferDesc;
   _ClearStructure(objectBufferDesc);
   objectBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   objectBufferDesc.ByteWidth = sizeof(_KSI_RENDER_::ObjectBufferData);
   objectBufferDesc.CPUAccessFlags = 0;
   objectBufferDesc.Usage = D3D11_USAGE_DEFAULT;

   _KSI_RENDER_::ObjectBufferData objData;
   objData.position = DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 0.0f);
   objData.objectMatrix =
      // DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z) *
      DirectX::XMMatrixRotationRollPitchYaw(
         DirectX::XMConvertToRadians(m_rotation.x),
         DirectX::XMConvertToRadians(m_rotation.y),
         DirectX::XMConvertToRadians(m_rotation.z)
      );
   subresourceData.pSysMem = &objData;

   _DirectXAssert(
      d3ddevice->CreateBuffer(&objectBufferDesc, &subresourceData, &m_dro.buff)
   );

   return m_dro;
}

KSI_END