#pragma once
#include "KsiMain.hpp"

#include <windows.h>

#include "GameObject.hpp"
#include "GameScript.hpp"
#include "Renderer.hpp"
#include "Vector.hpp"
#include "Camera.hpp"
#include "Array.hpp"
#include "Input.hpp"
#include "Mesh.hpp"

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
   HWND m_winHandle;

   _KSI_RENDER_::ConstantBufferData m_constantRenderData;
   _KSI_RENDER_::Renderer* m_renderer;
   Camera* m_camera;
   Input* m_input;
   
   Array<GameObject*> m_objects;
   Array<GameScript*> m_scripts;

   float m_deltaTime;

public:
   Camera* GetCamera() { return m_camera; }
   Input* GetInput() { return m_input; }
   
   _KSI_RENDER_::Renderer* GetRenderer() { return m_renderer; }

   float GetDeltaTime() { return m_deltaTime; }

   KSI_API void AddGameObject(GameObject* obj);

   KSI_API void AddScript(GameScript* scr);

   KSI_API int Run();

private:
   KSI_API void _InitWindow();

   KSI_API void _Process();

   KSI_API void _Update();
   KSI_API void _Render();

   static KSI_API LRESULT WINAPI _WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
};

KSI_END