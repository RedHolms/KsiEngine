#include "Input.hpp"

#include "Engine.hpp"
#include "Vector.hpp"
#include "Math.hpp"

#define HID_USAGE_PAGE_GENERIC     0x01;
#define HID_USAGE_GENERIC_MOUSE    0x02;
#define HID_USAGE_GENERIC_JOYSTICK 0x04;
#define HID_USAGE_GENERIC_GAMEPAD  0x05;

KSI_START

Input::Input(HWND windowHandle) {
   m_windowHandle = windowHandle;

   RAWINPUTDEVICE Rid[3];

   //Mouse
   Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
   Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
   Rid[0].dwFlags = RIDEV_INPUTSINK;
   Rid[0].hwndTarget = m_windowHandle;

   //Joystick
   Rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
   Rid[1].usUsage = HID_USAGE_GENERIC_JOYSTICK;
   Rid[1].dwFlags = 0;
   Rid[1].hwndTarget = 0;

   //Gamepad
   Rid[2].usUsagePage = HID_USAGE_PAGE_GENERIC;
   Rid[2].usUsage = HID_USAGE_GENERIC_GAMEPAD;
   Rid[2].dwFlags = 0;
   Rid[2].hwndTarget = 0;

   RegisterRawInputDevices(Rid, 3, sizeof(RAWINPUTDEVICE));

   memset(m_keysPressed, 0, sizeof(m_keysPressed));
   memset(m_keysDown, 0, sizeof(m_keysDown));
}

Vector2 Input::GetMouseMoveDirection() {
   return _KSI_MATH_::Vector2Normalize(m_mouseMove);
}

float Input::GetMouseMoveForce() {
   return _KSI_MATH_::Vector2Length(m_mouseMove);
}

void Input::Update(float) {
   if (!Engine::Get().IsWindowSelected())
      return;

   RECT windowRect;
   GetWindowRect(m_windowHandle, &windowRect);
   SetCursorPos(
      windowRect.left + (_RectWidth(windowRect) / 2),
      windowRect.top + (_RectHeight(windowRect) / 2)
   );
}

void Input::OnEndFrame() {
   memset(m_keysPressed, 0, sizeof(m_keysPressed));
   m_mouseMove = Vector2();
}

bool Input::WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult) {   
   switch(msg) {
      case WM_INPUT: {
         UINT rawInputDataSize = sizeof(m_rawInputData);
         GetRawInputData(
            (HRAWINPUT)lParam,
            RID_INPUT,
            &m_rawInputData,
            &rawInputDataSize,
            sizeof(RAWINPUTHEADER)
         );
         _ProcessInput();
         break;
      }
      case WM_KEYDOWN:
      case WM_SYSKEYDOWN: {
         m_keysPressed[wParam] = m_keysDown[wParam] = true;
         break;
      }
      case WM_KEYUP:
      case WM_SYSKEYUP: {
         m_keysDown[wParam] = false;
         break;
      }
   }
   return false;
}

void Input::_ProcessInput() {
   if (!Engine::Get().IsWindowSelected())
      return;

   if (m_rawInputData.header.dwType == RIM_TYPEMOUSE) {
      if (m_rawInputData.data.mouse.usFlags == MOUSE_MOVE_RELATIVE) {
         m_mouseMove.x += m_rawInputData.data.mouse.lLastX;
         m_mouseMove.y += m_rawInputData.data.mouse.lLastY;
      }
   }
}

KSI_END