#include <ksiEngine/Engine.hpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
   KsiEngine::Engine& eng = KsiEngine::Engine::Get();
   if (!eng.IsValid()) {
      MessageBoxA(NULL, "An initialization error occurred", "Oops", MB_ICONERROR);
      ExitProcess(2);
   }
   int status = eng.Run();
   KsiEngine::Engine::Destroy();
   return status;
}