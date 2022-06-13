#pragma once
#include "KsiMain.hpp"

#include <d3d11.h>

KSI_START

class Texture {
public:
   KSI_API Texture(void* data, uint32_t width, uint32_t height, uint32_t channelsCount);
   Texture(const Texture& other) : m_rawTexture(nullptr), m_textureView(nullptr) { operator=(other); }
   KSI_API ~Texture();

   static KSI_API Texture CreateFromFile(const char* filename, uint32_t forceChannelsCount = 4);

private:
   void* m_data;
   uint32_t m_width, m_height;
   uint32_t m_channelsCount;
   ID3D11Texture2D* m_rawTexture;
   ID3D11ShaderResourceView* m_textureView;

public:
   KSI_API void UpdateTexture();

   ID3D11ShaderResourceView* GetTextureView() { return m_textureView; }

   KSI_API void operator=(const Texture& other);
};

KSI_END