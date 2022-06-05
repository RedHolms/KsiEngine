#include "Engine.hpp"

#include <stdio.h>

#include <windows.h>
#include <timeapi.h>

#include "Renderer.hpp"
#include "Math.hpp"

static _KSI_::Engine* _instance = nullptr;

#define _RaiseStart(scripts) for (auto& scr : scripts) scr->OnStart()
#define _RaiseDestroy(scripts) for (auto& scr : scripts) scr->OnDestroy()
#define _RaiseUpdate(dt, input, scripts) input->Update(dt); for (auto& scr : scripts) scr->OnUpdate(dt)
#define _RaiseBeginFrame(input, scripts) input->OnBeginFrame(); for (auto& scr : scripts) scr->OnBeginFrame()
#define _RaiseEndFrame(input, scripts) for (auto& scr : scripts) scr->OnEndFrame(); input->OnEndFrame()

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
   : m_winHandle(NULL), m_renderer(nullptr), m_camera(nullptr)
{
   freopen("CON", "w", stdout);
   
   _ClearStructure(m_constantRenderData);

   _InitWindow();

   m_renderer = new _KSI_RENDER_::Renderer(m_winHandle);
   m_camera = new Camera(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
   m_input = new Input(m_winHandle);
}

Engine::~Engine() {
   _RaiseDestroy(m_scripts);
   for (auto& scr : m_scripts) delete scr;
   m_scripts.clear();

   _SafeDelete(m_input);
   _SafeDelete(m_camera);
   _SafeDelete(m_renderer);
}

void Engine::AddGameObject(GameObject* obj) {
   m_objects.push_back(obj);
}

void Engine::AddScript(GameScript* scr) {
   m_scripts.push_back(scr);
}

int Engine::Run() {
   _RaiseStart(m_scripts);

   ShowWindow(m_winHandle, SW_SHOWMAXIMIZED);
   ShowCursor(FALSE);
   SetWindowPos(m_winHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

   MSG msg;
   _ClearStructure(msg);
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

void Engine::_InitWindow() {
   WNDCLASSA wc;
   _ClearStructure(wc);
   wc.lpfnWndProc    = _WndProc;
   wc.hInstance      = GetModuleHandleA(NULL);
   wc.lpszClassName  = "KSIENG_MAINWND_CLS";

   ATOM atom = RegisterClassA(&wc);
   assert(atom != NULL);

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
   assert(m_winHandle != NULL);
}

void Engine::_Process() {
   static DWORD lastTime = timeGetTime();
   m_deltaTime = (float)(timeGetTime() - lastTime) / 1000.0f;
   lastTime = timeGetTime();
   
   _Update();
   _Render();
}

void Engine::_Update() {
   _RaiseUpdate(m_deltaTime, m_input, m_scripts);
}

void Engine::_Render() {
   RECT clientRect;
   GetClientRect(m_winHandle, &clientRect);

   Array<_KSI_RENDER_::DrawObject> toDraw(m_objects.count());
   for (auto& gObj : m_objects)
      toDraw.push_back(gObj->GetDrawObject());

   float clientWidth = (float)_RectWidth(clientRect);
   float clientHeight = (float)_RectHeight(clientRect);

   m_constantRenderData.projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), clientWidth / clientHeight, 0.1f, 100.0f);
   m_constantRenderData.viewMatrix = m_camera->GetViewMatrix();
   m_renderer->UpdateConstantBuffer(&m_constantRenderData);

   _RaiseBeginFrame(m_input, m_scripts);
   m_renderer->Render(toDraw.data(), toDraw.count());
   _RaiseEndFrame(m_input, m_scripts);
   m_renderer->Present();
}

/* static */
LRESULT WINAPI Engine::_WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
   switch (msg) {
      case WM_DESTROY: {
         PostQuitMessage(0);
         return 0;
      }
      case WM_SIZE: {
         // if (_instance && _instance->m_renderer)
         //    _instance->m_renderer->PerformResize();
         return 0;
      }
   }

   if (_instance) {
      LRESULT lResult = 0;

      if (
         _instance->m_input->WndProc(handle, msg, wParam, lParam, &lResult)
      ) return lResult;
   }

   return DefWindowProcA(handle, msg, wParam, lParam);
}

KSI_END
