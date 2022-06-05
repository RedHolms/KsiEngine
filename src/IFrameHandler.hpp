#pragma once
#include "KsiMain.hpp"

KSI_START

KsiInterface IFrameHandler {
KsiInterface_begin

   KsiInterface_method_
      void OnBeginFrame()
   _KsiInterface_method;

   KsiInterface_method_
      void OnEndFrame()
   _KsiInterface_method;
};

KSI_END