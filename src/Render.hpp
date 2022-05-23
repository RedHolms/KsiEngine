#pragma once
#include "KsiMain.hpp"

#include <windows.h>
#include <d3d11.h>

KSI_RENDER_START

struct Vertex {
   float pos_x, pos_y, pos_z;
};

struct DrawObject {
   ID3D11Buffer* vertices;
   size_t verticesCount;
};

class Renderer {
public:
   KSI_API Renderer(HWND windowHandle);
   KSI_API ~Renderer();

private:
   bool m_valid;
   HRESULT m_lastError;

   D3D_FEATURE_LEVEL m_featureLevel;

   DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
   IDXGISwapChain* m_swapChain;
   ID3D11RenderTargetView* m_renderTargetView;

   ID3D11Device* m_device;
   ID3D11DeviceContext* m_deviceContext;

   ID3D11InputLayout* m_inputLayout;
   ID3D11VertexShader* m_vertexShader;
   ID3D11PixelShader* m_pixelShader;

public:
   bool IsValid() { return m_valid; }
   HRESULT GetLastError() { return m_lastError; }

   ID3D11Device* GetDevice() { return m_device; }

   KSI_API void Render(DrawObject& dro);

   KSI_API ID3D11Buffer* CreateVertexBuffer(Vertex* vertices, size_t count);

private:
};

KSI_RENDER_END