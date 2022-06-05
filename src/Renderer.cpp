#include "Renderer.hpp"

#include <stdio.h> // snprintf

#include <windows.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <d3d11.h>

#include "Vector.hpp"

#define DirectXAssert(expr) _DirectXAssert(expr)

static const uint32_t c_verticesStride[] = { sizeof(_KSI_RENDER_::Vertex) };
static const uint32_t c_verticesOffset[] = { 0 };

KSI_RENDER_START

Renderer::Renderer(HWND windowHandle)
   : m_windowHandle(NULL),
   m_device(nullptr), m_deviceContext(nullptr), m_swapChain(nullptr),
   m_renderTargetView(nullptr), m_depthStencilBuffer(nullptr), m_depthStencilView(nullptr),
   m_depthStencilState(nullptr), m_rasterizerState(nullptr), m_samplerState(nullptr),
   m_constantBuffer(nullptr), m_vertexShader(nullptr), m_pixelShader(nullptr),
   m_inputLayout(nullptr)
{
   m_windowHandle = windowHandle;

   RECT clientRect;
   GetClientRect(m_windowHandle, &clientRect);

   /* Create device, device context and swap chain */
   DXGI_SWAP_CHAIN_DESC swapChainDesc;
   _ClearStructure(swapChainDesc);
   swapChainDesc.BufferCount = 1;
   swapChainDesc.BufferDesc.Width = _RectWidth(clientRect);
   swapChainDesc.BufferDesc.Height = _RectHeight(clientRect);
   swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
   swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
   swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   swapChainDesc.OutputWindow = m_windowHandle;
   swapChainDesc.SampleDesc.Count = 1;
   swapChainDesc.SampleDesc.Quality = 0;
   swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
   swapChainDesc.Windowed = TRUE;

   D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_11_0
   };

   DirectXAssert(
      D3D11CreateDeviceAndSwapChain(
         nullptr,
         D3D_DRIVER_TYPE_HARDWARE,
         NULL,
         0,
         featureLevels,
         _ConstArraySize(featureLevels),
         D3D11_SDK_VERSION,
         &swapChainDesc,
         &m_swapChain,
         &m_device,
         &m_featureLevel,
         &m_deviceContext
      )
   );

   HRESULT hr;

   /* Create render targer view */
   ID3D11Texture2D* backBuffer = nullptr;
   DirectXAssert(
      m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)
   );

   DirectXAssert(
      m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView)
   );

   /* Create depth stencil view */
   D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
   _ClearStructure(depthStencilBufferDesc);
   depthStencilBufferDesc.ArraySize = 1;
   depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
   depthStencilBufferDesc.CPUAccessFlags = 0;
   depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
   depthStencilBufferDesc.Width = _RectWidth(clientRect);
   depthStencilBufferDesc.Height = _RectHeight(clientRect);
   depthStencilBufferDesc.MipLevels = 1;
   depthStencilBufferDesc.SampleDesc.Count = 1;
   depthStencilBufferDesc.SampleDesc.Quality = 0;
   depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

   DirectXAssert(
      m_device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_depthStencilBuffer)
   );

   DirectXAssert(
      m_device->CreateDepthStencilView(m_depthStencilBuffer, nullptr, &m_depthStencilView)
   );

   /* Create depth stencil state */
   D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
   _ClearStructure(depthStencilStateDesc);
   depthStencilStateDesc.DepthEnable = TRUE;
   depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
   depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
   depthStencilStateDesc.StencilEnable = FALSE;

   DirectXAssert(
      m_device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthStencilState)
   );

   /* Create rasterizer state */
   D3D11_RASTERIZER_DESC rasterizerDesc;
   _ClearStructure(rasterizerDesc);
   rasterizerDesc.AntialiasedLineEnable = FALSE;
   rasterizerDesc.CullMode = D3D11_CULL_BACK;
   rasterizerDesc.DepthBias = 0;
   rasterizerDesc.DepthBiasClamp = 0.0f;
   rasterizerDesc.DepthClipEnable = TRUE;
   rasterizerDesc.FillMode = D3D11_FILL_SOLID;
   rasterizerDesc.FrontCounterClockwise = FALSE;
   rasterizerDesc.MultisampleEnable = FALSE;
   rasterizerDesc.ScissorEnable = FALSE;
   rasterizerDesc.SlopeScaledDepthBias = 0.0f;

   DirectXAssert(
      m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState)
   );

   D3D11_SAMPLER_DESC samplerDesc;
   _ClearStructure(samplerDesc);
   samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
   samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
   samplerDesc.BorderColor[0] = 1.0f;
   samplerDesc.BorderColor[1] = 1.0f;
   samplerDesc.BorderColor[2] = 1.0f;
   samplerDesc.BorderColor[3] = 1.0f;
   samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

   DirectXAssert(
      m_device->CreateSamplerState(&samplerDesc, &m_samplerState)
   );

   _ClearStructure(m_viewport);

   /* Create constant buffer */
   D3D11_BUFFER_DESC constantBufferDesc;
   ZeroMemory( &constantBufferDesc, sizeof(D3D11_BUFFER_DESC) );

   constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
   constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
   constantBufferDesc.CPUAccessFlags = 0;
   constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

   DirectXAssert(
      m_device->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBuffer)
   );

   /* Create shaders */
   ID3DBlob* vertexShaderBlob;
   ID3DBlob* pixelShaderBlob;

   DirectXAssert(
      D3DReadFileToBlob(L"vertex.cso", &vertexShaderBlob)
   );

   DirectXAssert(
      D3DReadFileToBlob(L"pixel.cso", &pixelShaderBlob)
   );

   DirectXAssert(
      m_device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &m_vertexShader)
   );

   DirectXAssert(
      m_device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &m_pixelShader)
   );

   _SafeRelease(pixelShaderBlob);

   /* Create input layout */
   D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] = {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, _MemberOffset(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, _MemberOffset(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, _MemberOffset(Vertex, textureUV), D3D11_INPUT_PER_VERTEX_DATA, 0}
   };

   DirectXAssert(
      m_device->CreateInputLayout(
         inputLayoutDesc,
         _ConstArraySize(inputLayoutDesc),
         vertexShaderBlob->GetBufferPointer(),
         vertexShaderBlob->GetBufferSize(),
         &m_inputLayout
      )
   );

   _SafeRelease(vertexShaderBlob);
}

