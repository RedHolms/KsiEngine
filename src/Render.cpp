#include "Render.hpp"

#include <stdio.h>

#include <windows.h>
#include <DirectXColors.h>
#include <d3d11.h>

#define _SafeRelease(p) if (p) p->Release(), p = nullptr

static const UINT verteciesStride[] = { sizeof(_KSI_::Render::Vertex) };
static const UINT verteciesOffset[] = { 0 };

static bool ReadFileContent(const char* name, void** outcontent, size_t* outsize) {
   FILE* f = fopen(name, "r");
   if (!f) return false;
   size_t size;
   void* content;

   fseek(f, 0L, SEEK_END);
   size = ftell(f);
   fseek(f, 0L, SEEK_SET);
   content = new char[size];
   fread(content, sizeof(char), size, f);
   fclose(f);

   *outcontent = content;
   if (outsize) *outsize = size;
   return true;
}

KSI_RENDER_START

Renderer::Renderer(HWND windowHandle)
   : m_swapChain(nullptr), m_depthStencilView(nullptr), m_renderTargetView(nullptr),
   m_device(nullptr), m_deviceContext(nullptr), m_inputLayout(nullptr),
   m_vertexShader(nullptr), m_pixelShader(nullptr), m_rasterizerState(nullptr),
   m_depthStencilState(nullptr), m_constantBuffer(nullptr)
{
   m_windowHandle = windowHandle;

   void* vertexShaderBc;
   size_t vertexShaderBcSize;

   if (!_InitDeviceAndSwapChain()) return;
   if (!_InitRenderTargetViewAndDepthStencilView()) return;
   if (!_InitVertexShader(&vertexShaderBc, &vertexShaderBcSize)) return;
   if (!_InitPixelShader()) return;
   if (!_InitInputLayout(vertexShaderBc, vertexShaderBcSize)) return;
   if (!_InitConstantBuffer()) return;
   if (!_InitRasterizerState()) return;
   if (!_InitDepthStencilState()) return;
   m_valid = true;
}

Renderer::~Renderer() {
   _SafeRelease(m_depthStencilView);
   _SafeRelease(m_renderTargetView);
   _SafeRelease(m_inputLayout);
   _SafeRelease(m_swapChain);
   _SafeRelease(m_vertexShader);
   _SafeRelease(m_pixelShader);
   _SafeRelease(m_deviceContext);
   _SafeRelease(m_device);
}

void Renderer::Render(DrawObject& dro) {
   RECT winRect;
   GetWindowRect(m_windowHandle, &winRect);

   /* === Setup Viewport === */
   D3D11_VIEWPORT vp;
   vp.Width = (FLOAT)winRect.right - winRect.left;
   vp.Height = (FLOAT)winRect.bottom - winRect.top;
   vp.MinDepth = 0.0f;
   vp.MaxDepth = 1.0f;
   vp.TopLeftX = 0;
   vp.TopLeftY = 0;
   m_deviceContext->RSSetViewports(1, &vp);

   m_deviceContext->ClearRenderTargetView(m_renderTargetView, DirectX::Colors::SkyBlue);
   m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

   m_deviceContext->RSSetState(m_rasterizerState);
   m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);

   m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

   m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
   m_deviceContext->IASetInputLayout(m_inputLayout);

   m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
   m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

   m_deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

   /* per draw object */
   m_deviceContext->IASetVertexBuffers(0, 1, &dro.vertices, verteciesStride, verteciesOffset);
   m_deviceContext->IASetIndexBuffer(dro.indices, DXGI_FORMAT_R16_UINT, 0);

   m_deviceContext->DrawIndexed(dro.indicesCount, 0, 0);

   m_swapChain->Present(1, 0);
}

ID3D11Buffer* Renderer::CreateVertexBuffer(Vertex* vertices, size_t count) {
   ID3D11Buffer* vertexBuffer;

   D3D11_BUFFER_DESC vertexBuffDesc;
   ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
   vertexBuffDesc.ByteWidth = count * sizeof(Vertex);
   vertexBuffDesc.Usage     = D3D11_USAGE_IMMUTABLE;
   vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

   D3D11_SUBRESOURCE_DATA subResourceData;
   ZeroMemory(&subResourceData, sizeof(subResourceData));
   subResourceData.pSysMem = vertices;

   m_device->CreateBuffer(&vertexBuffDesc, &subResourceData, &vertexBuffer);
   return vertexBuffer;
}

ID3D11Buffer* Renderer::CreateIndexBuffer(uint16_t* indices, size_t count) {
   ID3D11Buffer* indexBuffer;

   D3D11_BUFFER_DESC vertexBuffDesc;
   ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
   vertexBuffDesc.ByteWidth = count * sizeof(uint16_t);
   vertexBuffDesc.Usage     = D3D11_USAGE_IMMUTABLE;
   vertexBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

   D3D11_SUBRESOURCE_DATA subResourceData;
   ZeroMemory(&subResourceData, sizeof(subResourceData));
   subResourceData.pSysMem = indices;

   m_device->CreateBuffer(&vertexBuffDesc, &subResourceData, &indexBuffer);
   return indexBuffer;
}

/*
   ======= Initialization methods =======
*/

