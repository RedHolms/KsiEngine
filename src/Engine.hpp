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

public:
   bool IsValid() const { return m_valid; }
   DWORD GetLastError() const { return m_lastError; }

   KSI_API int Run();
private:
   KSI_API void Process();

   KSI_API void Update();
   KSI_API void Render();

   static KSI_API LRESULT WINAPI _KsiWndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
};

KSI_END