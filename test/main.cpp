#include <stdio.h>

#include <windows.h>

#include <ksiEngine/GameScript.hpp>
#include <ksiEngine/Engine.hpp>
#include <ksiEngine/Math.hpp>

const float MouseSensitivity = 0.05f;
const float CameraMoveSpeed = 10.0f;
const float CameraRotateSpeed = 80.0f;

class MainScript : public KsiEngine::GameScript {
private:
   KsiEngine::GameObject* m_cube;

public:
   void OnStart() {
      KsiEngine::Engine& eng = KsiEngine::Engine::Get();
      
      m_cube = eng.CreateGameObject(KsiEngine::Vector3(0.0f, 0.0f, 0.0f), KsiEngine::Vector3(0.0f, 0.0f, 0.0f));
      eng.GetCamera()->SetPosition(KsiEngine::Vector3(-5.0f, 0.0f, 0.0f));
   }

   void OnUpdate(float deltaTime) {
      KsiEngine::Engine& eng = KsiEngine::Engine::Get();
      
      KsiEngine::Input* input = eng.GetInput();
      KsiEngine::Camera* camera = eng.GetCamera();

      KsiEngine::Vector2 mouseMoveVec = input->GetMouseMoveDirection() * input->GetMouseMoveForce() * MouseSensitivity;
      
      KsiEngine::Vector3 cameraForward = camera->GetForwardDirection();
      KsiEngine::Vector3 cameraSide = KsiEngine::Math::Vector3Cross(cameraForward, KsiEngine::Vector3(0.0f, 1.0f, 0.0f));

      KsiEngine::Vector3 cameraMove;
      KsiEngine::Vector3 cameraRot = KsiEngine::Vector3(0.0f, mouseMoveVec.x, -mouseMoveVec.y);

      m_cube->GetRotation() += KsiEngine::Vector3(80.0f * deltaTime, 80.0f * deltaTime, 80.0f * deltaTime);

      if (input->IsKeyDown(KsiEngine::KSIVKEY_LEFT))
         cameraRot.y -= CameraRotateSpeed * deltaTime;
      if (input->IsKeyDown(KsiEngine::KSIVKEY_RIGHT))
         cameraRot.y += CameraRotateSpeed * deltaTime;
      if (input->IsKeyDown(KsiEngine::KSIVKEY_DOWN))
         cameraRot.z -= CameraRotateSpeed * deltaTime;
      if (input->IsKeyDown(KsiEngine::KSIVKEY_UP))
         cameraRot.z += CameraRotateSpeed * deltaTime;
      
      if (input->IsKeyDown('W'))
         cameraMove += cameraForward * CameraMoveSpeed * deltaTime;
      if (input->IsKeyDown('A'))
         cameraMove -= cameraSide * CameraMoveSpeed * deltaTime;
      if (input->IsKeyDown('S'))
         cameraMove -= cameraForward * CameraMoveSpeed * deltaTime;
      if (input->IsKeyDown('D'))
         cameraMove += cameraSide * CameraMoveSpeed * deltaTime;
      if (input->IsKeyDown('E'))
         cameraMove.y += CameraMoveSpeed * deltaTime;
      if (input->IsKeyDown('Q'))
         cameraMove.y -= CameraMoveSpeed * deltaTime;

      cameraRot += camera->GetRotation();
      cameraRot.z = _Clamp(cameraRot.z, -89.9, 89.9); // 90 degrees brokes everything
      printf("%f\n", cameraRot.z);
      camera->SetRotation(cameraRot);
      camera->GetPosition() += cameraMove;
   }
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int) {
   AllocConsole();
   freopen("CON", "w", stdout);

   KsiEngine::Engine& eng = KsiEngine::Engine::Get();

   MainScript scr;
   eng.AddScript(&scr);

   int status = eng.Run();
   KsiEngine::Engine::Destroy();
   return status;
}