// Initialize m_device, m_deviceContext and m_swapChain (m_swapChainDesc)
bool Renderer::_InitDeviceAndSwapChain() {
   HRESULT hr;

   RECT winRect;
   GetWindowRect(m_windowHandle, &winRect);

   ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
   m_swapChainDesc.BufferCount = 1;
   m_swapChainDesc.BufferDesc.Width = winRect.right - winRect.left;
   m_swapChainDesc.BufferDesc.Height = winRect.bottom - winRect.top;
   m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
   m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
   m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   m_swapChainDesc.OutputWindow = m_windowHandle;
   m_swapChainDesc.SampleDesc.Count = 1;
   m_swapChainDesc.SampleDesc.Quality = 0;
   m_swapChainDesc.Windowed = TRUE;

   D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0
   };

   hr = D3D11CreateDeviceAndSwapChain(
      NULL,
      D3D_DRIVER_TYPE_REFERENCE,
      NULL,
      0,
      featureLevels,
      _ConstArraySize(featureLevels),
      D3D11_SDK_VERSION,
      &m_swapChainDesc,
      &m_swapChain,
      &m_device,
      &m_featureLevel,
      &m_deviceContext
   );
   if (FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return false;
   }

   return true;
}

// Initialize m_depthStencilView and m_renderTargetView
bool Renderer::_InitRenderTargetViewAndDepthStencilView() {
   HRESULT hr;

   ID3D11Texture2D* renderTargetBuffer = nullptr;
   hr = m_swapChain->GetBuffer(0, __uuidof(decltype(*renderTargetBuffer)), (void**)&renderTargetBuffer);
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return false;
   }

   hr = m_device->CreateRenderTargetView(renderTargetBuffer, nullptr, &m_renderTargetView);
   if(FAILED(hr)) {
      renderTargetBuffer->Release();
      m_lastError = hr;
      m_valid = false;
      return false;
   }

   D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
   renderTargetBuffer->GetDesc(&depthStencilBufferDesc);
   renderTargetBuffer->Release();

   depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
   depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

   ID3D11Texture2D* depthStencilBuffer;
   m_device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &depthStencilBuffer);

   m_device->CreateDepthStencilView(depthStencilBuffer, nullptr, &m_depthStencilView);

   depthStencilBuffer->Release();

   return true;
}

// Initialize m_vertexShader
bool Renderer::_InitVertexShader(void** outshaderBc, size_t* outshaderBcSize) {
   HRESULT hr;

   void* vertexBc;
   size_t vertexBcSize;

   bool res = ReadFileContent("vertex.cso", &vertexBc, &vertexBcSize);
   if (!res) {
      m_lastError = -1;
      m_valid = false;
      return false;
   }

   hr = m_device->CreateVertexShader(vertexBc, vertexBcSize, nullptr, &m_vertexShader);
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return false;
   }

   *outshaderBc = vertexBc;
   *outshaderBcSize = vertexBcSize;

   return true;
}

// Initialize m_pixelShader
bool Renderer::_InitPixelShader() {
   HRESULT hr;

   void* pixelBc;
   size_t pixelBcSize;

   bool res = ReadFileContent("pixel.cso", &pixelBc, &pixelBcSize);
   if (!res) {
      m_lastError = -1;
      m_valid = false;
      return false;
   }

   hr = m_device->CreatePixelShader(pixelBc, pixelBcSize, nullptr, &m_pixelShader);
   delete[] pixelBc;
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return false;
   }

   return true;
}

// Initialize m_inputLayout, using data from _InitVertexShader
bool Renderer::_InitInputLayout(void* vertexShaderBc, size_t vertexShaderBcSize) {
   HRESULT hr;

   D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
      { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      /*
      { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      */
   };

   hr = m_device->CreateInputLayout(
      inputElementDesc,
      _ConstArraySize(inputElementDesc),
      vertexShaderBc, vertexShaderBcSize,
      &m_inputLayout
   );
   delete[] vertexShaderBc;
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return false;
   }

   return true;
}

// Initialize m_constantBuffer
bool Renderer::_InitConstantBuffer() {
   HRESULT hr;

   D3D11_BUFFER_DESC constantBufferDesc = {};
   constantBufferDesc.ByteWidth      = sizeof(ConstantBufferData);
   constantBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
   constantBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
   constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

   hr = m_device->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBuffer);
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return false;
   }

   return true;
}

// Initialize m_rasterizerState
bool Renderer::_InitRasterizerState() {
   HRESULT hr;

   D3D11_RASTERIZER_DESC rasterizerStateDesc;
   ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
   rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
   rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
   rasterizerStateDesc.FrontCounterClockwise = TRUE;

   m_device->CreateRasterizerState(&rasterizerStateDesc, &m_rasterizerState);

   return true;
}

// Initialize m_depthStencilState
bool Renderer::_InitDepthStencilState() {
   HRESULT hr;

   D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
   depthStencilStateDesc.DepthEnable      = TRUE;
   depthStencilStateDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
   depthStencilStateDesc.DepthFunc        = D3D11_COMPARISON_LESS;

   m_device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthStencilState);

   return true;
}

KSI_RENDER_END