Renderer::~Renderer() {
   _SafeRelease(m_inputLayout);
   _SafeRelease(m_pixelShader);
   _SafeRelease(m_vertexShader);
   _SafeRelease(m_constantBuffer);
   _SafeRelease(m_samplerState);
   _SafeRelease(m_rasterizerState);
   _SafeRelease(m_depthStencilState);
   _SafeRelease(m_depthStencilView);
   _SafeRelease(m_depthStencilBuffer);
   _SafeRelease(m_renderTargetView);
   _SafeRelease(m_swapChain);
   _SafeRelease(m_deviceContext);
   _SafeRelease(m_device);
}

void Renderer::UpdateConstantBuffer(const ConstantBufferData* data) {
   m_deviceContext->UpdateSubresource(m_constantBuffer, 0, nullptr, data, 0, 0);
}

void Renderer::Render(const DrawObject* drawObjects, size_t count) {
   RECT clientRect;
   GetClientRect(m_windowHandle, &clientRect);

   m_deviceContext->ClearRenderTargetView(m_renderTargetView, DirectX::Colors::SkyBlue);
   m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

   m_deviceContext->IASetInputLayout(m_inputLayout);
   m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

   m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
   m_deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);

   m_viewport.Width = _RectWidth(clientRect);
   m_viewport.Height = _RectHeight(clientRect);
   m_viewport.MinDepth = 0.0f;
   m_viewport.MaxDepth = 1.0f;
   m_viewport.TopLeftX = 0.0f;
   m_viewport.TopLeftY = 0.0f;

   m_deviceContext->RSSetState(m_rasterizerState);
   m_deviceContext->RSSetViewports(1, &m_viewport);

   m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
   m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);

   m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
   m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

   for (size_t i = 0; i < count; i++) {
      const DrawObject& dro = drawObjects[i];

      m_deviceContext->PSSetShaderResources(0, 1, &dro.textureView);
      m_deviceContext->VSSetConstantBuffers(1, 1, &dro.buff);
      m_deviceContext->IASetVertexBuffers(0, 1, &dro.vertices, c_verticesStride, c_verticesOffset);
      m_deviceContext->IASetIndexBuffer(dro.indices, DXGI_FORMAT_R16_UINT, 0);
      m_deviceContext->DrawIndexed(dro.indicesCount, 0, 0);
   }
}

void Renderer::Present() {
   m_swapChain->Present(1, 0);
}

KSI_RENDER_END

// Defined in KsiMain.hpp
void DirectXAssertImpl(HRESULT hr, const char* expr, const char* file, uint32_t line) {
   if (FAILED(hr)) {
      static char errorBuff[512];
      snprintf(errorBuff, _ConstArraySize(errorBuff),
         "An DirectX call failed\n"
         "In file: %s\n"
         "At line: %u\n"
         "\n"
         "Expression: %s\n"
         "\n"
         "Result: 0x%X",
      file, line,
      expr, hr);

      MessageBoxA(NULL, errorBuff, "Fatal error", MB_ICONERROR);
      ExitProcess(2);
   }
}