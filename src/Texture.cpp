#include "Texture.hpp"

#include <stdio.h>

#include <d3d11.h>

#include <stb_image.h>

#include "Engine.hpp"

KSI_START

Texture::Texture(uint32_t width, uint32_t height, const void* data, size_t bytesPerRow) 
   : m_rawTexture(nullptr), m_textureView(nullptr)
{
   m_data = new char[height * bytesPerRow];
   memcpy(m_data, data, height * bytesPerRow);
   m_width = width;
   m_height = height;
   m_bytesPerRow = bytesPerRow;
   UpdateTexture();
}

Texture::~Texture() {
   _SafeRelease(m_textureView);
   _SafeRelease(m_rawTexture);
   delete[] m_data;
}

void Texture::UpdateTexture() {
   ID3D11Device* d3ddevice = Engine::Get().GetRenderer()->GetDevice();

   _SafeRelease(m_textureView);
   _SafeRelease(m_rawTexture);

   D3D11_TEXTURE2D_DESC textureDesc;
   _ClearStructure(textureDesc);
   textureDesc.Width = m_width;
   textureDesc.Height = m_height;
   textureDesc.MipLevels = 1;
   textureDesc.ArraySize = 1;
   textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   textureDesc.SampleDesc.Count = 1;
   textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
   textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

   D3D11_SUBRESOURCE_DATA textureSubresourceData;
   textureSubresourceData.pSysMem = m_data;
   textureSubresourceData.SysMemPitch = m_bytesPerRow;

   _DirectXAssert(
      d3ddevice->CreateTexture2D(&textureDesc, &textureSubresourceData, &m_rawTexture)
   );

   D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
   _ClearStructure(textureViewDesc);
   textureViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
   textureViewDesc.Texture2D.MipLevels = 1;

   _DirectXAssert(
      d3ddevice->CreateShaderResourceView(m_rawTexture, &textureViewDesc, &m_textureView)
   );
}

const Texture& Texture::operator=(const Texture& other) {
   m_data = new char[other.m_height * other.m_bytesPerRow];
   memcpy(m_data, other.m_data, other.m_height * other.m_bytesPerRow);
   m_width = other.m_width;
   m_height = other.m_height;
   m_bytesPerRow = other.m_bytesPerRow;
   UpdateTexture();
   return other;
}

/* static */
Texture Texture::CreateFromFile(const char* filename, uint32_t forceChannelsNumber) {
   int width, height;
   int channelsNumber;
   
   void* textureData = stbi_load(filename, &width, &height, &channelsNumber, forceChannelsNumber);
   assert(textureData != nullptr && "Can't load texture. Does file exist?");

   printf("%i %i %i\n", width, height, channelsNumber);

   Texture texture(width, height, textureData, 4 * width);
   free(textureData);
   return texture;
}

KSI_END