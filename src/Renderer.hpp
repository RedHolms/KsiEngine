#pragma once
#include "KsiMain.hpp"

#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11.h>

#include "Vector.hpp"

KSI_RENDER_START

struct Vertex {
   Vector3 pos;
   Vector3 color;
   Vector2 textureUV;
};

struct DrawObject {
   ID3D11Buffer* vertices;
   ID3D11Buffer* indices;
   size_t indicesCount;
   ID3D11Buffer* buff;
   ID3D11ShaderResourceView* textureView;
};

struct ConstantBufferData {
   DirectX::XMMATRIX projectionMatrix;
   DirectX::XMMATRIX viewMatrix;
};

struct ObjectBufferData {
   DirectX::XMMATRIX objectMatrix;
   DirectX::XMVECTOR position;
};

class Renderer {
public:
   KSI_API Renderer(HWND windowHandle);
   KSI_API ~Renderer();

private:
   HWND m_windowHandle;

   D3D_FEATURE_LEVEL m_featureLevel;

   ID3D11Device* m_device;
   ID3D11DeviceContext* m_deviceContext;
   IDXGISwapChain* m_swapChain;
   ID3D11RenderTargetView* m_renderTargetView;
   ID3D11Texture2D* m_depthStencilBuffer;
   ID3D11DepthStencilView* m_depthStencilView;
   ID3D11DepthStencilState* m_depthStencilState;
   ID3D11RasterizerState* m_rasterizerState;
   ID3D11SamplerState* m_samplerState;

   ID3D11Buffer* m_constantBuffer;
   ID3D11VertexShader* m_vertexShader;
   ID3D11PixelShader* m_pixelShader;

   ID3D11InputLayout* m_inputLayout;

   D3D11_VIEWPORT m_viewport;

public:
   ID3D11Device* GetDevice() { return m_device; }

   KSI_API void UpdateConstantBuffer(const ConstantBufferData* data);

   KSI_API void Render(const DrawObject* drawObjects, size_t count);
   KSI_API void Present();
};

KSI_RENDER_END