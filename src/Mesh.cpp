#include "Mesh.hpp"

#include <stdio.h>

#include "ObjReader.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "Vector.hpp"

KSI_START

Mesh::Mesh(const _KSI_RENDER_::Vertex* vertices, size_t verticesCount, const uint16_t* indices, size_t indicesCount)
   : m_vertices(nullptr), m_indices(nullptr), m_indicesCount(0)
{
   m_rawVertices = Array<_KSI_RENDER_::Vertex>(vertices, verticesCount);
   m_rawIndices = Array<uint16_t>(indices, indicesCount);
   UpdateBuffers();
}

Mesh::~Mesh() {
   _SafeRelease(m_vertices);
   _SafeRelease(m_indices);
}

void Mesh::UpdateBuffers() {
   ID3D11Device1* d3ddevice = Engine::Get().GetRenderer()->GetDevice();

   _SafeRelease(m_vertices);
   _SafeRelease(m_indices);
   
   D3D11_SUBRESOURCE_DATA subresourceData;
   _ClearStructure(subresourceData);

   D3D11_BUFFER_DESC vertexBufferDesc;
   _ClearStructure(vertexBufferDesc);
   vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   vertexBufferDesc.ByteWidth = m_rawVertices.count() * sizeof(_KSI_RENDER_::Vertex);
   vertexBufferDesc.CPUAccessFlags = 0;
   vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

   subresourceData.pSysMem = m_rawVertices.data();

   _DirectXAssert(
      d3ddevice->CreateBuffer(&vertexBufferDesc, &subresourceData, &m_vertices)
   );

   D3D11_BUFFER_DESC indexBufferDesc;
   _ClearStructure(indexBufferDesc);
   indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
   indexBufferDesc.ByteWidth = m_rawIndices.count() * sizeof(uint16_t);
   indexBufferDesc.CPUAccessFlags = 0;
   indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

   subresourceData.pSysMem = m_rawIndices.data();

   _DirectXAssert(
      d3ddevice->CreateBuffer(&indexBufferDesc, &subresourceData, &m_indices)
   );

   m_indicesCount = m_rawIndices.count();
}

void Mesh::Scale(float scaler) {
   for (auto& vertex : m_rawVertices)
      vertex.pos *= scaler;
}

void Mesh::Move(const Vector3& offset) {
   for (auto& vertex : m_rawVertices)
      vertex.pos += offset;
}

const Mesh& Mesh::operator=(const Mesh& other) {
   m_rawVertices = other.m_rawVertices;
   m_rawIndices = other.m_rawIndices;
   UpdateBuffers();
   return other;
}

/* static */
Mesh Mesh::BOX(int sideSize) {
   float half = sideSize / 2.0f;
   static const _KSI_RENDER_::Vertex _vertices[] = {
      // right side
      { Vector3(-half,  half,  half), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f) }, // 0
      { Vector3(-half, -half,  half), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f) }, // 1
      { Vector3( half,  half,  half), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f) }, // 2
      { Vector3( half, -half,  half), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f) }, // 3

      // back side
      { Vector3( half,  half,  half), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) }, // 4
      { Vector3( half, -half,  half), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) }, // 5
      { Vector3( half,  half, -half), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) }, // 6
      { Vector3( half, -half, -half), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) }, // 7

      // left side
      { Vector3( half,  half, -half), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f) }, // 8
      { Vector3( half, -half, -half), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f) }, // 9
      { Vector3(-half,  half, -half), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f) }, // 10
      { Vector3(-half, -half, -half), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f) }, // 11

      // front side
      { Vector3(-half,  half, -half), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f) }, // 12
      { Vector3(-half, -half, -half), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f) }, // 13
      { Vector3(-half,  half,  half), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f) }, // 14
      { Vector3(-half, -half,  half), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f) }, // 15

      // top side
      { Vector3(-half,  half, -half), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f) }, // 16
      { Vector3(-half,  half,  half), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f) }, // 17
      { Vector3( half,  half, -half), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) }, // 18
      { Vector3( half,  half,  half), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f) }, // 19

      // bottom side
      { Vector3( half, -half,  half), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) }, // 20
      { Vector3( half, -half, -half), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) }, // 21
      { Vector3(-half, -half, -half), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f) }, // 22
      { Vector3(-half, -half,  half), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) }, // 23
   };
   static const uint16_t _indices[] = {
      // right side
      0, 1, 2,
      3, 2, 1,

      // back side
      4, 5, 6,
      7, 6, 5,

      // left side
      8, 9, 10,
      11, 10, 9,

      // front side
      12, 13, 14,
      15, 14, 13,

      // top side
      18, 16, 19,
      17, 19, 16,

      // bottom side
      20, 23, 21,
      22, 21, 23
   };
   return Mesh(
      _vertices,
      _ConstArraySize(_vertices),
      _indices,
      _ConstArraySize(_indices)
   );
}

/* static */
Mesh Mesh::PLATE(int width, int length) {
   float halfw = width / 2.0f;
   float halfl = length / 2.0f;
   static const _KSI_RENDER_::Vertex _vertices[] = {
      { Vector3(-halfw, 0,  halfl), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) },
      { Vector3( halfw, 0, -halfl), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) },
      { Vector3(-halfw, 0, -halfl), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f) },
      { Vector3( halfw, 0,  halfl), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f) }
   };
   static const uint16_t _indices[] = {
      0, 1, 2,
      0, 3, 1
   };
   return Mesh(
      _vertices,
      _ConstArraySize(_vertices),
      _indices,
      _ConstArraySize(_indices)
   );
}

/* static */
Mesh Mesh::CreateFromFile(const char* filename) {
   _KSI_NC_::ObjReaderResult r = _KSI_NC_::ObjRead(filename);
   Array<_KSI_RENDER_::Vertex> vertices;
   Array<uint16_t> indices;

   for (int i = r.faces.count() - 1; i >= 0; i--) {
      _KSI_NC_::ObjFace& face = r.faces[i];

      _KSI_NC_::ObjV v = r.vertices[face.vertexIdx];
      _KSI_NC_::ObjVN vn = r.norms[face.normIdx];
      _KSI_NC_::ObjVT vt = r.uvs[face.uvIdx];
      _KSI_RENDER_::Vertex vertex({ Vector3(v.x, v.y, v.z), Vector3(vn.x, vn.y, vn.z), Vector2(vt.u, vt.v) });

      size_t existVertexIdx = vertices.find(vertex);
      if (existVertexIdx != -1) {
         indices.push_back(existVertexIdx);
      } else {
         vertices.push_back(vertex);
         indices.push_back(vertices.count()-1);
      }
   }

   return Mesh(vertices.data(), vertices.count(), indices.data(), indices.count());
}

KSI_END