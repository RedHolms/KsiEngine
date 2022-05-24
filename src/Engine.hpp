#pragma once
#include "KsiMain.hpp"

#include <windows.h>

#include "Render.hpp"

KSI_START

/* singleton */
class Engine {
public:
   static KSI_API Engine& Get();
   static KSI_API void Destroy();

   Engine(Engine const&) = delete;
   void operator=(Engine const&) = delete;

private:
   KSI_API Engine();
   KSI_API ~Engine();

private:
   bool m_valid;
   DWORD m_lastError;

   HWND m_winHandle;

   Render::Renderer* m_renderer;
   Render::ConstantBufferData m_renderConstantData;

public:
   bool IsValid() const { return m_valid; }
   DWORD GetLastError() const { return m_lastError; }

   Render::Renderer* GetRenderer() { return m_renderer; }

   KSI_API int Run();

private:
   KSI_API void _InitWindow();

   KSI_API void _Process();

   KSI_API void _Update();
   KSI_API void _Render();

   static KSI_API LRESULT WINAPI _KsiWndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
};

KSI_END