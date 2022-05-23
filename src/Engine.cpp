#include "Engine.hpp"

#include <windows.h>

#include "Render.hpp"

static _KSI_::Engine* _instance = nullptr;
static _KSI_::Render::DrawObject _sampleTrinagle;

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
   WNDCLASSA wc {
      0,                      /* style */
      _KsiWndProc,            /* lpfnWndProc */
      0,                      /* cbClsExtra */
      0,                      /* cbWndExtra */
      GetModuleHandleA(NULL), /* hInstance */
      NULL,                   /* hIcon */
      NULL,                   /* hCursor */
      NULL,                   /* hbrBackground */
      NULL,                   /* lpszMenuName */
      "KSIENG_MAINWND_CLS"    /* lpszClassName */
   };

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

   m_renderer = new Render::Renderer(m_winHandle);
   if (!m_renderer->IsValid()) {
      m_lastError = m_renderer->GetLastError();
      m_valid = false;
   }

   float vertex_data_array[] = {
      0.0f,  0.5f,  0.0f,
      0.5f, -0.5f,  0.0f,
      -0.5f, -0.5f,  0.0f
   };
   _sampleTrinagle.vertices = m_renderer->CreateVertexBuffer((Render::Vertex*)vertex_data_array, 3);
   _sampleTrinagle.verticesCount = 3;

   m_valid = true;
}
Engine::~Engine() {
   _sampleTrinagle.vertices->Release();
   if (m_renderer)
      delete m_renderer;
}

void Engine::Update() {

}

void Engine::Render() {
   m_renderer->Render(_sampleTrinagle);
}

void Engine::Process() {
   Update();
   Render();
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

      Process();
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
