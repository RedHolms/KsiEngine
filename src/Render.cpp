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
   : m_swapChain(nullptr), m_renderTargetView(nullptr), m_device(nullptr),
   m_deviceContext(nullptr), m_inputLayout(nullptr), m_vertexShader(nullptr),
   m_pixelShader(nullptr)
{
   RECT winRect;
   GetWindowRect(windowHandle, &winRect);

   /* === Create Device and Swap Chain === */
   ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
   m_swapChainDesc.BufferCount = 1;
   m_swapChainDesc.BufferDesc.Width = winRect.right - winRect.left;
   m_swapChainDesc.BufferDesc.Height = winRect.bottom - winRect.top;
   m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
   m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
   m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   m_swapChainDesc.OutputWindow = windowHandle;
   m_swapChainDesc.SampleDesc.Count = 1;
   m_swapChainDesc.SampleDesc.Quality = 0;
   m_swapChainDesc.Windowed = TRUE;

   D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0
   };

   HRESULT hr = D3D11CreateDeviceAndSwapChain(
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
      return;
   }

   /* === Create Render Target View === */
   ID3D11Texture2D* backBuffer = nullptr;
   hr = m_swapChain->GetBuffer(0, __uuidof(decltype(*backBuffer)), (void**)&backBuffer);
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return;
   }

   hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
   backBuffer->Release();
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return;
   }
   m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

   /* === Setup Viewport === */
   D3D11_VIEWPORT vp;
   vp.Width = (FLOAT)winRect.right - winRect.left;
   vp.Height = (FLOAT)winRect.bottom - winRect.top;
   vp.MinDepth = 0.0f;
   vp.MaxDepth = 1.0f;
   vp.TopLeftX = 0;
   vp.TopLeftY = 0;
   m_deviceContext->RSSetViewports(1, &vp);

   /* === Load Shaders === */
   void* vertexBc;
   void* pixelBc;
   size_t vertexBcSize, pixelBcSize;

   bool res = ReadFileContent("vertex.cso", &vertexBc, &vertexBcSize);
   if (!res) {
      m_lastError = -1;
      m_valid = false;
      return;
   }

   res = ReadFileContent("pixel.cso", &pixelBc, &pixelBcSize);
   if (!res) {
      m_lastError = -1;
      m_valid = false;
      return;
   }

   hr = m_device->CreateVertexShader(vertexBc, vertexBcSize, nullptr, &m_vertexShader);
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return;
   }

   hr = m_device->CreatePixelShader(pixelBc, pixelBcSize, nullptr, &m_pixelShader);
   delete[] pixelBc;
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return;
   }

   /* === Create Input Layout === */
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
      vertexBc, vertexBcSize,
      &m_inputLayout
   );
   delete[] vertexBc;
   if(FAILED(hr)) {
      m_lastError = hr;
      m_valid = false;
      return;
   }

   m_valid = true;
}
Renderer::~Renderer() {
   _SafeRelease(m_renderTargetView);
   _SafeRelease(m_inputLayout);
   _SafeRelease(m_swapChain);
   _SafeRelease(m_vertexShader);
   _SafeRelease(m_pixelShader);
   _SafeRelease(m_deviceContext);
   _SafeRelease(m_device);
}

void Renderer::Render(DrawObject& dro) {
   m_deviceContext->ClearRenderTargetView(m_renderTargetView, DirectX::Colors::SkyBlue);

   m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
   m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);
   m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
   m_deviceContext->IASetInputLayout(m_inputLayout);

   /* === draw object === */
   m_deviceContext->IASetVertexBuffers(0, 1, &dro.vertices, verteciesStride, verteciesOffset);
   m_deviceContext->Draw(dro.verticesCount, 0);

   m_swapChain->Present(1, 0);
}

ID3D11Buffer* Renderer::CreateVertexBuffer(Vertex* vertices, size_t count) {
   ID3D11Buffer* vertexBuffer;

   D3D11_BUFFER_DESC vertexBuffDesc;
   ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
   vertexBuffDesc.ByteWidth = count * sizeof(Vertex);
   vertexBuffDesc.Usage     = D3D11_USAGE_DEFAULT;
   vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

   D3D11_SUBRESOURCE_DATA subResourceData;
   ZeroMemory(&subResourceData, sizeof(subResourceData));
   subResourceData.pSysMem = vertices;

   m_device->CreateBuffer(&vertexBuffDesc, &subResourceData, &vertexBuffer);
   return vertexBuffer;
}

KSI_RENDER_END