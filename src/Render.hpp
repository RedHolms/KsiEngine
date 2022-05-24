#pragma once
#include "KsiMain.hpp"

#include <stdint.h>

#include <windows.h>
#include <DirectXMath.h>
#include <d3d11.h>

KSI_RENDER_START

struct Vertex {
   float pos_x, pos_y, pos_z;
};

struct DrawObject {
   ID3D11Buffer* vertices;
   ID3D11Buffer* indices;
   size_t indicesCount;
};

struct ConstantBufferData {
   DirectX::XMMATRIX camera_matrix;
};

class Renderer {
public:
   KSI_API Renderer(HWND windowHandle);
   KSI_API ~Renderer();

private:
   bool m_valid;
   HRESULT m_lastError;

   HWND m_windowHandle;

   D3D_FEATURE_LEVEL m_featureLevel;

   DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
   IDXGISwapChain* m_swapChain;

   ID3D11DepthStencilView* m_depthStencilView;
   ID3D11RenderTargetView* m_renderTargetView;

   ID3D11Device* m_device;
   ID3D11DeviceContext* m_deviceContext;

   ID3D11InputLayout* m_inputLayout;
   ID3D11VertexShader* m_vertexShader;
   ID3D11PixelShader* m_pixelShader;

   ID3D11RasterizerState* m_rasterizerState;
   ID3D11DepthStencilState* m_depthStencilState;

   ID3D11Buffer* m_constantBuffer;

public:
   bool IsValid() { return m_valid; }
   HRESULT GetLastError() { return m_lastError; }

   ID3D11Device* GetDevice() { return m_device; }

   KSI_API void UpdateConstantBuffer(ConstantBufferData& data);
   KSI_API void Render(DrawObject& dro);

   KSI_API ID3D11Buffer* CreateVertexBuffer(Vertex* vertices, size_t count);
   KSI_API ID3D11Buffer* CreateIndexBuffer(uint16_t* indices, size_t count);

private:
   KSI_API bool _InitDeviceAndSwapChain();
   KSI_API bool _InitRenderTargetViewAndDepthStencilView();
   KSI_API bool _InitVertexShader(void** outshaderBc, size_t* outshaderBcSize);
   KSI_API bool _InitPixelShader();
   KSI_API bool _InitInputLayout(void* vertexShaderBc, size_t vertexShaderBcSize);
   KSI_API bool _InitConstantBuffer();
   KSI_API bool _InitRasterizerState();
   KSI_API bool _InitDepthStencilState();
};

KSI_RENDER_END