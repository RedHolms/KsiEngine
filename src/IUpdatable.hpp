#pragma once
#include "KsiMain.hpp"

KSI_START

KsiInterface IUpdatable {
KsiInterface_begin

   KsiInterface_method_
      void Update(float deltaTime)
   _KsiInterface_method;
};

KSI_END