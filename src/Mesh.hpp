#pragma once
#include "KsiMain.hpp"

#include <d3d11.h>

#include "Renderer.hpp"
#include "Array.hpp"

KSI_START

class Mesh {
public:
   KSI_API Mesh(const _KSI_RENDER_::Vertex* vertices, size_t verticesCount, const uint16_t* indices, size_t indicesCount);
   Mesh(const Mesh& other) { operator=(other); }
   KSI_API ~Mesh();

   static KSI_API Mesh BOX(int sideSize);
   static KSI_API Mesh PLATE(int width, int length);

private:
   Array<_KSI_RENDER_::Vertex> m_rawVertices;
   Array<uint16_t> m_rawIndices;
   ID3D11Buffer* m_vertices;
   ID3D11Buffer* m_indices;
   size_t m_indicesCount;

public:
   KSI_API void UpdateBuffers();

   ID3D11Buffer* GetVertices() { return m_vertices; }
   ID3D11Buffer* GetIndices() { return m_indices; }
   size_t GetIndicesCount() { return m_indicesCount; }

   KSI_API const Mesh& operator=(const Mesh& other);
};

KSI_END