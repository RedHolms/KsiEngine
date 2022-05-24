#include "Engine.hpp"

#include <windows.h>

#include "Render.hpp"

static _KSI_::Engine* _instance = nullptr;
static _KSI_::Render::DrawObject _sample_square;

KSI_START

Engine& Engine::Get() {
   if (!_instance)
      _instance = new Engine;
   return *_instance;
}
void Engine::Destroy() {
   if (!_instance) return;
   delete _instance;
}

Engine::Engine()
   : m_winHandle(NULL), m_renderer(nullptr)
{
   _InitWindow();

   m_renderer = new Render::Renderer(m_winHandle);
   if (!m_renderer->IsValid()) {
      m_lastError = m_renderer->GetLastError();
      m_valid = false;
   }

   Render::Vertex vertices[] = {
      { -0.5f, -0.5f, 0.0f },
      { -0.5f,  0.5f, 0.0f },
      {  0.5f,  0.5f, 0.0f },
      {  0.5f, -0.5f, 0.0f },
   };
   uint16_t indices[] = {
      0, 2, 1,
      0, 3, 2
   };
   _sample_square.vertices = m_renderer->CreateVertexBuffer(vertices, _ConstArraySize(vertices));
   _sample_square.indices = m_renderer->CreateIndexBuffer(indices, _ConstArraySize(indices));
   _sample_square.indicesCount = _ConstArraySize(indices);

   m_valid = true;
}

void Engine::_InitWindow() {
   WNDCLASSA wc;
   ZeroMemory(&wc, sizeof(wc));
   wc.lpfnWndProc    = _KsiWndProc;
   wc.hInstance      = GetModuleHandleA(NULL);
   wc.lpszClassName  = "KSIENG_MAINWND_CLS";

   ATOM atom = RegisterClassA(&wc);
   if (!atom) {
      m_lastError = GetLastError();
      m_valid = false;
      return;
   }

   m_winHandle = CreateWindowExA(
      0,
      "KSIENG_MAINWND_CLS",
      "Ksi Engine window",
      WS_POPUP,
      0, 0,
      GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
      NULL,
      NULL,
      GetModuleHandleA(NULL),
      NULL
   );
   if (!m_winHandle) {
      m_lastError = GetLastError();
      m_valid = false;
      return;
   }
}

Engine::~Engine() {
   _sample_square.vertices->Release();
   if (m_renderer)
      delete m_renderer;
}

void Engine::_Update() {
   return;
}

void Engine::_Render() {
   m_renderer->Render(_sample_square);
}

void Engine::_Process() {
   _Update();
   _Render();
}

int Engine::Run() {
   ShowWindow(m_winHandle, SW_SHOWMAXIMIZED);
   SetWindowPos(m_winHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

   MSG msg;
   ZeroMemory(&msg, sizeof(msg));
   while (true) {
      while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
         TranslateMessage(&msg);
         DispatchMessageA(&msg);
         if (msg.message == WM_QUIT) break;
      }
      if (msg.message == WM_QUIT) break;

      _Process();
   }
   return msg.wParam;
}

/* static */
LRESULT WINAPI Engine::_KsiWndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
   switch (msg) {
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   }
   return DefWindowProcA(handle, msg, wParam, lParam);
}

KSI_END
