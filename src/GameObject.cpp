#include "GameObject.hpp"

#include "Engine.hpp"
#include "Renderer.hpp"

static const _KSI_RENDER_::Vertex _vertices[] = {
   { _KSI_::Vector3(-1.0f, -1.0f, -1.0f), _KSI_::Vector3(0.0f, 0.0f, 0.0f) },
   { _KSI_::Vector3(-1.0f,  1.0f, -1.0f), _KSI_::Vector3(0.0f, 1.0f, 0.0f) },
   { _KSI_::Vector3( 1.0f,  1.0f, -1.0f), _KSI_::Vector3(1.0f, 1.0f, 0.0f) },
   { _KSI_::Vector3( 1.0f, -1.0f, -1.0f), _KSI_::Vector3(1.0f, 0.0f, 0.0f) },
   { _KSI_::Vector3(-1.0f, -1.0f,  1.0f), _KSI_::Vector3(0.0f, 0.0f, 1.0f) },
   { _KSI_::Vector3(-1.0f,  1.0f,  1.0f), _KSI_::Vector3(0.0f, 1.0f, 1.0f) },
   { _KSI_::Vector3( 1.0f,  1.0f,  1.0f), _KSI_::Vector3(1.0f, 1.0f, 1.0f) },
   { _KSI_::Vector3( 1.0f, -1.0f,  1.0f), _KSI_::Vector3(1.0f, 0.0f, 1.0f) }
};
static const uint16_t _indices[] = {
   0, 1, 2,
   0, 2, 3,
   4, 6, 5,
   4, 7, 6,
   4, 5, 1,
   4, 1, 0,
   3, 2, 6,
   3, 6, 7,
   1, 5, 6,
   1, 6, 2,
   4, 0, 3,
   4, 3, 7
};

KSI_START

GameObject::GameObject(const Vector3& position, const Vector3& rotation) {
   _ClearStructure(m_dro);

   m_position = position;
   m_rotation = rotation;
}

GameObject::~GameObject() {
   _SafeRelease(m_dro.vertices);
   _SafeRelease(m_dro.indices);
   _SafeRelease(m_dro.buff);
}

const _KSI_RENDER_::DrawObject& GameObject::GetDrawObject() {
   _KSI_RENDER_::Renderer* renderer = Engine::Get().GetRenderer();
   ID3D11Device* d3ddevice = renderer->GetDevice();

   _SafeRelease(m_dro.vertices);
   _SafeRelease(m_dro.indices);
   _SafeRelease(m_dro.buff);

   D3D11_SUBRESOURCE_DATA subresourceData;
   _ClearStructure(subresourceData);

   D3D11_BUFFER_DESC vertexBufferDesc;
   _ClearStructure(vertexBufferDesc);
   vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   vertexBufferDesc.ByteWidth = sizeof(_vertices);
   vertexBufferDesc.CPUAccessFlags = 0;
   vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

   subresourceData.pSysMem = _vertices;

   assert(SUCCEEDED(
      d3ddevice->CreateBuffer(&vertexBufferDesc, &subresourceData, &m_dro.vertices)
   ));

   D3D11_BUFFER_DESC indexBufferDesc;
   _ClearStructure(indexBufferDesc);
   indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
   indexBufferDesc.ByteWidth = sizeof(_indices);
   indexBufferDesc.CPUAccessFlags = 0;
   indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

   subresourceData.pSysMem = _indices;

   assert(SUCCEEDED(
      d3ddevice->CreateBuffer(&indexBufferDesc, &subresourceData, &m_dro.indices)
   ));

   D3D11_BUFFER_DESC objectBufferDesc;
   _ClearStructure(objectBufferDesc);
   objectBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   objectBufferDesc.ByteWidth = sizeof(_KSI_RENDER_::ObjectBufferData);
   objectBufferDesc.CPUAccessFlags = 0;
   objectBufferDesc.Usage = D3D11_USAGE_DEFAULT;

   _KSI_RENDER_::ObjectBufferData objData;
   objData.objectMatrix =
      DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z)
      *
      DirectX::XMMatrixRotationRollPitchYaw(
         DirectX::XMConvertToRadians(m_rotation.x),
         DirectX::XMConvertToRadians(m_rotation.y),
         DirectX::XMConvertToRadians(m_rotation.z)
      );
   subresourceData.pSysMem = &objData;

   assert(SUCCEEDED(
      d3ddevice->CreateBuffer(&objectBufferDesc, &subresourceData, &m_dro.buff)
   ));

   m_dro.indicesCount = _ConstArraySize(_indices);

   return m_dro;
}

KSI_END