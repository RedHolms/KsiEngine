#pragma once
#include "KsiMain.hpp"

KSI_START

/* abstract */
class GameScript {
public:
   virtual void OnStart() {} // Calls before first frame
   virtual void OnUpdate(float deltaTime) {} // Calls on every update (before rendering)
   virtual void OnBeginFrame() {} // Calls when renderer in ready for render, but render didn't started
   virtual void OnEndFrame() {} // Calls after rendering, but before presenting
   virtual void OnDestroy() {} // Calls on engine instance destroying
};

KSI_END