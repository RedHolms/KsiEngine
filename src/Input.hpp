#pragma once
#include "KsiMain.hpp"

#include <windows.h>

#include "IFrameHandler.hpp"
#include "IUpdatable.hpp"
#include "IWndProc.hpp"
#include "Vector.hpp"
#include "Keys.hpp"

KSI_START

class Input : public IFrameHandler, public IWndProc, public IUpdatable {
public:
   KSI_API Input(HWND windowHandle);

private:
   HWND m_windowHandle;

   RAWINPUT m_rawInputData;

   bool m_keysPressed[KSIVKEY__MAX];
   bool m_keysDown[KSIVKEY__MAX];

   Vector2 m_mouseMove;

public:
   KSI_API Vector2 GetMouseMoveDirection();
   KSI_API float GetMouseMoveForce();

   bool IsKeyPressed(unsigned char key) { return m_keysPressed[key]; }
   bool IsKeyDown(unsigned char key) { return m_keysDown[key]; }

   KSI_API void Update(float);
   KSI_API void OnEndFrame();
   KSI_API bool WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult);

   // BeginOfFrame doesn't used
   void OnBeginFrame() {}

private:
   KSI_API void _ProcessInput();
};

KSI_END