#pragma once
#include "KsiMain.hpp"

KSI_START

/* abstract */
class GameScript {
public:

   virtual void OnStart() {} // Calls when game is started
   virtual void OnUpdate(float deltaTime) {} // Calls on every update (before rendering)
   virtual void OnBeginFrame() {} // Calls when renderer setuped for render, but render doesn't started
   virtual void OnEndFrame() {} // Calls after rendering, but before displaying frame
   virtual void OnDestroy() {} // Calls on game destroy
};

KSI_END