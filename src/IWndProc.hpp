#pragma once
#include "KsiMain.hpp"

#include <windows.h>

KSI_START

KsiInterface IWndProc {
KsiInterface_begin

   KsiInterface_method_
      bool WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lResult)
   _KsiInterface_method;
};

KSI_END